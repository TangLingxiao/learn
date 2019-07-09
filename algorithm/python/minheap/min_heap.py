#coding : utf-8
from huffman import Cmp

class MinHeap(Cmp):
    def __init__(self, lst):
        self.list = []
        self.__build_heap(lst)
        
    def __build_heap(self, lst):
        for i in lst:
            self.insert(i)

    def empty(self):
        return len(self.list) == 0

    def pop(self):
        if len(self.list) == 0:
            return None

        minelm = self.list[0]
        self.list[0] = self.list[-1]
        self.list.pop()
        if len(self.list) > 0:
            self.__arrange(0)
        return minelm
    
    def insert(self, elm):
        self.list.append(elm)
        index = int(len(self.list) / 2  - 1)
        while index >= 0:
            self.__arrange(index)
            index = index - 1


    def __arrange(self, index):
        left = 2 * index + 1
        right = 2 * index + 2
        minindex = index
        minnum = self.list[index]
        if left < len(self.list) and self.cmp(self.list[left], self.list[index]):#self.list[left] < self.list[index]:
            minindex = left
            minnum = self.list[left]
            
        if right < len(self.list) and self.cmp(self.list[right], self.list[index]):#self.list[right] < self.list[index]:
            minindex = right
            minnum = self.list[right]

        if minindex != index:
            self.list[index], self.list[minindex] = self.list[minindex], self.list[index]
            self.__arrange(minindex)


    def dumpAll(self):
        for one in self.list:
            one.dump()

'''
if __name__ == "__main__":
    lst = [5,4,3,2,1]
    heap = MinHeap(lst)
    heap.dump()
    heap.pop()
    heap.dump()
    heap.insert(0)
    heap.dump()
    heap.insert(6)
    heap.dump()
'''