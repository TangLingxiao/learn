# coding: utf-8

import requests
import json
from url_downloader import UrlDownloader
from url_manager import UrlManager
from url_parser import UrlParser
import threading
import time
import random

class Wenku():
    def __init__(self):
        self.authority = r'https://www.wenku8.net'
        self.loginurl = r'https://www.wenku8.net/login.php?do=submit&jumpurl=http%3A%2F%2Fwww.wenku8.net%2Findex.php'
        self.pageurl = r"/modules/article/articlelist.php?page="
        self.username = r'username'
        self.password = r'password'

        self.formdata = {}
        self.formdata['username'] = self.username
        self.formdata['password'] = self.password
        self.formdata['usecookie'] = '0'
        self.formdata['action'] = r'login'
        self.formdata['submit'] = r'%26%23160%3B%B5%C7%26%23160%3B%26%23160%3B%C2%BC%26%23160%3B'

        self.headers = {}
        self.headers['origin'] = r'https://www.wenku8.net'
        self.headers['referer'] = r'https://www.wenku8.net/login.php?jumpurl=http%3A%2F%2Fwww.wenku8.net%2Findex.php'
        self.headers['upgrade-insecure-requests'] = '1'
        self.headers['user-agent'] = r'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36'
        
        self.session = requests.Session()
        self.manager = UrlManager()
        self.downloader = UrlDownloader()
        self.parser = UrlParser(self.session)

    def login(self):
        response = self.session.post(self.loginurl, data=self.formdata, headers=self.headers)
        if response.status_code == 200:
            return True
        return False

    def parser_some_pages(self, begin, end):
        for turn in range(begin, end):
            index = self.session.get(self.authority + self.pageurl + str(turn))
            if index.status_code != 200:
                print('get page error page num: ' + str(turn) + ' ,error code: ' + str(index.status_code))
                return
            index.encoding = 'gbk'
            self.parser_one_page(index)
            print('parser page ' + str(turn) + ' done!')
            time.sleep(random.random() * 3)

    def parser_one_page(self, index):
        data = self.parser.parser(index, index.text)
        self.manager.add_new_urls(data)

    def save_2_files(self, filename):
        self.manager.save_2_file(filename)

    def Run(self):
        thread_pool = []
        for i in range(0, 10):
            t = threading.Thread(target=self.parser_some_pages, args=(1 + i * 10, 1 + (i + 1) * 10))
            thread_pool.append(t)
        for t in thread_pool:
            t.start()
        for t in thread_pool:
            t.join()
        self.save_2_files('dict.txt')
        
    def load_and_download(self):
        with open('dict.txt', 'r') as f:
            urls = json.load(fp=f)
        for k, v in urls.items():
            name = (k + '.txt').replace('?', '!')
            self.downloader.download(v[0], name)
            print('download done ' + name)
            time.sleep(random.random() * 3)
        

if __name__ == '__main__':
    conn = Wenku()
    if conn.login():
        #conn.Run()
        conn.load_and_download()
    print('done!')