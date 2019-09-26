#coding : utf-8
import socket

if __name__ == "__main__":
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    host = socket.gethostbyname(socket.gethostname())
    port = 6666
    s.connect(("172.19.130.180",port))
    s.close()