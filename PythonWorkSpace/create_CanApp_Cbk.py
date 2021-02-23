#tkinter：Label 、Button 、标签、按钮
#详见：
#1、os：https://blog.csdn.net/xxlovesht/article/details/80913193
#2、shutil：https://www.jb51.net/article/157891.htm
#3、tkinter：https://blog.csdn.net/mingshao104/article/details/79591965 
#            https://blog.csdn.net/sinat_41104353/article/details/79313424
#            https://blog.csdn.net/Bugest/article/details/81557112

#import : 使用import xx 可以修改模块对象的属性（无论属性是不是可变类型）
#from xx import x使用from xx import x 只能修改模块对象的属性是可变类型的（不可变类型不能修改,会发生属性错误）

#===========================================《import》======================================================
import re
import os
import shutil
import tkinter as tk 
from tkinter import filedialog


import tkinter.messagebox #弹窗库
import sys 
import datetime
import socket



curPyDirect = os.getcwd()#获取当前fileOperation.py的路径
curSysTime = datetime.datetime.now().strftime('%F %T')#获取当前系统时间 字符类型 str

#===========================================《window》===========================================
window=tk.Tk()#指定tkinter窗口
window.title('my window')#tkinter窗口名字
window.geometry('600x300')#tkinter窗口大小

#===========================================《Menu》===========================================
#for item in ['新建', '打开', '保存', '另存为']:
#    fmenu1.add_command(label=item,command=File_Deal_Event)# 如果该菜单是顶层菜单的一个菜单项，则它添加的是下拉菜单的菜单项。

#===========================================《Menu》 1st：指定一个菜单项，类似于导航栏,顶层菜单
menubar=tk.Menu(window)#指定tkinter菜单

    
    
def File_Open_EventC():
    #FolderPath = filedialog.askdirectory()#打开提示框，选则文件夹，输出文件夹绝对路径
    FilePath = filedialog.askopenfilename(filetypes=( ("C file", "*.c*"),("Text file", "*.txt*"),("HTML files", "*.html;*.htm")))#打开提示框，选则文件，输出文件绝对路径
    fp = open(FilePath, 'r')
    
    flag_1 = 0#原括弧
    slash_char = '/'
    slash_flag = 0
    
    slash_char2 = '/'
    slash_flag2 = 0
    
    star_char='*'
    star_flag = 0
    s1 = []
    for s in fp.readlines():
        #1.排除/* */
        slash_flag = s.find(slash_char)
        #1.1 /
        if (slash_flag != -1 ):#找到了/
            #1.2 *
            star_flag = s.find(star_char)
            if( star_flag!=-1):#找到了*
                if(star_flag - slash_flag == 1):#找到了/*
                    print(s)
                
                star_flag = 0
                slash_flag = 0
                slash_flag2 = 0
            else:
                star_flag = 0
            
            #1.3 /
            slash_flag2 = s.find(slash_char2)
            if (slash_flag2 != -1 ):
                if(slash_flag2 - slash_flag == 1):#找到了//
                    print(s)
                
                star_flag = 0
                slash_flag = 0
                slash_flag2 = 0
            else:
                slash_flag2 = 0
        else:
            slash_flag = 0
               
    fp.close()
    
    
    
    
    
#===========================================《Menu》 2nd：创建菜单栏
#=================第1个菜单项：
fmenu1 = tk.Menu(window)
fmenu1.add_command(label='新建',command=None)
fmenu1.add_command(label='打开',command=File_Open_EventC)
fmenu1.add_command(label='保存',command=None)
fmenu1.add_command(label='另存为',command=None)

#=================第2个菜单项：
fmenu2 = tk.Menu(window)
for item in ['复制', '粘贴', '剪切']:
    fmenu2.add_command(label=item)

#=================第3个菜单项：
fmenu3 = tk.Menu(window)
for item in ['默认视图', '新式视图']:
    fmenu3.add_command(label=item)

#=================第4个菜单项：
fmenu4 = tk.Menu(window)
fmenu4.add_command(label='版权信息',command=None)
fmenu4.add_command(label='其他说明',command=None)

#===========================================《Menu》 3rd：级联菜单栏
# add_cascade 的一个很重要的属性就是 menu 属性，它指明了要把那个菜单级联到该菜单项上，
# 当然，还必不可少的就是 label 属性，用于指定该菜单项的名称
menubar.add_cascade(label="文件", menu=fmenu1)#菜单项：文件
menubar.add_cascade(label="编辑", menu=fmenu2)#菜单项：编辑
menubar.add_cascade(label="视图", menu=fmenu3)#菜单项：视图
menubar.add_cascade(label="关于", menu=fmenu4)#菜单项：关于

#===========================================《Menu》 4th：激活菜单
#最后可以用窗口的 menu 属性指定我们使用哪一个作为它的顶层菜单
window.config(menu=menubar)

#===============================激活窗口
window.mainloop()