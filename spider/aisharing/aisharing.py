# coding : utf-8

import requests, re, threading, os
from bs4 import BeautifulSoup


start_url = r'http://www.aisharing.com/'
headers = {}
headers['Accept'] = r'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8'
headers['Accept-Encoding'] = 'gzip, deflate'
headers['Accept-Language'] = 'zh-CN,zh;q=0.9'
headers['Cache-Control'] = 'max-age=0'
headers['Connection'] = 'keep-alive'
headers['Host'] = 'www.aisharing.com'
headers['If-Modified-Since'] = 'Thu, 24 Jan 2019 02:20:00 GMT'
headers['Referer'] = 'http://www.aisharing.com/'
headers['Upgrade-Insecure-Requests'] = '1'
headers['User-Agent'] = 'Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36'
#session = requests.Session()
#response = session.get(start_url, headers=headers)
#print(response.status_code, response.encoding)

class AiSharing():
    def __init__(self):
        self.urls = set()
        self.session = requests.Session()

    def start(self, url):
        response = self.session.get(url, headers=headers)
        if response.status_code == 200:
            next_page = self.parser(response.text)
            if next_page is not None:
                print('begin parse next_page...')
                self.start(next_page)
            else:
                print('begin download page...')
                self.multidownload(8)

    def parser(self, html):
        soup = BeautifulSoup(html, 'lxml')
        urls = soup.find_all(href=re.compile(r'http://www.aisharing.com/archives/\d+'))
        for url in urls:
            self.urls.add(url.get('href'))
        next_page = soup.find_all(class_='next page-numbers')
        if len(next_page) > 0:
            return next_page[0].get('href')
        else:
            return None

    def download(self):
        while len(self.urls) > 0:
            url = self.urls.pop()
            if url is None:
                return
            response = self.session.get(url, headers=headers)
            if response.status_code == 200:
                soup = BeautifulSoup(response.text, 'lxml')
                name = soup.find_all(class_='entry-title', itemprop='name')[0].string
                with open('article/'+name+'.html', 'wb') as f:
                    f.write(response.content)
                    f.close()
                print('download %s ended' % name)

    def multidownload(self, num):
        if os.path.exists('article') == False:
            os.mkdir('article')
        tp = []
        for i in range(0, num):
            t = threading.Thread(target=self.download)
            tp.append(t)
        for t in tp:
            t.start()
        for t in tp:
            t.join()

if __name__ == '__main__':
    AI = AiSharing()
    AI.start(start_url)


