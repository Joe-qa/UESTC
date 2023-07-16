package chandy_lamport

import (
	"fmt"
	"io/ioutil"
	"log"
	"path"
	"reflect"
	"regexp"
	"sort"
	"strconv"
	"strings"
)

// ==================================
//  Helper methods used in test code
// ==================================

// Directory containing all the test files
const testDir = "test_data"

// Read the topology from a ".top" file.
// The expected format of the file is as follows:
//   - The first line contains number of servers N (e.g. "2")
//   - The next N lines each contains the server ID and the number of tokens on
//     that server, in the form "[serverId] [numTokens]" (e.g. "N1 1")
//   - The rest of the lines represent unidirectional links in the form "[src dst]"
//     (e.g. "N1 N2")

// 整个网络是一个无向图,每个server拥有自己的token余额
func readTopology(fileName string, sim *Simulator) {
	b, err := ioutil.ReadFile(path.Join(testDir, fileName))
	//检查是否正常读取文件
	checkError(err)
	lines := strings.FieldsFunc(string(b), func(r rune) bool { return r == '\n' })
	//需要去除每行的\r，否则不能正常解析文件
	for i, _ := range lines {
		lines[i] = strings.Trim(lines[i], "\r")
		//fmt.Println(lines[i])
	}
	// Must call this before we start logging
	//初始化log event
	sim.logger.NewEpoch()

	// Parse topology from lines

	//server的数量
	numServersLeft := -1
	for _, line := range lines {
		// Ignore comments
		if strings.HasPrefix(line, "#") {
			continue
		}
		if numServersLeft < 0 {
			numServersLeft, err = strconv.Atoi(line)
			checkError(err)
			continue
		}
		// Otherwise, always expect 2 tokens
		//判断每一行是否是两个参数
		parts := strings.Fields(line)
		if len(parts) != 2 {
			log.Fatal("Expected 2 tokens in line: ", line)
		}
		//记录每一个server以及他的token数量
		if numServersLeft > 0 {
			// This is a server
			serverId := parts[0]
			numTokens, err := strconv.Atoi(parts[1])
			checkError(err)
			sim.AddServer(serverId, numTokens)
			numServersLeft--
		} else {
			// This is a link
			//记录server之间的连接信息
			src := parts[0]
			dest := parts[1]
			sim.AddForwardLink(src, dest)
		}
	}
}

// Read the events from a ".events" file and inject the events into the simulator.
// The expected format of the file is as follows:
//   - "tick N" indicates N time steps has elapsed (default N = 1)
//   - "send N1 N2 1" indicates that N1 sends 1 token to N2
//   - "snapshot N2" indicates the beginning of the snapshot process, starting on N2
//
// Note that concurrent events are indicated by the lack of ticks between the events.
// This function waits until all the snapshot processes have terminated before returning
// the snapshots collected.
func injectEvents(fileName string, sim *Simulator) []*SnapshotState {
	b, err := ioutil.ReadFile(path.Join(testDir, fileName))
	checkError(err)

	snapshots := make([]*SnapshotState, 0)
	//声明一个通道用来放置所有收集的快照，设置通道的buffer为100
	getSnapshots := make(chan *SnapshotState, 100)
	numSnapshots := 0

	lines := strings.FieldsFunc(string(b), func(r rune) bool { return r == '\n' })
	//去除\r,否则会解析文件错误
	for i, _ := range lines {
		lines[i] = strings.Trim(lines[i], "\r")
		//fmt.Println(lines[i])
	}
	for _, line := range lines {
		// Ignore comments
		if strings.HasPrefix("#", line) {
			continue
		}
		parts := strings.Fields(line)
		switch parts[0] {
		case "send":
			src := parts[1]
			dest := parts[2]
			tokens, err := strconv.Atoi(parts[3])
			checkError(err)
			sim.InjectEvent(PassTokenEvent{src, dest, tokens})
		case "snapshot":
			numSnapshots++
			serverId := parts[1]
			snapshotId := sim.nextSnapshotId
			sim.InjectEvent(SnapshotEvent{serverId})
			//为每次快照都创建一个线程，并且把收集到的这个快照的信息写入通道
			//启动一个线程，开始收集快照，写入到一个SnapshotState指针类型的通道中，参数是快照id
			go func(id int) {
				getSnapshots <- sim.CollectSnapshot(id)
			}(snapshotId)
		case "tick":
			numTicks := 1
			if len(parts) > 1 {
				numTicks, err = strconv.Atoi(parts[1])
				checkError(err)
			}
			//向前推进tick步
			for i := 0; i < numTicks; i++ {
				sim.Tick()
			}
		default:
			log.Fatal("Unknown event command: ", parts[0])
		}
	}
	// Keep ticking until snapshots complete
	//如果通道中有已经完成 的快照，就读取通道中的数据
	for numSnapshots > 0 {
		//select是go语言专有的，类似Switch，但是只能用于通道的读取和写入
		select {
		case snap := <-getSnapshots:
			snapshots = append(snapshots, snap)
			numSnapshots--
		default:
			//当从通道中读取阻塞（通道为空）的时候执行
			sim.Tick()
		}
	}

	// Keep ticking until we're sure that the last message has been delivered
	for i := 0; i < maxDelay+1; i++ {
		sim.Tick()
	}
	//返回实际的快照，用来和快照文件进行对比
	return snapshots
}

