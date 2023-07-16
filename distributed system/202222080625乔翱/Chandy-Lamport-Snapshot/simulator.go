package chandy_lamport

import (
	"log"
	"math/rand"
)

// Max random delay added to packet delivery
const maxDelay = 5

// Simulator 模拟器 模拟整个分布式快照系统
// Simulator is the entry point to the distributed snapshot application.
//
// It is a discrete time simulator, i.e. events that happen at time t + 1 come
// strictly after events that happen at time t. At each time step, the simulator
// examines messages queued up across all the links in the system and decides
// which ones to deliver to the destination.
//
// The simulator is responsible for starting the snapshot process, inducing servers
// to pass tokens to each other, and collecting the snapshot state after the process
// has terminated.
type Simulator struct {
	time           int
	nextSnapshotId int
	servers        map[string]*Server // key = server ID
	logger         *Logger
	// TODO: ADD MORE FIELDS HERE
	//采用通道，在Goroutine中间传递数据
	//记录某次快照中的server是否完成了记录
	completeSnapshot map[int]map[string]chan bool //completed[snapshotID][serverID]
}

func NewSimulator() *Simulator {
	return &Simulator{
		0,
		0,
		make(map[string]*Server),
		NewLogger(),
		make(map[int]map[string]chan bool),
	}
}

// GetReceiveTime Return the reception time of a message after adding a random delay.
// 默认设置一个消息（tokens的传递/marker的发送）是有一个随机的延迟（实际上是指定了一个固定大小的延迟，因为随机数种子是确定的）
// Note: since we only deliver one message to a given server at each time step,
// the message may be received *after* the time step returned in this function.
func (sim *Simulator) GetReceiveTime() int {
	return sim.time + 1 + rand.Intn(5)
}

// AddServer Add a server to this simulator with the specified number of starting tokens
func (sim *Simulator) AddServer(id string, tokens int) {
	server := NewServer(id, tokens, sim)
	sim.servers[id] = server
}

// AddForwardLink 整个分布式系统是一个有向图
// AddForwardLink 在两台服务器间添加单向链接
// AddForwardLink Add a unidirectional link between two servers
func (sim *Simulator) AddForwardLink(src string, dest string) {
	server1, ok1 := sim.servers[src]
	server2, ok2 := sim.servers[dest]
	if !ok1 {
		log.Fatalf("Server %v does not exist\n", src)
	}
	if !ok2 {
		log.Fatalf("Server %v does not exist\n", dest)
	}
	server1.AddOutboundLink(server2)
}

// InjectEvent 在系统中注入事件，每个事件有一个到达时间
// InjectEvent Run an event in the system
func (sim *Simulator) InjectEvent(event interface{}) {
	switch event := event.(type) {
	case PassTokenEvent:
		src := sim.servers[event.src]
		src.SendTokens(event.tokens, event.dest)
	case SnapshotEvent:
		sim.StartSnapshot(event.serverId)
	default:
		log.Fatal("Error unknown event: ", event)
	}
}

// Tick Advance the simulator time forward by one step, handling all send message events
// that expire at the new time step, if any.
func (sim *Simulator) Tick() {
	sim.time++
	sim.logger.NewEpoch()
	// Note: to ensure deterministic ordering of packet delivery across the servers,
	// we must also iterate through the servers and the links in a deterministic way
	for _, serverId := range getSortedKeys(sim.servers) {
		server := sim.servers[serverId]
		for _, dest := range getSortedKeys(server.outboundLinks) {
			link := server.outboundLinks[dest]
			// Deliver at most one packet per server at each time step to
			// establish total ordering of packet delivery to each server
			if !link.events.Empty() {
				e := link.events.Peek().(SendMessageEvent)
				if e.receiveTime <= sim.time {
					link.events.Pop()
					sim.logger.RecordEvent(
						sim.servers[e.dest],
						ReceivedMessageEvent{e.src, e.dest, e.message})
					sim.servers[e.dest].HandlePacket(e.src, e.message)
					break
				}
			}
		}
	}
}

// StartSnapshot 开启一个新的快照
// StartSnapshot Start a new snapshot process at the specified server
func (sim *Simulator) StartSnapshot(serverId string) {
	snapshotId := sim.nextSnapshotId
	sim.nextSnapshotId++
	server, ok := sim.servers[serverId]
	if !ok {
		log.Fatalf("Server %v does not exist\n", serverId)
	}
	sim.logger.RecordEvent(sim.servers[serverId], StartSnapshot{serverId, snapshotId})
	// TODO: IMPLEMENT ME
	//为每个server建立一个通道，判断是否完成snap，每个server收到他的所有输入通道上的标记后终止
	//初始化completed
	sim.completeSnapshot[snapshotId] = make(map[string]chan bool)
	for ID := range sim.servers {
		sim.completeSnapshot[snapshotId][ID] = make(chan bool)
	}
	server.StartSnapshot(snapshotId)
}

// NotifySnapshotComplete Callback for servers to notify the simulator that the snapshot process has
// completed on a particular server
func (sim *Simulator) NotifySnapshotComplete(serverId string, snapshotId int) {
	sim.logger.RecordEvent(sim.servers[serverId], EndSnapshot{serverId, snapshotId})
	// TODO: IMPLEMENT ME
	//对输入的服务器id进行检测，检测是否有该id的server
	_, ok := sim.servers[serverId]
	if !ok {
		log.Fatalf("Server %v does not exist\n", serverId)
		return
	}
	//向通道写入true，表明对于当前的快照，这个服务器已经完成了记录
	sim.completeSnapshot[snapshotId][serverId] <- true
}

// CollectSnapshot 收集所有服务器上的快照，只有所有的服务器上的快照都完成了才能执行完毕，否则会一直阻塞
// CollectSnapshot Collect and merge snapshot state from all the servers.
// This function blocks until the snapshot process has completed on all servers.
func (sim *Simulator) CollectSnapshot(snapshotId int) *SnapshotState {
	// TODO: IMPLEMENT ME
	//为每一个服务器创建一个通道，用来记录是否这个服务器完成了快照记录
	completed := make(chan bool, len(sim.completeSnapshot[snapshotId]))
	for _, snapState := range sim.completeSnapshot[snapshotId] {
		//读取每个通道，如果当前通道没有数据，就阻塞（通道的特点）
		//创建一个线程往done里面写数据
		go func(completed chan bool, snapState chan bool) {
			//通道之间不能直接互相写
			<-snapState
			completed <- true
		}(completed, snapState)
	}
	//读是否所有server都完成了快照，如果有没有完成的就阻塞（通道的特点）
	//完成的话done这个通道里面会有servers的数量个true，之前声明的done的buffer大小就是server的数量
	for range sim.completeSnapshot[snapshotId] {
		<-completed
	}
	//每个服务器都完成了snap，记录快照信息，包括每个server拥有的token以及传递的信息
	tokens := make(map[string]int)
	messages := make([]*SnapshotMessage, 0)
	for _, serverId := range getSortedKeys(sim.servers) {
		server := sim.servers[serverId]
		server.snapshotState.Range(func(k, v interface{}) bool {
			if ID, ok1 := k.(int); ok1 && ID == snapshotId {
				if snapshot, ok2 := v.(*Snapshot); ok2 {
					tokens[serverId] = snapshot.tokens
					for _, message := range snapshot.messages {
						messages = append(messages, message)
					}
				}
			}
			return true
		})
	}
	snapshotState := SnapshotState{snapshotId, tokens, messages}
	return &snapshotState
}
