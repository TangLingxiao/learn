

import sys
import os
import base64
import struct
import time 
import random
import ctypes
import threading

from PyQt4 import QtCore, QtGui, uic
from PyQt4.QtCore import *  
from PyQt4.QtGui import *  


sys.path.append("proto_py/")
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
g_str_ui_test = "test_base64_msg.ui"
test_class, base_test_class = uic.loadUiType(g_str_ui_test)
g_dict_class = {}


def ShowBuff(str_buff):
	str_out = ""
	for one_ in str_buff:
		str_out = str_out + "/" + str( ord(one_) )
	print str_out
	return str_out
	
def DecodeBuff(encode_buff):
	str_out = ""
	for one_ in encode_buff.split("/"):
		if len(one_) == 0:
			continue

		str_out = str_out + chr( int(one_) )
		
	return str_out

def Qstring2utf(str_):
	return unicode(str_).encode("utf")
	
	
		
def aes_encrypt(str_key , str_): 
	from Crypto.Cipher import AES
	obj = AES.new(str_key , AES.MODE_ECB)
	ciphertext = obj.encrypt(str_)
	result = base64.b64encode(ciphertext)  #base64 encode
	return result 
	
def aes_decode(str_key , str_base64):
	from Crypto.Cipher import AES
	str_data = base64.b64decode(str_base64)
	obj2 = AES.new(str_key , AES.MODE_ECB)
	return  obj2.decrypt(str_data)
	
def test_AES():
	str_key = 'hbbhehedadatttts'
	str_msg = '1234567890123456'
	str_ = aes_encrypt(str_key , str_msg)
	str_2 = aes_decode(str_key ,str_)
	print 'python'
	print "源数据\t\t:".decode("utf").encode("gbk"),str_msg
	print "加密数据\t:".decode("utf").encode("gbk"),str_
	print "解密数据\t:".decode("utf").encode("gbk"),str_2
	

class Uitest(QtGui.QDialog, test_class):
	def __init__(self, *args):
		super(Uitest, self).__init__(*args)
		self.setupUi(self)
		
		self.dict_id_2_proto = {}
		self.dict_id_2_proto_name = {}
		self.init_box_1()

	def init_box_1(self):
		self.box_1.clear()
		
		self.init_proto_obj("client_pb2" , client_pb2)
		self.init_proto_obj("login_pb2" , login_pb2)
		self.init_proto_obj("gs_db_pb2" , gs_db_pb2)
		self.init_proto_obj("gt_db_pb2" , gt_db_pb2)
		#self.init_proto_obj("base_pb2" , base_pb2)
		
		#将对象更新到容器
		
	def init_proto_obj(self , str_key , obj):
		self.box_1.addItem(str_key)
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
		
	def decode_str(self):
		str_ = Qstring2utf(self.input_2.toPlainText() )
		str_ = str_.replace("\r" , "")
		
		if str_.find("msg_id = ") >= 0:
			list_ = str_.split("\n")
			if len(list_) >= 2:
				str_key = "msg_id = "
				index_1 = list_[0].find(str_key)
				str_msg_id = list_[0][index_1 + len(str_key) :]
				self.input_1.setText(str_msg_id)
				str_mgs_base64 = list_[1].replace("\n" , "").replace(" " , "")
				self.input_2.setPlainText(str_mgs_base64)
				
			else:
				print "decode error"
		else:
			str_mgs_base64 = str_.replace("\n" , "").replace(" " , "")
			self.input_2.setPlainText(str_mgs_base64)

		pass
		
	def get_msg_id(self):
		#获取_输入框的协议id
		str_msg_id = Qstring2utf(self.input_1.text() )
		if str_msg_id == "":
			print "msg id error" 
			return 0
		return int( str_msg_id)
		
	def on_button_1_pressed(self ):
		self.decode_str()
		
		str_msg = Qstring2utf(self.input_2.toPlainText() )
		str_msg = base64.b64decode(str_msg)
		str_format = "!h" + str(len(str_msg) - 2) + "s"
		n_msg_id_2 , str_proto = struct.unpack(str_format,str_msg )
		
		str_proto_file = Qstring2utf(self.box_1.currentText() )
		print n_msg_id_2 , str_proto_file
		
		
		n_msg_id_name = self.dict_id_2_proto_name[str_proto_file].get(n_msg_id_2, "none")
		one_proto = self.dict_id_2_proto[str_proto_file].get(n_msg_id_2, None)
		if one_proto == None:
			print "not find obj " 
			return

		one_proto.ParseFromString(str_proto)
		
		print n_msg_id_2 ,type( one_proto ) , str(one_proto)
		
		self.input_1.setText(str(n_msg_id_2) )
		self.out_1.setText(str(one_proto) )
		self.out_2.setText(n_msg_id_name )
		self.out_3.setText( str(len(str_msg)) )
		self.out_4.setText( str(len(str_msg) - 2) )
		
	#测试
	def on_button_2_pressed(self ):
		#950:CN3sARDcJRgBIDk=
		#######
		self.decode_str()
		
		str_msg = Qstring2utf(self.input_2.toPlainText() )
		
		list_ = str_msg.split(":")
		n_msg_id_2 = int(list_[0])
		str_proto = base64.b64decode(list_[1])
		
		str_proto_file = Qstring2utf(self.box_1.currentText() )
		print n_msg_id_2 , str_proto_file
		
		
		n_msg_id_name = self.dict_id_2_proto_name[str_proto_file].get(n_msg_id_2, "none")
		one_proto = self.dict_id_2_proto[str_proto_file].get(n_msg_id_2, None)
		if one_proto == None:
			print "not find obj " 
			return

		one_proto.ParseFromString(str_proto)
		
		print n_msg_id_2 ,type( one_proto ) , str(one_proto)
		
		self.input_1.setText(str(n_msg_id_2) )
		self.out_1.setText(str(one_proto) )
		self.out_2.setText(n_msg_id_name )
		self.out_3.setText( str(len(str_proto) + 2) )
		self.out_4.setText( str(len(str_proto) ) )
		
		
		

#类_初始化
g_thread = None
app = QtGui.QApplication(sys.argv)

ui_test = Uitest()
ui_test.show()
#ui_test.hide()


def OnUiManage():
	#刷新_操作_begin
	#刷新_操作_end

	global g_thread
	g_thread = threading.Timer(0.2, OnUiManage)
	g_thread.start()

#OnUiManage()

sys.exit(app.exec_())

