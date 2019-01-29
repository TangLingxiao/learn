#coding: utf-8
import os
import sys
import base_check

def SetPath():
    global g_old_path
    g_old_path = os.getcwd()
    os.chdir(os.path.split(os.path.realpath(__file__))[0] )

if __name__ == '__main__':
    SetPath()
    os.system('svn up')

    #邮件相关
    sender = 'tanglingxiao@qq.com'
    mypass = 'wwsdravsrznqbcjg'#'mcqmcwakyplobbbd'
    receivers = ['shirao.wang@mokun.net']#,'binbin.huang@mokun.net','chen.liu@mokun.net','zhiyi.wen@mokun.net','tenglong.li@mokun.net','zhaocan.wang@mokun.net','xiaohao.chen@mokun.net']
        
    #扫描列表
    scan_list = ["battlecore", "battlegateserver", "battleserver", "centerserver", "common", 
    "crossserver", "dbserver", "gameserver", "gateserver", "loginserver", "mailserver", 
    "managerserver", "proxyserver", "publicserver"]
    
    #目录
    target_path = '/home/wsr_dir/server/servers/'#'/home/server/server/servers/'
    result_path = '/var/www/html/snk/tscan_result/'#'/home/scan_result/'
    
    cc = base_check.ScanCode(sender, mypass, receivers)
    cc.Run(scan_list, target_path, result_path)
    #os.system('pause')
