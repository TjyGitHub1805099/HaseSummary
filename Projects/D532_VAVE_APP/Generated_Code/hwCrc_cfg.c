/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : hwCrc_cfg.c
**     Project     : D532_VAVE_APP
**     Processor   : S32K148_176
**     Component   : crc
**     Version     : Component SDK_S32K14x_08, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K14x_08
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-11-26, 14:42, # CodeGen: 106
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
** @file hwCrc_cfg.c
** @version 01.00
*/
/*!
**  @addtogroup hwCrc_cfg_module hwCrc_cfg module documentation
**  @{
*/

/* MODULE hwCrc_cfg.
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.
 * The external variable will be used in other source file that user initialize
 * to use this module.
 */

#include "hwCrc_cfg.h"

/*! @brief Configuration structure hwCrc_cfg_InitConfig0 */
const crc_user_config_t hwCrc_cfg_InitConfig0 = {
    .crcWidth = CRC_BITS_16,
    .seed = 0xFFFFU,
    .polynomial = 0x1021U,
    .writeTranspose = CRC_TRANSPOSE_NONE,
    .readTranspose = CRC_TRANSPOSE_NONE,
    .complementChecksum = false
};

/*! @brief Configuration structure hwCrc_cfg_InitConfig1 */
const crc_user_config_t hwCrc_cfg_InitConfig1 = {
    .crcWidth = CRC_BITS_32,
    .seed = 0xFFFFFFFFU,
    .polynomial = 0x04C11DB7U,
    .writeTranspose = CRC_TRANSPOSE_NONE,
    .readTranspose = CRC_TRANSPOSE_NONE,
    .complementChecksum = false
};

/*! @brief Configuration structure hwCrc_cfg_InitConfig2 */
const crc_user_config_t hwCrc_cfg_InitConfig2 = {
    .crcWidth = CRC_BITS_32,
    .seed = 0xFFFFFFFFU,
    .polynomial = 0xF4ACFB13U,
    .writeTranspose = CRC_TRANSPOSE_NONE,
    .readTranspose = CRC_TRANSPOSE_NONE,
    .complementChecksum = false
};


/* END hwCrc_cfg. */

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
