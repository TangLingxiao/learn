# coding : utf-8

import pymysql
import pymysql.cursors
import json, sys, os, re
sys.path.append('../')
import base_create_xml
sys.path.append('../../../client_py/proto_py/')
import server_base_pb2

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

def get_data(host, user, password, db, account):
    connection = pymysql.connect(host=host, user=user, password=password, db=db, charset='utf8', cursorclass=pymysql.cursors.DictCursor)
    try:
        with connection.cursor() as cursor:
            sql = "select `role_ext` from role where `account_id` = '%s';" % account
            cursor.execute(sql)
            result = cursor.fetchone()
            if result != None:
                proto = server_base_pb2.db_role_all_data()
                proto.ParseFromString(result['role_ext'])
                data = str(proto)
                fd = os.open('result.cpp', os.O_RDWR|os.O_CREAT)
                os.write(fd, data)
                os.close(fd)
            else:
                print('not find account')
    finally:
        connection.close()
        #connection.commit()

if __name__ == '__main__':
    dic = get_ip_name()
    print('input server name:')
    server = raw_input()
    ip = dic.get(server)
    if ip is None:
        print('not find server name, will use hostname')
        server = None
    print('input account:')
    account = raw_input()
    dic_ = base_create_xml.fix_my_info(ip, server)
    mysql_list = dic_.get('mysql_list')
    if mysql_list is None or len(mysql_list) == 0:
        print('not find mysql_list')
        exit()
    print(mysql_list[0])
    mysql_data = mysql_list[0].replace("@",":").replace("/",":").split(":")
    user = mysql_data[0]
    pwd = mysql_data[1]
    host = mysql_data[2]
    port = mysql_data[3]
    db = mysql_data[4]
    get_data(host, user, pwd, db, account)
