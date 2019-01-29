#coding: utf-8
import os
import sys
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.header import Header
try:
    import xml.etree.cElementTree as ET
except ImportError:
    import xml.etree.ElementTree as ET

class ScanCode:
    def __init__(self, sender, passwd, recver):
        self.sender = sender
        self.mypass = passwd
        self.receivers = recver
        
        #错误统计
        self.res = {'Critical':0,'Serious':0,'Warning':0,'Information':0, 'Total': 0}
        self.res_critical = []
        self.res_serious = []

        #创建一个带附件的实例
        self.message = MIMEMultipart()
        self.message['From']=Header("auto send by 王诗饶","utf-8")
        self.message['To']=Header("SNK_服务器","utf-8")
        subject="静态代码扫描结果"
        self.message['Subject']=Header(subject, 'utf-8')
        
        #邮件正文
        #content='点击链接下载结果文件，导入tscancode客户端查看. '+'链接：http://192.168.0.49/snk/tscan_result/'
        #self.message.attach(MIMEText(content,'plain','utf-8'))

    #添加附件
    def Attach2Mail(self,attach, attach_name):
        att = MIMEText(open(attach, 'rb').read(), 'base64', 'utf-8')
        att["Content-Type"] = 'application/octet-stream'
        att["Content-Disposition"] = 'attachment; filename = '+attach_name
        self.message.attach(att)
    
    #添加正文
    def AttachContent(self):
        content='静态代码检测结果：'+self.AppendStatistics(self.res)
        if len(self.res_critical) > 0:
            content+='-------------------------Critical------------------------\n'
            for res in self.res_critical:
                content+= res
        if len(self.res_serious) > 0:
            content+='-------------------------Serious------------------------\n'
            for res in self.res_serious:
                content+= res
        self.message.attach(MIMEText(content,'plain','utf-8'))

    #发送邮件
    def SendMail(self):
        smtpObj = smtplib.SMTP_SSL('smtp.qq.com', 465)
        smtpObj.connect("smtp.qq.com", 465)
        smtpObj.login(self.sender, self.mypass)
        smtpObj.sendmail(self.sender, self.receivers, self.message.as_string())
        smtpObj.quit()
    
    #删除旧文件
    def ClearAllResult(self, result_path):
        os.system("rm -rf " + result_path + "*xml")
    
    #扫描工程
    def ScanProject(self,server_name, server_path, result_path):
        str_cmd = "./tscancode --enable=all --xml " + server_path + " 2>" + result_path + server_name + ".xml"
        print str_cmd
        os.system(str_cmd)

    #构造正文详细信息
    def AppendRes(self,level, file, line, msg):
        return "Level: "+level+" File: "+file+" Line: "+line+" Msg: "+msg#+'\n'
    
    #构造正文错误统计
    def AppendStatistics(self, res):
        return "Critical: "+str(res['Critical'])+' Serious: '+str(res['Serious'])+' Warning: '+str(res['Warning'])+' Infomation: '+str(res['Information'])+' Total: '+str(res['Total'])+'\n'
    
    #整理结果
    def AnalyzeXml(self, path):
        tree = ET.parse(path)
        root = tree.getroot()
        errors = root.findall('error')
        #没有错误则删除该文件
        if len(errors) == 0:
            print len(errors)
            #os.remove(path)

        for child in errors:
            key = child.get('severity')
            #只统计Critical 和 Serious 级别的详细信息
            if key == 'Critical' or key == 'Serious':
                res_format= self.AppendRes(key, child.get('file'), child.get('line'), child.get('msg'))
                if key == 'Critical':
                    self.res_critical.append(res_format)
                else:
                    self.res_serious.append(res_format)

            self.res[key] += 1
            self.res['Total'] += 1
    
    def Run(self, scan_list, target_path, result_path):
        #删除旧文件
        self.ClearAllResult(result_path)
        #开始扫描
        for one in scan_list:
            self.ScanProject(one, target_path + one, result_path)
        #处理结果
        lsrst = os.listdir(result_path)
        for i in range(0, len(lsrst)):
            path = os.path.join(result_path, lsrst[i])
            if os.path.isfile(path):
                #添加附件
                #self.Attach2Mail(path, lsrst[i])
                #处理xml
                self.AnalyzeXml(path)

        self.AttachContent()
        #发送邮件
        self.SendMail()
        