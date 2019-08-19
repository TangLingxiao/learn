package main

import (
	"datastructrue"
	"fmt"
	"mysort"
)

func testStack() {
	var stack datastructrue.Stack
	stack.Push(interface{}(1))
	stack.Push(interface{}("str"))
	stack.Push(interface{}(func() bool { return true }))
	stack.Dump()
	stack.Pop()
	stack.Pop()
	stack.Pop()
	stack.Dump()
}

func testLinkedList() {
	var list datastructrue.List
	list.PushFront(1)
	list.PushFront("str")
	list.PushFront(true)
	list.Dump()
	list.PopFront()
	list.PopFront()
	list.PopFront()
}

func testMinheap() {
	var heap datastructrue.MinHeap
	heap.Insert(5)
	heap.Insert(4)
	heap.Insert(9)
	heap.Insert(2)
	heap.Insert(1)
	heap.Insert(3)
	heap.Dump()
	heap.Pop()
	heap.Dump()
	heap.Pop()
	heap.Dump()
	heap.Pop()
	heap.Dump()
	heap.Pop()
	heap.Dump()
	heap.Pop()
	heap.Dump()
	heap.Pop()
	heap.Dump()
}

func testSort(data []int64, fn func(data []int64)) {
	fn(data)
	fmt.Printf("%v\n", data)
}
func main() {
	arr := []int64{9, 5, 6, 4, 2, 1, 7}
	testSort(arr, mysort.HeapSort)
}
