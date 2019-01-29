# coding :urf-8

from urllib.request import urlretrieve
import os


class UrlDownloader():
    def download(self, url, name):
        if url is None:
            return None
        if os.path.exists('articles') == False:
            os.mkdir('articles')
        urlretrieve(url, 'articles/'+name)

