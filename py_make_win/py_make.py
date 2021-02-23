import os
import sys
import shutil
import time
import re
from genIocSWVersion import *

OS_TYPE = sys.platform

PROJECT_PATH = ''
BIN_PATH = ''
BINARY_PATH = ''
JFLASH_PATH = ''
PATH_SYMBOL = ''
if(OS_TYPE == 'win32'):
	PATH_SYMBOL = '\\'
	PROJECT_PATH = os.path.abspath('..')+'\\'
	BIN_PATH = 'C:/NXP/S32DS_ARM_v2.0/Cross_Tools/gcc-arm-none-eabi-4_9/bin/'
	BINARY_PATH = '..\\Binary\\'
	JFLASH_PATH = '..\\..\\JFlash\\'
elif('linux' in OS_TYPE):
	PATH_SYMBOL = '/'
	PROJECT_PATH = os.path.abspath('..')+'/'
	BIN_PATH = '/home/jenkins/MCU_install_package/Cross_Tools/gcc-arm-none-eabi-4_9/bin/'
	BINARY_PATH = '../Binary/'
	JFLASH_PATH = '../../JFlash/'
	
PREFIX = BIN_PATH+'arm-none-eabi-'
#GCC = PREFIX+"gcc"
GCC = "arm-none-eabi-gcc"
AR = PREFIX+"ar"
LD = PREFIX+"ld"
OBJCOPY = PREFIX+'objcopy'

#OUTPUT_PATH = 'output'+PATH_SYMBOL
OUTPUT_PATH = 'output/'
TARGET_NAME = 'app'


GCC_CFG_PATH = 'cfg'+'/'

GCC_CFG_GEN_PATH = GCC_CFG_PATH+'gen'+'/'


GEN_APP_ELF_OPTIONS_FILE  = GCC_CFG_PATH+TARGET_NAME+'.args'
GCC_C_OPTIONS_FILE = GCC_CFG_PATH+'gcc_c_options.args'
GCC_ASSEMBLE_OPTIONS_FILE = GCC_CFG_PATH+'startup_S32K148.args'
GCC_EXCLUDING_FOLDER_FILE = GCC_CFG_PATH+'gcc_excluding_folder.args'


GCC_HEADER_CFG = GCC_CFG_GEN_PATH+'gcc_header_cfg.args'
GCC_LIB_CFG = GCC_CFG_GEN_PATH+'gcc_lib_cfg.args'
OBJ_LIST_FILE = GCC_CFG_GEN_PATH+'objListFile.args'


APP_PATH = PROJECT_PATH+'APP'+PATH_SYMBOL
GEN_CODE_PATH = PROJECT_PATH+'Generated_Code'+PATH_SYMBOL
SDK_PATH = PROJECT_PATH+'SDK'+PATH_SYMBOL
PROJECT_SETTINGS_PATH = PROJECT_PATH+'Project_Settings'+PATH_SYMBOL


COMPILE_LOG_FILE = OUTPUT_PATH+'compile.log'


C_FILE_LIST =[]
ASSEMBLE_FILE_LIST = []
HEADER_PATH_LIST = []
LIB_PATH_LIST = []
LIB_FILE_LIST = []
EXCLUDING_FOLDER_LIST = []


#add by tjy:start
EXCEPT_OPTION_LIST = ""
EXCEPT_OPTION_LIST_FILE = GCC_CFG_PATH+'excepOptionFile.txt'
GCC_C_OPTIONS_O0_FILE = GCC_CFG_PATH+'gcc_c_options_o0.args'

def searchExceptOptionFiles():
    global EXCEPT_OPTION_LIST
    fp = open(EXCEPT_OPTION_LIST_FILE)
    text_lines=fp.readlines()
    print("<<<<<<< not need optimize file is started >>>>>>>>")
    for excpt_file in text_lines:
        print(excpt_file)
        EXCEPT_OPTION_LIST=EXCEPT_OPTION_LIST+excpt_file+"\r\n"
    print("<<<<<<< not need optimize file is ending >>>>>>>>>")
    fp.close()
#add by tjy:ending

