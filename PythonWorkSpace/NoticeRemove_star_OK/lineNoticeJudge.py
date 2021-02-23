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
def lineStarStartNoticeJudge(pre_line_notice_vaild,line):
    l_star_valid = False
    l_line = ""
    l_line_end = ""
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
        #若存在注释开始 再次寻找注释是否结束
        if( l_star_valid == True):
            l_star_arry_s = line.split("/*",1)#分成2个
            l_star_valid,l_line_end = lineStarEndNoticeJudge(l_star_valid,l_star_arry_s[1])
            l_line = l_line+l_line_end            
    return l_star_valid ,l_line


#=====================================================================*/结束的判断
#以 */符号开始 的判断
#以 */结束的行
def lineStarEndNoticeJudge(pre_line_notice_vaild,line):
    l_star_valid = False
    l_line = ""
    
    if(pre_line_notice_vaild == False):
        l_star_valid = False
        l_line = ""
    else:
        l_sprit_arry = line.split("//")
        l_sprit_arry_len = len(l_sprit_arry)#//
        l_star_arry_s = line.split("/*")
        l_star_arry_s_len = len(l_star_arry_s)#/*
        l_star_arry_e = line.split("*/")
        l_star_arry_e_len = len(l_star_arry_e)#*/
        
        
        if(l_star_arry_e_len > 1):#找到*/
            l_star_valid = False
            l_line = l_star_arry_e[1]
            l_star_valid,l_line=lineStarStartNoticeJudge(l_star_valid,l_line)#再次找start
        else:
            l_star_valid = True
            l_line = ""
            
    return l_star_valid,l_line


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

def lineNoticeJudge(pre_line_notice_vaild,line):
    l_notice_valid = False
    l_line = ""
    
    #过滤 /*   */ 和 过滤 //
    if(pre_line_notice_vaild==True):
        l_notice_valid = True
        l_line = ""
        #寻找注释是否结束
        l_notice_valid ,l_line=lineStarEndNoticeJudge(l_notice_valid,line)
    else:
        #寻找注释是否开始
        l_notice_valid ,l_line=lineStarStartNoticeJudge(l_notice_valid,line)
    return l_notice_valid ,l_line
    