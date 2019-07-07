#pragma once
#include <vector>
#include <algorithm>
#include <array>
/*
2,9,3,6,5,1,7
*/
constexpr int ArraySize = 10;
//最长递增子序列长度
class CMaxIncreaseLength
{

public:
	void Init(std::vector<int>&& vec)
	{
		vecNum = vec;
	}
	int Cal(std::array<std::array<int, ArraySize>, ArraySize> &temp, std::vector<int> & res)
	{
		std::vector<std::vector<int> > result;
		result.resize(ArraySize);
		int max = 1;
		int n = 0;
		int m = 0;
		for (size_t i = 0; i < vecNum.size(); ++i)
		{
			temp[i][i] = 1;
			auto &o = result[i];
			o.push_back(vecNum[i]);
		}
		for (size_t i = 0; i < vecNum.size(); ++i)
		{
			int j = i;
			n = vecNum[j];
			for (; j > 0; --j)
			{
				if (vecNum[j - 1] < n)
				{
					n = std::min<int>(n, vecNum[j - 1]);
					temp[i][j - 1] = temp[i][j] + 1;
					result[i].insert(result[i].begin(), vecNum[j - 1]);
				}
				else
				{
					temp[i][j - 1] = temp[i][j];
				}
				if (max < temp[i][j - 1])
				{
					m = i;
				}
				max = std::max<int>(max, temp[i][j - 1]);
			}
		}
		res = result[m];
		return max;
	}
	std::vector<int> vecNum;
};
