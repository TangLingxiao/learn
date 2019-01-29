# coding: utf-8

import json


class UrlManager():
    def __init__(self):
        self.new_urls = {}
        self.old_urls = {}

    def add_new_urls(self, urls):
        if urls is None or len(urls) == 0:
            return
        self.new_urls.update(urls)

    def new_url_size(self):
        return len(self.new_urls)

    def old_url_size(self):
        return len(self.old_urls)

    def save_2_file(self, filename):
        with open(filename, 'w') as f:
            json.dump(self.new_urls, fp=f, ensure_ascii=False, indent=4)