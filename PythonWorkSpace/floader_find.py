#coding:utf-8
#×¢ÊÍ¿É²ÉÓÃºº×Ö

import os
import shutil
import time
import re

CUR_PY_DIR =  os.getcwd()

HC_Folder = "hc_FileList:"+CUR_PY_DIR

C_FolderList = "C_Folder:"+ CUR_PY_DIR
C_FileList = "c_List:"+CUR_PY_DIR

H_FolderList = "H_Folder:"+CUR_PY_DIR
H_FileList = "h_List:"+CUR_PY_DIR

def findFloader(rootDir):
    global HC_Folder
    global C_FolderList
    global C_FileList
    global H_FolderList
    global H_FileList
    try:
        for filename in os.listdir(rootDir):#list file and folde
            if filename.endswith('c'):#if exist .c file
                C_FolderList += "\\" + rootDir + '\r\n'
                C_FileList += "\\" + filename
                #print(C_FileList)
            if os.path.isdir(filename):#judge floader
                findFloader(rootDir+'\\'+filename)
                #continue

    finally:
        print('1');

	
def main():
	print('>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>start')

	findFloader(CUR_PY_DIR)
	print('<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<end')
#if __name__ == "__main__":
#	main()
    
    print("123")