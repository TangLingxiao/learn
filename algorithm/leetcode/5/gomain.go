package main

import (
	"fmt"
	"os"
)

func longestPalindrome(s string) string {
	if len(s) < 2 {
		return s
	}
	start, end, length := 0, 0, 0
	for i, _ := range s {
		left, right := i, i
		for right < len(s)-1 && s[right] == s[right+1] {
			right++
		}
		for left > 0 && right < len(s)-1 && s[left-1] == s[right+1] {
			left--
			right++
		}
		if right-left > length {
			start, end, length = left, right, right-left
		}
	}
	return s[start : end+1]
}
func main() {
	fmt.Println(longestPalindrome(os.Args[1]))
}
