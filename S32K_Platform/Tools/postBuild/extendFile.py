#*-*coding:utf-8 *-*  
import os
import sys

def gen_file(path,size,pad):   
	file = open(path,'wb')    
	file.seek(size)
	file.write(chr(int(pad,16)))
	file.close()  

def extend_file(sourceFile,newFile,extendLen,pad):
	binLen = os.path.getsize(sourceFile)
	infile = open(sourceFile, 'rb')
	outfile = open(newFile, 'wb')
	outfile.write(infile.read())
	if extendLen > (binLen+1):
		diffLen = int(extendLen,16)-binLen-1
		gen_file('temp.bin',diffLen,pad)
		tempfile = open('temp.bin', 'rb')
		outfile.write(tempfile.read())
		tempfile.close()
		os.remove('temp.bin')
	infile.close()
	outfile.close()

if __name__ == "__main__":
	if 5!= len(sys.argv):
		if (2==len(sys.argv)) and (("-h"==sys.argv[1]) or ("-H"==sys.argv[1])):
			print ">extendFile.exe oldFileName newFileName newFileLen(hex) pad(hex)"
			print "example:\n>extendFile.exe app.bin new.bin 0x40000 0x00"
		else:
			print "\t!Parameter Error!\n>extendFile.exe oldFileName newFileName newFileLen(hex) pad(hex)"
			print "example:\n>extendFile.exe app.bin new.bin 0x40000 0x00"
	else:
		extend_file(sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4])