def searchFiles(rootDir,file_type_list,c_file_list,assem_file_list,header_path_list,lib_path_list,lib_file_list):
	try:
		for filename in os.listdir(rootDir):
			#print filename
			if(filename == '.svn'):
				continue
			
			pathname = os.path.join(rootDir, filename)
			
			if (os.path.isdir(pathname)):
				
				new_path_name = pathname
				if(OS_TYPE == 'win32'):
					new_path_name = pathname.replace(PATH_SYMBOL,'/')
				skip_path_flag = False
				for excluding_folder in EXCLUDING_FOLDER_LIST:
					if(new_path_name.find(excluding_folder[0:-1]) >= 0):
						print('SKIP folder:',excluding_folder[0:-1])
						skip_path_flag = True
						break
				if(skip_path_flag == False):
					searchFiles(pathname,file_type_list,c_file_list,assem_file_list,header_path_list,lib_path_list,lib_file_list)
			else:
				file_len = len(filename)
				file_type_postfix = filename[file_len-2:file_len]
				if( file_type_postfix in file_type_list):
					#############################
					#TODO: RECORD FILE DATE
					#######################
					#print(pathname)
					if(file_type_postfix.upper() == '.C'):
						c_file_list.append(pathname)
						#file_stat_infor = os.stat(pathname)
						#print(pathname.split(PATH_SYMBOL)[-1],file_stat_infor.st_mtime)
					elif(file_type_postfix.upper() == '.S'):
						assem_file_list.append(pathname)
					elif(filename[file_len-2:file_len].upper() == '.A'):
						lib_file_list.append(filename)
						#print(filename)
						path_list = pathname.split(PATH_SYMBOL)
						#print(path_list)
						subfolder_count = len(path_list)
						lib_path = ''
						for i in range(subfolder_count):
							if(i < subfolder_count-1):
								lib_path += (path_list[i]+'/')
						if(lib_path not in lib_path_list):
							lib_path_list.append(lib_path)
							#print('LIB path:',lib_path)
					elif(filename[file_len-2:file_len].upper() == '.H'):
						#print(pathname)
						path_list = pathname.split(PATH_SYMBOL)
						#print(path_list)
						subfolder_count = len(path_list)
						header_path = ''
						for i in range(subfolder_count):
							if(i < subfolder_count-1):
								header_path += (path_list[i]+'/')
						if(header_path not in header_path_list):
							header_path_list.append(header_path)
							#print('HEADER path:',header_path)
		#print('files sum count = ',len(dst_list))
		return True
	except Exception as e:
		print('Error:searchFiles Got Exception:',e)
		return False
	

def getObjFileList(rootDir,objFileList):
	for filename in os.listdir(rootDir):
		if (os.path.isdir(filename) == False):
			file_len = len(filename)
			if(filename[file_len-2:file_len] == '.o'):
				print(filename)
				objFileList.append(filename)

def createPath(path_name):
	if(os.path.exists(path_name) == True):
			print ('Delete Old Path',path_name)
			shutil.rmtree(path_name)
	print("Create New Path",path_name)
	os.mkdir(path_name)


'''
def createPath(path_name):
	if(os.path.exists(path_name) == False):
		print("Create New Path",path_name)
		os.mkdir(path_name)
'''

def createCMakefile():
    c_file_makefile_handler = open(GCC_CFG_GEN_PATH+'c_makefile.mk','w')
    c_file_makefile_handler.write('''OBJS += \\\n''')
    file_count = len(C_FILE_LIST)
    index = 0
    NEW_C_FILE_LIST = []

    for cFileName in C_FILE_LIST:
        new_path_name = cFileName.replace(PROJECT_PATH,'../')
        NEW_C_FILE_LIST.append(new_path_name.replace('\\','/'))

    path_sum_count = len(NEW_C_FILE_LIST)

    for cFileName in NEW_C_FILE_LIST:
        #print(cFileName)
        nameList = cFileName.split('/')
        objName = nameList[-1]
        objFileName = objName.replace('.c','.o')
        c_file_makefile_handler.write('\t'+objFileName+''' \\\n''')

    c_file_makefile_handler.write('''\n\n''')

    for cFileName in NEW_C_FILE_LIST:
        nameList = cFileName.split('/')
        objName = nameList[-1]
        objFileName = objName.replace('.c','.o')
        c_file_makefile_handler.write(objFileName+': '+cFileName+'''\n''')
        
        if cFileName in EXCEPT_OPTION_LIST:
            #add by tjy
            cmd = '\t'+GCC+" "+"\"@"+GCC_HEADER_CFG+'\"'+' '+"\"@"+GCC_C_OPTIONS_O0_FILE+'\"'+' '+cFileName
            cmd = cmd+" -o "+OUTPUT_PATH+objName[0:len(objName)-2]+'.o'+'\n'
        else:
            cmd = '\t'+GCC+" "+"\"@"+GCC_HEADER_CFG+'\"'+' '+"\"@"+GCC_C_OPTIONS_FILE+'\"'+' '+cFileName
            cmd = cmd+" -o "+OUTPUT_PATH+objName[0:len(objName)-2]+'.o'+'\n'
        c_file_makefile_handler.write(cmd)

    c_file_makefile_handler.close()

