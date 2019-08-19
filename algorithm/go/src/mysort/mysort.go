package mysort

import (
	"datastructrue"
)

func PopSort(p []int64) {
	for i := 0; i < len(p); i++ {
		for j := 0; j < i; j++ {
			if p[i] < p[j] {
				p[i], p[j] = p[j], p[i]
			}
		}
	}
}

func HeapSort(data []int64) {
	var heap datastructrue.MinHeap
	for _, one := range data {
		heap.Insert(one)
	}
	data = data[:0]
	for min, ret := heap.Pop(); ret == nil; min, ret = heap.Pop() {
		data = append(data, min)
	}

}
