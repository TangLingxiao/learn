



import os
import sys
import MySQLdb
from PyQt4 import QtCore, QtGui, uic
from PyQt4.QtCore import *  
from PyQt4.QtGui import *  

#加载proto
sys.path.append("proto_py/")
import gs_db_pb2
import base_pb2
#配置ui界面
g_str_ui_db = "show_mysql_data.ui"
db_class, base_db_class = uic.loadUiType(g_str_ui_db)


g_role_data = ""
g_hero_data = ""
g_itme_data = ""
g_store_data = ""
g_job_data = ""
g_offline_data = ""
g_friends_data = ""
g_equip_data = ""
g_relation_data = ""
g_game_friends_data = ""
#获取基本数据
def GetBaseData(cursor , str_key):
	global g_role_data
	str_sql = "select `role_id`,`base` from role left join role_ext on id = role_id where account_id = '[key]';"
	str_sql = str_sql.replace("[key]" , str_key)
	cursor.execute(str_sql)
	ret_data = cursor.fetchone()
	
	if ret_data == None:
		print "帐号不存在".decode("utf").encode("gbk")
		g_role_data = "帐号不存在"
		return 0
	
	n_id = ret_data[0]
	str_ = ret_data[1]

	#print len(str_)

	#解析proto字段
	one_proto = gs_db_pb2.db_role_base_data()
	one_proto.ParseFromString(str_)
	
	g_role_data = str(one_proto)

	return n_id
	
#获取英雄
def GetHeroData(cursor , str_key):
	str_sql = "select `hero` from role_ext where role_id = [key];"
	str_sql = str_sql.replace("[key]" , str_key)
	cursor.execute(str_sql)
	ret_data = cursor.fetchone()
	str_ = ret_data[0]
	if str_ == None:
		return
	
	#解析proto字段
	one_proto = gs_db_pb2.db_role_hero()
	one_proto.ParseFromString(str_)
	global g_hero_data
	g_hero_data = str(one_proto)

#获取物品
def GetItemData(cursor , str_key):
	str_sql = "select `item` from role_ext where role_id = [key];"
	str_sql = str_sql.replace("[key]" , str_key)
	cursor.execute(str_sql)
	ret_data = cursor.fetchone()
	str_ = ret_data[0]
	if str_ == None:
		return
	
	#解析proto字段
	one_proto = gs_db_pb2.db_role_item()
	one_proto.ParseFromString(str_)
	global g_itme_data
	g_itme_data = str(one_proto)
	
#获取商店
def GetStoreData(cursor , str_key):
	str_sql = "select `store` from role_ext where role_id = [key];"
	str_sql = str_sql.replace("[key]" , str_key)
	cursor.execute(str_sql)
	ret_data = cursor.fetchone()
	str_ = ret_data[0]
	if str_ == None:
		return
	#解析proto字段
	one_proto = gs_db_pb2.db_role_store_data()
	one_proto.ParseFromString(str_)
	global g_store_data
	g_store_data = str(one_proto)

def GetTaskData(cursor , str_key):
	str_sql = "select `job` from role_ext where role_id = [key];"
	str_sql = str_sql.replace("[key]", str_key)
	cursor.execute(str_sql)
	ret_data = cursor.fetchone()
	str_ = ret_data[0]
	if str_ == None:
		return
	
	#解析proto字段
	one_proto = gs_db_pb2.db_role_job()
	one_proto.ParseFromString(str_)
	global g_job_data
	g_job_data = str(one_proto)

def GetGameFriendsData(cursor , str_key):
	str_sql = "select `friends` from role_ext where role_id = [key];"
	str_sql = str_sql.replace("[key]" , str_key)
	cursor.execute(str_sql)
	ret_data = cursor.fetchone()
	str_ = ret_data[0]
	if str_ == None:
		return
	#解析proto字段
	one_proto = gs_db_pb2.db_role_friends()
	one_proto.ParseFromString(str_)
	global g_game_friends_data
	g_game_friends_data = str(one_proto)

def GetOfflineData(cursor , str_key):
	str_sql = "select `offline_data` from role_ext where role_id = [key];"
	str_sql = str_sql.replace("[key]" , str_key)
	cursor.execute(str_sql)
	ret_data = cursor.fetchone()
	str_ = ret_data[0]
	if str_ == None:
		return
	#解析proto字段
	one_proto = base_pb2.db_offline_data()
	one_proto.ParseFromString(str_)
	global g_offline_data
	g_offline_data = str(one_proto)

def GetFriendsData(cursor , str_key):
	str_sql = "select `offline_friends_data` from role_ext where role_id = [key];"
	str_sql = str_sql.replace("[key]" , str_key)
	cursor.execute(str_sql)
	ret_data = cursor.fetchone()
	str_ = ret_data[0]
	if str_ == None:
		return
	#解析proto字段
	one_proto = base_pb2.db_offline_friends_data()
	one_proto.ParseFromString(str_)
	global g_friends_data
	g_friends_data = str(one_proto)

