#ifndef __MAX_PRIORITY_H__
#define __MAX_PRIORITY_H__
#include <assert.h>
#include <vector>
class CMaxPriority
{
public:
	CMaxPriority() :m_vecNums() {}
	~CMaxPriority() = default;
	int Top() { return m_vecNums.at(0); }
	int Pop() {
		const int max = m_vecNums[0];
		m_vecNums[0] = m_vecNums.back();
		m_vecNums.pop_back();
		__HeapArrange(0, m_vecNums.size());
		return max;
	}
	void Insert(int num) {
		m_vecNums.insert(m_vecNums.begin(), num);
		__HeapArrange(0, m_vecNums.size());
	}
	void Increase(int i, int num) {
		assert((unsigned int)i < m_vecNums.size());
		m_vecNums[i] += num;
		int pi = (i + 1) / 2 - 1;
		while (pi >= 0) {
			__HeapArrange(pi, m_vecNums.size());
			pi--;
		}
	}
	void Print() {
		for (const int i : m_vecNums) {
			printf("%d ", i);
		}
		printf("\n");
	}
private:
	void __HeapArrange(int i, int size) {
		int li = 2 * i + 1;
		int ri = 2 * i + 2;
		int mi = i;
		int mn = m_vecNums[mi];
		if (li < size && m_vecNums[li] > mn) {
			mn = m_vecNums[li];
			mi = li;
		}
		if (ri < size && m_vecNums[ri] > mn) {
			mn = m_vecNums[ri];
			mi = ri;
		}
		if (mi == i) { return; }
		m_vecNums[mi] = m_vecNums[i];
		m_vecNums[i] = mn;
		__HeapArrange(mi, size);
	}
	std::vector<int> m_vecNums;
};
#endif // !__MAX_PRIORITY_H__
