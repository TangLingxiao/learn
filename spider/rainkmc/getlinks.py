# coding : utf-8

import re
from bs4 import BeautifulSoup


class CGetLinks():
    def __init__(self, linkmanager):
        self.linkmanager = linkmanager
    
    def getsubjects(self, html):
        soup = BeautifulSoup(html, 'lxml')
        subjects = soup.find_all(href=re.compile(r'forum\.php\?mod=viewthread&tid=.*'), onclick=True)
        return subjects
    
    def getdata(self, url, name, html):
        soup = BeautifulSoup(html, 'lxml')
        data = soup.find_all(href=re.compile(r'https?://pan\.baidu\.com/s/.*'))
        pan = ''
        code = ''
        if len(data) > 0:
            pan = data[0].get('href')
        else:
            return
        pattern = re.compile(r'提取码：\d*\w* | 密码：\d*\w*')
        res = re.findall(pattern, html)
        if len(res) > 0:
            code = res[0].split('：')[-1]
        else:
            print('get pan code error %s' % url)
            return
        self.linkmanager.add_done_links(url, name, pan, code)
        print('add name % s done' % name)