def GetEquipData(cursor , str_key):
	str_sql = "select `equip` from role_ext where role_id = [key];"
	str_sql = str_sql.replace("[key]" , str_key)
	cursor.execute(str_sql)
	ret_data = cursor.fetchone()
	str_ = ret_data[0]
	if str_ == None:
		return
	#解析proto字段
	one_proto = gs_db_pb2.db_role_equip()
	one_proto.ParseFromString(str_)
	global g_equip_data
	g_equip_data = str(one_proto)

def GetRelationData(cursor , str_key):
	str_sql = "select `soul_hero_relation` from role_ext where role_id = [key];"
	str_sql = str_sql.replace("[key]" , str_key)
	cursor.execute(str_sql)
	ret_data = cursor.fetchone()
	str_ = ret_data[0]
	if str_ == None:
		return
	#解析proto字段
	one_proto = gs_db_pb2.db_role_soul_hero_relation()
	one_proto.ParseFromString(str_)
	global g_relation_data
	g_relation_data = str(one_proto)

def Qstring2utf(str_):
	return unicode(str_).encode("utf")

class UiDB(QtGui.QDialog, db_class):
	def __init__(self, *args):
		super(UiDB, self).__init__(*args)
		
		self.setupUi(self)
		
		self.box1.addItem( "50_db")
		self.box1.addItem( "hbb_db")
		self.box1.addItem( "wsr_db")

		self.box2.addItem( "玩家信息".decode("utf") )
		self.box2.addItem( "物品".decode("utf") )
		self.box2.addItem( "英雄".decode("utf") )
		self.box2.addItem( "任务".decode("utf") )
		self.box2.addItem( "离线".decode("utf") )
		self.box2.addItem( "商店".decode("utf"))
		self.box2.addItem( "好友".decode("utf"))
		self.box2.addItem( "斗魂".decode("utf"))
		self.box2.addItem( "斗魂关系".decode("utf"))
		self.box2.addItem( "game好友".decode("utf"))
		#
		self.on_box1_activated(self.box1.currentText())
		self.is_stop = False

	def closeEvent(self , event):
		self.is_stop = True

	def on_box1_activated(self , num_or_str):
		if type(num_or_str) != type(1):
			str_ = Qstring2utf(num_or_str)
			if str_ == "hbb_db":
				self.in_1.setText("127.0.0.1")
				self.in_2.setText("root")
				self.in_3.setText("123456")
				self.in_4.setText("test")
			elif str_ == "50_db":
				self.in_1.setText("192.168.0.50")
				self.in_2.setText("root")
				self.in_3.setText("123456")
				self.in_4.setText("snk_inner")
			elif str_ == "wsr_db":
				self.in_1.setText("127.0.0.1")
				self.in_2.setText("root")
				self.in_3.setText("123456")
				self.in_4.setText("test")

	def on_box2_activated(self , num_or_str):
		if type(num_or_str) != type(1):
			str_ = Qstring2utf(num_or_str)
			
			if str_ == "玩家信息":
				self.out_1.setText(g_role_data)
			elif str_ == "物品" :
				self.out_1.setText(g_itme_data)
			elif str_ == "英雄" :
				self.out_1.setText(g_hero_data)
			elif str_ == "任务" :
				self.out_1.setText(g_job_data)
			elif str_ == "离线" :
				self.out_1.setText(g_offline_data)
			elif str_ == "商店" :
				self.out_1.setText(g_store_data)
			elif str_ == "好友" :
				self.out_1.setText(g_friends_data)
			elif str_ == "斗魂" :
				self.out_1.setText(g_equip_data)
			elif str_ == "斗魂关系" :
				self.out_1.setText(g_relation_data)
			elif str_ == "game好友" :
				self.out_1.setText(g_game_friends_data)
	#查询
	def on_b1_pressed(self ):
		str_ip = Qstring2utf(self.in_1.text() ) 
		str_acount = Qstring2utf(self.in_2.text() )
		str_pwd = Qstring2utf(self.in_3.text() )
		str_db_name = Qstring2utf(self.in_4.text() )
		str_key = Qstring2utf(self.in_5.text() )
		
		db = MySQLdb.connect(str_ip , str_acount , str_pwd , str_db_name)
		cursor = db.cursor()
		cursor.execute("SET NAMES utf8")
		
		n_id = GetBaseData(cursor , str_key)
		if n_id == 0:
			self.out_1.setText("not found")
			return
			
		GetHeroData(cursor , str(n_id) )
		GetItemData(cursor , str(n_id) )
		GetStoreData(cursor , str(n_id) )
		GetTaskData(cursor , str(n_id) )
		GetOfflineData(cursor , str(n_id) )
		GetFriendsData(cursor , str(n_id))
		GetEquipData(cursor, str(n_id))
		GetRelationData(cursor, str(n_id))
		GetGameFriendsData(cursor, str(n_id))
		#刷新_输出窗口
		self.on_box2_activated( self.box2.currentText() )

app = QtGui.QApplication(sys.argv)
ui_db = UiDB()
ui_db.show()

sys.exit(app.exec_())




