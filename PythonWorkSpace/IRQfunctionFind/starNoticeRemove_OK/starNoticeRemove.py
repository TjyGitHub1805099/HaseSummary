#coding=utf-8

#��ݼ���Ctrl+/��ע�����
#��ݼ���Ctrl+Alt+O��������������䣬����δʹ�õ�import����ɫ��ʾ��
#��ݼ�������࣬import
#�ⲿ�������ã�global xx

import os
import shutil
import time
import re



import re
import datetime
import socket
import operator


global CUR_PY_DIR
CUR_PY_DIR =  os.getcwd()
In_Project_Dir = CUR_PY_DIR + "\\" + "in_1234567890_Project"
Out_Project_Dir = CUR_PY_DIR + "\\" + "out_0987654321_Project"

import sys;
sys.path.append(os.getcwd()+"\\"+"__mypy__")

import starNoticeJudge as snj
import colorPrint as cp
import fileFind as fFind



global C_FILE_LIST
C_FILE_LIST = []
C_FILE_LIST_GEN = []

#=====================================================================�ļ�����ע�Ϳ�ʼ
def removeNoticeAndGenFiles(inDir_fileName,outDir_fileName):
    #clear back_file
    if(os.path.isfile(outDir_fileName)):
        cp.cPrint("clear " + outDir_fileName ,"Green")
        fp_back = open(outDir_fileName,mode="w",encoding="GB2312", errors="ignore")
        fp_back.truncate()#clear file
    else:
        print(outDir_fileName)
        cp.cPrint("create " + outDir_fileName ,"Red")
        fp_back = open(outDir_fileName, mode="w", encoding="utf-8")
    
    #1
    fp = open(inDir_fileName, errors="ignore")
    text_lines=fp.readlines()
    #text_lines.splitlines()
    line_num = 1
    #2
    l_notice_valid = False
    l_line = ""
    for line in text_lines:
        #2.1
        l_notice_valid ,l_line= snj.lineNoticeJudge(l_notice_valid,line)
        #2.2
        #print(l_line.strip('\n'))
        #time.sleep(999)
        #print(l_line)
        fp_back.write(l_line)
        #2.2
        #2.3    
        line_num = line_num+1
       
    #3
    fp.close()
    fp_back.close()
    s = inDir_fileName + " : remove notice and gen done !! "
    cp.cPrint(s,"Green")
 

#=====================================================================main
def main():
    cp.cPrint('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>',"Red")
    cp.cPrint('starting - ' + CUR_PY_DIR + '\\' + os.path.basename(__file__), "Red")  
   
    C_FILE_LIST = fFind.findFileDir(In_Project_Dir).split("\n")
    
    for list in C_FILE_LIST:
        if(len(list)>1):
            removeNoticeAndGenFiles(list,list.replace("in_1234567890_Project","out_0987654321_Project"))
    #print(C_FILE_LIST[0])
    #print(fFind.findFileDir(In_Project_Dir))
   
    #removeNoticeAndGenFiles(In_Project_Dir,Out_Project_Dir,'main.c')
    
    cp.cPrint('ending - '+ CUR_PY_DIR + '\\' + os.path.basename(__file__),"Red")
    cp.cPrint('<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<',"Red")
    
    
#ֻ�е�ǰ�ű�ֱ������ʱ�Ż�ִ��main()����������ִ�У��磺���ű�import�������ű���
if __name__ == "__main__":
	main()  