// Read the state of snapshot from a ".snap" file.
// The expected format of the file is as follows:
//   - The first line contains the snapshot ID (e.g. "0")
//   - The next N lines contains the server ID and the number of tokens on that server,
//     in the form "[serverId] [numTokens]" (e.g. "N1 0"), one line per server
//   - The rest of the lines represent messages exchanged between the servers,
//     in the form "[src] [dest] [message]" (e.g. "N1 N2 token(1)")
func readSnapshot(fileName string) *SnapshotState {
	b, err := ioutil.ReadFile(path.Join(testDir, fileName))
	checkError(err)
	snapshot := SnapshotState{0, make(map[string]int), make([]*SnapshotMessage, 0)}
	lines := strings.FieldsFunc(string(b), func(r rune) bool { return r == '\n' })
	//note:需要去除读取数据中的\r，否则会报错
	for i, _ := range lines {
		lines[i] = strings.Trim(lines[i], "\r")
		//fmt.Println(lines[i])
	}
	for _, line := range lines {
		// Ignore comments
		if strings.HasPrefix(line, "#") {
			continue
		}
		parts := strings.Fields(line)
		if len(parts) == 1 {
			// Snapshot ID
			snapshot.id, err = strconv.Atoi(line)
			checkError(err)
		} else if len(parts) == 2 {
			// Server and its tokens
			serverId := parts[0]
			numTokens, err := strconv.Atoi(parts[1])
			checkError(err)
			snapshot.tokens[serverId] = numTokens
		} else if len(parts) == 3 {
			// Src, dest and message
			src := parts[0]
			dest := parts[1]
			messageString := parts[2]
			var message interface{}
			if strings.Contains(messageString, "token") {
				pattern := regexp.MustCompile(`[0-9]+`)
				matches := pattern.FindStringSubmatch(messageString)
				if len(matches) != 1 {
					log.Fatal("Unable to parse token message: ", messageString)
				}
				numTokens, err := strconv.Atoi(matches[0])
				checkError(err)
				message = TokenMessage{numTokens}
			} else {
				log.Fatal("Unknown message: ", messageString)
			}
			snapshot.messages =
				append(snapshot.messages, &SnapshotMessage{src, dest, message})
		}
	}
	return &snapshot
}

// Helper function to pretty print the tokens in the given snapshot state
func tokensString(tokens map[string]int, prefix string) string {
	str := make([]string, 0)
	for _, serverId := range getSortedKeys(tokens) {
		numTokens := tokens[serverId]
		maybeS := "s"
		if numTokens == 1 {
			maybeS = ""
		}
		str = append(str, fmt.Sprintf(
			"%v%v: %v token%v", prefix, serverId, numTokens, maybeS))
	}
	return strings.Join(str, "\n")
}

// Helper function to pretty print the messages in the given snapshot state
func messagesString(messages []*SnapshotMessage, prefix string) string {
	str := make([]string, 0)
	for _, msg := range messages {
		str = append(str, fmt.Sprintf(
			"%v%v -> %v: %v", prefix, msg.src, msg.dest, msg.message))
	}
	return strings.Join(str, "\n")
}

