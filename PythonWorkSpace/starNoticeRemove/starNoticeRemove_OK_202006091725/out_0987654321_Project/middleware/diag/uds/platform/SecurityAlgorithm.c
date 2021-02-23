



#include "SecurityAlgorithm.h"


#define ROTATE_LEFT(x,n)  ((x) << (n)) | ((x) >> ((sizeof(x)*8) - (n)))
#define ROTATE_RIGHT(x,n) ((x) >> (n)) | ((x) << ((sizeof(x)*8) - (n)))

#define rounds 2
uint32 k[4]={0};
uint32 v[2]={0};
uint8  SA_constant[4]={0};

static void CreateSA(uint8* SerialNum,uint8 SerialNumLen,uint8 SecurityLevel);
static void encipher(unsigned int num_rounds, uint32 v[2], uint32 const k[4]);


static void CreateSA(uint8* SerialNum,uint8 SerialNumLen,uint8 SecurityLevel)
{
	uint8 l_Loop=0;
	uint8 l_Len=0;
	
	l_Len = SerialNumLen;
	for(l_Loop=0;l_Loop<4;l_Loop++)
	{
		if(0 < l_Len)
		{
			SA_constant[3-l_Loop] = SerialNum[SerialNumLen-1-l_Loop];
		}
		else
		{
			SA_constant[3-l_Loop] = 0x00;
		}
		l_Len = (l_Len>0)?(l_Len - 1):0;
	}
	for(l_Loop=1;l_Loop<SecurityLevel;l_Loop++)
	{
		SA_constant[0] = (uint8)(ROTATE_RIGHT(SA_constant[0],3));
		SA_constant[1] = (uint8)(ROTATE_RIGHT(SA_constant[1],3));
		SA_constant[2] = (uint8)(ROTATE_RIGHT(SA_constant[2],3));
		SA_constant[3] = (uint8)(ROTATE_RIGHT(SA_constant[3],3));
	}
}

static void encipher(unsigned int num_rounds, uint32 v[2], uint32 const k[4])
{
	unsigned int i; 
	uint32 v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9; 
	for (i=0; i < num_rounds; i++) 
	{
		v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]); 
		sum += delta; 
		v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]); 
	}
	v[0]=v0; 
	v[1]=v1; 
}

void CreateKey(uint8* ECUSerNum,uint8 ECUSerNumLen,const uint8* seed,uint8 SecurityLevel,uint8* RetKey)
{
	
	CreateSA(ECUSerNum,ECUSerNumLen,SecurityLevel);
	
	v[0] = (uint32)((seed[0]<<24)|(seed[1]<<16)|(seed[2]<<8)|(seed[3]));
	v[1] = (uint32)(((0xff^seed[0])<<24)|((0xff^seed[1])<<16)|((0xff^seed[2])<<8)|(0xff^seed[3]));
	k[0] = (uint32)(SA_constant[0]);
	k[1] = (uint32)(SA_constant[1]);
	k[2] = (uint32)(SA_constant[2]);
	k[3] = (uint32)(SA_constant[3]);
	encipher(rounds,&v[0], &k[0]);
	RetKey[0] = (uint8)(v[0]>>24);
	RetKey[1] = (uint8)(v[0]>>16);
	RetKey[2] = (uint8)(v[0]>>8);
	RetKey[3] = (uint8)v[0];
}






