#coding: utf-8
import os
import sys
import chardet
import io

CRLF = '\r\n'
LF = '\n'

#文件筛选条件
list_pass_dirs = ['thirdparty', 'aes', 'md5', 'config_code_py', '.vs','protocol']
list_pass_files = ['RapidJson.h', 'markupstl.h', 'markupstl.cpp']
list_extentsions = ['.cpp', '.h', '.c', '.hpp']

#对文件转码
def convert_encoding(filename, target_encoding='utf-8'):
    file = open(filename, 'r')
    content = file.read()
    source_encoding = chardet.detect(content)['encoding']
    file.close()
    if source_encoding is None or source_encoding == 'ascii':
        return
    if source_encoding != 'utf-8':
        print source_encoding, filename
        content = content.decode(source_encoding)
        file = io.open(filename, 'w', encoding=target_encoding)
        file.write(content)
        file.close()

def crlf_2_lf(filename, target_encoding='utf-8'):
    file = open(filename, 'rb')
    content = file.read()
    content = content.replace(CRLF,LF)
    file.close()
    print filename
    file = open(filename, 'wb')
    file.write(content)
    file.close()

#列出符合条件的文件
def get_files(path, list_files):
    files = os.listdir(path)
    for file in files:
        if file in list_pass_dirs or file in list_pass_files:
            continue
        file_d = os.path.join(path,file)
        if os.path.isdir(file_d):
            get_files(file_d, list_files)
        elif os.path.isfile(file_d):
            if os.path.splitext(file)[1] in list_extentsions:
                list_files.append(file_d)
        else:
            pass



def Run(list_files, target_encoding = 'utf-8'):
    for file in list_files:
        #convert_encoding(file, target_encoding)
        crlf_2_lf(file, target_encoding);

if __name__ == "__main__":
    #argvs
    file_path = 'E:\server2\lib3rd\sgdp\\include'
    target_encoding = 'utf-8'

    list_files = []
    if os.path.isdir(file_path):
        get_files(file_path, list_files)
    elif os.path.isfile(file_path):
        list_files.append(file_path)

    Run(list_files, target_encoding)

