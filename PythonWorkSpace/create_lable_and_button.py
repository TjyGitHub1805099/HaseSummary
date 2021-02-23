#tkinter：Label & Button 标签和按钮
#详见：
#1、https://blog.csdn.net/mingshao104/article/details/79591965
#2、https://blog.csdn.net/sinat_41104353/article/details/79313424
import re
import tkinter as tk #tkinter 窗口

#from tkinter.filedialog import askdirectory#
#import : 使用import xx 可以修改模块对象的属性（无论属性是不是可变类型）
#from xx import x使用from xx import x 只能修改模块对象的属性是可变类型的（不可变类型不能修改,会发生属性错误）

#===========================================《window》===========================================
window=tk.Tk()
window.title('my window')#窗口名字
window.geometry('600x300')#窗口大小

#===========================================《Menu》===========================================
#===========================================Menu：创建一个菜单项，类似于导航栏,顶层菜单
menubar=tk.Menu(window)
#===========================================Button：事件执行
def File_Deal_Event():
    global fmenu1#引用本Moulde的全局变量
    if fmenu1.label == '文件':
        print('you have selected file ##')
#===========================================Menu：创建菜单项
fmenu1 = tk.Menu(window)
for item in ['新建', '打开', '保存', '另存为']:
    fmenu1.add_command(label=item)# 如果该菜单是顶层菜单的一个菜单项，则它添加的是下拉菜单的菜单项。

fmenu2 = tk.Menu(window)
for item in ['复制', '粘贴', '剪切']:
    fmenu2.add_command(label=item)

fmenu3 = tk.Menu(window)
for item in ['默认视图', '新式视图']:
    fmenu3.add_command(label=item)

fmenu4 = tk.Menu(window)
for item in ["版权信息", "其他说明"]:
    fmenu4.add_command(label=item)


#===========================================Menu：创建菜单项
# add_cascade 的一个很重要的属性就是 menu 属性，它指明了要把那个菜单级联到该菜单项上，
# 当然，还必不可少的就是 label 属性，用于指定该菜单项的名称
menubar.add_cascade(label="文件", menu=fmenu1)
menubar.add_cascade(label="编辑", menu=fmenu2)
menubar.add_cascade(label="视图", menu=fmenu3)
menubar.add_cascade(label="关于", menu=fmenu4)

# 最后可以用窗口的 menu 属性指定我们使用哪一个作为它的顶层菜单
window.config(menu=menubar)

#===========================================《Label》===========================================
#===========================================Label：变量
Label_A_var = tk.StringVar()# 这时文字变量储存器
#===========================================Label：创建
Label_A = tk.Label(window,
                    text="Radiobutton A",#默认值
                    textvariable=Label_A_var,# 使用 textvariable 替换 text, 因为这个可以变化
                    bg='green', font=('Arial', 12), width=35, height=2)#背景色 字体大小，长，宽
#===========================================Label：加载
Label_A.pack()
#===========================================Label：位置调整
Label_A.place(x=0,y=250)




#===========================================《Button》===========================================
#===========================================Button：全局变量
global_val = False  #定义一个变量，默认 False
#===========================================Button：事件执行
def Button_Tick_Event():
    global global_val#引用本Moulde的全局变量
    if global_val == False:
        global_val = True
        Label_A_var.set('you tick me')#设置标签的文字为 'you tick me'
    else:
        global_val = False
        Label_A_var.set('') #设置文字为空
#===========================================Button：创建
Button_A = tk.Button(window,
    text='Button A',#显示在按钮上的文字
    width=15, height=2,#按钮大小
    command=Button_Tick_Event)#点击按钮式执行的命令
#===========================================Button：加载
Button_A.pack()
#===========================================Button：位置调整
Button_A.place(x=450,y=250)







#===========================================《Radiobutton》===========================================
#===========================================构造一个Radiobutton所需变量
x_position = 150
y_position = 50
you_like_language=[('python',0),('C',1),('C++',2),('Java',3)]
mutualIntVar = tk.IntVar()

