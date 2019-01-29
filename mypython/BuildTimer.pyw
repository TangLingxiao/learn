# -*- coding: utf-8 -*-
import os
import sys
import platform
import datetime

reload(sys);
sys.setdefaultencoding("utf8")
def SetPath():
    global g_old_path
    g_old_path = os.getcwd()
    os.chdir(os.path.split(os.path.realpath(__file__))[0] )
	
def ResetPath():
    os.chdir(g_old_path )

g_str_dst_dir = "bin"
g_list_server = ["gameserver","mailserver","centerserver","dbserver","loginserver","managerserver","gateserver","crossserver","publicserver"]
g_dst_dir = "E:/intermediate/snk_私服/bin"

def cy_cmd(str_0 , str_1):
    one_cmd = str_0 + " " + str_1
    one_cmd = one_cmd.replace("/" , "\\")
    one_cmd = one_cmd.encode('GBK')
    os.system("copy /Y " + one_cmd)

def ch_cmd(str_):
    one_cmd = str_.replace("/", "\\")
    os.chdir(one_cmd)
def Run():
    #os.system("python build&copy.py")
    SetPath()#
    os.system("svn up")
    os.system("devenv /build \"Release|Win32\" .\\servers\\servers.sln")
    str_dir_name = "Release"
    for one_ in g_list_server:
        str_dst = g_dst_dir+ "/" + one_
        str_dst2 = str_dst + "/" + str_dir_name
        ch_cmd(g_old_path+'/'+g_str_dst_dir+'/'+one_+'/'+str_dir_name+"/")
        cy_cmd(one_+".exe" , str_dst2)
        ResetPath()

if __name__ == '__main__':
    Run()
    g_next_time = datetime.datetime.now() + datetime.timedelta(hours = 2)
    while True:
        now = datetime.datetime.now()
        if now == g_next_time:
            Run()
            g_next_time = now + datetime.timedelta(hours = 2)