def createAssmbleMakefile():
	assmble_file_makefile_handler = open(GCC_CFG_GEN_PATH+'assmble_makefile.mk','w')
	assmble_file_makefile_handler.write('''OBJS += \\\n''')
	
	NEW_ASSEMBLE_FILE_LIST = []
	for sFileName in ASSEMBLE_FILE_LIST:
		NEW_ASSEMBLE_FILE_LIST.append(sFileName.replace(PROJECT_PATH,'../').replace('\\','/'))
	
	for sFileName in NEW_ASSEMBLE_FILE_LIST:
		objName = sFileName.split('/')[-1].replace('.S','.o')
		assmble_file_makefile_handler.write('\t'+objName+' \\')
		
	assmble_file_makefile_handler.write('''\n\n''')
	
	for sFileName in NEW_ASSEMBLE_FILE_LIST:
		objName = sFileName.split('/')[-1].replace('.S','.o')
		assmble_file_makefile_handler.write(objName+': '+sFileName+'\n')
		cmd = '\t'+GCC+" "+"\"@"+GCC_ASSEMBLE_OPTIONS_FILE+'\"'+' '+'-c'+' '+sFileName
		cmd = cmd+" -o "+OUTPUT_PATH+objName+ '\n'
		assmble_file_makefile_handler.write(cmd)

def createMakefile():
	createCMakefile()
	createAssmbleMakefile()

	
def createOBJListCfg():
	objFileHandler = open(OBJ_LIST_FILE,'w')
	for c_file in C_FILE_LIST:
		objFileName = c_file.split(PATH_SYMBOL)[-1].replace('.c','.o')
		objFileHandler.write('\"output/'+objFileName+'\"\n')
	
	for assemble_file in ASSEMBLE_FILE_LIST:
		objFileName = assemble_file.split(PATH_SYMBOL)[-1].replace('.S','.o')
		objFileHandler.write('\"output/'+objFileName+'\"\n')
	
	objFileHandler.close()
	

def copySWVersion_windows():
	
	cmd = 'rd /s/q ..\\APP\\SWVersion'
	print(cmd)
	os.system(cmd)
	
	cmd = 'del /s/q ..\\APP\\appVersion.txt'
	print(cmd)
	os.system(cmd)

	cmd = 'md ..\\APP\\SWVersion'
	print(cmd)
	os.system(cmd)
	
	cmd = 'move SWVersion\\* ..\\APP\\SWVersion'
	print(cmd)
	os.system(cmd)
	
	cmd = 'rd /s/q SWVersion'
	print(cmd)
	os.system(cmd)
	
	cmd = 'move appVersion.txt ..\\APP\\appVersion.txt'
	print(cmd)
	os.system(cmd)

def copySWVersion_linux():
	cmd = 'rm -rf ../APP/SWVersion; mv ./SWVersion ./appVersion.txt ../APP/'
	print(cmd)
	os.system(cmd)	

def cleanTargetFiles():
	cmd = ''
	if(OS_TYPE == 'win32'):
		cmd = 'del /s/q '+TARGET_NAME+'.bin '+TARGET_NAME+'.elf '+TARGET_NAME+'.map'
		print(cmd)
	elif('linux' in OS_TYPE):
		cmd = 'rm -rf '+TARGET_NAME+'.bin '+TARGET_NAME+'.elf '+TARGET_NAME+'.map'
	print(cmd)
	os.system(cmd)
	

def copyFilesToJFlashPath():
	cp_cmd = ''
	if(OS_TYPE=='win32'):
		cp_cmd = 'copy'
	elif('linux' in OS_TYPE):
		cp_cmd = 'cp'
	
	cmd = cp_cmd + ' '+TARGET_NAME+'.bin'+ ' ' + JFLASH_PATH
	print(cmd)
	os.system(cmd)
	
	cmd = cp_cmd + ' '+APP_PATH + 'appVersion.txt' + ' '+JFLASH_PATH
	print(cmd)
	os.system(cmd)

def copyFilesToBinaryPath():
	cp_cmd = ''
	if(OS_TYPE=='win32'):
		cp_cmd = 'copy'
	elif('linux' in OS_TYPE):
		cp_cmd = 'cp'
	
	cmd = cp_cmd + ' '+TARGET_NAME+'.map' + ' ' + BINARY_PATH
	print(cmd)
	os.system(cmd)
	
	cmd = cp_cmd + ' '+TARGET_NAME+'.elf' + ' ' + BINARY_PATH
	print(cmd)
	os.system(cmd)
	
	#cmd = cp_cmd + ' '+TARGET_NAME+'.bin'+ ' ' + BINARY_PATH
	#print(cmd)
	#os.system(cmd)
	
