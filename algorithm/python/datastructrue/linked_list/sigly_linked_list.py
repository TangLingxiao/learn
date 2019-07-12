class SNode:
    def __init__(self, data):
        self.next = None
        self.data = data
    
class SList:
    def __init__(self):
        self.head = None
        self.size = 0

    def push_front(self, node):
        if self.head is None:
            self.head = node
        else:
            node.next = self.head
            self.head = node
        self.size += 1

    def pop_front(self):
        if self.head is None or self.size == 0:
            raise IndexError("list is empty")
            return        
        node = self.head
        self.head = node.next
        self.size -= 1
        return node

    def reverce(self):
        pre = None
        cur = self.head
        while cur is not None:
            nxt = cur.next
            cur.next = pre
            pre = cur
            cur = nxt
        self.head = pre

    def dumpall(self):
        cur = self.head
        while cur:
            print(cur.data, " ")
            cur = cur.next

    def size(self):
        return self.size
    
    def empty(self):
        return self.size == 0
'''
if __name__ == "__main__":
    sl = SList()
    for i in range(0,10):
        sl.push_front(SNode(i))
    sl.reverce()
    sl.dumpall()
'''