#tkinter：Label & Button 标签和按钮
#详见：
#1、https://blog.csdn.net/mingshao104/article/details/79591965
#2、https://blog.csdn.net/sinat_41104353/article/details/79313424
#3、https://www.cnblogs.com/xiehy/p/10826495.html

#from tkinter.filedialog import askdirectory#
#import : 使用import xx 可以修改模块对象的属性（无论属性是不是可变类型）
#from xx import x使用from xx import x 只能修改模块对象的属性是可变类型的（不可变类型不能修改,会发生属性错误）

import re
import os
import datetime
import tkinter as tk #tkinter 窗口
from tkinter import ttk
from tkinter import messagebox          #导入消息盒子模块
from tkinter import scrolledtext        # 导入滚动文本框的模块
from threading import Timer             #导入定时器模块
import serial                           #导入串口模块
import serial.tools.list_ports

from tkinter import  *

window = tk.Tk()

#===========================================全局变量===========================================
x_position = 0
y_position = 0
#===========================================《window》：窗口===========================================
#==定义关闭窗口时的提示
def window_closing():
    print("Quit")
    if messagebox.askokcancel("Quit", "Do you want to quit?"):
        window.destroy()


def SerialChoice_Event():
    window=tk.Tk()
    window.title('serial')#窗口名字
    window.geometry('600x300')#窗口大小

    window.protocol("WM_DELETE_WINDOW", window_closing)
    window.mainloop()  # 让窗口活起来 程序阻塞在此
