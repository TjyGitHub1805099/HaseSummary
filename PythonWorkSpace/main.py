#快捷键：Ctrl+/，注释语句
#快捷键：Ctrl+Alt+O，可清除冗余的语句，例如未使用的import（灰色显示）
#快捷键：添加类，import
import re
import datetime
import first_py_file

def main():
    print('main start at ',datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S %A'))
    f1 = open('532.dbc')
    val = 'BU_: APA S_BOX LCM METER DCT VCM SHVM PBD DVR AVM STRG TPMS GW DA TCU Radar LaneCamera HVAC BSW I_key ACU IPDM BCM EPS ABS VDC CVT ECM'  # 待查找的字符'hi'
    first_py_file.find_val_in_dbc_file(f1,val)
    f1.close()
    #引用first_py_file.py里面的函数
    #first_py_file.replace_hi_for_hello_in_myhellofile()
    #first_py_file.find_val_in_dbc_file()
    #first_py_file.replace_hi_for_hello_in_myhellofile()
    #first_py_file.find_recvByMeter_in_dbc_file()
    print('main end at ',datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S %A'))

if __name__ == '__main__':#只有当前脚本直接运行时才会执行main()函数，否则不执行（如：本脚本import到其他脚本）
    main()