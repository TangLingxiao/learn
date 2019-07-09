class Trie:
    def __init__(self):
        self.nodes = {}
        self.isleaf = False

    def insert(self, string):
        cur = self
        for c in string:
            if c in cur.nodes.keys():
                cur = cur.nodes[c]
                continue
            cur.nodes[c] = Trie()
            cur = cur.nodes[c]
        cur.isleaf = True

    def find(self, string):
        cur = self
        for c in string:
            if c not in cur.nodes.keys():
                return False
            cur = cur.nodes[c]
        return cur.isleaf

    def dumpall(self, out):
        if self.isleaf:
            print(out)
            if len(self.nodes) == 0:
                return
        for k, v in self.nodes.items():
            v.dumpall(out + k)

'''
if __name__ == "__main__":
    lst = ['banana', 'bananas', 'apple']
    trie = Trie()
    for one in lst:
        trie.insert(one)

    trie.dumpall('')
'''