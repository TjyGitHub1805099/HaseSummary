# -*- coding: utf-8 -*- 
import  xdrlib
import xlrd
import sys
import os
import time
NvM_CfgCFilePath = '..\\..\\SDK\\middleware\\NVRAM\\NvM\\NvM_Cfg.c'
NvM_CfgHFilePath = '..\\..\\SDK\\middleware\\NVRAM\\NvM\\NvM_Cfg.h'
Ea_CfgCFilePath  = '..\\..\\SDK\\middleware\\NVRAM\\Ea\\Ea_Cfg.c'
Ea_CfgHFilePath  = '..\\..\\SDK\\middleware\\NVRAM\\Ea\\Ea_Cfg.h'
CfgListFilePath  = 'MemoryBlockCfgList.xls'

'''
NvM_CfgCFilePath = 'NvM_Cfg.c'
NvM_CfgHFilePath = 'NvM_Cfg.h'
Ea_CfgCFilePath  = 'Ea_Cfg.c'
Ea_CfgHFilePath  = 'Ea_Cfg.h'
CfgListFilePath  = 'MemoryBlockCfgList.xls'
'''

ExcelStartRow = 3
EaBlockHeaderLength = 2

Col_Modifiy     = 0
Col_BlockNumber = 1
Col_Type        = 2
Col_Priority    = 3
Col_Length      = 4
Col_NvBlockNum  = 5
Col_CRCType     = 6
Col_Cycles      = 7
Col_RamAddress  = 8
Col_IncludeFile = 9
Col_VarType     = 10
Col_SingleBlockCallback = 11
Col_READALL     = 12
Col_WRITEALL    = 13
Col_CHANGEDSW   = 14
Col_RAM_BLOCK_CRC = 15
Col_WRITE_PRO     = 16
Col_USE_CRC       = 17
Col_ONCE          = 18


#open cfg excel
def open_excel(file= 'file.xls'):
	try:
		data = xlrd.open_workbook(file)
		return data
	except Exception,e:
		print str(e)

