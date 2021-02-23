#coding=utf-8

#快捷键：Ctrl+/，注释语句
#快捷键：Ctrl+Alt+O，可清除冗余的语句，例如未使用的import（灰色显示）
#快捷键：添加类，import
#外部变量引用：global xx

import os
import shutil
import time
import re
import colorPrint as cp
import lineNoticeJudge as lnj

import re
import datetime
import socket
import operator
from enum import Enum
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











#=====================================================================================================================


#if(l_var in line ):#fing out l_var
#if(line.find(l_var) != -1):#find out l_var



#==============枚举值
N_LINE_MUT_START = 0# /* 多行注释开始
N_LINE_MUT_END = 1# */ 多行注释结束
N_LINE_SINGLE = 2# // 单行注释
N_BLOCK_START_IF = 3# #if  块注释开始1
N_BLOCK_START_IFDEF = 4# #ifdef  块注释开始2
N_BLOCK_START_IFNDEF = 5# #ifndef  块注释开始3
N_BLOCK_END_ENDIF = 6# #endif  块注释结束
NOTICE_MAX_TYPE = 7
g_notice_tag = ["/*" , "*/" , "//" ,"#if","#ifdef","#ifndef","#endif"]
g_notice_tag_cnt = [0 for x in range(0,NOTICE_MAX_TYPE)]
g_block_notice_appear = False
















#=====================================================================文件查找注释开始
def findVaribleRelation(fileName,l_var):
    #1
    fp = open(fileName)
    text_lines=fp.readlines()
    line_num = 1
    #2
    l_notice_valid = [False,False]
    for line in text_lines:
        #2.1
        l_notice_valid[0] = lnj.lineNoticeJudge(line)
        #2.2
        if(l_notice_valid[0] == False) and (l_notice_valid[1] == False):
            print(line)
        #2.3    
        line_num = line_num+1
        l_notice_valid[1]=l_notice_valid[0]     
       
    #3
    fp.close()
    
#=====================================================================main
def main():
    global cFileDirs
    global g_varibles
    global g_FileDirs
    global g_VaribleNum
    
    l_varavleName = []
    l_fileName = []
    s ="    "

   # print(s.isspace())
    cp.cPrint('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>',"Red")
    cp.cPrint('starting - ' + CUR_PY_DIR + '\\' + os.path.basename(__file__), "Red")
    
    
    findVaribleRelation('function.map','gSleepM_WakeUp_Flag')

    cp.cPrint('ending - '+ CUR_PY_DIR + '\\' + os.path.basename(__file__),"Red")
    cp.cPrint('<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<',"Red")
    
    
#只有当前脚本直接运行时才会执行main()函数，否则不执行（如：本脚本import到其他脚本）
if __name__ == "__main__":
	main()  