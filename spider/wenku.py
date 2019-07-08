# coding : utf-8

import requests
from bs4 import BeautifulSoup
import re
import json
from urllib.request import urlretrieve

class wenku8():
    def __init__(self):
        self.loginurl = 'https://www.wenku8.net/login.php?do=submit&jumpurl=http%3A%2F%2Fwww.wenku8.net%2Findex.php'
        self.indexurl = 'http://www.wenku8.net/index.php'
        self.username = ''
        self.password = ''
        self.formdata = {}
        self.formdata['username'] = self.username
        self.formdata['password'] = self.password
        self.formdata['usecookie'] = '0'
        self.formdata['action'] = 'login'
        self.formdata['submit'] = '%26%23160%3B%B5%C7%26%23160%3B%26%23160%3B%C2%BC%26%23160%3B'
        self.headers = {}
        self.headers['origin'] = 'https://www.wenku8.net'
        self.headers['referer'] = 'https://www.wenku8.net/login.php?jumpurl=http%3A%2F%2Fwww.wenku8.net%2Findex.php'
        self.headers['upgrade-insecure-requests'] = '1'
        self.headers['user-agent'] = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36'
        self.session = requests.Session()
        self.index = ''
        self.ll = []

    def login(self):
        response = self.session.post(self.loginurl,data = self.formdata, headers = self.headers)
        self.index = self.session.get(self.indexurl)
        self.index.encoding = 'gbk'
        #print(index.text)
    
    def regex(self):
        pattern = re.compile(r'w{3}.*?htm')
        ll = re.findall(pattern,self.index.text)
        #print(ll)

    def saveindex(self):
        soup = BeautifulSoup(self.index.text, 'lxml')
        ll = soup.find_all(href = True,target="_blank")
        self.ll = []
        for one in ll:
            if one.has_attr('title'):
                dic = {}
                dic['url'] = self.getdownloadurl(one.get('href'))
                if dic['url'] == None:
                    continue
                dic['title'] = one.get('title')
                if dic in self.ll:
                    continue
                self.ll.append(dic)

        #print(res)
        with open('dict.txt','w') as f:
            json.dump(self.ll,fp = f,ensure_ascii=False,indent=4)

    def getdownloadurl(self, parent):
        url = self.session.get(parent)
        soup = BeautifulSoup(url.text,'lxml')
        ll = soup.find_all(href = re.compile(r'.*txtfull'))
        if len(ll) == 0:
            return
        downloadpage = ll[0].get('href')
        p = self.session.get(downloadpage)
        soup = BeautifulSoup(p.text,'lxml')
        return soup.find_all(href = re.compile(r'.*down.php'))[3].get('href')
        
    def loadurl(self):
        with open('dict.txt','r') as f:
            urls = json.load(fp=f)
            for one in urls:
                self.download(one['url'],one['title'])

    def download(self, url, name):
        urlretrieve(url, name + '.txt')

if __name__ == '__main__':
    conn = wenku8()
    conn.login()
    #conn.regex()
    conn.saveindex()
    #conn.loadurl()
