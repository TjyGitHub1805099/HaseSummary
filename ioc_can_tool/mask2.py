def get_mask(id):
	m = 0
	sum = 0
	for i in id:
		m = m + 1
		for j in id[m:]:
			sum |= i ^ j
	return hex(~sum & 0xffffffff)	
#金康S106-IVI-1.6.5 DBC 文件
can_id = [
[0x074,0x68],#mailbox1 
[0x122,0x111],#mailbox2 , 10ms 10ms
[0x144,0x142],#mailbox3 , 10ms 10ms
[0x21A,0x219,0x211],#mailbox4 , 20ms 20ms 50ms
[0x239,0x231,0x22B,0x221],#mailbox5 , 20ms 50ms 20ms 50ms
[0x271,0x261,0x251,0x24E,0x241],#mailbox6 ,20ms 50ms 50ms 50ms 20ms
[0x2F2,0x2C2,0x2C1,0x2A1,0x281],#mailbox7 ,20ms 20ms 20ms 20ms 50ms
[0x321,0x31E,0x318,0x30E,0x301],#mailbox8 ,100ms 100ms 100ms 50ms 100ms
[0x334,0x331,0x330,0x324,0x323,0x322],#mailbox9 ,100ms 100ms 100ms 100ms 100ms 100ms
[0x36E,0x369,0x35E,0x35A,0x349],#mailbox10 ,50ms 50ms 500ms 50ms 50ms
[0x394,0x38D,0x387,0x380,0x37E],#mailbox11 ,100ms 100ms 100ms 100ms 100ms
[0x3AE,0x3AD,0x3A9,0x3A1,0x3A0],#mailbox12 ,100ms 100ms 100ms 100ms 100ms
[0x3C1,0x3BE,0x3B7,0x3B6],#mailbox13 ,100ms 100ms 100ms 100ms
[0x3D6,0x3D5,0x3D1,0x3CE],#mailbox14 ,100ms 100ms 100ms 100ms
[0x3FA,0x3F6,0x3F4,0x3F3,0x3F2],#mailbox15 ,100ms 100ms 100ms 100ms 100ms
[0x484,0x482,0x421,0x41F,0x411],#mailbox16 ,100ms 500ms 200ms 200ms 200ms
[0x561,0x514,0x511,0x4A2,0x492],#mailbox17 ,1000ms 1000ms 10000ms 500ms 500ms
[0x7DF,0x725],#mailbox18
]
mailbox_num =1
for y in can_id:
	print("mailbox ",mailbox_num,end=", include[ ")
	for x in y:
		print (hex(x),end=" ")
	print("], mask= ",get_mask(y))
	mailbox_num+=1
	 


while True:
	pass