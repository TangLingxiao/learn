#coding: utf-8

import os

class Solution:
    def longestPalindrome(self, s: str) -> str:
        start, end, length = 0,0,0
        for i in range(0, len(s)):
            left, right = i,i
            while right < len(s)-1 and s[right]==s[right+1]:
                right=right+1
            while left > 0 and right < len(s)-1 and s[left-1]==s[right+1]:
                left,right = left-1,right+1
            if right-left+1>length:
                start,end,length = left,right,right-left+1
        return s[start:end+1]


        
if __name__ == "__main__":
    s = Solution()
    print(s.longestPalindrome(os.sys.argv[1]))
    