#--!coding:utf-8

#��ݼ���Ctrl+/��ע�����
#��ݼ���Ctrl+Alt+O��������������䣬����δʹ�õ�import����ɫ��ʾ��
#��ݼ�������࣬import
#�ⲿ�������ã�global xx

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


#===============�������.c��.h�ļ�
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

    #����������д�С���д�С��������
    rows_num = len(text_lines)
    #��������
    text_arry = [[] for i in range(rows_num)]
 
    sgl_line_arry = []
    fileName = ""
    fileAbsDir = ""
    
    #����������� �� ·��
    i = 0
    for line in text_lines:
        text_arry[i]=line.split('\t')#������
        g_varibles = g_varibles + text_arry[i][1] + '\r\n'#�����������
        
        sgl_line_arry = text_arry[i][2].split(' ')#����·��1
        fileName = sgl_line_arry[0]
        fileAbsDir = sgl_line_arry[1] + '\\'+fileName
        fileAbsDir = fileAbsDir.replace("(","")
        fileAbsDir = fileAbsDir.replace(")","")
        fileAbsDir = fileAbsDir.replace("\n","")
        
        g_FileDirs = g_FileDirs + fileAbsDir + '\r\n'#������ļ���
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
    
    l_varavleName= g_varibles.split('\r\n')#�����ļ���
    l_fileName = g_FileDirs.split('\r\n')#���������
    
    i = 0
    for i in range(g_VaribleNum):
        cp.cPrint(l_fileName[i],"Red")
        #print(l_fileName[i])
        addExternToCorFile(l_fileName[i],l_varavleName[i])
        input()
    
    cp.cPrint('ending - '+ CUR_PY_DIR + '\\' + os.path.basename(__file__),"Red")
    cp.cPrint('<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<',"Red")
    
    
#ֻ�е�ǰ�ű�ֱ������ʱ�Ż�ִ��main()����������ִ�У��磺���ű�import�������ű���
if __name__ == "__main__":
	main()  