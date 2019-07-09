# coding : utf-8
import sys
import os

import min_heap

class Cmp:
    def cmp(self, a, b):
        return a.times < b.times
        

class Letter:
    def __init__(self, c, times):
        self.c = c
        self.times = times
        self.bitstring = ''

    def dump(self):
        print(self.c, self.times, self.bitstring)

class Node:
    def __init__(self, times, left, right):
        self.left = left
        self.right = right
        self.times = times
    
    def dump(self):
        print(self.times)
        
def parser_file(file, heap):
    #fp = open(file, 'r', encoding='utf')
    content = 'abcbadabfdgeadkffjk;gakeiqdk;adfj'#fp.read()
    cal = {}
    for c in content:
        cal[c] = cal[c] + 1 if c in cal.keys() else 1
    
    for k,v in cal.items():
        heap.insert(Letter(k,v), Cmp().cmp)
    
    #heap.dumpAll()

def build_tree(heap):
    while not heap.empty():
        left = heap.pop(Cmp().cmp)
        right = heap.pop(Cmp().cmp)
        if left is not None and right is not None:
            heap.insert(Node(left.times + right.times, left, right), Cmp().cmp)
        elif left is None:
            return right
        elif right is None:
            return left

def travel_tree(root, lst, bitstring=''):
    if type(root) == Letter: # python3 only
        root.bitstring = bitstring
        lst.append(root)
        return
    travel_tree(root.left, lst, bitstring + '0')
    travel_tree(root.right, lst, bitstring + '1')


if __name__ == "__main__":
    heap = min_heap.MinHeap([])
    parser_file(None, heap)
    root = build_tree(heap)
    lst = []
    travel_tree(root,lst, '')
    for one in lst:
        one.dump()