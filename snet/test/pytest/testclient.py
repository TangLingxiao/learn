#coding : utf-8
import socket
import time
if __name__ == "__main__":
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    host = socket.gethostbyname(socket.gethostname())
    print(host)
    port = 6666
    s.connect(("172.19.130.180",port))
    msg = 'test'
    s.send(msg.encode('utf-8'))
    print(s.recv(1024))
    s.close()

