# coding: utf-8

import json
import os


class CLinkManger():
    def __init__(self):
        self.done = []

    def add_done_links(self, url, name, pan, code):
        done = {'link': url, 'title': name, 'pan': pan, 'code': code}
        self.done.append(done)

    def save(self, dir_, filename):
        if not os.path.exists(dir_):
            os.makedirs(dir_)
        with open(dir_ + '/' + filename, 'w', encoding='utf-8') as f:
            json.dump(self.done, fp=f, ensure_ascii=False, indent=4)
        print('save successfully')
