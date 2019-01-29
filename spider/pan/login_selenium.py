#coding : utf-8
#自动保存度盘工具

from selenium import webdriver
from selenium.webdriver import Chrome
from selenium.webdriver.chrome.options import Options
import time
import json


class CLogin():
    def __init__(self):
        self.login_url = r'https://pan.baidu.com/'
        self.chrome_option = Options()
        self.driver = webdriver.Chrome()
        with open('source.txt', 'rb') as fp:
            self.source = json.load(fp=fp)

    def login(self):
        self.driver.get(self.login_url)
        time.sleep(10)
        for one in self.source:
            title = one.get('title')  
            page = one.get('pan')
            code = one.get('code')
            self.save_file(page, code, title)
        self.driver.close()

    def save_file(self, page, code, name):
        self.driver.get(page)
        try:
            input_ = self.driver.find_element_by_id('eqqo3Jx')
            input_.send_keys(code)
            submit = self.driver.find_element_by_xpath(r'//*[@id="skgr8ZKx"]/a/span/span')
            submit.click()
            time.sleep(2)
            try:
                select = self.driver.find_element_by_xpath(r'//*[@id="shareqr"]/div[2]/div[2]/div/ul[1]/li[1]/div')
                select.click()
                save = self.driver.find_element_by_xpath(r'//*[@id="shareqr"]/div[2]/div[2]/div/div/div/div[2]/a[1]')
            except Exception:
                save = self.driver.find_element_by_xpath(r'//*[@id="layoutMain"]/div[1]/div[1]/div/div[2]/div/div/div[2]/a[1]/span')    
            save.click()
            time.sleep(2)
            ok = self.driver.find_element_by_css_selector(r'#fileTreeDialog > div.dialog-footer.g-clearfix > a.g-button.g-button-blue-large')
            ok.click()
            print('save name %s success' % name)
        except Exception:
            print('url error')


if __name__ == '__main__':
    login = CLogin()
    login.login()