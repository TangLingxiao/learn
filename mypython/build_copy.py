# -*- coding: utf-8 -*-
import os
import sys
import platform
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

def revert_cmd(str_0):
    os.system("svn revert -R " + str_0)
    os.system("svn update")
    
def commit_cmd(str_0):
    os.system("svn commit " + str_0 + " -m \"update server and config by python\"")

def ch_cmd(str_):
    one_cmd = str_.replace("/", "\\")
    one_cmd = one_cmd.encode('GBK')
    os.chdir(one_cmd)

if __name__ == "__main__":
    SetPath()#
    os.system("svn up")
    os.system("devenv /build \"Release|Win32\" .\\servers\\servers.sln")
    str_dir_name = "Release"
    ch_cmd(g_dst_dir)
    revert_cmd(g_dst_dir.encode('GBK'))
    os.system("python excelToCsv_wsr.py")
    #os.system('cd config && svn commit -m \"update config by python\"')
    for one_ in g_list_server:
        str_dst = g_dst_dir+ "/" + one_
        str_dst2 = str_dst + "/" + str_dir_name
        ch_cmd(g_old_path+'/'+g_str_dst_dir+'/'+one_+'/'+str_dir_name+"/")
        cy_cmd(one_+".exe" , str_dst2)
        ch_cmd(str_dst2)

    #commit_cmd(g_dst_dir.encode('GBK'))
        #ResetPath()
    os.system("pause")