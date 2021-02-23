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
global CUR_PY_DIR
CUR_PY_DIR =  os.getcwd()






#=====================================================================文件查找注释开始
def findVaribleRelation(fileName,l_var):
    #1
    fp = open(fileName)
    text_lines=fp.readlines()
    #text_lines.splitlines()
    line_num = 1
    #2
    l_notice_valid = False
    l_line = ""
    for line in text_lines:
        #2.1
        l_notice_valid ,l_line= lnj.lineNoticeJudge(l_notice_valid,line)
        #2.2
        print(l_line.strip('\n'))
        #2.2
        #2.3    
        line_num = line_num+1
       
    #3
    fp.close()
 







def lineStarStartNoticeJudge(pre_line_notice_vaild,line):
    l_star_valid = False
    l_line = ""
    
    if(pre_line_notice_vaild == True):
        l_star_valid = True
        l_line = ""
    else:
        l_sprit_arry = line.split("//")
        l_sprit_arry_len = len(l_sprit_arry)#//
        l_star_arry_s = line.split("/*")
        l_star_arry_s_len = len(l_star_arry_s)#/*
        l_star_arry_e = line.split("*/")
        l_star_arry_e_len = len(l_star_arry_e)#*/
    
        
        if( l_star_arry_s_len > 1):#找到 /*
            if( l_sprit_arry_len > 1):#找到//
                if( len(l_sprit_arry[0]) < len(l_star_arry_s[0])):#// 先于 /*
                    l_star_valid = False
                    l_line = l_sprit_arry[0]
                else:#/* 先于 //
                    l_star_valid = True
                    l_line = l_star_arry_s[0]
            else:#未找到//
                l_star_valid = True
                l_line = l_star_arry_s[0]
        else:#未找到 /*
            if( l_sprit_arry_len > 1):#找到//
                l_star_valid = False
                l_line = l_sprit_arry[0]
            else:#未找到//
                l_star_valid = False
                l_line = line
   
    return l_star_valid ,l_line









 
#=====================================================================main
def main():

    """
        l_flag = False
        c = "12/*dnoisad//a/*ds//i"
        d=""
        l_flag , d = lineStarStartNoticeJudge(l_flag,c)


        print(l_flag,"+",d)
    """

    cp.cPrint('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>',"Red")
    cp.cPrint('starting - ' + CUR_PY_DIR + '\\' + os.path.basename(__file__), "Red")
    
    findVaribleRelation('function.map','gSleepM_WakeUp_Flag')

    cp.cPrint('ending - '+ CUR_PY_DIR + '\\' + os.path.basename(__file__),"Red")
    cp.cPrint('<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<',"Red")
    
    
#只有当前脚本直接运行时才会执行main()函数，否则不执行（如：本脚本import到其他脚本）
if __name__ == "__main__":
	main()  