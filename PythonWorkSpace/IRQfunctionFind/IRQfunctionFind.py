#coding=utf-8

#��ݼ���Ctrl+/��ע�����
#��ݼ���Ctrl+Alt+O��������������䣬����δʹ�õ�import����ɫ��ʾ��
#��ݼ�������࣬import
#�ⲿ�������ã�global xx

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






#=====================================================================�ļ�����ע�Ϳ�ʼ
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
    
        
        if( l_star_arry_s_len > 1):#�ҵ� /*
            if( l_sprit_arry_len > 1):#�ҵ�//
                if( len(l_sprit_arry[0]) < len(l_star_arry_s[0])):#// ���� /*
                    l_star_valid = False
                    l_line = l_sprit_arry[0]
                else:#/* ���� //
                    l_star_valid = True
                    l_line = l_star_arry_s[0]
            else:#δ�ҵ�//
                l_star_valid = True
                l_line = l_star_arry_s[0]
        else:#δ�ҵ� /*
            if( l_sprit_arry_len > 1):#�ҵ�//
                l_star_valid = False
                l_line = l_sprit_arry[0]
            else:#δ�ҵ�//
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
    
    
#ֻ�е�ǰ�ű�ֱ������ʱ�Ż�ִ��main()����������ִ�У��磺���ű�import�������ű���
if __name__ == "__main__":
	main()  