def getOSType():
	#windows--'win32'
	#linux --'linux'--'linux2'
	#Windows/Cygwin ---'cygwin'
	#Mac OS X---'darwin'
	return sys.platform

def createHeaderPathCfg():
	
	headerPathFileHandler = open(GCC_HEADER_CFG,'w')
	for header_path in HEADER_PATH_LIST:
		headerPathFileHandler.write('-I'+'\"'+header_path+'\"'+'\n')
	headerPathFileHandler.close()

def createLibCfg():
	
	libCfgHandler = open(GCC_LIB_CFG,'w')
	for lib_path in LIB_PATH_LIST:
		libCfgHandler.write('-L'+'\"'+lib_path[0:-1]+'\"'+'\n')
	for lib_file in LIB_FILE_LIST:
		libCfgHandler.write('-l'+'\":'+lib_file+'\"'+'\n')
	libCfgHandler.close()
	
	
def parseExcludingFolderCfg():
	
	try:
		cfg_file_handler = open(GCC_EXCLUDING_FOLDER_FILE,'r')
		folder_name = ''
		while(True):
			folder_name = cfg_file_handler.readline()
			if(folder_name):
				if(folder_name[0]!='#' and len(folder_name)>2):
					EXCLUDING_FOLDER_LIST.append(folder_name)
			else:
				break
		#print(EXCLUDING_FOLDER_LIST)
		cfg_file_handler.close()
	except Exception as e:
		pass
	pass
	
def main():
	
	compile_result = 0
	OS_TYPE = getOSType()
	
	cleanTargetFiles()
	
	genIOCVersionFile()
	if(os.path.exists(APP_PATH)):
		if(OS_TYPE != 'win32'):
			copySWVersion_linux()
		else:
			pass
	else:
		print('WARNNING: NO PATH:',APP_PATH)
		return
	print('OS_TYPE=',OS_TYPE)
	
	parseExcludingFolderCfg()
	
	file_type_list = ['.c','.S','.h','.a']
	if(searchFiles(APP_PATH,file_type_list,C_FILE_LIST,ASSEMBLE_FILE_LIST,HEADER_PATH_LIST,LIB_PATH_LIST,LIB_FILE_LIST) == False):
		return
	if(searchFiles(GEN_CODE_PATH,file_type_list,C_FILE_LIST,ASSEMBLE_FILE_LIST,HEADER_PATH_LIST,LIB_PATH_LIST,LIB_FILE_LIST) == False):
		return
	if(searchFiles(SDK_PATH,file_type_list,C_FILE_LIST,ASSEMBLE_FILE_LIST,HEADER_PATH_LIST,LIB_PATH_LIST,LIB_FILE_LIST) == False):
		return
	if(searchFiles(PROJECT_SETTINGS_PATH,file_type_list,C_FILE_LIST,ASSEMBLE_FILE_LIST,HEADER_PATH_LIST,LIB_PATH_LIST,LIB_FILE_LIST) == False):
		return
	
	
	if(len(C_FILE_LIST) == 0):
		print('Error: NO C FILES !!!')
		return
	
	if(len(ASSEMBLE_FILE_LIST) == 0):
		print('Error: NO ASSEMBLE FILES !!!')
		return

	if(len(LIB_FILE_LIST) == 0):
		print('Error: NO LIB FILES !!!')
		return
	
	createPath(OUTPUT_PATH)
	createPath(GCC_CFG_GEN_PATH)
	
	createHeaderPathCfg()
	#createLibCfg()
	createOBJListCfg()
	
	start_time = time.time()
	
    #add by tjy findout not need optimize file
	searchExceptOptionFiles()
	
	createMakefile()	
	
	
	ret = os.system('make')
	if(ret != 0):
		print('ERROR:Fail to compile, refer to output/compile.log')
		return

	total_time = time.time() - start_time
	print('compile total time = ',total_time,' seconds')

	print('Compile DONE !!')
	
	copyFilesToBinaryPath()
	
	if(os.path.exists(JFLASH_PATH)):
		copyFilesToJFlashPath()
	else:
		print('WARNNING:NO PATH: ',JFLASH_PATH)
		return
		
	cleanTargetFiles()

	
if __name__ == "__main__":
	main()
