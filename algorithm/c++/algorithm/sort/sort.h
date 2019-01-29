#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8

#ifndef __SORT_H__
#define __SORT_H__
#include <array>
#include <vector>
template<int SIZE>
class CSort
{
public:
	explicit CSort(const std::array<int, SIZE>& aNums)noexcept : m_aNums(aNums) {}
	void InsertSort() {
		for (size_t j = 1; j < m_aNums.size(); ++j) {
			int key = m_aNums[j];
			int i = j - 1;
			while (i >= 0 && m_aNums[i] > key) {
				m_aNums[i + 1] = m_aNums[i];
				--i;
			}
			m_aNums[i + 1] = key;
		}
		print();
	}

	void SelectSort() {
		for (size_t i = 0; i < m_aNums.size(); ++i) {
			size_t mi = i;
			int mn = m_aNums[i];
			size_t j = i;
			for (; j < m_aNums.size(); ++j) {
				if (mn > m_aNums[j]) {
					mn = m_aNums[j];
					mi = j;
				}
			}
			m_aNums[mi] = m_aNums[i];
			m_aNums[i] = mn;
		}
		print();
	}

	void MergeSort(size_t begin, size_t end) {
		if (begin == end) {
			return;
		}
		size_t p = begin;
		size_t q = (begin + end) / 2;
		size_t r = end;
		MergeSort(p, q);
		MergeSort(q + 1, r);
		std::vector<int> aLeft;
		for (size_t i = p; i <= q; ++i) {
			aLeft.push_back(m_aNums[i]);
		}
		std::vector<int> aRight;
		for (size_t i = q + 1; i <= r; ++i) {
			aRight.push_back(m_aNums[i]);
		}
		size_t li = 0;
		size_t ri = 0;
		for (size_t i = p; i <= r; ++i) {
			if (li >= aLeft.size() && ri < aRight.size()) {
				m_aNums[i] = aRight[ri];
				++ri;
			}
			else if (ri >= aRight.size() && li < aLeft.size()) {
				m_aNums[i] = aLeft[li];
				++li;
			}
			else if (ri >= aRight.size() && li >= aLeft.size()) {
				return;
			}
			else if (aLeft[li] > aRight[ri]) {
				m_aNums[i] = aRight[ri];
				++ri;
			}
			else {
				m_aNums[i] = aLeft[li];
				++li;
			}
		}
	}

	void Heap_Arrange(int i, int size) {
		int ln = 2 * i + 1;
		int rn = 2 * i + 2;
		int max = m_aNums.at(i);
		int mi = i;
		if (ln < size && m_aNums.at(ln) > max) {
			max = m_aNums.at(ln);
			mi = ln;
		}
		if (rn < size && m_aNums.at(rn) > max) {
			max = m_aNums.at(rn);
			mi = rn;
		}
		if (max == m_aNums.at(i)) {
			return;
		}
		m_aNums.at(mi) = m_aNums.at(i);
		m_aNums.at(i) = max;
		Heap_Arrange(mi, size);
	}

	void Build_Max_Heap() {
		int i = (m_aNums.size() + 1) / 2 - 1;
		while (i >= 0) {
			Heap_Arrange(i, m_aNums.size());
			--i;
		}
	}

	void Heap_Sort() {
		Build_Max_Heap();
		for (int i = m_aNums.size() - 1; i > 0; --i) {
			int temp = m_aNums[0];
			m_aNums.at(0) = m_aNums[i];
			m_aNums[i] = temp;
			Heap_Arrange(0, i);
		}
	}
	
	int Partition(int p, int r) {
		int key = m_aNums[r];
		int mid = p - 1;
		int iter = p;
		while (iter < r) {
			if (m_aNums[iter] < key) {
				mid++;
				if (mid != iter) {
					std::swap(m_aNums[mid], m_aNums[iter]);
				}
			}
			iter++;
		}
		std::swap(m_aNums[mid + 1], m_aNums[r]);
		return mid + 1;
	}

	int Partition2(int p, int r) {
		int key = m_aNums[r];
		int li = p;
		int ri = r;
		while (ri > li) {
			if (m_aNums[ri] < key) {
				while (ri > li) {
					if (m_aNums[li] > key) {
						std::swap(m_aNums[li], m_aNums[ri]);
					}
					li++;
				}
			}
			ri--;
		}
		std::swap(m_aNums[li], m_aNums[r]);
		return li;
	}
	void Quick_Sort(int p, int r) {
		if (p >= r) {
			return;
		}
		int q = Partition2(p, r);
		Quick_Sort(p, q - 1);
		Quick_Sort(q + 1, r);
	}
	void print() {
		for (int& i : m_aNums) {
			printf("%d ", i);
		}
		printf("\n");
	}
private:
	std::array<int, SIZE> m_aNums;
};
#endif // !__SORT_H__
