class Stack:
    def __init__(self):
        self.list = []

    def push(self, elm):
        self.list.append(elm)
        
    def top(self):
        if self.empty():
            raise IndexError("empty stack")
        return self.list[-1]
        

    def pop(self):
        if self.empty():
            raise IndexError("empty stack")
        ret = self.top()
        self.list.pop(-1)
        return ret

    def empty(self):
        return len(self.list) == 0
