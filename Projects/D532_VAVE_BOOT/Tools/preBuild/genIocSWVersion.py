# -*- coding: utf-8 -*-

import datetime
import time
import os
import shutil
#import ctypes
import socket
import string

filePath = "./SWVersion"

'''
def getPcName():
	pcName = ctypes.c_char_p(''.encode('utf-8'))
	pcSize = 16
	pcName = ctypes.cast(pcName, ctypes.c_char_p)
	try:
		ctypes.windll.kernel32.GetComputerNameA(pcName, ctypes.byref(ctypes.c_int(pcSize)))
	except Exception:
		print "Sth wrong in getname!"
	print pcName.value.decode('utf-8')
'''

def getHostName():
	return socket.gethostname()
	
class GenIocVersionC:
	
	def __init__(self):
		
		if(os.path.exists(filePath) == True):
			#print 'Delete Old Path'
			shutil.rmtree(filePath)
		os.mkdir(filePath)
		self.innerVersionfileHandler = open(filePath+'/SWVersion.c',"w+")
		self.innerVersionHeaderFileHandler = open(filePath+'/SWVersion.h',"w+")
		self.externalVersionfileHandler = open('./bootloaderVersion.txt',"w+")
	def __del(self):
		if(self.innerVersionfileHandler != 0):
			self.innerVersionfileHandler.close()
			self.innerVersionHeaderFileHandler.close()
			self.externalVersionfileHandler.close()
		
	def GenFile(self):		
				
		ISOTIMEFORMAT = '%y%m%d%H%M%S'
		strCurTime = time.strftime(ISOTIMEFORMAT, time.localtime())
		#print strCurTime
		
		year = time.strftime('%y', time.localtime())
		month = time.strftime('%m', time.localtime())
		day = time.strftime('%d', time.localtime())
		hour = time.strftime('%H', time.localtime())
		min = time.strftime('%M', time.localtime())
		sec = time.strftime('%S', time.localtime())

		
		yearNum = hex(string.atoi(year))+','
		monthNum =  hex(string.atoi(month))+','
		dayNum = hex(string.atoi(day))+','
		hourNum = hex(string.atoi(hour))+','
		minNum = hex(string.atoi(min))+','
		secNum = hex(string.atoi(sec))
		hexVersion = yearNum+monthNum+dayNum+hourNum+minNum+secNum
		'''
		hexVersion = '0x'+year+',0x'+month+',0x'+day+',0x'+hour+',0x'+min+',0x'+sec
		'''
		#print hexVersion
		
		pcName = getHostName()
		#print pcName
		
		pcNameLen = len(pcName)
		if(pcNameLen >=31):
			pcName = pcName[0:30]
		#print pcName
		
		self.externalVersionfileHandler.write(
'''author  = '''+pcName+'''\n'''+
'''version = '''+strCurTime+'''\n''')

		self.innerVersionHeaderFileHandler.write('''/**************************************
Author		:'''+pcName+'''
Version		: 1.1
Timestamp	: '''+strCurTime+'''
***************************************/''')

		self.innerVersionHeaderFileHandler.write('''
#include"std_Types.h"

#define IOC_SOFTWARE_VERSION_ROM_HEX_LEN 6
#define IOC_SOFTWARE_VERSION_LEN 13
#define IOC_SW_SUBMITTER_NAME_LEN 32

extern uint8 getIocSWVersion(uint8 *versionBuff,uint8 bufLen);
extern uint8 getIocSWVersionRomHex(uint8 *versionBuff,uint8 bufLen);
''')


		self.innerVersionfileHandler.write('''/**************************************
Author		:'''+pcName+'''
Version		: 1.2
Timestamp	: '''+strCurTime+'''
***************************************/''')
		
		self.innerVersionfileHandler.write('''
#include"SWVersion.h"

const uint8 gIocSWVersion[IOC_SOFTWARE_VERSION_LEN] = \"'''+strCurTime+'''\";
const uint8 gIocSWVersionRomHex[IOC_SOFTWARE_VERSION_ROM_HEX_LEN] __attribute__ ((section(".InforConfig")))= {'''+hexVersion+'''};
const uint8 gIocSWSubmitterName[IOC_SW_SUBMITTER_NAME_LEN] = \"'''+pcName+'''\";

uint8 getIocSWVersion(uint8 *versionBuff,uint8 bufLen)
{
	uint8 i = 0;
	if(versionBuff!=NULL && bufLen>=IOC_SOFTWARE_VERSION_LEN)
	{
		for(i=0;i<IOC_SOFTWARE_VERSION_LEN;i++)
			versionBuff[i] = gIocSWVersion[i];
		
		return IOC_SOFTWARE_VERSION_LEN;
	}
	
	return 0;
}

uint8 getIocSWVersionRomHex(uint8 *versionBuff,uint8 bufLen)
{
	uint8 i = 0;
	if(versionBuff!=NULL && bufLen>=IOC_SOFTWARE_VERSION_ROM_HEX_LEN)
	{
		for(i=0;i<IOC_SOFTWARE_VERSION_ROM_HEX_LEN;i++)
			versionBuff[i] = gIocSWVersionRomHex[i];
		
		return IOC_SOFTWARE_VERSION_ROM_HEX_LEN;
	}
	
	return 0;
}

\n''')
		
		

		self.innerVersionfileHandler.close()
		self.innerVersionHeaderFileHandler.close()
		self.externalVersionfileHandler.close()
		
		self.innerVersionfileHandler = 0
		self.innerVersionHeaderFileHandler = 0
		self.externalVersionfileHandler = 0

if __name__ == "__main__":
	
	genIocVersionObj = GenIocVersionC()
	genIocVersionObj.GenFile()
