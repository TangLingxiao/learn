

import sys
import os
import base64
import struct
import time 
import random
import ctypes

sys.path.append("E:/server/client_py/proto_py/")
#加载proto
import login_pb2
import client_pb2
import base_pb2
import mail_pb2
import gs_db_pb2
import gt_db_pb2
import gt_center_pb2
import gs_public_pb2
#配置ui界面

g_dict_class = {}

#设置工作目录
g_old_path = ""
def SetPath():
	global g_old_path
	g_old_path = os.getcwd()
	os.chdir(os.path.split(os.path.realpath(__file__))[0] )
	
def ResetPath():
	os.chdir(g_old_path )

	
class Uitest:
	def __init__(self):
		
		self.dict_id_2_proto = {}
		self.dict_id_2_proto_name = {}
		self.init_box_1()
		self.aes_key = 'hbbhehedadatttts'
		self.aes_encode_len = 16

	def init_box_1(self):
		
		self.init_proto_obj("client_pb2" , client_pb2)
		self.init_proto_obj("login_pb2" , login_pb2)
		self.init_proto_obj("gs_db_pb2" , gs_db_pb2)
		self.init_proto_obj("gt_db_pb2" , gt_db_pb2)
		
		#将对象更新到容器
		
	def init_proto_obj(self , str_key , obj):
		self.dict_id_2_proto[str_key] = {}
		self.dict_id_2_proto_name[str_key] = {}
		
		is_find = False
		all_list = dir(obj)
		for one_ in all_list:
			if one_ == "_reflection":
				is_find = True

			if is_find == False:
				continue
			
			#获取到协议id
			if one_[-3:] == "_id":
				str_obj_name = one_[:-3]
				if str_obj_name in all_list:
					n_msg_id = getattr(obj , one_)
					pFun = getattr(obj , str_obj_name )
					self.dict_id_2_proto[str_key][n_msg_id] = pFun()
					self.dict_id_2_proto_name[str_key][n_msg_id] = one_
				elif str_obj_name in dir(base_pb2):
					n_msg_id = getattr(obj , one_)
					pFun = getattr(base_pb2 , str_obj_name )
					self.dict_id_2_proto[str_key][n_msg_id] = pFun()
					self.dict_id_2_proto_name[str_key][n_msg_id] = one_
				else:
					print "not find" , str_obj_name 
				
		pass
		
	def AES_Decode(self, str_):
		from Crypto.Cipher import AES
		if len(str_) < self.aes_encode_len:
			return str_	
		obj = AES.new(self.aes_key, AES.MODE_ECB)
		return obj.decrypt(str_[0:self.aes_encode_len]) + str_[self.aes_encode_len:]

	#解码
	def msg_decode(self , str_type , str_base64, c2s ):
		str_base64 = str_base64.replace("\r" , "").replace("\n" , "").replace(" " , "")
		#
		str_msg = base64.b64decode(str_base64)
		
		n_msg_id_2 = 0
		str_proto =""

		if c2s == True:
			str_format = "!HLH" + str(len(str_msg) - 2 - 4 - 2 ) + "s"
			n_add_id, n_crc, n_msg_id_2, str_proto = struct.unpack(str_format, str_msg)
		else:
			str_format = "!HH" + str(len(str_msg) - 2 - 2) + "s"
			n_add_id , n_msg_id_2 , str_proto = struct.unpack(str_format, str_msg)
		
		n_msg_id_name = self.dict_id_2_proto_name[str_type].get(n_msg_id_2, "none")
		one_proto = self.dict_id_2_proto[str_type].get(n_msg_id_2, None)
		if one_proto == None:
			print "not find obj " 
			return

		str_proto = self.AES_Decode(str_proto)
		one_proto.ParseFromString(str_proto)
		
		print n_msg_id_2 ,type( one_proto ) , str(one_proto)
		
		str_out = n_msg_id_name + "\n"
		str_out += "#if \n"
		str_out += str(one_proto) + "\n"
		str_out += "#end \n"
		return str_out

def Write2NewFile( file_ ,list_ ):
	f = open( file_ , 'w')
	f.writelines(list_)
	f.close()

		
if __name__ == "__main__":
	fun_tool = Uitest()
	#
	str_path = "E:/server/server/"
	win_str_path = str_path.replace("/" , "\\")
	#
	str_file = "tt.log"
	#
	f_1 = open(str_path + str_file , "r")
	list_1 = f_1.readlines()
	f_1.close()
	#
	list_2 = []
	c2s = True
	for i_ in range( len(list_1) ):
		if i_ % 2 == 0:
			#时间
			list_2.append(list_1[i_])
			c2s = 'client2Gate' in list_1[i_]
		else:
			#base64
			list_2.append( fun_tool.msg_decode("client_pb2" , list_1[i_], c2s ) )
	
	if len(list_2) > 0:
		Write2NewFile(str_path+ "change_" + str_file + ".cc" , list_2)
		os.system("del " + win_str_path + str_file)
	
	print time.time()
	os.system("pause")





