#快捷键：Ctrl+/，注释语句
#快捷键：Ctrl+Alt+O，可清除冗余的语句，例如未使用的import（灰色显示）
#快捷键：添加类，import
#外部变量引用：global xx
import re
import os
import datetime


CUR_PY_ABS_DIR = ""


#获取绝对路径
def getCurAbsDir():
    return os.getcwd()


def main():
    global CUR_PY_ABS_DIR
    CUR_PY_ABS_DIR = CUR_PY_ABS_DIR + getCurAbsDir()
    print(CUR_PY_ABS_DIR)
    print("hello morning!")










#只有当前脚本直接运行时才会执行main()函数，否则不执行（如：本脚本import到其他脚本）
if __name__ == '__main__':
    main()