// 判断两个快照状态是否相等，包括，每个server 的余额以及发送的信息
// Assert that the two snapshot states are equal.
// If they are not equal, throw an error with a helpful message.
func assertEqual(expected, actual *SnapshotState) {
	if expected.id != actual.id {
		log.Fatalf("Snapshot IDs do not match: %v != %v\n", expected.id, actual.id)
	}
	//判断server的数量是否相等
	if len(expected.tokens) != len(actual.tokens) {
		log.Fatalf(
			"Snapshot %v: Number of tokens do not match."+
				"\nExpected:\n%v\nActual:\n%v\n",
			expected.id,
			tokensString(expected.tokens, "\t"),
			tokensString(actual.tokens, "\t"))
	}
	//判断信息的数量是否相等
	if len(expected.messages) != len(actual.messages) {
		log.Fatalf(
			"Snapshot %v: Number of messages do not match."+
				"\nExpected:\n%v\nActual:\n%v\n",
			expected.id,
			messagesString(expected.messages, "\t"),
			messagesString(actual.messages, "\t"))
	}
	//判断每个server 的余额是否相等
	for id, tok := range expected.tokens {
		if actual.tokens[id] != tok {
			log.Fatalf(
				"Snapshot %v: Tokens on %v do not match."+
					"\nExpected:\n%v\nActual:\n%v\n",
				expected.id,
				id,
				tokensString(expected.tokens, "\t"),
				tokensString(actual.tokens, "\t"))
		}
	}
	//判断信息是否相等
	// Ensure message order is preserved per destination
	// Note that we don't require ordering of messages across all servers to match
	expectedMessages := make(map[string][]*SnapshotMessage)
	actualMessages := make(map[string][]*SnapshotMessage)
	for i := 0; i < len(expected.messages); i++ {
		em := expected.messages[i]
		am := actual.messages[i]
		_, ok1 := expectedMessages[em.dest]
		_, ok2 := actualMessages[am.dest]
		if !ok1 {
			expectedMessages[em.dest] = make([]*SnapshotMessage, 0)
		}
		if !ok2 {
			actualMessages[am.dest] = make([]*SnapshotMessage, 0)
		}
		expectedMessages[em.dest] = append(expectedMessages[em.dest], em)
		actualMessages[am.dest] = append(actualMessages[am.dest], am)
	}
	// Test message order per destination
	for dest := range expectedMessages {
		ems := expectedMessages[dest]
		ams := actualMessages[dest]
		if !reflect.DeepEqual(ems, ams) {
			log.Fatalf(
				"Snapshot %v: Messages received at %v do not match."+
					"\nExpected:\n%v\nActual:\n%v\n",
				expected.id,
				dest,
				messagesString(ems, "\t"),
				messagesString(ams, "\t"))
		}
	}
}

// Helper function to sort the snapshot states by ID.
// 按照id对快照进行排序
func sortSnapshots(snaps []*SnapshotState) {
	sort.Slice(snaps, func(i, j int) bool {
		s1 := snaps[i]
		s2 := snaps[j]
		return s2.id > s1.id
	})
}

// Verify that the total number of tokens recorded in the snapshot preserves
// the number of tokens in the system
// 保持一致性，系统中的token总数保持不变
func checkTokens(sim *Simulator, snapshots []*SnapshotState) {
	expectedTokens := 0
	for _, server := range sim.servers {
		expectedTokens += server.Tokens
	}
	for _, snap := range snapshots {
		snapTokens := 0
		// Add tokens recorded on servers
		for _, tok := range snap.tokens {
			snapTokens += tok
		}
		// Add tokens from messages in-flight
		for _, message := range snap.messages {
			switch msg := message.message.(type) {
			case TokenMessage:
				snapTokens += msg.numTokens
			}
		}
		if expectedTokens != snapTokens {
			log.Fatalf("Snapshot %v: simulator has %v tokens, snapshot has %v:\n%v\n%v",
				snap.id,
				expectedTokens,
				snapTokens,
				tokensString(snap.tokens, "\t"),
				messagesString(snap.messages, "\t"))
		}
	}
}
