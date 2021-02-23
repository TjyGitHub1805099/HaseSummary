# -*- coding: utf-8 -*-

import datetime
import time
import os
import shutil
#import ctypes
import socket
import string

TOOL_VERSION = '1.4'
SW_VERSION_PATH = "./SWVersion/"

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
		
		if(os.path.exists(SW_VERSION_PATH) == True):
			print ('Delete Old Path:',SW_VERSION_PATH)
			shutil.rmtree(SW_VERSION_PATH)
		os.mkdir(SW_VERSION_PATH)
		print ('Create New Path:',SW_VERSION_PATH)
		self.innerVersionfileHandler = open(SW_VERSION_PATH+'SWVersion.c',"w+")
		self.innerVersionHeaderFileHandler = open(SW_VERSION_PATH+'SWVersion.h',"w+")
		self.externalVersionfileHandler = open('./appVersion.txt',"w+")
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

		'''
		yearNum = hex(string.atoi(year))+','
		monthNum =  hex(string.atoi(month))+','
		dayNum = hex(string.atoi(day))+','
		hourNum = hex(string.atoi(hour))+','
		minNum = hex(string.atoi(min))+','
		secNum = hex(string.atoi(sec))
		'''
		yearNum = hex(int(year))+','
		monthNum =  hex(int(month))+','
		dayNum = hex(int(day))+','
		hourNum = hex(int(hour))+','
		minNum = hex(int(min))+','
		secNum = hex(int(sec))
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
Author		: '''+pcName+'''
Version		: '''+TOOL_VERSION+'''
Timestamp	: '''+strCurTime+'''
***************************************/''')

		self.innerVersionHeaderFileHandler.write('''
#ifndef _SWVERSION_H_
#define _SWVERSION_H_

#include"Std_Types.h"

#define IOC_SOFTWARE_VERSION_ROM_HEX_LEN 6
#define IOC_SOFTWARE_VERSION_LEN 13
#define IOC_SW_SUBMITTER_NAME_LEN 32

extern uint8 getIocSWVersion(uint8 *versionBuff,uint8 bufLen);
extern uint8 getIocSWVersionRomHex(uint8 *versionBuff,uint8 bufLen);

extern const uint8 gIocSWVersion[];

#endif
''')


		self.innerVersionfileHandler.write('''/**************************************
Author		: '''+pcName+'''
Version		: '''+TOOL_VERSION+'''
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

def genIOCVersionFile():
	genIocVersionObj = GenIocVersionC()
	genIocVersionObj.GenFile()

if __name__ == "__main__":
	
	genIocVersionObj = GenIocVersionC()
	genIocVersionObj.GenFile()
