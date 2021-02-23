#coding=utf-8

#快捷键：Ctrl+/，注释语句
#快捷键：Ctrl+Alt+O，可清除冗余的语句，例如未使用的import（灰色显示）
#快捷键：添加类，import
#外部变量引用：global xx

import os
import shutil
import time
import re
import msvcrt

import re
import datetime
import socket
import operator


global CUR_PY_DIR
CUR_PY_DIR =  os.getcwd()

Project_Dir = ""

In_Project_Dir = CUR_PY_DIR + "\\" + "src"
Out_Project_Dir = CUR_PY_DIR + "\\" + "dest"

import sys;
sys.path.append(os.getcwd()+"\\"+"__mypy__")

import starNoticeJudge as snj
import colorPrint as cp
import fileFind as fFind



global C_FILE_LIST
C_FILE_LIST = []
C_FILE_LIST_GEN = []
#=====================================================================文件查找注释开始
def findOutUnnecessaryExtern(inDir_fileName):
	if inDir_fileName.endswith('.c'):
		cp.cPrint(inDir_fileName,"Red")
		fp = open(inDir_fileName, errors="ignore")
		text_lines=fp.readlines()
		for line in text_lines:
			if "extern " in line:
				print(line)
				#os.system('pause')



#=====================================================================文件查找注释开始
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
        
    flag = False
    flag1 = False
    if inDir_fileName.endswith('.c'):
        flag = True
        
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
        if 'extern ' in line and '(' not in line and ')' not in line and flag == True:
            print(line)
            flag1 = True
        #2.2
        #2.3    
        line_num = line_num+1
       
    #3
    fp.close()
    fp_back.close()
    s = inDir_fileName + " : remove notice and gen done !! "
    cp.cPrint(s,"Green")
    #pending
    #if flag == True and flag1 ==True:
    #    os.system('pause')
 
def copyFileAndFolderFromInToOut(src_dir,des_dir):
    if os.path.exists(src_dir):
        #1:remove des_dir 
        if os.path.exists(des_dir):
            shutil.rmtree(des_dir)
        #2:create des_dir
        os.makedirs(des_dir)
        #3:copy all file and folder use CMD
        cmd = "xcopy "
        cmd = cmd + src_dir + " " + des_dir + " /e"
        print(cmd)
        os.system(cmd)
        return True
    else:
        cp.cPrint("pleace check project_dir: at ___cfg___ file","Red")
        return False
    
def openCfgFile(dir,file_name):
    global Project_Dir
    __cfg__file = dir + "\\" + file_name
    if os.path.exists(__cfg__file):
        fp = open(__cfg__file, errors="ignore")
        text_lines=fp.readlines()
        for i in text_lines:
            if "project_dir:" in i:
                cfg = []
                cfg = i.split("project_dir:")
                Project_Dir = Project_Dir + cfg[1]
                break
        fp.close()
        
        if not os.path.exists(Project_Dir):
            cp.cPrint("error : -- 'project_dir:'not exists  !!!!","Red")
            print(i)
            cp.cPrint("notice: -- pleace check 'project_dir:' at ___cfg___ file","Red")
            return False
        else:
            return True
    else:
        cp.cPrint("not exists ___cfg___.txt , please created it","Red")
        cp.cPrint("and write such as : project_dir:E:\\workspace\\S32K\\Git\\GIT_N534_MC\\s32k\\N534_MC_APP","Red")
        return False
     

#=====================================================================main
def main():
    cp.cPrint('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>',"Green")
    #cp.cPrint('starting - ' + CUR_PY_DIR + '\\' + os.path.basename(__file__), "Green")  
    #check project dir from __cfg__ ,and check it
    if openCfgFile(CUR_PY_DIR,"__cfg__.txt"):
        #copy file and folder
        copyFileAndFolderFromInToOut(Project_Dir,In_Project_Dir)

        C_FILE_LIST = fFind.findFileDir(In_Project_Dir).split("\n")
        print(C_FILE_LIST)
        for list in C_FILE_LIST:
            if(len(list)>1):
                findOutUnnecessaryExtern(list)
    
    #cp.cPrint('ending - '+ CUR_PY_DIR + '\\' + os.path.basename(__file__),"Green")
    cp.cPrint('<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<',"Green")
    
    
#只有当前脚本直接运行时才会执行main()函数，否则不执行（如：本脚本import到其他脚本）
if __name__ == "__main__":
	main()  