#快捷键：Ctrl+/，注释语句
#快捷键：Ctrl+Alt+O，可清除冗余的语句，例如未使用的import（灰色显示）
#快捷键：添加类，import
import re
import datetime
import socket

#===================替换test.txt里面的hello为hi
from unittest.test.test_result import __init__
def replace_hi_for_hello_in_myhellofile():
    f1 = open('test.txt')
    f2 = open('myhello.txt','r+')
    for s in f1.readlines():
        f2.write(s.replace('hello','hi'))
    f1.close()
    f2.close()

#===================查找532.dbc里面的字符串
def find_val_in_dbc_file(f1,val):

    local_val = False  #定义一个局部变量，默认 False
    for s in f1.readlines():
        if val in s:
            local_val=True
            print('find val of:\r\n',val,'\r\nsuccess!')
            
    if local_val == False:
        print('not find out')


def find_val_in_dbc_file2(FilePath, val):
    linenum = 0
    f1 = open(FilePath, 'r')  # 打开文件 追加文本
    local_val = False  # 定义一个局部变量，默认 False
    for s in f1.readlines():
        linenum = linenum + 1
        if val in s:
            local_val = True
            print('find val of: ', val, ' success ','\r\nat line:',linenum)
    f1.close()
    if local_val == False:
        print('not find out')


def append_change_list(FilePath, val):
    linenum = 0
    text_lines = []
    f1 = open(FilePath, 'r') # 打开文件
    for s in f1.readlines():
        linenum = linenum + 1
        if val in s:
            hostname = socket.gethostname()
            curSysTime = datetime.datetime.now().strftime('%F %T')  # 获取当前系统时间 字符类型 str
            text_lines.append(hostname+curSysTime)
        text_lines.append(s)
    f1.close()
    s = '1111111\n'.join(text_lines)
    f1 = open(FilePath, 'w')  # 打开文件
    f1.write(s)
    f1.close()

#===================查找532.dbc里面的METER发送的报文
def find_recvByMeter_in_dbc_file():
    head_string = "BO_ "#代表新的：CAN ID
    end_string = "METER\n"#代表是METER发送

    dbc_f=open('532.dbc','r')#只读
    dbc_data=dbc_f.readlines()
    dbc_f.close()

    out_f=open('532_out.txt',"r+")#读写
    out_f.truncate()#清空文件
    for line in dbc_data:
        if isinstance(line, str):#检查是否为字符类型
            if line.startswith(head_string): #检查开头是：BO_
                if line.endswith(end_string):#检查结尾是：METER
                    arr = line.split( )#按空格分隔
                    out_f.writelines(line)
                    print(line)
                    out = '/*MessageID: '
                    out = out + arr[1]
                    print(out)

    out_f.close()
