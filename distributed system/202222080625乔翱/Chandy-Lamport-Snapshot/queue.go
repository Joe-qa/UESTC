package chandy_lamport

import "container/list"

// go语言中没有队列，利用list模拟队列，包括清空队列，在队列头部添加元素移除队尾元素，获取队尾元素的值
// Define a queue -- simple implementation over List
type Queue struct {
	elements *list.List
}

func NewQueue() *Queue {
	return &Queue{list.New()}
}

func (q *Queue) Empty() bool {
	return (q.elements.Len() == 0)
}

func (q *Queue) Push(v interface{}) {
	q.elements.PushFront(v)
}

func (q *Queue) Pop() interface{} {
	return q.elements.Remove(q.elements.Back())
}

func (q *Queue) Peek() interface{} {
	return q.elements.Back().Value
}
