# coding: utf-8

import re
from bs4 import BeautifulSoup


class UrlParser():
    def __init__(self, session):
        self.session = session

    def parser(self, index, content):
        soup = BeautifulSoup(content, 'lxml')
        childern = soup.find_all(href=re.compile(r'https.*book.*htm'))
        result = {}
        for child in childern:
            if child.has_attr('title'):
                if child.get('title') in result:
                    continue
                download_url = self.get_download_url(child.get('href'))
                if download_url is None or len(download_url) == 0:
                    continue
                result[child.get('title')] = download_url
        return result

    def match_url_by_regex(self, url, pattern):
        if url is None:
            return None
        response = self.session.get(url)
        if response.status_code != 200:
            return None
        response.encoding = 'utf-8'
        soup = BeautifulSoup(response.text, 'lxml')
        textfull = soup.find_all(href=pattern)
        if len(textfull) == 0:
            return None
        return textfull

    def get_download_url(self, url):
        textfull = self.match_url_by_regex(url, re.compile(r'.*txtfull'))
        if textfull is None or len(textfull) == 0:
            return None
        downloadurl = []
        for one in textfull:
            res = self.match_url_by_regex(one.get('href'), re.compile(r'.*down.php'))
            if res is None or len(res) == 0:
                continue
            downloadurl.append(res[0].get('href'))
        return downloadurl

