#coding : utf-8
import os

class Solution:
    def lengthOfLongestSubstring(self, s: str) -> int:
        nMaxLen, curlen, start = 0,0,0
        m = {}
        for i in range(0,len(s)):
            if s[i] in m:
                m[s[i]] = m[s[i]]+1
            else:
                m[s[i]] = 1
            curlen= curlen+1
            j = start
            while m[s[i]]>1 and j < i:
                m[s[j]] = m[s[j]]-1
                curlen = curlen - 1
                j = j+1
            start = j
            if curlen >nMaxLen:
                nMaxLen = curlen
        return nMaxLen

if __name__ == "__main__":
    s = Solution()
    print(s.lengthOfLongestSubstring(os.sys.argv[1]))