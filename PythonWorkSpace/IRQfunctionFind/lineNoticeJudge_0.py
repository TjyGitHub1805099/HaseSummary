#coding:utf-8
#可以采用汉字注释

import os
import shutil
import time
import re
import ctypes,sys 


# /* */ 必须连在一起才有效
# #if #ifdef #ifndef #endif 可以分开#
   
#=====================================================================/*开始的判断
#以 /*符号开始 的判断 
#以/*开始的行
def lineStarStartNoticeJudge(line):
    l_star_valid = False
    l_star_arry = []
    l_star = line.find("/*")#查找#
    
    ##以 /* 符号开始 的判断 
    if( -1 != l_star ):#找到/*
        l_star_arry = line.split("/*")#以/*号分割
        if(len(l_star_arry[0])==0):#/*在最开始
            l_star_valid = True
        if( l_star_arry[0].isspace() ):# /*号前面全是空格
            l_star_valid = True
    
    return l_star_valid


#=====================================================================*/结束的判断
#以 */符号开始 的判断
#以 */结束的行
def lineStarEndNoticeJudge(line):
    l_star_valid = False
    l_star = line.find("*/")#查找*/
    
    ##以 */符号结束 的判断 
    if( -1 != l_star ):#找到*/
        l_star_arry = []
        l_star_arry_len = 0
        l_star_arry = line.split("*/")#以#号分割
        l_star_arry_len = len(l_star_arry)
        if(len(l_star_arry[l_star_arry_len-1])==0) or (l_star_arry[l_star_arry_len-1].isspace()):#内容全空格
            l_star_valid = True
    return l_star_valid


#=====================================================================#开始的判断

#以 #注释/条件编译 开始 的判断 
#以这些开始的行：#if #ifdef #ifndef  
#以这些开始的行：# if # ifdef # ifndef  
def lineWellStartNoticeJudge(line):
    l_well_notice_valid = False
    l_well_valid = False
    l_well_arry = []
    l_well = line.find('#')#查找#
    
    ##以 #符号开始 的判断 
    if( -1 != l_well ):#找到#
        l_well_arry = []
        l_well_arry_len = 0
        l_well_arry = line.split("#")#以#号分割
        
        
        if(len(l_well_arry[0])==0):# #在最开始
            l_well_valid = True
        if( l_well_arry[0].isspace() ):# #号前面全是空格
            l_well_valid = True

        if(l_well_valid):
            # #defined ,#   defined
            l_well_arry_defined = []
            l_well_arry_defined = l_well_arry[1].split("defined")# defined 分割
            if(len(l_well_arry_defined) != 1):
                if(len(l_well_arry_defined[0])==0) or \
                  (l_well_arry_defined[0].isspace()):#
                        l_well_notice_valid = True

            # #ifndef ,#   ifndef
            l_well_arry_ifndef = []
            l_well_arry_ifndef = l_well_arry[1].split("ifndef")# ifndef 分割
            if(len(l_well_arry_ifndef) != 1):
                if(len(l_well_arry_ifndef[0])==0) or \
                  (l_well_arry_ifndef[0].isspace()):#
                        l_well_notice_valid = True
                        
            # #ifdef ,#   ifdef
            l_well_arry_ifdef = []
            l_well_arry_ifdef = l_well_arry[1].split("ifdef")# ifdef 分割
            if(len(l_well_arry_ifdef) != 1):
                if(len(l_well_arry_ifdef[0])==0) or \
                  (l_well_arry_ifdef[0].isspace()):#
                        l_well_notice_valid = True
    
            # #if ,#   if
            l_well_arry_if = []
            l_well_arry_if = l_well_arry[1].split("if")# if 分割
            if(len(l_well_arry_if) != 1):
                if(len(l_well_arry_if[0])==0) or \
                  (l_well_arry_if[0].isspace()):#
                        l_well_notice_valid = True
    
    return l_well_notice_valid

#以 #注释/条件编译 开始 的判断 
#以这些开始的行：#endif 
def lineWellEndNoticeJudge(line):
    l_well_notice_valid = False
    l_well_valid = False
    l_well_arry = []
    l_well = line.find('#')#查找#
    
    ##以 #符号开始 的判断 
    if( -1 != l_well ):#找到#
        l_well_arry = []
        l_well_arry_len = 0
        l_well_arry = line.split("#")#以#号分割

        if(len(l_well_arry[0])==0):# #在最开始
            l_well_valid = True
        if( l_well_arry[0].isspace() ):# #号前面全是空格
            l_well_valid = True

        if(l_well_valid):
            # #endif ,#   endif
            l_well_arry_endif = []
            l_well_arry_endif = l_well_arry[1].split("endif")# endif 分割
            if(len(l_well_arry_endif) != 1):
                if(len(l_well_arry_endif[0])==0) or \
                  (l_well_arry_endif[0].isspace()):#
                        l_well_notice_valid = True

    return l_well_notice_valid

#=====================================================================文件是否注释

def lineNoticeJudge(line):
    l_notice = False
    if not hasattr(lineNoticeJudge, 'l_notice_valid'):
        lineNoticeJudge.l_notice_valid = False
    if not hasattr(lineNoticeJudge, 'l_star_notice_valid'):
        lineNoticeJudge.l_star_notice_valid = False
    if not hasattr(lineNoticeJudge, 'l_well_notice_valid'):
        lineNoticeJudge.l_well_notice_valid = False
        
    #整个文件 未出现 注释
    if(lineNoticeJudge.l_notice_valid == False):
        if(lineStarStartNoticeJudge(line)):#查找以/* */为注释的段
            lineNoticeJudge.l_star_notice_valid = True#找到以/* */为注释的段
            lineNoticeJudge.l_notice_valid = True
            #print("/*...*/line start at:",line_num)
        else:
            if(lineWellStartNoticeJudge(line)):#找到以#if...#endif ,#ifdef...#endif ,#ifndef...#endif为注释段
                lineNoticeJudge.l_well_notice_valid = True
                lineNoticeJudge.l_notice_valid = True
                #print("#..line start at:",line_num)
    #整个文件 出现 注释
    l_notice = lineNoticeJudge.l_notice_valid
    if(lineNoticeJudge.l_notice_valid == True):
        if(lineNoticeJudge.l_star_notice_valid == True):
            if(lineStarEndNoticeJudge(line)):#查找以*/结束的行
                lineNoticeJudge.l_star_notice_valid = False#找到以*/结束的行
                lineNoticeJudge.l_notice_valid = False
                #print("/*...*/line end at:")
        if(lineNoticeJudge.l_well_notice_valid == True):
            if(lineWellEndNoticeJudge(line)):
                lineNoticeJudge.l_well_notice_valid = False
                lineNoticeJudge.l_notice_valid = False
                #print("#..line end at:")
    
    if(l_notice == lineNoticeJudge.l_notice_valid):
        return lineNoticeJudge.l_notice_valid
    else:
        return l_notice
