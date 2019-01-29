# coding: utf-8


class MaxPriority:
    def __init__(self):
        self.nums = []

    def out(self):
        print self.nums

    def Max(self):
        if len(self.nums) == 0:
            return 'empty queue'
        return self.nums[0]

    def __HeapArrange(self, i, size):
        li = 2 * i + 1
        ri = 2 * i + 2
        mi = i
        mn = self.nums[mi]
        if li < size and self.nums[li] > mn:
            mn = self.nums[li]
            mi = li
        if ri < size and self.nums[ri] > mn:
            mn = self.nums[ri]
            mi = ri
        if mi == i:
            return
        self.nums[mi] = self.nums[i]
        self.nums[i] = mn
        self.__HeapArrange(mi, size)

    def Pop(self):
        mi = self.nums[0]
        self.nums[0] = self.nums[-1]
        self.nums.pop(-1)
        self.__HeapArrange(0, len(self.nums))
        return mi

    def Insert(self, num):
        self.nums.insert(0, num)
        self.__HeapArrange(0, len(self.nums))

    def Increase(self, key, num):
        self.nums[key] += num
        if key < 0:
            key = len(self.nums) + key
        pi = (key + 1) / 2 - 1
        while pi >= 0:
            self.__HeapArrange(pi, len(self.nums))
            pi -= 1


if __name__ == '__main__':
    cc = MaxPriority()
    for i in range(0, 5):
        cc.Insert(i)

    cc.Pop()
    cc.out()
    cc.Increase(-1, 10)
    cc.out()