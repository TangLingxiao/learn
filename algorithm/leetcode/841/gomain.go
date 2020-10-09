package main

import (
	"fmt"
)
func canVisitAllRooms(rooms [][]int) bool {
	roomMap := make(map[int]bool)
	roomMap[0] = true
	visit(rooms, rooms[0], roomMap)
	for i := range rooms {
		if v, ok := roomMap[i]; !ok || !v {
			return false
		}
	}
	return true

}

func visit(rooms [][]int, room []int, roomMap map[int]bool) {
	for _, v := range room {
		//没访问过
		if !roomMap[v] {
			roomMap[v] = true
			visit(rooms, rooms[v], roomMap)
		}
	}
}

func main() {
	a := [][]int{
		{1},
		{2},
		{3},
		{},
	}
	fmt.Println(canVisitAllRooms(a))
}