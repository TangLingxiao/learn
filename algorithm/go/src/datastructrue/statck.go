package datastructrue

import (
	"errors"
	"fmt"
)

//stack
type Stack struct {
	data []interface{}
}

//stack push
func (stack *Stack) Push(data interface{}) {
	stack.data = append(stack.data, data)
}

//stack pop
func (stack *Stack) Pop() (interface{}, error) {
	if len(stack.data) <= 0 {
		return nil, errors.New("empty stack")
	}
	data := stack.data[len(stack.data)-1]
	stack.data = stack.data[0 : len(stack.data)-1]
	return data, nil
}

//dump
func (stack *Stack) Dump() {
	fmt.Printf("%v\n", stack.data)
}
