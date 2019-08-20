package mysort

import (
	"datastructrue"
	"fmt"
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

func InsertSort(data []int64) {
	for i := 1; i < len(data); i++ {
		a := data[i]
		j := i - 1
		for ; j >= 0; j-- {
			if a >= data[j] {
				break
			}
			data[j+1] = data[j]
		}
		data[j+1] = a
	}
}

func SelectSort(data []int64) {
	for i := 0; i < len(data); i++ {
		min := int64(^uint64(0) >> 1)
		mi := i
		for j := i; j < len(data); j++ {
			if data[j] < min {
				min = data[j]
				mi = j
			}
		}
		data[i], data[mi] = data[mi], data[i]
	}
}

func partitionAndmerge(data []int64, i, j int) {
	if i == j {
		return
	}

	mid := (i + j) / 2
	partitionAndmerge(data, i, mid)
	partitionAndmerge(data, mid+1, j)

	//merge
	left, right := []int64{}, []int64{}
	for _, one := range data[i : mid+1] {
		left = append(left, one)
	}
	for _, one := range data[mid+1 : j+1] {
		right = append(right, one)
	}
	ret := data[i : j+1]
	li, ri := 0, 0
	for k := 0; k < j-i+1; k++ {
		if li < len(left) && ri < len(right) {
			if left[li] < right[ri] {
				ret[k] = left[li]
				li++
			} else {
				ret[k] = right[ri]
				ri++
			}
		} else if li < len(left) {
			ret[k] = left[li]
			li++
		} else if ri < len(right) {
			ret[k] = right[ri]
			ri++
		}
	}
	fmt.Printf("left:%v,right:%v,ret:%v\n", left, right, ret)
}
func MergeSort(data []int64) {
	partitionAndmerge(data, 0, len(data)-1)
}

func partition(arr []int64, i, j int) {
	if i >= j {
		return
	}
	key := arr[j]
	ri, li := i, i-1
	for ri < j {
		if arr[ri] < key {
			li++
			arr[ri], arr[li] = arr[li], arr[ri]
		}
		ri++
	}
	arr[li+1], arr[j] = arr[j], arr[li+1]
	partition(arr, i, li)
	partition(arr, li+2, j)
}

func partition2(arr []int64, i, j int) {
	if i >= j {
		return
	}
	key := arr[j]
	index := j
	ri, li := j-1, i
	for li < ri {
		if arr[li] > key {
			arr[index] = arr[li]
			index = li
			for ri > li {
				if arr[ri] < key {
					arr[index] = arr[ri]
					index = ri
					break
				}
				ri--
			}
		}
		li++
	}
	arr[index] = key
	partition2(arr, i, index-1)
	partition2(arr, index+1, j)
}
func QuickSort(arr []int64) {
	partition2(arr, 0, len(arr)-1)
}
