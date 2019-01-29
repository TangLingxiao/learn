import os
import platform
import time
import sys

#全局变量



	
#设置工作目录
g_old_path = ""
def SetPath():
	global g_old_path
	g_old_path = os.getcwd()
	os.chdir(os.path.split(os.path.realpath(__file__))[0] )
	
def ResetPath():
	os.chdir(g_old_path )

def Run():
	SetPath()

	str_cmd = '''rm -rf *.log*'''

	os.system(str_cmd)


if __name__ == "__main__":
	print "begin clear_log.py"
	Run()
	print "end clear_log.py"


