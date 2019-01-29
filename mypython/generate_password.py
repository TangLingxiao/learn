import hashlib
import os

g_record_file = ""
def Write2NewFile( str_ ):
	import datetime
	now = datetime.datetime.now()
	str_file = now.strftime('%y_%m_%d_%H_%M.txt')
	f = open(str_file , 'w')
	f.write(str_)
	f.close()

	global g_record_file
	g_record_file = str_file
	
def ShowStrMd5(str_):
	m = hashlib.md5()
	m.update(str_)
	return m.hexdigest()

def Add2File( file_ , str_ ):
	f = open(file_, 'a')
	f.write(str_)
	f.close()

def RandPwd():
	import random,string
	
	
	n_begin = 140	#
	n_end = n_begin + 30		#
	
	dict_ = {}
	dict_["O"] = "a"
	dict_["o"] = "a"
	dict_["0"] = "b"
	dict_["I"] = "t"
	dict_["L"] = "p"
	dict_["l"] = "d"
	dict_["1"] = "8"
	#
	src = string.ascii_letters + string.digits
	list_name = []
	list_pwd = []
	for i in range(n_begin , n_end):
		#list_passwd_all = random.sample(src, 5) #从字母和数字中随机取5位
		#list_passwd_all.extend(random.sample(string.digits, 1))  #让密码中一定包含数字
		#list_passwd_all.extend(random.sample(string.ascii_lowercase, 1)) #让密码中一定包含小写字母
		#list_passwd_all.extend(random.sample(string.ascii_uppercase, 1)) #让密码中一定包含大写字母
		#random.shuffle(list_passwd_all) #打乱列表顺序
		#str_passwd = ''.join(list_passwd_all) #将列表转化为字符串
		#
		#str_passwd = ReplaceAll(str_passwd ,dict_ ) 
		
		str_passwd = "123456"
		list_name.append("snk" + str(i) )
		list_pwd.append(str_passwd)
	
	str_file = "wai_wang_pwd.txt"
	Add2File(str_file , "\n")
	
	str_sql = '''INSERT INTO `account`(`username`,`password`,`accountid`) VALUES ( '[name]','[pwd]',NULL);'''
	
	for i in range( len(list_name) ):
		Add2File(str_file , list_name[i] + "," + list_pwd[i] + "\n")
	
	for i in range( len(list_name) ):
		#Add2File(str_file , str_sql.replace("[name]" , list_name[i]).replace("[pwd]" , ShowStrMd5(list_pwd[i])) + "\n")
		Add2File(str_file , str_sql.replace("[name]" , list_name[i]).replace("[pwd]" , ShowStrMd5("123456")) + "\n")
	os.system("start " + str_file)

if __name__ == "__main__":
    RandPwd()