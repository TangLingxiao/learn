# coding:utf-8


class CSort:
    def __init__(self, nums):
        self.nums = nums

    def out(self):
        print self.nums

    def insert_sort(self):
        for j in range(1, len(self.nums)):
            key = self.nums[j]
            i = j - 1
            while i >= 0 and self.nums[i] < key:
                self.nums[i + 1] = self.nums[i]
                i = i - 1
            self.nums[i + 1] = key
        print self.nums

    def select_sort(self):
        for i in range(0, len(self.nums)):
            mi = i
            mn = self.nums[i]
            for j in range(i, len(self.nums)):
                if self.nums[j] < mn:
                    mn = self.nums[j]
                    mi = j
            self.nums[mi] = self.nums[i]
            self.nums[i] = mn
        print self.nums

    def merge_sort(self, begin, end):
        if begin == end:
            return
        p = begin
        q = (begin + end) / 2
        r = end
        self.merge_sort(p, q)
        self.merge_sort(q+1, r)
        ll = [self.nums[x] for x in range(p, q + 1)]
        rl = [self.nums[x] for x in range(q + 1, r + 1)]
        li = 0
        ri = 0
        for i in range(p, r + 1):
            if li >= len(ll) and ri >= len(rl):
                return
            elif li >= len(ll) and ri < len(rl):
                self.nums[i] = rl[ri]
                ri = ri + 1
            elif li < len(ll) and ri >= len(rl):
                self.nums[i] = ll[li]
                li = li + 1
            elif ll[li] > rl[ri]:
                self.nums[i] = rl[ri]
                ri = ri + 1
            else:
                self.nums[i] = ll[li]
                li = li + 1

    def heap_arrange(self, i, len):
        li = 2 * i + 1
        ri = 2 * i + 2
        mn = self.nums[i]
        mi = i
        if li < len and mn < self.nums[li]:
            mn = self.nums[li]
            mi = li
        if ri < len and mn < self.nums[ri]:
            mn = self.nums[ri]
            mi = ri
        if mi == i:
            return
        self.nums[mi] = self.nums[i]
        self.nums[i] = mn
        self.heap_arrange(mi, len)

    def build_heap(self):
        i = (len(self.nums) + 1) / 2 - 1
        while i >= 0:
            self.heap_arrange(i, len(self.nums))
            i -= 1

    def heap_sort(self):
        self.build_heap()
        for i in range(len(self.nums) - 1, -1, -1):
            tmp = self.nums[i]
            self.nums[i] = self.nums[0]
            self.nums[0] = tmp
            self.heap_arrange(0, i)

    def quick_sort(self, p, r):
        if p >= r:
            return
        q = self.partition4(p, r)
        self.quick_sort(p, q - 1)
        self.quick_sort(q + 1, r)

    def partition3(self, begin, end):
        key = self.nums[end]
        leftindex = begin
        rightindex = end - 1
        while rightindex > leftindex:
            if self.nums[rightindex] < key:
                while leftindex < rightindex:
                    if self.nums[leftindex] > key:
                        self.nums[leftindex], self.nums[rightindex] = self.nums[rightindex], self.nums[leftindex]
                        leftindex = leftindex + 1
                        break
                    leftindex = leftindex + 1
            rightindex = rightindex - 1
        self.nums[end], self.nums[leftindex] = self.nums[leftindex], key
        return leftindex

    def partition4(self, begin, end):
        key = self.nums[end]
        mid = begin - 1
        quick = begin
        while quick < end:
            if self.nums[quick] < key:
                mid += 1
                if mid != quick:
                    self.nums[mid], self.nums[quick] = self.nums[quick], self.nums[mid]
            quick += 1
        self.nums[end], self.nums[mid + 1] = self.nums[mid + 1], key
        return mid + 1

if __name__ == "__main__":
    nums = [8, 6, 10, 4, 1]
    c = CSort(nums)
    #c.insert_sort()
    #c.select_sort()
    #c.merge_sort(0,len(nums) - 1)
    #c.heap_sort()
    c.quick_sort(0, len(nums) - 1)
    c.out()
