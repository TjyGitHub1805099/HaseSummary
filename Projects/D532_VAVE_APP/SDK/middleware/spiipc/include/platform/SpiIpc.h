

#ifndef _SPI_IPC_H_
#define _SPI_IPC_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"


#define SPI_CQUEUE_TXCH_SIZE      	640
#define SPI_CQUEUE_RXCH_SIZE      	320
#define SPI_MAX_FRAME_SIZE			128
#define SPI_MAX_CRC_SIZE			4
#define SPI_HEAD_SIZE				2
#define SPI_FUN_FEATURE_SIZE		4
#define SPI_GAP_SIZE				2
#define SPI_MAX_DATA_SIZE			(SPI_MAX_FRAME_SIZE-SPI_MAX_CRC_SIZE-SPI_HEAD_SIZE-SPI_FUN_FEATURE_SIZE-SPI_GAP_SIZE)


#define IPC_FLOWCTRL_CHANNEL		0
#define IPC_CHANNEL_MASK			0x3F
#define IPC_MULTI_MASK				0x80
#define IPC_CRC_MASK				0x40
#define IPC_MULTI_SHIFT				7
#define IPC_CRC_SHIFT				6


typedef enum 
{
    IPC_ERR_OK = 0,

    IPC_ERR_LINK_DONW,

    IPC_ERR_SEND_BUF_FULL,
    IPC_ERR_RECV_BUF_FULL,       

    IPC_ERR_PARA_ILLEGAL,    

    IPC_ERR_OTHER_ERRORS
} IpcCoreErr;

typedef struct{
	uint8 Num;
	uint8 HwChannel;
	uint8 ChannelNum;
	uint16 TxBufSize;
	uint16 RxBufSize;
}SpiIpcChCfg;

typedef struct{
	uint8 Tx;
	uint8 Rx;
	uint8 Global;
}SpiCqMutex;


typedef struct{
	uint8 spiHwMaxCh;
	uint8 spiIpcMutexPoolSize;
	uint8 spiIpcSemPoolSize;
}SpiIpcGlobalStruct;

typedef enum
{
	SPI_IPC_SINGLE_CH = 0,
	SPI_IPC_MULTI_CH,
}SPI_IPC_MERGE_CH_TYPE;


typedef void (*spi_initfun)(void);
typedef void (*spi_ReqAckfun)(void);
typedef boolean (*spi_TxRxfun)(uint8 *pBuf,uint16 Len,boolean WaitFlag);
typedef boolean (*spi_Transfun)(uint8 *pReceiveBuf,uint8 *pSendBuf,uint16 Len,boolean WaitFlag);
typedef uint8 (*spi_GetStatus)(void);
typedef uint8 (*spi_AbortTransfer)(void);
typedef uint8 (*spi_GetLastTransErrorState)(void);
typedef void (*spi_SetLastTransErrorState)(uint8);
typedef SPI_IPC_MERGE_CH_TYPE (*spi_GetChMergeType)(void);


typedef IpcCoreErr (*ipcChannelRecvFn) (uint16 channel,const uint8* buf, int len);   // ipcProcessChannelData
typedef void (*Spi_notification_callback)(uint16 function, uint16 feature, uint8 *rx_data, sint32 len);
typedef void (*invoke_callback)(const void *ctx, uint16 function, uint8 *rx_data,uint32 len);
#ifdef __cplusplus
}
#endif 

#endif