#===========================================Radiobutton：事件执行
def you_like_language_fun():
    Label_A_var.set('you like language is : '+you_like_language[mutualIntVar.get()][0])

#===========================================Radiobutton：创建
for l_text,l_value in you_like_language:
    Radiobutton_G = tk.Radiobutton(window, text=l_text, value=l_value, variable=mutualIntVar, command=you_like_language_fun)
    Radiobutton_G.pack()
    Radiobutton_G.place(x=x_position,y=y_position)  # 位置调整x y 坐标
    y_position+=20
you_like_language_fun()



#===========================================《Checkbutton》===========================================
#===========================================构造一个Checkbutton所需变量
Checkbutton_A_Var = tk.IntVar()
Checkbutton_B_Var = tk.IntVar()
#===========================================Checkbutton：事件执行
def Checkbutton_Event():
    print('you have selected a',Checkbutton_A_Var.get())

#===========================================Checkbutton：创建
Checkbutton_A = tk.Checkbutton(window,text = "Checkbutton A",variable = Checkbutton_A_Var,command=Checkbutton_Event(),
                    onvalue = 1, offvalue = 0)
Checkbutton_A.pack()
y_position += 20
Checkbutton_A.place(x=x_position,y=y_position)#位置调整x y 坐标

Checkbutton_B = tk.Checkbutton(window,text = "Checkbutton B",variable = Checkbutton_B_Var,command=Checkbutton_Event(),
                    onvalue = 1, offvalue = 0)
Checkbutton_B.pack()
y_position += 20
Checkbutton_B.place(x=x_position,y=y_position)#位置调整x y 坐标








# #===========================================构造一个Radiobutton所需变量
# StringVar = tk.StringVar()#构造一个整型变量
# #===========================================Radiobutton：事件执行
# def Radiobutton_Event():
#     Label_A_var.set('you have selected '+StringVar.get())
#
# #===========================================Radiobutton：创建
# Radiobutton_A = tk.Radiobutton(window, text="Radiobutton A", variable=StringVar, value='Radiobutton A', command=Radiobutton_Event)
# Radiobutton_A.pack()
# Radiobutton_A.place(x=130,y=130)#位置调整x y 坐标
#
# Radiobutton_B = tk.Radiobutton(window, text="Radiobutton B", variable=StringVar, value='Radiobutton B', command=Radiobutton_Event)
# Radiobutton_B.pack()
# Radiobutton_B.place(x=130,y=150)#位置调整x y 坐标
#
# Radiobutton_C = tk.Radiobutton(window, text="Radiobutton C", variable=StringVar, value='Radiobutton C', command=Radiobutton_Event)
# Radiobutton_C.pack()
# Radiobutton_C.place(x=130,y=170)#位置调整x y 坐标
#
#
# #===========================================《Checkbutton》===========================================
# #===========================================构造一个Checkbutton所需变量
# Checkbutton_A_Var = tk.IntVar()
# Checkbutton_Var = tk.IntVar()
# #===========================================Checkbutton：事件执行
# def Checkbutton_Event():
#     print('you have selected ',Checkbutton_Var.get())
# #===========================================Checkbutton：创建
# Checkbutton_A = tk.Checkbutton(window,text = "Checkbutton A",variable = Checkbutton_A_Var,command=Checkbutton_Event,
#                     onvalue = 1, offvalue = 0)
# Checkbutton_A.pack()
# Checkbutton_A.place(x=130,y=190)#位置调整x y 坐标
#
# Checkbutton_B = tk.Checkbutton(window,text = "Checkbutton B",variable = Checkbutton_Var,command=Checkbutton_Event,
#                     onvalue = 1, offvalue = 0)
# Checkbutton_B.pack()
# Checkbutton_B.place(x=130,y=210)#位置调整x y 坐标
#





window.mainloop()#让窗口活起来