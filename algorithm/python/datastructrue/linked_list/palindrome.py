import stack

def is_palindrome(string):
    ss = stack.Stack()
    j = 0
    for i in range(0, len(string), 2):
        ss.push(string[j])
        j = j + 1
    
    if 2 * j - 1 == len(string):
        ss.pop()
            
    for i in range(j, len(string)):
        elm = ss.pop()
        if elm != string[i]:
            return False
    return True

if __name__ == "__main__":
    print (is_palindrome('abcba'))
    print (is_palindrome('abccba'))
    print (is_palindrome('abcdba'))
    
