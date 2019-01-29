#coding: utf-8
import os
import sys
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.header import Header

def SetPath():
    global g_old_path
    g_old_path = os.getcwd()
    os.chdir(os.path.split(os.path.realpath(__file__))[0] )

sender = '723654638@qq.com'
mypass = 'mcqmcwakyplobbbd'
receivers = ['shirao.wang@mokun.net','binbin.huang@mokun.net','chen.liu@mokun.net','zhiyi.wen@mokun.net','tenglong.li@mokun.net','zhaocan.wang@mokun.net','xiaohao.chen@mokun.net']

#创建一个带附件的实例
message = MIMEMultipart()
message['From']=Header("AUTO","utf-8")
message['To']=Header("SNK_ALL","utf-8")
subject="静态代码扫描结果,可以导入tscancode客户端打开查看"
message['Subject']=Header(subject, 'utf-8')
#邮件正文
message.attach(MIMEText('各个工程的结果见附件','plain','utf-8'))

#添加附件
def Attach2Mail(attach, attach_name):
    att = MIMEText(open(attach, 'rb').read(), 'base64', 'utf-8')
    att["Content-Type"] = 'application/octet-stream'
    att["Content-Disposition"] = 'attachment; filename = '+attach_name
    message.attach(att)

#发送邮件
def SendMail(sender, receivers, message):
    smtpObj = smtplib.SMTP_SSL('smtp.qq.com', 465)
    smtpObj.connect("smtp.qq.com", 465)
    smtpObj.login(sender, mypass)
    smtpObj.sendmail(sender, receivers, message.as_string())
    smtpObj.quit()



def ClearAllResult(result_path):
    os.system("rm -rf " + result_path + "*xml")

#扫描工程
def ScanProject(server_name, server_path, result_path):
    str_cmd = "./tscancode --enable=all --xml-version=2 " + server_path + " 2>" + result_path + server_name + ".xml"
    os.system(str_cmd)

def Run():
    #扫描列表
    scan_list = ["battlecore", "battlegateserver", "battleserver", "centerserver", "common", 
"crossserver", "dbserver", "gameserver", "gateserver", "loginserver", "mailserver", 
"managerserver", "proxyserver", "publicserver"]

    #目录
    target_path = '/home/server/server/servers/'
    result_path = '/home/scan_result/'

    #删除旧文件
    ClearAllResult(result_path)
    
    #开始扫描
    for one in scan_list:
        ScanProject(one, target_path + one, result_path)

    #添加附件
    
    lsrst = os.listdir(result_path)
    for i in range(0, len(lsrst)):
        path = os.path.join(result_path, lsrst[i])
        if os.path.isfile(path):
            Attach2Mail(path, lsrst[i])
    
    #发送邮件
    SendMail(sender, receivers, message)


if __name__ == '__main__':
    SetPath()
    os.system('svn up')
    Run()
    #os.system('pause')
