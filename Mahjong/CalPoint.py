# coding: utf-8
import sys
from PyQt4 import QtCore, QtGui, uic
from PyQt4.QtCore import *  
from PyQt4.QtGui import *  
import time
import chardet


ui_main = "main.ui"
main_class, base_main_class = uic.loadUiType(ui_main)

dictQuetou = {
    '数字': 0, 
    '三元': 2,
    '自场风':4, 
    '自风': 2,
    '场风': 2,
    '客风': 0
}

dictMianzi = {
    '中张明刻': 2,
    '中张暗刻': 4,
    '幺九明刻': 4,
    '幺九暗刻': 8,
    '中张明杠': 8,
    '中张暗杠': 16,
    '幺九明杠': 16,
    '幺九暗杠': 32
}

dictPaixing = {
    '单骑': 2,
    '边张': 2,
    '嵌张': 2,
    '两面': 0,
    '双碰': 0
}

dictHule = {
    '自摸': 2,
    '门清荣和': 10,
    '副露荣和': 0
}

dictFeng = {
    1:'东',
    2:'南',
    3:'西',
    4:'北'
}

dictFan = {
    '1':1,
    '2':2,
    '4':3,
    '5':4,
    '6~7':5,
    '8~10':6,
    '11~12':7,
    '13~':8
}

dictQin = {
    'True':True,
    'False':False
}
class UiMain(QtGui.QDialog, main_class):
    def __init__(self, *args):
        super(UiMain, self).__init__(*args)
        self.setupUi(self)
        self.quetoupt = 0
        self.mianzipt = 0
        self.paixingpt = 0
        self.hulept = 0
        self.fanpt = 0
        self.isqin = False
        self.init_all()

    def init_all(self):
        self.init_quetou()
        self.init_mianzi()
        self.init_paixing()
        self.init_hule()
        self.init_ziqin()
        self.init_fan()

    def init_quetou(self):
        for k,v in dictQuetou.items():
            self.quetou.addItem(k.decode('utf'))
        self.on_quetou_activated(1)

    def init_mianzi(self):
        for k,v in dictMianzi.items():
            self.mianzi.addItem(k.decode('utf'))
        self.on_mianzi_activated(1)
    
    def init_paixing(self):
        for k,v in dictPaixing.items():
            self.paixing.addItem(k.decode('utf'))
        self.on_paixing_activated(1)

    def init_hule(self):
        for k,v in dictHule.items():
            self.hule.addItem(k.decode('utf'))
        self.on_hule_activated(1)

    def init_ziqin(self):
        self.qin.addItem('True')
        self.qin.addItem('False')
        self.on_qin_activated(1)

    def init_fan(self):
        for k,v in dictFan.items():
            self.fan.addItem(k.decode('utf'))
        self.on_fan_activated(1)

    def on_quetou_activated(self, param):
        if type(param) != type(1):
            return
        quetou = str(self.quetou.currentText().toUtf8())
        self.quetoupt = dictQuetou[quetou]

    def on_mianzi_activated(self, param):
        if type(param) != type(1):
            return
        mianzi = str(self.mianzi.currentText().toUtf8())
        self.mianzipt = dictMianzi[mianzi]

    def on_paixing_activated(self, param):
        if type(param) != type(1):
            return
        paixing = str(self.paixing.currentText().toUtf8())
        self.paixingpt = dictPaixing[paixing]

    def on_hule_activated(self, param):
        if type(param) != type(1):
            return
        hule = str(self.hule.currentText().toUtf8())
        self.hulept = dictHule[hule]

    def on_fan_activated(self, param):
        if type(param) != type(1):
            return
        fan = str(self.fan.currentText().toUtf8())
        self.hulept = 1

    def on_qin_activated(self, param):
        if type(param) != type(1):
            return
        isqin = str(self.qin.currentText().toUtf8())
        self.isqin = dictQin[isqin]

'''
dictFan = {
    '1':1,
    '2':2,
    '4':3,
    '5':4,
    '6~7':5,
    '8~10':6,
    '11~12':7,
    '13~':8
}
'''
    def on_Cal_pressed(self):
        #先判断番数
        point = 0
        if self.fan == 4:
            point = 8000
            if self.isqin:
                point = 12000
        elif self.fan == 5:
            point = 12000
            if self.isqin:
                point = 16000
        elif self.fan == 6:
            point = 16000
            if self.isqin:
                point = 24000
        elif self.fan == 7:
            point = 24000
            if self.isqin:
                pass

    
if __name__ == "__main__":
	app = QtGui.QApplication(sys.argv)
	main = UiMain()
	main.show()
	sys.exit(app.exec_())

