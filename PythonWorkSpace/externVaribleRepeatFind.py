#coding:utf-8

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


CUR_PY_DIR =  os.getcwd()


'''
def pinrtMap(name):
    rodatacnt = 0
    fp = open(name)
    map_data=fp.readlines()
    pattern = 'rodata'
    for line in map_data:
        if (line.find(".rodata") != -1):
            rodatacnt=rodatacnt+1
        if (line.find(".rodata") != -1):
            rodatacnt=rodatacnt+1
    print('total varible number :')
    print('rodata = ',rodatacnt)

    fp.close()
    return cnt
    
    
def findRepeatExtern22(name):
    fileAbsDir = ""
    fileName = ""
    fileVarible = ""
    
    fp = open(name)
    text_lines=fp.readlines()
    #print("total extern varible:",len(text_lines))
 
    i=0
    line_num = len(text_lines)
    rows_num = 10
    text_arry = [[] for i in range(line_num)]
    text_arry_sgl = []
    i=0
    j = 0
    c = ''
    #fileAbsDir ="E123"
    #fileAbsDir = fileAbsDir.replace('E','AAA')
    #print(fileAbsDir)
    for line in text_lines:
        text_arry[i]=line.split('\t')
        fileVarible = text_arry[i][1]
        text_arry_sgl = text_arry[i][2].split(' ')
        fileName = text_arry_sgl[0]
        fileAbsDir = text_arry_sgl[1] + '\\'+fileName
        fileAbsDir = fileAbsDir.replace("(","")
        fileAbsDir = fileAbsDir.replace(")","")
        fileAbsDir = fileAbsDir.replace("\n","")
        print(fileAbsDir)
        #print(fileName)

    fp.close()
    #===========================================================================
    fp = open(fileAbsDir)
    text_lines=fp.readlines()
    print(fileVarible)
    for line in text_lines:
        if('extern' in line):
            if(fileVarible in line ):
                if(not('volatile' in line)):
                    print(line)
    fp.close()

'''

def findRepeatExtern(name):
    fp = open(name)
    text_lines=fp.readlines()
    print("total extern varible:",len(text_lines))
 
    i=0
    line_num = len(text_lines)
    rows_num = 10
    text_arry = [[] for i in range(line_num)]
    
    i=0
    j = 0
    c = ''
    for line in text_lines:
        text_arry[i]=line.split('\t')
        #print(text_arry[i][1])
        
        if( i > 0 ):
            if(operator.eq(text_arry[i][1],text_arry[i-1][1])):#��ͬ����
                j = j+1
                if(not operator.eq(c,text_arry[i][1])):#��ʷ��¼����ͬ����
                    c = text_arry[i][1]
                    print(text_arry[i][1])
        
        i = i+1
     
    print("\r\n\r\ntotal extern varible:",len(text_lines))
    print("repetition varible:",j)
    fp.close()
  
def main():
    cp.cPrint('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>',"Red")
    cp.cPrint('starting - ' + CUR_PY_DIR + '\\' + os.path.basename(__file__), "Red")
    
    findRepeatExtern('app.map')
    
    cp.cPrint('ending - '+ CUR_PY_DIR + '\\' + os.path.basename(__file__),"Red")
    cp.cPrint('<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<',"Red")
    
    
#ֻ�е�ǰ�ű�ֱ������ʱ�Ż�ִ��main()����������ִ�У��磺���ű�import�������ű���
if __name__ == "__main__":
	main()  