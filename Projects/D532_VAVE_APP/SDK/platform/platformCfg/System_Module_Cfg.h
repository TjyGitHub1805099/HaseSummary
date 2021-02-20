#ifndef _SYSTEM_MODULE_CFG_H_
#define _SYSTEM_MODULE_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
//User configure area start
#define SYS_MODULE_ADD_CAN STD_ON
#define SYS_MODULE_ADD_ADC STD_ON

#define SYS_MODULE_ADD_IIC STD_ON
#define SYS_MODULE_ADD_EEP STD_ON
#define SYS_MODULE_ADD_EXT_RTC STD_ON

#define SYS_MODULE_ADD_UART STD_ON
#define SYS_MODULE_ADD_SPI STD_ON

#define SYS_MODULE_ADD_TRACE STD_ON

#define SYS_MODULE_ADD_WDG STD_ON

//User configure area end

//Auto relative
#if (STD_ON == SYS_MODULE_ADD_TRACE)
#undef SYS_MODULE_ADD_UART
#define SYS_MODULE_ADD_UART STD_ON
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
/*============================== END OF FILE: System_Module_Cfg.h =============================*/

