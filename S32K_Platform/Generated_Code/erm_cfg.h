/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : erm_cfg.h
**     Project     : S32K_146
**     Processor   : S32K146_144
**     Component   : erm
**     Version     : Component SDK_S32K14x_08, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K14x_08
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-05-15, 15:28, # CodeGen: 60
**     Contents    :
**         ERM_DRV_Init               - void ERM_DRV_Init(uint32_t instance,uint8_t channelCnt,const...
**         ERM_DRV_Deinit             - void ERM_DRV_Deinit(uint32_t instance);
**         ERM_DRV_SetInterruptConfig - void ERM_DRV_SetInterruptConfig(uint32_t instance,uint8_t...
**         ERM_DRV_GetInterruptConfig - void ERM_DRV_GetInterruptConfig(uint32_t instance,uint8_t...
**         ERM_DRV_ClearEvent         - void ERM_DRV_ClearEvent(uint32_t instance,uint8_t channel,erm_ecc_event_t...
**         ERM_DRV_GetErrorDetail     - erm_ecc_event_t ERM_DRV_GetErrorDetail(uint32_t instance,uint8_t...
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
** @file erm_cfg.h
** @version 01.00
*/
/*!
**  @addtogroup erm_cfg_module erm_cfg module documentation
**  @{
*/
#ifndef erm_cfg_H
#define erm_cfg_H

/* MODULE erm_cfg.
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The global macro will be used in function call of the module.
 */

/* Include inherited beans */
#include "Cpu.h"

/*! @brief Device instance number */
#define INST_ERM_CFG (0U)

/*! @brief The number of configured channel(s) */
#define ERM_CHANNEL_COUNT0 (1U)

/*! @brief Interrupt configuration */
extern const erm_interrupt_config_t erm1_Interrupt0;

/*! @brief ERM user configuration declaration */
extern const erm_user_config_t erm_InitConfig[ERM_CHANNEL_COUNT0];


#endif
/* ifndef erm_cfg_H */
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
