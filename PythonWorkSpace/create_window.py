#创建自己的窗口
#Python自带的可编辑的GUI界面，是一个图像窗口。
#Tkinter是使用 python 进行窗口视窗设计的模块。
#简单的构造，多平台，多系统的兼容性， 能让它成为让你快速入门定制窗口文件的好助手。
#它在 python 窗口视窗模块中是一款简单型的。所以用来入门，熟悉窗口视窗的使用，非常有必要。

import re
import tkinter as tk #tkinter 窗口

window=tk.Tk()
window.title('my window')#窗口名字
window.geometry('300x100')#窗口大小

window.mainloop()#让窗口活起来