package chandy_lamport

import (
	"fmt"
	"log"
)

// =================================
//
//	Event logger, internal use only
//
// =================================
// 定义日志数据结构，记录事件信息
type Logger struct {
	// index = time step
	// value = events that occurred at that time step
	events [][]LogEvent
}

type LogEvent struct {
	serverId string
	// Number of tokens before execution of event
	serverTokens int
	event        interface{}
}

func (event LogEvent) String() string {
	prependWithTokens := false
	switch evt := event.event.(type) {
	case SentMessageEvent:
		switch evt.message.(type) {
		case TokenMessage:
			prependWithTokens = true
		}
	case ReceivedMessageEvent:
		switch evt.message.(type) {
		case TokenMessage:
			prependWithTokens = true
		}
	case StartSnapshot:
		prependWithTokens = true
	case EndSnapshot:
	default:
		log.Fatal("Attempted to log unrecognized event: ", event.event)
	}
	if prependWithTokens {
		return fmt.Sprintf("%v has %v token(s)\n\t%v",
			event.serverId,
			event.serverTokens,
			event.event)
	} else {
		return fmt.Sprintf("%v", event.event)
	}
}

// 创建新的logger，初始值为0
func NewLogger() *Logger {
	return &Logger{make([][]LogEvent, 0)}
}

// 打印事件信息，某个时刻发生的所有事件，debug的时候打印出每个时间发生的信息
func (log *Logger) PrettyPrint() {
	for epoch, events := range log.events {
		if len(events) != 0 {
			fmt.Printf("Time %v:\n", epoch)
		}
		for _, event := range events {
			fmt.Printf("\t%v\n", event)
		}
	}
}

// 每一个新的时间，初始化一个logevent，并且添加到events中
func (log *Logger) NewEpoch() {
	log.events = append(log.events, make([]LogEvent, 0))
}

// 记录最新事件信息，添加到logger中
func (logger *Logger) RecordEvent(server *Server, event interface{}) {
	mostRecent := len(logger.events) - 1
	events := logger.events[mostRecent]
	events = append(events, LogEvent{server.Id, server.Tokens, event})
	logger.events[mostRecent] = events
}
