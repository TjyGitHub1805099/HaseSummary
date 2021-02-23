#coding:utf-8
#×¢ÊÍ¿É²ÉÓÃºº×Ö

import os
import shutil
import time
import re

c_file_list = ""
c_file_i = 0

def findFileDir(rootDir):
    global c_file_list
    global c_file_i
    #print(rootDir)
    for name in os.listdir(rootDir):#list file and folde
        if name.endswith('.c'):#if exist .c file
            c_file_list = c_file_list + rootDir+"\\"+name + "\n"
        if os.path.isdir(rootDir+'\\'+name):#judge floader
            findFileDir(rootDir+'\\'+name)
    
    return c_file_list