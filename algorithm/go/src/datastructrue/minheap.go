package datastructrue

import (
	"errors"
	"fmt"
)

type MinHeap struct {
	data []int64
}

func (heap *MinHeap) Insert(data int64) {
	heap.data = append(heap.data, data)
	for i := (len(heap.data) - 1) / 2; i >= 0; i-- {
		heap.arrange(i)
	}
}

func (heap *MinHeap) Pop() (int64, error) {
	if len(heap.data) <= 0 {
		return -1, errors.New("empty heap")
	}
	data := heap.data[0]
	heap.data[0], heap.data[len(heap.data)-1] = heap.data[len(heap.data)-1], heap.data[0]
	heap.data = heap.data[0 : len(heap.data)-1]
	heap.arrange(0)
	return data, nil
}

func (heap *MinHeap) arrange(i int) {
	len := len(heap.data)
	if i >= len {
		return
	}
	min := i
	left := 2*i + 1
	right := 2*i + 2

	if left < len && heap.data[left] < heap.data[min] {
		min = left
	}
	if right < len && heap.data[right] < heap.data[min] {
		min = right
	}

	heap.data[i], heap.data[min] = heap.data[min], heap.data[i]
	if min != i {
		heap.arrange(min)
	}
}

func (heap *MinHeap) Dump() {
	fmt.Printf("%v\n", heap.data)
}
