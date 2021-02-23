import time
pre_time = time.perf_counter()
big_ary=[[[]for i in range(3)]for i in range(1000)]
def big_ary_init():
	global big_ary
	for i in range(1000):
		big_ary[i][0]=1+i
		big_ary[i][1]=(10+10*i)/1000
		big_ary[i][2]=time.perf_counter()

#10ms系统
def fuc_my():
	global pre_time
	global count
	global big_ary
	flag = 1
	big_ary_init()
	while 1:
		cur_clock = time.perf_counter()
		if( cur_clock - pre_time ) >= 0.010:
			pre_time = cur_clock
			for i in range(1000):
				if(cur_clock > (big_ary[i][2] + big_ary[i][1])):
					big_ary[i][2] = cur_clock
					print(big_ary[i][0],time.perf_counter())

def main():
	fuc_my()

if __name__ == "__main__":
	main()