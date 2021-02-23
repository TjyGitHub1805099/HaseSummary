#tkinter：
#详见：
#1、https://blog.csdn.net/mingshao104/article/details/79591965
#2、https://blog.csdn.net/sinat_41104353/article/details/79313424
#3、https://www.cnblogs.com/xiehy/p/10826495.html

#from tkinter.filedialog import askdirectory#
#import : 使用import xx 可以修改模块对象的属性（无论属性是不是可变类型）
#from xx import x使用from xx import x 只能修改模块对象的属性是可变类型的（不可变类型不能修改,会发生属性错误）

import re
import os                               #系统相关，如文件路径
import datetime                         #系统相关，如当前时间
import socket                           #系统相关，如计算机名
import tkinter as tk                    #tkinter 窗口
import shutil
#import tkinter.messagebox
import serial                           #导入串口模块
import serial.tools.list_ports
import create_serial
from tkinter import ttk
from tkinter import messagebox          #弹窗库导入消息盒子模块
from tkinter import scrolledtext        # 导入滚动文本框的模块
from threading import Timer             #导入定时器模块
from tkinter import  *
from tkinter import filedialog

#===========================================《system》===========================================
#=========================================== 系统相关 ===========================================
#===========================================《system》===========================================
curHostName = socket.gethostname()#获取计算机名字
print(curHostName)
curSysTime = datetime.datetime.now().strftime('%F %T')#获取当前系统时间 字符类型 str
print(curSysTime)
curPyDirect = os.getcwd()#获取当前fileOperation.py的路径
print(curPyDirect)

#===========================================《variable》===========================================
#=========================================== 自定义变量 ===========================================
#===========================================《variable》===========================================
x_position = 0
y_position = 0

#===========================================《window》===========================================
#===========================================   窗口   ===========================================
#===========================================《window》===========================================
window=tk.Tk()
window.title('my window')#窗口名字
window.geometry('1200x600')#窗口大小


#=====串口属性选择
g_LableSerialCom = tk.Label(window, text="串口号")
g_LableSerialCom.pack()
x_position += 10
y_position += 10
g_LableSerialCom.place(x=x_position,y=y_position)


def ComBox_Event(*args):   #处理事件，*args表示可变参数
    print(comboxlist.get()) #打印选中的值

comvalue=tk.StringVar()#窗体自带的文本，新建一个值
comboxlist=ttk.Combobox(window,textvariable=comvalue,width=8, height=2) #初始化
comboxlist["values"]=("COM1","COM2","COM3")
comboxlist.current(0)#默认选择第一个
comboxlist.bind("<<ComboboxSelected>>",ComBox_Event)#绑定事件,(下拉列表框被选中时，绑定go()函数)
comboxlist.pack()
x_position += 50
y_position += 0
comboxlist.place(x=x_position,y=y_position)


global_val = False  #定义一个变量，默认 False
#===========================================Button：事件执行
def Button_Tick_Event():
    create_serial.SerialChoice_Event()

#===========================================Button：创建
Button_A = tk.Button(window,
    text='Button A',#显示在按钮上的文字
    width=15, height=2,#按钮大小
    command=Button_Tick_Event)#点击按钮式执行的命令
#===========================================Button：加载
Button_A.pack()
#===========================================Button：位置调整
x_position+=50
y_position+=50
Button_A.place(x=x_position,y=y_position)


#===========================================《window》:激活窗口===========================================
#===========================================《window》:激活窗口===========================================
#===========================================《window》:激活窗口===========================================
#==定义关闭窗口时的提示
def window_closing():
    if messagebox.askokcancel("Quit", "Do you want to quit?"):
        window.destroy()
window.protocol("WM_DELETE_WINDOW", window_closing)
window.mainloop()#让窗口活起来 程序阻塞在此
