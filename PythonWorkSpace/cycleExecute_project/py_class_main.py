#coding:utf8
import os
import time
import sched
from enum import Enum
	

#================================================pdu message configer=========================================
#枚举值
class PduCycleType(Enum):
	CYCLE = 0#周期
	EVENT = 1#事件
	MIXT = 2#混合
#如果PduMesg没有指定的继承，默认使用object
class PduMesg(object):
	#__init__初始化时需要传入的参数
	def __init__(self,
		num,
		id,
		PduLength,
		QueueDepth,
		RxMode,
		Period,
		Timeout,
		FirstOffset,
		FirstTimeout,
		PreTime,
		CurTime):
		
		self.num = num
		self.id = id
		self.Period = Period
		self.RxMode = RxMode
	#定义类内部访问函数
	def print_info(self):
		#:x 十六进制打印
		print("id:{:x}\nPeriod:{}\n".format(self.id,self.Period))
	def change_id(self,new_id):
		self.id=new_id
		
class SysPduMesg(object):
	def __init__(self,name):
		self.name = name
		self.PduMesgDict={}
		self.PduMesgList=[]
	def add_Mesg(self,pduMesg):
		self.PduMesgDict[pduMesg.num] = pduMesg
		self.PduMesgList.append(pduMesg)
	def del_Mesg(self,num):
		mesg_sanchu = self.PduMesgDict.pop(num)
		self.PduMesgList.remove(mesg_sanchu)
	def show_SysPduMesg(self):
		print(self.name)
		for i in self.PduMesgList:
			print("{:x}\r".format(i.id))
		
class CurSysCfg(object):
	def __init__(self,name,count):
		self.name = name
		self.count = count
	def add_count(self):
		self.count = self.count + 1
		
		
		
#================================================SYS VOLATILE==============================================
g_CurSysCfg = CurSysCfg('CUR SYS CFG',0)

#================================================define timer task=========================================
#定义scheduler
g_sched = sched.scheduler(time.time, time.sleep)
#定义task
def task():
	global g_CurSysCfg
	g_CurSysCfg.add_count()
	if ( g_CurSysCfg.count % 1000 ) == 0 :
		print("run time: {}".format(float(time.time())))
def perform(inc):
	g_sched.enter(inc, 0, perform, (inc,))
	task()
#0.001s = 1ms系统
def main(inc=0.001):
	#pdu_35d = PduMesg(0,0x35d,8,0,PduCycleType.CYCLE,100,200,0,0)
	"""
	g_SysPduMesg = SysPduMesg('SYS PDU MESSAGE')
	for i in range(10):
		name = 'pdu'+str(i)
		l_time = (float)(time.time())
		pdu = PduMesg(i,(0x35d+i),8,0,PduCycleType.CYCLE,(100*i),200,0,0,l_time,l_time)
		g_SysPduMesg.add_Mesg(pdu)
	g_SysPduMesg.show_SysPduMesg()
	"""
	#启用task
	g_sched.enter(0, 0, perform, (inc,))
	g_sched.run()


if __name__ == "__main__":
	main()