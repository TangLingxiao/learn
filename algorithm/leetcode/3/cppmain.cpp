#include <vector>
#include <string>
#include <iostream>
#include <map>
using namespace std;

class Solution
{
public:
    int lengthOfLongestSubstring(string s)
    {
        std::map<char, int> m;
        int nMaxLen = 0;
        int nLen = 0;
        size_t start = 0;
        for (size_t i = 0; i < s.size(); ++i)
        {
            char c = s[i];
            m[c]++;
            nLen++;
            size_t j = start;
            while (m[c] > 1 && j < i)
            {
                m[s[j]]--;
                nLen--;
                j++;
            }
            start = j;
            if (nLen > nMaxLen)
            {
                nMaxLen = nLen;
            }
        }
        return nMaxLen;
    }
};

int main(int argc, char *argv[])
{
    Solution s;
    std::cout << s.lengthOfLongestSubstring(argv[1]) << endl;
    return 0;
}