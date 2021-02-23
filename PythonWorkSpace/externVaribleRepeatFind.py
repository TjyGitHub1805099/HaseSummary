#coding:utf-8

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
            if(operator.eq(text_arry[i][1],text_arry[i-1][1])):#相同变量
                j = j+1
                if(not operator.eq(c,text_arry[i][1])):#历史记录无相同变量
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
    
    
#只有当前脚本直接运行时才会执行main()函数，否则不执行（如：本脚本import到其他脚本）
if __name__ == "__main__":
	main()  