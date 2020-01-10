package main

import (
	"fmt"
	"os"
)

func lengthOfLongestSubstring(s string) int {
	if len(s) <= 1 {
		return len(s)
	}
	window := make(map[byte]int)
	maxlen := 0
	curlen := 0
	begin := 0
	end := 0
	for i := 0; i < len(s); i++ {
		c := s[i]
		window[c]++
		end++
		curlen++
		if window[c] > 1 {
			for window[c] > 1 && begin < end {
				window[s[begin]]--
				begin++
				curlen--
			}
		}
		if maxlen < curlen {
			maxlen = curlen
		}
	}
	return maxlen
}

func main() {
	fmt.Println(lengthOfLongestSubstring(os.Args[1]))
}
