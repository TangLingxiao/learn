
import os
import sys
import ctypes
import random
import base64
import time
import threading
import redis
import copy
import shutil
import getpass


def ClearMySql(str_):
    import MySQLdb
    list_sql = ['TRUNCATE TABLE `chat_group`;',
	'TRUNCATE TABLE `comment`;',
	'TRUNCATE TABLE `competition_data`;',
	'TRUNCATE TABLE `cur_mail_id`;',
	'TRUNCATE TABLE `mail`;',
	'TRUNCATE TABLE `player_names`;',
	'TRUNCATE TABLE `role`;',
	'TRUNCATE TABLE `role_ext`;',
	'TRUNCATE TABLE `survival_rank`;',
	'TRUNCATE TABLE `sys_mail`;']
	
    #db = MySQLdb.connect("127.0.0.1" ,"root","123456",str_)
    db = MySQLdb.connect("127.0.0.1" ,"root","123456",str_)
    cursor = db.cursor()
    cursor.execute("SET NAMES utf8")
    for one_ in list_sql:
		cursor.execute(one_)
	
    db.commit()
    db.close()
    print "clean mysql" + str_ + "ok"

def ClearOneKey(str_):
	r = redis.Redis(host="127.0.0.1", port=6379,db=0)
	global list_
	if str_ == "test":
		list_ = r.keys("*")
	else:
		list_ = r.keys(str_ + ":*")

	print len(list_)
	#print dir(r)
	for one_ in list_:
		r.delete(one_)
	
	
if __name__ == "__main__":
    #变量初始化
	#初始化redis和mysql对应列表
	dict_redis2sql = {}
	dict_redis2sql["var1"] = "online_show" #体验服
	dict_redis2sql["var3"] = "online_show3"#展示服
	dict_redis2sql["test"] = "test"

	#######################

	key = "test" #选择一个类型
	if key in dict_redis2sql:
		ClearOneKey(key)
		ClearMySql(dict_redis2sql[key])
	else:
		print "error key"

