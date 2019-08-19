package datastructrue

import (
	"errors"
	"fmt"
)

type listNode struct {
	data interface{}
	next *listNode
}

type List struct {
	head *listNode
}

func (list *List) PushFront(data interface{}) {
	node := new(listNode)
	node.data = data
	if nil == list.head {
		node.next = nil
	} else {
		node.next = list.head
	}
	list.head = node
}

func (list *List) PopFront() (interface{}, error) {
	if list.head == nil {
		return nil, errors.New("empty list")
	}
	data := list.head.data
	list.head = list.head.next
	return data, nil
}

func (list *List) Dump() {
	for head := list.head; nil != head; head = head.next {
		fmt.Printf("%v ", head.data)
	}
}
