#coding:utf8
import os
import time
import sched


#初始化scheduler类
#第一个参数是一个可以返回时间戳的函数，第二个参数可以在定时未到达之前阻塞。
s = sched.scheduler(time.time, time.sleep)


# 被周期性调度的任务
def task():
	print("run time: {}".format(float(time.time())))


def perform(inc):
	s.enter(inc, 0, perform, (inc,))
	task()


def main(inc=0.001):
	s.enter(0, 0, perform, (inc,))
	s.run()


if __name__ == "__main__":
	main()