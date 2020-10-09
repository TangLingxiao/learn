package main
import(
	"fmt"
)

 type TreeNode struct {
    Val int
    Left *TreeNode
    Right *TreeNode
 }

func inorderTraversal(root *TreeNode) []int {
	var res []int
	if root != nil{
		res = append(res, inorderTraversal(root.Left)...)
		res = append(res, root.Val)
		res = append(res, inorderTraversal(root.Right)...)
	}
	return res
}
type Stack []*TreeNode

func (s *Stack) Push(node *TreeNode) {
	*s = append(*s, node)
}

func (s *Stack) Pop() *TreeNode {
	n := []*TreeNode(*s)[len(*s)-1]
	*s = []*TreeNode(*s)[:len(*s)-1]
	return n
}

func inorderTraversal2(root *TreeNode) []int {
	var res []int
	var st Stack
	for len(st) != 0 || root != nil{
		for(root != nil){
			st.Push(root)
			root = root.Left
		}
		node:= st.Pop()
		res = append(res, node.Val)
		root = node.Right
	}
	return res
}

func main(){
	var root *TreeNode
	root = new(TreeNode)
	root.Val = 1
	root.Left = nil
	root.Right = new(TreeNode)
	root.Right.Val = 2
	root.Right.Left = new(TreeNode)
	root.Right.Left.Val = 3

	fmt.Printf("%v",inorderTraversal2(root))
}