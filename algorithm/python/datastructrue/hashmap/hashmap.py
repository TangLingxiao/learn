# coding : utf-8

import sigly_linked_list

#use linkedlist for collision resolve
    
class Data:
    def __init__(self, key, value):
        self.key = key
        self.value = value

class Dict:
    def __init__(self, size):
        self.size = size
        self.used = 0
        self.data = {}

class HashMap:
    def __init__(self, size, charge_factor):
        self.charge_factor = charge_factor if charge_factor != 0 else 0.75
        self.using = Dict(size)
        self.aux = Dict(size)
        self.rehash = False

    def __hash_function(self, key):
        string = str(key)
        length = len(string)
        hash = 5381
        bak = length
        while length > 0:
            hash = ((hash << 5) + hash) + int(string[bak - length])
            length -= 1
        return hash

    def __cal_collision(self):
        if self.rehash:
            return True
        if self.using.used * 1.0 / self.using.size >= self.charge_factor:
            self.rehash = True
            return True
        return False
        
    def __rehash_all(self):
        if self.rehash == False:
            return
        print("begin rehashing")
        self.using, self.aux = self.aux, self.using
        self.using.size  = self.aux.size * 2
        for _, v in self.aux.data.items():
            while not v.empty():
                data = v.pop_front()
                self.__insert(data.data.key, data.data.value)
        self.rehash = False
        print("using:" , self.using.used)
        print("rehashing done")

    def __insert(self, key, value):
        hashkey = self.__hash_function(key)
        self.using.data[hashkey] = sigly_linked_list.SList() if hashkey not in self.using.data.keys() else self.using.data[hashkey]
        cur = self.using.data[hashkey].head
        while cur:
            if cur.data.key == key:
                cur.data.value = value
                return
            cur = cur.next
        self.using.data[hashkey].push_front(sigly_linked_list.SNode(Data(key, value)))
        self.using.used += 1

    def insert(self, key, value):
        if self.__cal_collision():
            self.__rehash_all()
        self.__insert(key, value)

    def find(self, key):
        hashkey = self.__hash_function(key)
        if hashkey not in self.using.data.keys():
            return None
        cur = self.using.data[hashkey].head
        while cur:
            if cur.data.key == key:
                return cur.data.value
            cur = cur.next
        return None

    def erase(self, key):
        hashkey = self.__hash_function(key)
        if hashkey not in self.using.data.keys():
            return None
        pre = None
        cur = self.using.data[hashkey].head
        while cur:
            nxt = cur.next
            if cur.data.key == key:
                self.using.used -= 1
                if pre is None:
                    self.using.data[hashkey].head = nxt
                    return
                pre.next = nxt
                return
            pre = cur
            cur = nxt

    def dumpall(self):
        for _, v in self.using.data.items():
            cur = v.head
            while cur:
                print("key, value:", cur.data.key, cur.data.value)
                cur = cur.next
                

    
if __name__ == "__main__":
    hashmap = HashMap(100, 0.75)
    for i in range(0, 100):
        hashmap.insert(i,i)
    hashmap.dumpall()

    