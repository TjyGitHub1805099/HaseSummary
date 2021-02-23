#tkinter：Label 、Button 、标签、按钮
#详见：
#1、https://blog.csdn.net/mingshao104/article/details/79591965
#2、https://blog.csdn.net/sinat_41104353/article/details/79313424

#import : 使用import xx 可以修改模块对象的属性（无论属性是不是可变类型）
#from xx import x使用from xx import x 只能修改模块对象的属性是可变类型的（不可变类型不能修改,会发生属性错误）

#===========================================《import》======================================================
import re
import tkinter as tk #tkinter 窗口
from tkinter import filedialog
import first_py_file#自定义的.py
import os
import shutil

curPyDirect = os.getcwd()#获取当前.py路径

#===========================================《window》===========================================
window=tk.Tk()#指定tkinter窗口
window.title('my window')#tkinter窗口名字
window.geometry('600x300')#tkinter窗口大小

#===========================================《Menu》===========================================
#===========================================Menu：创建一个菜单项，类似于导航栏,顶层菜单
menubar=tk.Menu(window)#指定tkinter菜单

#===========================================文件选项：事件执行
def File_New_Event():
    fname = filedialog.askopenfilename(filetypes=( ("Text file", "*.txt*"),("DBC file", "*.dbc*"),("HTML files", "*.html;*.htm")))
def File_Open_Event():
    #FolderPath = filedialog.askdirectory()#打开提示框，选则文件夹，输出文件夹绝对路径
    FilePath = filedialog.askopenfilename(filetypes=( ("DBC file", "*.dbc*"),("Text file", "*.txt*"),("HTML files", "*.html;*.htm")))#打开提示框，选则文件，输出文件绝对路径
    f1 = open(FilePath)
    val = "hi"  # 待查找的字符'hi'
    first_py_file.find_val_in_dbc_file(f1,val)
    f1.close()
    
    #==============删除文件夹Gen
    global curPyDirect
    delDirect = curPyDirect+"\Gen"
    print(delDirect)
    if os.path.isdir(delDirect):
        #os.rmdir(delDirect)#删除空文件夹
        shutil.rmtree(delDirect)#删除非空文件夹
        print('delte Gen Floder success!!\r\n')

    #==============添加文件夹Gen123
    delDirect += "123"
    os.makedirs(delDirect)
    
    #==============添加文件new.txt
    fileName = delDirect+"\\new.txt"
    file= open(fileName,'w')
    file.close()


def File_Save_Event():
    fname = filedialog.askopenfilename(filetypes=( ("Text file", "*.txt*"),("HTML files", "*.html;*.htm")))
def File_SaveAs_Event():
    fname = filedialog.askopenfilename(filetypes=( ("Text file", "*.txt*"),("HTML files", "*.html;*.htm")))  
    
#===========================================Menu：创建菜单项
fmenu1 = tk.Menu(window)
fmenu1.add_command(label='新建',command=None)
fmenu1.add_command(label='打开',command=File_Open_Event)
fmenu1.add_command(label='保存',command=None)
fmenu1.add_command(label='另存为',command=None)

#for item in ['新建', '打开', '保存', '另存为']:
#    fmenu1.add_command(label=item,command=File_Deal_Event)# 如果该菜单是顶层菜单的一个菜单项，则它添加的是下拉菜单的菜单项。

fmenu2 = tk.Menu(window)
for item in ['复制', '粘贴', '剪切']:
    fmenu2.add_command(label=item)

fmenu3 = tk.Menu(window)
for item in ['默认视图', '新式视图']:
    fmenu3.add_command(label=item)

fmenu4 = tk.Menu(window)
for item in ["版权信息", "其他说明"]:
    fmenu4.add_command(label=item)


#===========================================Menu：级联菜单项
# add_cascade 的一个很重要的属性就是 menu 属性，它指明了要把那个菜单级联到该菜单项上，
# 当然，还必不可少的就是 label 属性，用于指定该菜单项的名称
menubar.add_cascade(label="文件", menu=fmenu1)#菜单项：文件
menubar.add_cascade(label="编辑", menu=fmenu2)#菜单项：编辑
menubar.add_cascade(label="视图", menu=fmenu3)#菜单项：视图
menubar.add_cascade(label="关于", menu=fmenu4)#菜单项：关于

# 最后可以用窗口的 menu 属性指定我们使用哪一个作为它的顶层菜单
window.config(menu=menubar)



window.mainloop()#让窗口活起来