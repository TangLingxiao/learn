# coding: utf-8

import redis
import pymysql
import pymysql.cursors
import json, sys, os, re

sys.path.append('../')
import base_create_xml

def get_ip_name():
	fp = open('../base_create_xml.py', 'r')
	s = fp.read()
	pattern = re.compile(r'str_my_ip.*and?\s*str_user_name.*:')
	ret = re.findall(pattern, s)
	dic = {}
	for one in ret:
		ip_pattern = re.compile(r"(?:[0-9]{1,3}\.){3}[0-9]{1,3}")
		ip = re.findall(ip_pattern,one)[0]
		name_pattern = re.compile(r'str_user_name\s?==.*:')
		name = re.findall(name_pattern, one)[0].split('"')[1]
		dic[name] = ip
	return dic

def ClearMySql(host, usr, password, db):
	list_sql = [
	'TRUNCATE TABLE `arena_rank`;',
	'TRUNCATE TABLE `champion_rank`;',
	'TRUNCATE TABLE `competition_data`;',
	'TRUNCATE TABLE `cur_mail_id`;',
	'TRUNCATE TABLE `mail`;',
	'TRUNCATE TABLE `role`;',
	'TRUNCATE TABLE `survival_rank`;',
	'TRUNCATE TABLE `sys_mail`;']
	
	db = pymysql.connect(host=host ,user=usr,password=password,db=db, charset='utf8', cursorclass=pymysql.cursors.DictCursor)
	cursor = db.cursor()
	for one_ in list_sql:
		cursor.execute(one_)
	
	db.commit()
	db.close()
	print ("clean mysql ok")

def ClearOneKey(host, port):
	r = redis.Redis(host=host, port=port,db=0)
	list_ = r.keys("*")
	#len(list_))
	for one_ in list_:
		r.delete(one_)
	print( 'clean redis ok')
	
if __name__ == "__main__":
	dic = get_ip_name()
	print('input server name:')
	server = raw_input()
	ip = dic.get(server)
	if ip is None:
		print('not find server name, will use hostname')
		server = None
	dic_ = base_create_xml.fix_my_info(ip, server)
	mysql_list = dic_.get('mysql_list')
	redis_list = dic_.get('redis_list')
	if mysql_list is None or len(mysql_list) == 0 or redis_list is None or len(redis_list) == 0:
		print('not find mysql_list or redis_list')
		exit()
	print(mysql_list)
	print(redis_list)
	print('continue [y/n]?')
	ok = raw_input()
	if ok.lower() != 'y':
		exit()
	for one in mysql_list:
		mysql_data = one.replace("@",":").replace("/",":").split(":")
		user = mysql_data[0]
		pwd = mysql_data[1]
		host = mysql_data[2]
		port = mysql_data[3]
		db = mysql_data[4]
		ClearMySql(host, user, pwd, db)
	
	for one in redis_list:
		redis_data = one.replace("@",":").replace("/",":").split(":")
		user = redis_data[0]
		host = redis_data[1]
		port = redis_data[2]
		ClearOneKey(host, port)
