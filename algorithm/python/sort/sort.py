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
        
    def partition(self, p, r):
        key = self.nums[r]
        mid = p - 1
        iter_ = p
        while iter_ < r:
            if self.nums[iter_] < key:
                mid += 1
                if mid != iter_:
                    temp = self.nums[iter_]
                    self.nums[iter_] = self.nums[mid]
                    self.nums[mid] = temp
            iter_ += 1
        self.nums[r] = self.nums[mid + 1]
        self.nums[mid + 1] = key
        return mid + 1

    def partition2(self, p, r):
        key = self.nums[r]
        li = p
        ri = r
        while ri > li:
            if self.nums[ri] < key:
                while li < ri:
                    if self.nums[li] > key:
                        temp = self.nums[li]
                        self.nums[li] = self.nums[ri]
                        self.nums[ri] = temp
                        li = li + 1
                        break
                    li = li + 1
            ri = ri - 1  
        self.nums[r] = self.nums[li]
        self.nums[li] = key
        return li

    def quick_sort(self, p, r):
        if p >= r:
            return
        q = self.partition2(p, r)
        self.quick_sort(p, q - 1)
        self.quick_sort(q + 1, r)


if __name__ == "__main__":
    nums = [8, 6, 10, 4, 1]
    c = CSort(nums)
    #c.insert_sort()
    #c.select_sort()
    #c.merge_sort(0,len(nums) - 1)
    #c.heap_sort()
    c.quick_sort(0, len(nums) - 1)
    c.out()
