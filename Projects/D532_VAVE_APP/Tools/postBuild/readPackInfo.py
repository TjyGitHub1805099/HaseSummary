#*-*coding:utf-8 *-*  
import os
import sys
import binascii
import ctypes

def readVersionInfo(inFile,outFile,startAddr):
	inputFileName = inFile
	outputfile = open(outFile,'w')

	inputfile = open(inputFileName,'rb')
	inputfile.seek(0x410,0)

	outputfile.write("jenkinsVerNum = None\n")

	outputfile.write("iocCompileDate = ")
	readContent = inputfile.read(11)
	outputfile.write(readContent)
	outputfile.write("\n")

	outputfile.write("iocCompileTime = ")
	readContent = inputfile.read(8)
	outputfile.write(readContent)
	outputfile.write("\n")

	outputfile.write("SysSupplierID = ")
	readContent = inputfile.read(5)
	outputfile.write('"'+readContent+'"')
	outputfile.write("\n")

	outputfile.write("EcuName = ")
	readContent = inputfile.read(15)
	outputfile.write('"'+readContent+'"')
	outputfile.write("\n")

	outputfile.write("partNum&&Ver = ")
	readContent = inputfile.read(4)
	tempValue1 = binascii.hexlify(readContent)
	tempValue2 = inputfile.read(1)
	readContent = inputfile.read(1)
	tempValue3 = binascii.hexlify(readContent)
	outputfile.write(tempValue1+tempValue2+tempValue3)
	outputfile.write("\n")

	outputfile.write("ecuSWVerison = ")
	readContent = inputfile.read(7)
	tempValue = binascii.hexlify(readContent)
	outputfile.write(tempValue)
	outputfile.write("\n")

	outputfile.write("appSWVersion = ")
	readContent = inputfile.read(7)
	tempValue = binascii.hexlify(readContent)
	outputfile.write(tempValue)
	outputfile.write("\n")

	outputfile.write("bscSWVersion = ")
	readContent = inputfile.read(7)
	tempValue = binascii.hexlify(readContent)
	outputfile.write(tempValue)
	outputfile.write("\n")

	outputfile.write("hmiSWVersion = ")
	readContent = inputfile.read(7)
	tempValue = binascii.hexlify(readContent)
	outputfile.write(tempValue)
	outputfile.write("\n")

	# inputfile.seek(2,1)

	outputfile.write("supplierPartNum = ")
	readContent = inputfile.read(10)
	outputfile.write(readContent)
	outputfile.write("\n")

	outputfile.write("supplierSWVersion = ")
	readContent = inputfile.read(11)
	outputfile.write(readContent)
	outputfile.write("\n")

	outputfile.write("adaptiveHWVersion = ")
	readContent = inputfile.read(10)
	outputfile.write(readContent)
	outputfile.write("\n")

	outputfile.write("dbcVersion = ")
	readContent = inputfile.read(2)
	outputfile.write('0x'+binascii.hexlify(readContent).upper())
	outputfile.write("\n")

	outputfile.write("packStartAddr = ")
	outputfile.write(str(startAddr))
	outputfile.write("\n")

	outputfile.write("packLen = ")
	tempLen = os.path.getsize(inputFileName)
	result = ctypes.c_uint32(tempLen).value
	outputfile.write(str(result)+' / 0x'+str(hex(result).replace('L','').upper()[2:]))
	outputfile.write("\n")

	inputfile.seek(0, 0)
	file_content = inputfile.read()
	l_tempCrc = binascii.crc32(file_content)
	result = ctypes.c_uint32(l_tempCrc).value
	outputfile.write("packCrc = ")
	outputfile.write('0x'+str(hex(result).replace('L','').upper()[2:]))
	outputfile.write("\n")

	outputfile.close()
	inputfile.close()
	print "generate pack information over!"



if __name__ == '__main__':
	if 4!= len(sys.argv):
		if (2==len(sys.argv)) and (("-h"==sys.argv[1]) or ("-H"==sys.argv[1])):
			print ">readPackInfo.exe inputFile outputFile startAddr(hex)"
			print "example:\n>readPackInfo.exe app.bin packInfo.txt 0x80000"
		else:
			print "\t!Parameter Error!\n>readPackInfo.exe inputFile outputFile startAddr(hex)"
			print "example:\n>readPackInfo.exe app.bin packInfo.txt 0x80000"
	else:
		readVersionInfo(sys.argv[1],sys.argv[2],sys.argv[3])


	
