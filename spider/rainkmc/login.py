# coding: utf-8

from getlinks import CGetLinks
from linkmanger import CLinkManger
from selenium import webdriver
from selenium.webdriver import Chrome
from selenium.webdriver.chrome.options import Options
import time


class CLoginRainkmc():
    def __init__(self):
        self.linkmanager = CLinkManger()
        chrome_options = Options()
        chrome_options.add_argument('--headless')
        chrome_options.add_argument('--disable_gpu')
        chrome_options.add_argument('log-level=3')#headless模式下屏蔽日志
        self.driver = webdriver.Chrome(chrome_options=chrome_options)

    def login_with_selenium(self):
        login_url = r'https://rainkmc.com/forum.php'
        self.driver.get(login_url)
        username = self.driver.find_element_by_name('username')
        username.send_keys('723654638')
        password = self.driver.find_element_by_name('password')
        password.send_keys('password')
        submit = self.driver.find_element_by_tag_name('button')
        submit.click()
        time.sleep(2)

    def get_one_page(self, pagenum):
        page_url = r'https://rainkmc.com/forum.php?mod=forumdisplay&fid=42&page=%d' % pagenum
        self.driver.get(page_url)
        print('enter page: %d successfully' % pagenum)
        getLinks = CGetLinks(self.linkmanager)
        subjects = getLinks.getsubjects(self.driver.page_source)
        if len(subjects) > 0:
            for one in subjects:
                url = r'https://rainkmc.com/' + one.get('href')
                title = str(one.string)
                self.driver.get(url)
                time.sleep(2)
                getLinks.getdata(url, title, self.driver.page_source)
            
    def get_pages(self, begin, end):
        for i in range(begin, end):
            self.get_one_page(i)
        self.linkmanager.save('data', 'source.txt')
        self.driver.close()


if __name__ == '__main__':
    login = CLoginRainkmc()
    login.login_with_selenium()
    login.get_pages(1, 20)
