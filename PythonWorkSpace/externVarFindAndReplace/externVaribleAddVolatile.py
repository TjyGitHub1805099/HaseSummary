#--!coding:utf-8

#快捷键：Ctrl+/，注释语句
#快捷键：Ctrl+Alt+O，可清除冗余的语句，例如未使用的import（灰色显示）
#快捷键：添加类，import
#外部变量引用：global xx

import os
import shutil
import time
import re
import colorPrint as cp
import re
import datetime
import socket
import operator

#strcmp(sStr1,sStr2)
global cFileDirs
global CUR_PY_DIR
CUR_PY_DIR =  os.getcwd()


#===============待处理的.c和.h文件
g_varibles = ''
g_FileDirs = ''
g_VaribleNum = 0
cFileDirs =''
hFileDirs = ''


def findOurVariblesAndItDirs(name):
    global g_varibles
    global g_FileDirs
    global g_VaribleNum
    fp = open(name)
    text_lines=fp.readlines()

    #定义数组的行大小、列大小、及本身
    rows_num = len(text_lines)
    #定义数组
    text_arry = [[] for i in range(rows_num)]
 
    sgl_line_arry = []
    fileName = ""
    fileAbsDir = ""
    
    #分离出变量名 及 路径
    i = 0
    for line in text_lines:
        text_arry[i]=line.split('\t')#分离行
        g_varibles = g_varibles + text_arry[i][1] + '\r\n'#分离出变量名
        
        sgl_line_arry = text_arry[i][2].split(' ')#分离路径1
        fileName = sgl_line_arry[0]
        fileAbsDir = sgl_line_arry[1] + '\\'+fileName
        fileAbsDir = fileAbsDir.replace("(","")
        fileAbsDir = fileAbsDir.replace(")","")
        fileAbsDir = fileAbsDir.replace("\n","")
        
        g_FileDirs = g_FileDirs + fileAbsDir + '\r\n'#分离出文件名
        #print(fileAbsDir)
        
        i = i+1
        
    #print(g_varibles)
    #print(g_FileDirs)

    fp.close()
    return rows_num
    
def addExternToCorFile(fileName,l_var):
    #print(fileName)
    #print(l_var)

    fp = open(fileName)
    text_lines=fp.readlines()
   
    for line in text_lines:
        #print(line)
        if(l_var in line ):
            if('extern' in line):
                if(not('volatile' in line)):
                    print(line)
    fp.close()





  
def main():
    global cFileDirs
    global g_varibles
    global g_FileDirs
    global g_VaribleNum
    
    l_varavleName = []
    l_fileName = []

    cp.cPrint('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>',"Red")
    cp.cPrint('starting - ' + CUR_PY_DIR + '\\' + os.path.basename(__file__), "Red")
    
    
    g_VaribleNum =  findOurVariblesAndItDirs('app.map')
    
    l_varavleName= g_varibles.split('\r\n')#分离文件名
    l_fileName = g_FileDirs.split('\r\n')#分离变量名
    
    i = 0
    for i in range(g_VaribleNum):
        cp.cPrint(l_fileName[i],"Red")
        #print(l_fileName[i])
        addExternToCorFile(l_fileName[i],l_varavleName[i])
        input()
    
    cp.cPrint('ending - '+ CUR_PY_DIR + '\\' + os.path.basename(__file__),"Red")
    cp.cPrint('<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<',"Red")
    
    
#只有当前脚本直接运行时才会执行main()函数，否则不执行（如：本脚本import到其他脚本）
if __name__ == "__main__":
	main()  