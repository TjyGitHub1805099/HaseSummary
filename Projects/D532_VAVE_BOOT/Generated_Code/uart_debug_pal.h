/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : uart_debug_pal.h
**     Project     : D532_MC_USB_SWDL
**     Processor   : S32K148_176
**     Component   : uart_pal
**     Version     : Component SDK_S32K14x_08, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K14x_08
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-02-20, 20:27, # CodeGen: 0
**     Contents    :
**         UART_Init                - status_t UART_Init(uart_instance_t instance, const uart_user_config_t *config);
**         UART_Deinit              - status_t UART_Deinit(uart_instance_t instance);
**         UART_SetBaudRate         - status_t UART_SetBaudRate(uart_instance_t instance, uint32_t desiredBaudRate);
**         UART_GetBaudRate         - status_t UART_GetBaudRate(uart_instance_t instance, uint32_t  *...
**         UART_SendDataBlocking    - status_t UART_SendDataBlocking(uart_instance_t instance, const uint8_t *...
**         UART_SendData            - status_t UART_SendData(uart_instance_t instance, const uint8_t * txBuff,...
**         UART_AbortSendingData    - status_t UART_AbortSendingData(uart_instance_t instance);
**         UART_GetTransmitStatus   - status_t UART_GetTransmitStatus(uart_instance_t instance, uint32_t *...
**         UART_ReceiveDataBlocking - status_t UART_ReceiveDataBlocking(uart_instance_t instance, uint8_t * rxBuff,...
**         UART_ReceiveData         - status_t UART_ReceiveData(uart_instance_t instance, uint8_t * rxBuff,...
**         UART_AbortReceivingData  - status_t UART_AbortReceivingData(uart_instance_t instance);
**         UART_GetReceiveStatus    - status_t UART_GetReceiveStatus(uart_instance_t instance, uint32_t *...
**
**     Copyright 1997 - 2015 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     All Rights Reserved.
**     
**     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
**     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
**     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
**     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
**     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
**     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**     THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file uart_debug_pal.h
** @version 01.00
*/
/*!
**  @addtogroup uart_debug_pal_module uart_debug_pal module documentation
**  @{
*/
#ifndef uart_debug_pal_H
#define uart_debug_pal_H

/* MODULE uart_debug_pal. */

/* Include inherited beans */
#include "clockManager_cfg.h"
#include "dmaController_cfg.h"
#include "Cpu.h"

/*! @brief Device instance number */
#define INST_UART_DEBUG_PAL (0U)

/*! @brief Serial module configurations */
extern uart_user_config_t uart_debug_pal_config;

/*! @brief Define external callbacks for RX and TX */

#endif
/* ifndef __uart_debug_pal_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/

