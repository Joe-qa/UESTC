package chandy_lamport

import (
	"log"
)

type Snapshot struct {
	id       int
	tokens   int // key = server ID, value = num tokens
	messages []*SnapshotMessage
}

// Server The main participant of the distributed snapshot protocol.
// Servers exchange token messages and marker messages among each other.
// Token messages represent the transfer of tokens from one server to another.
// Marker messages represent the progress of the snapshot process. The bulk of
// the distributed protocol is implemented in `HandlePacket` and `StartSnapshot`.
type Server struct {
	Id            string
	Tokens        int
	sim           *Simulator
	outboundLinks map[string]*Link // key = link.dest
	inboundLinks  map[string]*Link // key = link.src
	// TODO: ADD MORE FIELDS HERE
	snapshotState    *SyncMap                //map[int]*Snapshot
	receivedMarker   map[int]map[string]bool // key = src  记录输入通道是否收到marker，只有所有的输入通道都收到了marker，这个server此次快照记录完毕
	completeSnapshot map[int]map[string]bool // key = src
}

// Link A unidirectional communication channel between two servers
// Each link contains an event queue (as opposed to a packet queue)
type Link struct {
	src    string
	dest   string
	events *Queue
}

func NewServer(id string, tokens int, sim *Simulator) *Server {
	return &Server{
		id,
		tokens,
		sim,
		make(map[string]*Link),
		make(map[string]*Link),
		NewSyncMap(),
		make(map[int]map[string]bool),
		make(map[int]map[string]bool),
	}
}

// AddOutboundLink 添加一条单向边在两个服务器之间
// Add a unidirectional link to the destination server
func (server *Server) AddOutboundLink(dest *Server) {
	if server == dest {
		return
	}
	l := Link{server.Id, dest.Id, NewQueue()}
	server.outboundLinks[dest.Id] = &l
	dest.inboundLinks[server.Id] = &l
}

// SendToNeighbors 给server的所有邻居节点发送marker
// Send a message on all  the server's outbound links
func (server *Server) SendToNeighbors(message interface{}) {
	for _, serverId := range getSortedKeys(server.outboundLinks) {
		link := server.outboundLinks[serverId]
		server.sim.logger.RecordEvent(
			server,
			SentMessageEvent{server.Id, link.dest, message})
		//fmt.Println(server.Id, link.dest, message)
		link.events.Push(SendMessageEvent{
			server.Id,
			link.dest,
			message,
			server.sim.GetReceiveTime()})
	}
}

// SendTokens 发送以太币
// Send a number of tokens to a neighbor attached to this server
func (server *Server) SendTokens(numTokens int, dest string) {
	if server.Tokens < numTokens {
		log.Fatalf("Server %v attempted to send %v tokens when it only has %v\n",
			server.Id, numTokens, server.Tokens)
	}
	message := TokenMessage{numTokens}
	server.sim.logger.RecordEvent(server, SentMessageEvent{server.Id, dest, message})
	// Update local state before sending the tokens
	server.Tokens -= numTokens
	link, ok := server.outboundLinks[dest]
	if !ok {
		log.Fatalf("Unknown dest ID %v from server %v\n", dest, server.Id)
	}
	//fmt.Println("sendTokens", server.Id, dest, message)
	//把发送token的事件push到两个服务器的连接通道上
	link.events.Push(SendMessageEvent{
		server.Id,
		dest,
		message,
		server.sim.GetReceiveTime()})
}

// HandlePacket Callback for when a message is received on this server.
// When the snapshot algorithm completes on this server, this function
// should notify the simulator by calling `sim.NotifySnapshotComplete`.
func (server *Server) HandlePacket(src string, message interface{}) {
	// TODO: IMPLEMENT ME
	switch message := message.(type) {
	case MarkerMessage:
		if _, ok := server.snapshotState.LoadOrStore(message.snapshotId, &Snapshot{message.snapshotId, server.Tokens, make([]*SnapshotMessage, 0)}); !ok {
			//如果ok为false，表明在这个server中记录快照状态的map中并没有此快照id的记录
			//也就是该server第一次收到marker，表示刚知道要进行快照
			server.completeSnapshot[message.snapshotId] = make(map[string]bool)
			server.receivedMarker[message.snapshotId] = make(map[string]bool)
			//给所有邻居发送marker，告诉他们记录输入通道的信息
			server.SendToNeighbors(message)
			//收到src发来的marker，那么表示src->server的通道已经记录完毕
			//此通道已经收到marker
			//此server的其他通道还没有收到marker
			for _, inserver := range getSortedKeys(server.inboundLinks) {
				if src == inserver {
					server.receivedMarker[message.snapshotId][inserver] = true
				} else {
					server.receivedMarker[message.snapshotId][inserver] = false
				}
			}
		} else {
			//已经知道开始了快照，收到了src 的marker，表示src->server这条通道记录完毕
			server.receivedMarker[message.snapshotId][src] = true
		}
		//当前server收到了src 发送的一个marker，对于server来说这条通道信息已经记录完毕
		server.completeSnapshot[message.snapshotId][src] = true
		//判断当前server的输入通道是否已经收集到了所有的marker，如果是，那么这个server已经完成了这次快照记录
		//就通知simulator对于这个server，快照记录已经完成
		if len(server.completeSnapshot[message.snapshotId]) == len(server.inboundLinks) {
			server.sim.NotifySnapshotComplete(server.Id, message.snapshotId)
		}
	case TokenMessage:
		server.Tokens += message.numTokens
		server.snapshotState.Range(func(k, v interface{}) bool {
			if snapshotId, ok1 := k.(int); ok1 {
				//如果还没有收到此输入通道的marker，那么就需要记录信息
				if receivedMarker, ok2 := server.receivedMarker[snapshotId][src]; ok2 && !receivedMarker {
					if snapshot, ok3 := v.(*Snapshot); ok3 {
						snapshot.messages = append(snapshot.messages, &SnapshotMessage{src, server.Id, message})
					}
				}
			}
			return true
		})
	default:
		log.Fatal("error message: ", message)
	}
}

// StartSnapshot Start the chandy-lamport snapshot algorithm on this server.
// This should be called only once per server.
func (server *Server) StartSnapshot(snapshotId int) {
	// TODO: IMPLEMENT ME
	server.receivedMarker[snapshotId] = make(map[string]bool)
	server.completeSnapshot[snapshotId] = make(map[string]bool)
	//在存储快照记录的map中创建一条记录，用来记录此次快照的状态信息
	server.snapshotState.LoadOrStore(snapshotId, &Snapshot{snapshotId, server.Tokens, make([]*SnapshotMessage, 0)})
	//表示对于发起快照的server，他的输入通道是否收到marker，没收到的话就需要记录通道中的信息
	for _, src := range getSortedKeys(server.inboundLinks) {
		server.receivedMarker[snapshotId][src] = false
	}
	//告诉所有其他server开始快照，也就是给他们都发送marker
	server.SendToNeighbors(MarkerMessage{snapshotId})
}
