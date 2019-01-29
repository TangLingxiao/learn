
import sys
import os
from PyQt4 import QtCore, QtGui, uic
from PyQt4.QtCore import *  
from PyQt4.QtGui import *  
import sftp

reload(sys);
sys.setdefaultencoding("utf8")

g_str_ui_notice = "notice.ui"
notice_class, base_notice_class = uic.loadUiType(g_str_ui_notice)

class UINotice(QtGui.QDialog, notice_class):
    def __init__(self, *args):
        super(UINotice, self).__init__(*args)
        self.setupUi(self)
        self.openfile_name = ''
        self.name = ''

    def openfile(self):
        self.openfile_name = QFileDialog.getOpenFileName(self,'select','','XML files(*.xml)')
        self.name = str(self.openfile_name.split('/')[-1]).decode('utf8')

    def on_select_pressed(self):
        self.openfile()
        self.out.setText(self.openfile_name)

    def on_upload_pressed(self):
        local = str(self.openfile_name).decode('utf8').replace('/',"\\")
        result = sftp.sftp_upload(sftp.host,sftp.port,sftp.username,sftp.password,local,str(sftp.remote+self.name))
        QMessageBox.information(self, "result", result)

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    ui_main = UINotice()
    ui_main.show()
    sys.exit(app.exec_())

