/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : lpi2c0_cfg.c
**     Project     : D532_MC_USB_SWDL
**     Processor   : S32K148_176
**     Component   : lpi2c
**     Version     : Component SDK_S32K14x_08, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K14x_08
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-02-20, 20:27, # CodeGen: 0
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
** @file lpi2c0_cfg.c
** @version 01.00
*/
/*!
**  @addtogroup lpi2c0_cfg_module lpi2c0_cfg module documentation
**  @{
*/

/* lpi2c0_cfg. */

#include "lpi2c0_cfg.h"

/*!
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in
 * writing dynamic code is that the stack segment may be different from the data
 * segment.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.1, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.2, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.4, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.5, Identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, Could be made static.
 * Functions are APIs, so they shall not be made static.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * The symbols are declared in the driver common file as external; they are needed
 * at driver initialization to install the correct interrupt handler, but are not
 * a part of the public API.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.1, Conversion between a pointer
 * to function and another type.
 * The cast is required to define a callback function.
 */


const lpi2c_master_user_config_t lpi2c0_MasterCfg_Amp = {
  .slaveAddress = 108U,
  .is10bitAddr = false,
  .operatingMode = LPI2C_FAST_MODE,
  .baudRate = 400000U,
  .transferType = LPI2C_USING_INTERRUPTS,
  .dmaChannel = 0U,
  .masterCallback = lpi2c0_cfg_MasterCallbackAMP,
  .callbackParam = NULL,
};

const lpi2c_master_user_config_t lpi2c0_MasterCfg_Rtc = {
  .slaveAddress = 0U,
  .is10bitAddr = false,
  .operatingMode = LPI2C_FAST_MODE,
  .baudRate = 400000U,
  .transferType = LPI2C_USING_INTERRUPTS,
  .dmaChannel = 0U,
  .masterCallback = lpi2c0_cfg_MasterCallbackRTC,
  .callbackParam = NULL,
};

const lpi2c_master_user_config_t lpi2c0_MasterCfg_Gyro = {
  .slaveAddress = 0U,
  .is10bitAddr = false,
  .operatingMode = LPI2C_FAST_MODE,
  .baudRate = 400000U,
  .transferType = LPI2C_USING_INTERRUPTS,
  .dmaChannel = 0U,
  .masterCallback = lpi2c0_cfg_MasterCallbackGYRO,
  .callbackParam = NULL,
};

const lpi2c_master_user_config_t lpi2c0_MasterCfg_Acce = {
  .slaveAddress = 0U,
  .is10bitAddr = false,
  .operatingMode = LPI2C_FAST_MODE,
  .baudRate = 400000U,
  .transferType = LPI2C_USING_INTERRUPTS,
  .dmaChannel = 0U,
  .masterCallback = lpi2c0_cfg_MasterCallbackACCE,
  .callbackParam = NULL,
};


/* END lpi2c0_cfg. */

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
