/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file eim_driver.h
 */

#ifndef EIM_DRIVER_H
#define EIM_DRIVER_H

#include <stddef.h>
#include "device_registers.h"


/*!
 * @defgroup eim_drv EIM Driver
 * @ingroup eim
 * @brief Error Injection Module Peripheral Driver.@n
 * EIM PD provides a set of high-level APIs/services to configure the
 * Error Injection Module (EIM) module.
 * @addtogroup eim_drv
 * @{
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The value default of EIM check-bit mask  */
#define EIM_CHECKBITMASK_DEFAULT    (0x01U)
/*! @brief The value default of EIM data mask  */
#define EIM_DATAMASK_DEFAULT        (0x00U)

/*!
 * @brief EIM channel configuration structure
 *
 * This structure holds the configuration settings for the EIM channel
 * Implements : eim_user_channel_config_t_Class
 */
typedef struct
{
    uint8_t channel;      /*!< EIM channel number                                          */
    uint8_t checkBitMask; /*!< Specifies whether the corresponding bit of the check-bit bus
                               from the target RAM should be inverted or remain unmodified */
    uint32_t dataMask;    /*!< Specifies whether the corresponding bit of the read data bus
                               from the target RAM should be inverted or remain unmodified */
    bool enable;          /*!< true : EIM channel operation is enabled
                               false : EIM channel operation is disabled                   */
} eim_user_channel_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name EIM Driver API
 * @{
 */

#if defined(__cplusplus)
extern "C"
{
#endif

/*!
 * @brief Initializes the EIM module.
 *
 * This function configures for EIM channels.
 * The EIM channel configuration structure array and number of configured channels
 * shall be passed as arguments.
 * This function should be called before calling any other EIM driver function.
 *
 * This is an example demonstrating how to define a EIM channel configuration structure array:
 @code
 eim_user_channel_config_t channelConfigArr[] =
 {
 {
 .channel = 0x0U,
 .checkBitMask = 0x12U,
 .dataMask = 0x01234567U,
 .enable = true
 },
 {
 .channel = 0x1U,
 .checkBitMask = 0x22U,
 .dataMask = 0x01234444U,
 .enable = false
 }
 };
 @endcode
 *
 * @param[in] instance EIM module instance number.
 * @param[in] channelCnt Number of configured channels
 * @param[in] channelConfigArr EIM channel configuration structure array
 */
void EIM_DRV_Init(uint32_t instance,
                  uint8_t channelCnt,
                  const eim_user_channel_config_t *channelConfigArr);

/*!
 * @brief De-initializes the EIM module.
 *
 * This function sets all registers to reset value and disables EIM module.
 * In order to use the EIM module again, EIM_DRV_Init must be called.
 *
 * @param[in] instance EIM module instance number
 */
void EIM_DRV_Deinit(uint32_t instance);

/*!
 * @brief Configures the EIM channel.
 *
 * This function configures check-bit mask, data mask and
 * operation status(enable/disable) for EIM channel.
 * The EIM channel configuration structure shall be
 * passed as arguments.
 *
 * This is an example demonstrating how to define a EIM channel configuration structure:
   @code
   eim_user_channel_config_t eimTestInit = {
        .channel = 0x1U,
        .checkBitMask = 0x25U,
        .dataMask = 0x11101100U,
        .enable = true
   };
   @endcode
 *
 * @param[in] instance EIM module instance number
 * @param[in] userChannelConfig Pointer to EIM channel configuration structure
 */
void EIM_DRV_ConfigChannel(uint32_t instance,
                           const eim_user_channel_config_t *userChannelConfig);

/*!
 * @brief Gets the EIM channel configuration.
 *
 * This function gets check bit mask, data mask and operation status of EIM channel.
 *
 * @param[in] instance EIM module instance number
 * @param[in] channel EIM channel number
 * @param[out] channelConfig Pointer to EIM channel configuration structure
 */
void EIM_DRV_GetChannelConfig(uint32_t instance,
                              uint8_t channel,
                              eim_user_channel_config_t *channelConfig);

/*!
 * @brief Gets the EIM channel configuration default.
 *
 * This function gets check bit mask, data mask and operation status default of EIM channel.
 *
 * @param[in] channel EIM channel number
 * @param[out] channelConfig Pointer to EIM channel configuration structure default
 */
void EIM_DRV_GetDefaultConfig(uint8_t channel,
                              eim_user_channel_config_t *channelConfig);

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* EIM_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
