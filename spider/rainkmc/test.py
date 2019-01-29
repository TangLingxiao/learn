# coding : utf-8

from selenium import webdriver
from selenium.webdriver import Chrome


driver = webdriver.Chrome()
driver.get(r'https://www.wenku8.net/login.php?jumpurl=http%3A%2F%2Fwww.wenku8.net%2Findex.php')
#username = driver.find_element_by_name('username')
#username.send_keys('723654638')
#driver.find_element_by_name('password').send_keys('')

t = driver.find_element_by_name('submit')
print(dir(t))

#driver.close()