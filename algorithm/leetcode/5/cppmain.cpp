#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Solution
{
public:
    string longestPalindrome(string s)
    {
        const int nLength = s.length();
        if (nLength <= 1)
        {
            return s;
        }

        vector<vector<int>> vDp(nLength, vector<int>(nLength, 0));
        int nMaxLeft = 0;
        int nMaxRight = 0;
        int nMaxLength = 1;
        auto func = [&](int nLeft, int nRight) {
            if (nRight - nLeft + 1 > nMaxLength)
            {
                nMaxLength = nRight - nLeft + 1;
                nMaxLeft = nLeft;
                nMaxRight = nRight;
            }
        };
        for (int i = nLength - 1; i >= 0; --i)
        {
            for (int j = i; j < nLength; ++j)
            {
                if (s[i] == s[j] && (j - i <= 2 || vDp[i + 1][j - 1]))
                {
                    vDp[i][j] = 1;
                    func(i, j);
                }
            }
        }
        return s.substr(nMaxLeft, nMaxLength);
    }

    string longestPalindrome2(string s)
    {
        if (s.size() <= 1)
            return s;
        int nMaxLength = 0;
        int nMaxLeft = 0;
        int nMaxRight = 0;
        for (int i = 0; i < s.size(); ++i)
        {
            int nLeft = i;
            int nRight = i;
            while (nRight < s.size() - 1 && s[nRight] == s[nRight + 1])
                nRight++;
            while (nLeft > 0 && nRight < s.size() - 1 && s[nLeft - 1] == s[nRight + 1])
            {
                nLeft--;
                nRight++;
            }
            if (nRight - nLeft + 1> nMaxLength)
            {
                nMaxLeft = nLeft;
                nMaxRight = nRight;
                nMaxLength = nRight - nLeft + 1;
            }
        }
        return s.substr(nMaxLeft, nMaxLength);
    }
};

int main(int argc, char *argv[])
{
    Solution s;
    cout << s.longestPalindrome(argv[1]) << endl;
}