def main():

	NvmBlockNumberAll = 0
	data  = open_excel(CfgListFilePath)
	data  = data
	table = data.sheets()[0]
	nrows = table.nrows
	ncols = table.ncols
	NvmBlockNumberAll = nrows - ExcelStartRow #减去excel的表头部分
	print 'NvmBlockNumberAll =',NvmBlockNumberAll
	#print nrows,ncols
	counter = 0
	blockDict = {}
	#获取系统时间
	modfiyTime = str(time.strftime("%Y-%m-%d %H:%M"))
	#sys.exit(0)
	#查找的匹配字符串
	NvmC_String  = '''/*** Start , DO NOT MODIFY THIS TEXT!!! ***/\n'''
	NvmC_String += '''/*** Don't write any code pass this line, or it will be deleted during code generation. ***/\n'''
	NvmC_String += '''/*** ***********************The below lines is generated by tool*********************** ***/\n'''
	EaC_String   = '''/*** Start , DO NOT MODIFY THIS TEXT!!! ***/\n'''
	EaC_String  += '''/*** Don't write any code pass this line, or it will be deleted during code generation. ***/\n'''
	EaC_String  += '''/*** ***********************The below lines is generated by tool*********************** ***/\n'''
	#写nvmcfg.c的头文件包含和外部变量声明部分
	#写include文件
	counter = ExcelStartRow-1
	for i in range(0,NvmBlockNumberAll):
		counter += 1
		blockDict['IncludeFile'] = str(table.cell(counter,Col_IncludeFile).value.encode('utf-8'))
		if '//' != blockDict['IncludeFile'][0:2]:
			NvmC_String += '''#include  ''' + blockDict['IncludeFile'] + '''\n'''
	counter = ExcelStartRow-1
	#写外部变量声明
	for i in range(0,NvmBlockNumberAll):
		counter += 1
		blockDict['VarType']     = str(table.cell(counter,Col_VarType).value.encode('utf-8'))
		if '//' != blockDict['VarType'][0:2]:
			NvmC_String += '''#extern  ''' + blockDict['VarType'] + ''';\n'''
	#写block0
	NvmC_String += '''const NvM_BlockDescriptorType NvM_BlockDescriptor[NVM_BLOCK_NUM_ALL + 1] =\n{
    {0,/*it not used,start with block1*/
        NVM_BLOCK_NATIVE,
    #if ((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))
        255,
    #endif
        0,NVM_CRC16,0,0,0,0,NULL_PTR,NULL_PTR,NULL_PTR,NULL_PTR 
    },\n'''
	#写eacfg.c的version等
	VERSION   = str(int(table.cell(1,1).value))
	FORMAT    = bool(str(table.cell(1,3).value)=="Yes") and "TRUE" or "FALSE"
	#FORMAT    = str(table.cell(1,3).value)
	EaC_String  += '''const uint16 EA_VERSION='''+VERSION+''';\n'''
	EaC_String  += '''const EaBlockConfigType EaBlockConfiguration[EA_BLOCK_NUM_ALL+1u]=\n{\n'''
	EaC_String  += '''  {0,0,0,0},\n'''
	#开始循环写block的配置到ea.c和nv.c中
	NvmNvBlockBaseNumber = 1
	EaBlockNumberAll  = 0
	NvBlockLenMax = 0
	counter = ExcelStartRow-1
	IdDefineString = '''//Nvm BlockID Define\n'''
	for loop in range(0,NvmBlockNumberAll):
		counter += 1
		#按照excel的行来读取数据并处理
		FlagGroupString = ''
		blockDict['Modifiy']         = str(table.cell(counter,Col_Modifiy).value.encode('utf-8'))
		blockDict['BlockNum']        = int(table.cell(counter,Col_BlockNumber).value)
		blockDict['Type']            = str(table.cell(counter,Col_Type).value.encode('utf-8'))
		blockDict['Priority']        = int(table.cell(counter,Col_Priority).value)
		blockDict['Length']          = int(table.cell(counter,Col_Length).value)
		blockDict['NvBlockNum']      = int(table.cell(counter,Col_NvBlockNum).value)
		blockDict['WriteCycles']     = str(table.cell(counter,Col_Cycles).value)
		blockDict['CRCType']         = str(table.cell(counter,Col_CRCType).value)
		blockDict['RamAddress']      = str(table.cell(counter,Col_RamAddress).value.encode('utf-8'))
		blockDict['SingleCallback']  = str(table.cell(counter,Col_SingleBlockCallback).value.encode('utf-8'))
		if 'YES' == str(table.cell(counter,Col_READALL).value.encode('utf-8')):
			FlagGroupString  = FlagGroupString+ 'NVM_SELCT_BLOCK_4READALL|'
		if 'YES' == str(table.cell(counter,Col_WRITEALL).value.encode('utf-8')):
			FlagGroupString  = FlagGroupString+ 'NVM_SELCT_BLOCK_4WRITEALL|'
		if 'YES' == str(table.cell(counter,Col_CHANGEDSW).value.encode('utf-8')):
			FlagGroupString  = FlagGroupString+ 'NVM_RESISTANT2CHANGEDSW|'
		if 'YES' == str(table.cell(counter,Col_RAM_BLOCK_CRC).value.encode('utf-8')):
			FlagGroupString  = FlagGroupString+ 'NVM_CALC_RAM_BLOCK_CRC|'
		if 'YES' == str(table.cell(counter,Col_WRITE_PRO).value.encode('utf-8')):
			FlagGroupString  = FlagGroupString+ 'NVM_BLOCK_WRITE_PROT|'
		if 'YES' == str(table.cell(counter,Col_USE_CRC).value.encode('utf-8')):
			FlagGroupString  = FlagGroupString+ 'NVM_BLOCK_USE_CRC|'
		if 'YES' == str(table.cell(counter,Col_ONCE).value.encode('utf-8')):
			FlagGroupString  = FlagGroupString+ 'NVM_WRITE_BLOCK_ONCE|'
		FlagGroupString  = FlagGroupString+ '0'
		blockDict['FlagGroup'] = FlagGroupString
		IdDefineString += '''#define  '''+str(str(blockDict['RamAddress'])[1:])+'''NvMBlockID    '''+str(loop+1)+'''U\n'''
		#print blockDict
		#写ncm_cfg
		#NvFileHnd.write(
		NvmC_String +='''    {/*Block '''+str(blockDict['BlockNum'])+'''*/\n'''
		NvmC_String +='''        0, /* NvmNvramDeviceId */\n'''
		NvmC_String +='''        NVM_BLOCK_'''+blockDict['Type']+''',    /* NvmBlockManagementType */\n'''
		NvmC_String +='''        #if ((NVM_API_CONFIG_CLASS_1!=NVM_API_CONFIG_CLASS)&&(STD_ON==NVM_JOB_PRIORITIZATION))\n'''
		NvmC_String +='''        '''+str(blockDict['Priority'])+''',/* NvmBlockJobPriority */\n'''
		NvmC_String +='''        #endif\n'''
		NvmC_String +='''        '''+blockDict['FlagGroup']+''',\n'''
		NvmC_String +='''        NVM_'''+str(blockDict['CRCType'])+''',/* NvmBlockCRCType */\n'''
		NvmC_String +='''        '''+str(NvmNvBlockBaseNumber)+''',/* NvmNvBlockBaseNumber */\n'''
		NvmC_String +='''        '''+str(blockDict['Length'])+''',/* NvmNvBlockLength */\n'''
		NvmC_String +='''        '''+str(blockDict['NvBlockNum'])+''',/*NvmNvBlockNum*/\n'''
		NvmC_String +='''        0,/* NvmRomBlockNum */\n'''
		NvmC_String +='''        (uint8*)'''+blockDict['RamAddress']+''',   /* NvmRamBlockDataAddress */\n'''
		NvmC_String +='''        (uint8*)((uint8*)('''+blockDict['RamAddress']+''')+NVM_ROM_RAM_SEC_OFFSET),   /* NvmRomBlockDataAddress */\n'''
		NvmC_String +='''        NULL_PTR, /* NvmInitBlockCallback */\n'''
		NvmC_String +='''        '''+blockDict['SingleCallback']+  '''/* NvmSingleBlockCallback */\n'''
		NvmC_String +='''    },\n'''
		#写Nvm_Cfg.c结束
		#找长度最大的block
		if NvBlockLenMax < int(blockDict['Length']):
			NvBlockLenMax = int(blockDict['Length'])
		#写ea_cfg.c
		temp = NvmNvBlockBaseNumber+int(blockDict['NvBlockNum'])
		CrcLength = 0
		for EaCount in range(NvmNvBlockBaseNumber,temp):
			if str(blockDict['CRCType']) is '''CRC8''':
				CrcLength = 1
			if blockDict['CRCType'] is '''CRC16''':
				CrcLength = 2
			if blockDict['CRCType'] is '''CRC32''':
				CrcLength = 4
			EaC_String += '''  {/*Block '''+str(EaCount)+'''*/\n'''
			EaC_String += '''    '''+str(int(blockDict['Length'])+CrcLength)+''',/*EaBlockSize*/\n'''
			if 0==int(blockDict['Priority']):
				EaC_String += '''    0,/*EaPageNumber*/\n'''
			else:
				EaC_String += '''    1,/*EaPageNumber*/\n'''
			#info = str(blockDict['FlagGroup'])
			if blockDict['Modifiy'] == "DELETE":
				EaC_String += '''    Blockinvalid,/*EaNumberOfWriteCycles*/\n'''
			else:
				EaC_String += '''    '''+blockDict['WriteCycles']+''',/*EaNumberOfWriteCycles*/\n'''
				#if -1==info.find('NVM_WRITE_BLOCK_ONCE'):
				#	EaC_String += '''    BlockWriteCycleForever,/*EaNumberOfWriteCycles*/\n'''
				#else:
				#	EaC_String += '''    0x0001,/*EaNumberOfWriteCycles*/\n'''
			if blockDict['Modifiy'] == "NO":
				EaC_String += '''    EA_BLOCK_NO_MODFIED\n'''
			else:
				EaC_String += '''    EA_BLOCK_'''+blockDict['Modifiy']+'''\n'''
			EaC_String += '''  },\n'''
			EaCount +=1
			EaBlockNumberAll  +=1
		#写ea_cfg.c block项结束
		NvmNvBlockBaseNumber=NvmNvBlockBaseNumber+int(blockDict['NvBlockNum'])
	#end of "for i in range(0,NvmBlockNumberAll):"

	#替换原Nvm_Cfg.c中的配置信息 开始
	NvmC_String += '''\n};\n'''
	NvmC_String += '''/*** Generation data:'''+modfiyTime+''' ***/\n'''
	NvmC_String += '''/*** The above lines is generated by tool ***/\n'''
	NvFileHnd = open(NvM_CfgCFilePath, 'r+')
	NvMfile = str(NvFileHnd.read())
	line1 = NvMfile.find('''/*** Start , DO NOT MODIFY THIS TEXT!!! ***/''');
	line2 = NvMfile.find('''/*** End , DO NOT MODIFY THIS TEXT!!! ***/''');
	string = NvMfile[line1:line2]
	NvFileHnd.close
	string = NvMfile[line1:line2]
	NvMfile = NvMfile.replace(''''''+str(string)+'''''',''''''+str(NvmC_String)+'''''')
	NvFileHnd = open(NvM_CfgCFilePath, 'w+')
	NvFileHnd.write(''''''+NvMfile+'''''')
	NvFileHnd.close
	#替换原Nvm_Cfg.c中的配置信息 结束
	#替换原Ea_Cfg.c中的配置信息 开始
	EaC_String += '''\n};\n'''
	EaC_String += '''/*** Generation data:'''+modfiyTime+''' ***/\n'''
	EaC_String += '''/*** The above lines is generated by tool ***/\n'''
	NvFileHnd = open(Ea_CfgCFilePath, 'r+')
	Eafile = str(NvFileHnd.read())
	line1 = Eafile.find('''/*** Start , DO NOT MODIFY THIS TEXT!!! ***/''');
	line2 = Eafile.find('''/*** End , DO NOT MODIFY THIS TEXT!!! ***/''');
	string = Eafile[line1:line2]
	NvFileHnd.close
	string = Eafile[line1:line2]
	Eafile = Eafile.replace(''''''+str(string)+'''''',''''''+str(EaC_String)+'''''')
	NvFileHnd = open(Ea_CfgCFilePath, 'w+')
	NvFileHnd.write(''''''+Eafile+'''''')
	NvFileHnd.close
	#写nvc和eac结束
	
	#写nvh和eah文件开始
	#写Nvm_Cfg.h开始
	FileHnd = open(NvM_CfgHFilePath, 'r+')
	NvMfile = str(FileHnd.read())
	FileHnd.close
	line1  = NvMfile.find('''/*** Start , DO NOT MODIFY THIS TEXT!!! ***/''');
	line2  = NvMfile.find('''/*** End , DO NOT MODIFY THIS TEXT!!! ***/''');
	string = NvMfile[line1:line2]
	R_String  = '''/*** Start , DO NOT MODIFY THIS TEXT!!! ***/\n'''
	R_String += '''/*** Don't write any code pass this line, or it will be deleted during code generation. ***/\n'''
	R_String += '''/*** ***********************The below lines is generated by tool*********************** ***/\n'''
	R_String += '''#define NVM_BLOCK_NUM_ALL          ''' + str(NvmBlockNumberAll) + '''U\n\n'''
	R_String += '''#define NVM_MAX_LENGTH_NV_BLOCK    ''' + str(NvBlockLenMax) + '''U\n\n'''
	R_String += IdDefineString
	R_String += '''/*** The above lines is generated by tool ***/\n'''
	R_String += '''/*** Generation data:'''+modfiyTime+''' ***/\n'''
	NvMfile = NvMfile.replace(str(string),R_String) 
	FileHnd = open(NvM_CfgHFilePath, 'w+')
	FileHnd.write(''''''+NvMfile+'''''')
	FileHnd.close
	#写Nvm_Cfg.h结束
	#写Ea_Cfg.h开始
	FileHnd = open(Ea_CfgHFilePath, 'r+')
	Eafile = str(FileHnd.read())
	FileHnd.close
	line1  = Eafile.find('''/*** Start , DO NOT MODIFY THIS TEXT!!! ***/''');
	line2  = Eafile.find('''/*** End , DO NOT MODIFY THIS TEXT!!! ***/''');
	string = Eafile[line1:line2]
	R_String  = '''/*** Start , DO NOT MODIFY THIS TEXT!!! ***/\n'''
	R_String += '''/*** Don't write any code pass this line, or it will be deleted during code generation. ***/\n'''
	R_String += '''/*** ***********************The below lines is generated by tool*********************** ***/\n'''
	R_String += '''#define EA_BLOCK_NUM_ALL          ''' + str(EaBlockNumberAll) + '''U\n\n'''
	R_String += '''/*** The above lines is generated by tool ***/\n'''
	R_String += '''/*** Generation data:'''+modfiyTime+''' ***/\n'''
	Eafile = Eafile.replace(str(string),R_String) 
	FileHnd = open(Ea_CfgHFilePath, 'w+')
	FileHnd.write(''''''+Eafile+'''''')
	FileHnd.close
	print 'success!'
	os.system('pause')
if __name__=="__main__":
	main()
