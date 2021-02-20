
#ifndef _SPI_IPC_GW_API_H_
#define _SPI_IPC_GW_API_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "Std_Types.h"
#include "SpiIpc.h"
#include "msg_def.h"

extern error_type Ic_Ipc_send_notification(
uint8 HwChannel,
    uint8 channel,
   	uint16  function,
    uint16 feature,
    const uint8 *data,
    sint32 len);


extern error_type register_Spi_ipc_callback(uint8 HwChannel,uint8 Schannel, Spi_notification_callback ntf_cb);
extern  error_type unregister_Spi_ipc_callback(uint8 HwChannel,uint8 Schannel);
extern Spi_notification_callback  get_Spi_ipc_callback(uint8 HwChannel,uint8 Schannel);

extern error_type ipc_send_notification(
uint8 HwChannel,
    uint8 channel,
    uint16 function,
    const uint8 *data,
    sint32 len);

extern error_type ipc_gw_send_request(
	uint8 HwChannel,
    uint8 channel,
    uint16 function,
    const uint8 *data,
    uint32 len,
    const void *context,
    invoke_callback cb);

extern error_type ipc_gw_send_request_no_reply(
	uint8 HwChannel,
    uint8 channel,
    uint16 function,
    const uint8 *data,
    uint32 len);

extern error_type ipc_gw_send_reply(
	uint8 HwChannel,
    request_context_t *context,
    const uint8 *data,
    uint32 len);

extern error_type ipc_gw_register(
    uint32 channel,
    notification_callback ntf_cb,
    request_callback req_cb);

extern error_type ipc_gw_unregister(uint32 channel);

extern boolean GetM_IpcComStatus(void);
extern IpcCoreErr ipcChannelRecv(int channel, const uint8* buf, int len);
extern void ipcRegisterChannelRecv(ipcChannelRecvFn output_callback);
extern void ipcUnregisterChannelRecv(void);
extern void ipc_gw_init();
extern void ipc_gw_init_M();
#ifdef __cplusplus
}
#endif
#endif


