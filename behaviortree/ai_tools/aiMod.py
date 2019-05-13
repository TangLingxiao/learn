# coding: utf-8
import sys
import os
from PyQt4 import QtCore, QtGui, uic
from PyQt4.QtCore import *  
from PyQt4.QtGui import *  
import time
import getpass
try:
    import xml.etree.cElementTree as ET
except ImportError:
    import xml.etree.ElementTree as ET

sys.path.append("base/")
from element import *
import collections

rule = "base/ai_rules.xml"

def SetPath():
    global g_old_path
    g_old_path = os.getcwd()
    os.chdir(os.path.split(os.path.realpath(__file__))[0] )

ai_path = r"../ConfigXlsx/aiconfig/"
ui_main = "base/main.ui"
main_class, base_main_class = uic.loadUiType(ui_main)


class UiMain(QtGui.QDialog, main_class):
    def __init__(self, *args):
        super(UiMain, self).__init__(*args)
        self.setupUi(self)
        self.list = []
        self.out_list = [self.out1,self.out2,self.out3,self.out4,self.out5,self.out6,
        self.out7,self.out8,self.out9,self.out10]
        self.in_list = [self.in1,self.in2,self.in3,self.in4,self.in5,self.in6,self.in7,
        self.in8,self.in9,self.in10]
        self.md_list = [self.md1,self.md2,self.md3,self.md4,self.md5,self.md6,self.md7,
        self.md8,self.md9,self.md10]
        self.tree = ET.parse(rule)
        self.root = self.tree.getroot()
        self.list_params = []
        self.load_xml()

        
    def load_xml(self):
        for one in self.root:
            type_ = one.get('type')
            if type_ is not None:
                self.list.append(type_)
                self.types.addItem(type_)
        
    def on_types_activated(self, param):
        if param == str(self.types.currentText()):
            self.list_params = []
            for one in self.root.getchildren():
                if one.get('type') == param:
                    for k, v in one.attrib.items():
                        self.list_params.append(k)
                    break
            self.list_params.sort(reverse=True)
            for one, out in zip(self.list_params, self.out_list):
                out.setText(one)

    def on_ok_pressed(self):
        ls = os.listdir(ai_path)
        for i in range(0, len(ls)):
            file_ = os.path.join(ai_path, ls[i])
            if os.path.isfile(file_):
                print("%s begin........." %(file_,))
                tree = ET.parse(file_, OrderedXMLTreeBuilder())
                root = tree.getroot()
                self.walkXML(root)
                tree.write(file_)
                print("%s end........." %(file_,))
                

    def walkXML(self, root):
        #deal
        if root.get('type') == str(self.types.currentText()):
            find = True
            for param, before in zip(self.out_list, self.in_list):
                if len(param.toPlainText()) == 0:
                    break
                type_ = root.get(str(param.toPlainText()))
                
                if type_ is None:
                    continue
                if len(before.toPlainText()) == 0:
                    continue
                if str(before.toPlainText()) != type_:
                    find = False
                    break
            if find is True:
                for param, after in zip(self.out_list, self.md_list):
                    if len(after.toPlainText()) == 0:
                        continue
                    root.set(str(param.toPlainText()), str(after.toPlainText()))


        children = root.getchildren()
        if len(children) == 0:
            return
        for child in children:
            self.walkXML(child)
  
if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    main = UiMain()
    main.show()
    sys.exit(app.exec_())

