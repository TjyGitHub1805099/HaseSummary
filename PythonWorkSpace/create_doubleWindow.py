import tkinter as tk
from tkinter import messagebox          #弹窗库导入消息盒子模块
'''松耦合'''
# 弹窗类
class MyWindow_Slave(tk.Toplevel):
  def __init__(self):#初始化
    super().__init__()
    #1、弹窗显示在主窗顶层
    self.attributes("-topmost", 1)
    self.wm_attributes("-topmost", 1)
    #2、弹窗名字
    self.title('Serial Options')
    #3、弹窗大小及位置
    sw = self.winfo_screenwidth()# 获取屏幕长宽
    sh = self.winfo_screenheight()# 获取屏幕宽宽
    ww = 600# 所需串口长度
    wh = 300# 所需串口宽度
    self.geometry("%dx%d+%d+%d" % (ww, wh, ((sw - ww) / 2), ((sh - wh) / 2)))#新窗口大小(居中)
    #4、弹窗内容设定
    self.setup_UI()

  def setup_UI(self):
   # 第一行（两列）
    row1 = tk.Frame(self)
    row1.pack(fill="x")#
    tk.Label(row1, text='Port:', width=8).pack(side=tk.LEFT)
    self.Port = tk.StringVar()
    tk.Entry(row1, textvariable=self.Port, width=10).pack(side=tk.LEFT)
    # 第二行
    row2 = tk.Frame(self)
    row2.pack(fill="x", ipadx=1, ipady=1)
    tk.Label(row2, text='Baud rete:', width=8).pack(side=tk.LEFT)
    self.BaudRate = tk.IntVar()
    tk.Entry(row2, textvariable=self.BaudRate, width=10).pack(side=tk.LEFT)
    # 第二行
    row3 = tk.Frame(self)
    row3.pack(fill="x", ipadx=1, ipady=1)
    tk.Label(row3, text='Data bits:', width=8).pack(side=tk.LEFT)
    self.DataBits = tk.IntVar()
    tk.Entry(row3, textvariable=self.DataBits, width=10).pack(side=tk.LEFT)
    # 第二行
    row4 = tk.Frame(self)
    row4.pack(fill="x", ipadx=1, ipady=1)
    tk.Label(row4, text='Parity:', width=8).pack(side=tk.LEFT)
    self.Parity = tk.StringVar()
    tk.Entry(row4, textvariable=self.Parity, width=10).pack(side=tk.LEFT)
    # 第二行
    row5 = tk.Frame(self)
    row5.pack(fill="x", ipadx=1, ipady=1)
    tk.Label(row5, text='Stop bits:', width=8).pack(side=tk.LEFT)
    self.StopBits = tk.IntVar()
    tk.Entry(row5, textvariable=self.StopBits, width=10).pack(side=tk.LEFT)
    # 第三行
    row3 = tk.Frame(self)
    row3.pack(fill="x")
    tk.Button(row3, text="取消", command=self.cancel).pack(side=tk.RIGHT)
    tk.Button(row3, text="确定", command=self.ok).pack(side=tk.RIGHT)
    self.protocol("WM_DELETE_WINDOW", self.window_closing)

  def ok(self):
    self.userinfo = [self.Port.get(), self.BaudRate.get(), self.DataBits.get(), self.Parity.get(), self.StopBits.get()]  # 设置数据
    self.destroy()#销毁窗口
  def cancel(self):
    self.userinfo = None # 空！
    self.destroy()

  def window_closing(self):
    self.wm_attributes("-topmost", 0)  # 取消本窗口置顶
    if messagebox.askokcancel("Quit Setup", "Do you want to quit setup?"):#确认 Quit 事件
      self.userinfo = None  # 空！
      self.destroy()
    else:#取消 Quit 事件
        self.wm_attributes("-topmost", 1)#本窗口置顶


# 主窗类
class MyWindow_Host(tk.Tk):
  def __init__(self):
    super().__init__()
    #1、主窗名字
    self.title('My Window')
    #2、主窗大小及位置
    sw = self.winfo_screenwidth()# 获取屏幕长宽
    sh = self.winfo_screenheight()# 获取屏幕宽宽
    ww = 1200# 所需串口长度
    wh = 600# 所需串口宽度
    self.geometry("%dx%d+%d+%d" % (ww, wh, ((sw - ww) / 2), ((sh - wh) / 2)))#新窗口大小(居中)
    #类的参数
    self.Port = 'COM1'#端口名字
    self.BaudRate = 9600#波特率
    self.DataBits = 8#8位数据位
    self.Parity = 'None'#无校验
    self.StopBits = 1#1个停止位
    self.SerialSet = False#用于设置功能互斥
    #主窗内容设定
    self.setup_UI()

  def setup_UI(self):
    # # 第一行（两列）
    # row1 = tk.Frame(self)
    # row1.pack(fill="x")
    # tk.Label(row1, text='Port:', width=8,justify='left').pack(side=tk.LEFT)
    # self.l1 = tk.Label(row1, text=self.Port, width=10,justify='right')
    # self.l1.pack(side=tk.LEFT)
    # # 第二行
    # row2 = tk.Frame(self)
    # row2.pack(fill="x")
    # tk.Label(row2, text='Baud rete:', width=8).pack(side=tk.LEFT)
    # self.l2 = tk.Label(row2, text=self.BaudRate, width=10)
    # self.l2.pack(side=tk.LEFT)
    # # 第三行
    # row3 = tk.Frame(self)
    # row3.pack(fill="x")
    # tk.Label(row3, text='Data bits:', width=8).pack(side=tk.LEFT)
    # self.l3 = tk.Label(row3, text=self.DataBits, width=10)
    # self.l3.pack(side=tk.LEFT)
    # # 第三行
    # row4 = tk.Frame(self)
    # row4.pack(fill="x")
    # tk.Label(row4, text='Parity:', width=8).pack(side=tk.LEFT)
    # self.l4 = tk.Label(row4, text=self.Parity, width=10)
    # self.l4.pack(side=tk.LEFT)
    # # 第三行
    # row5 = tk.Frame(self)
    # row5.pack(fill="x")
    # tk.Label(row5, text='Stop bits:', width=8).pack(side=tk.LEFT)
    # self.l5 = tk.Label(row5, text=self.StopBits, width=10)
    # self.l5.pack(side=tk.LEFT)
    # 第三行
    row3 = tk.Frame(self)
    row3.pack(fill="x")
    tk.Button(row3, text="Serial Options", command=self.setup_config).pack(side=tk.LEFT)

  # 设置参数
  def setup_config(self):
      if self.SerialSet is False:
        self.SerialSet = True#自定义，防止再次点击设置
        # 接收弹窗的数据
        res = self.ask_userinfo()
        #print(res)
        if res is None: return
        # 更改参数
        self.Port, self.BaudRate, self.DataBits, self.Parity, self.StopBits = res
        # 更新界面里面数据
        print(self.Port)
        print(self.BaudRate)
        print(self.DataBits)
        print(self.Parity)
        print(self.StopBits)
        # self.l1.config(text=self.Port)
        # self.l2.config(text=self.BaudRate)
        # self.l3.config(text=self.DataBits)
        # self.l4.config(text=self.Parity)
        # self.l5.config(text=self.StopBits)
  # 弹窗
  def ask_userinfo(self):
    inputDialog = MyWindow_Slave()
    self.wait_window(inputDialog) # 这一句很重要！！！
    # 使能主窗口再设置
    self.SerialSet = False
    return inputDialog.userinfo

if __name__ == '__main__':
  app = MyWindow_Host()
  app.mainloop()