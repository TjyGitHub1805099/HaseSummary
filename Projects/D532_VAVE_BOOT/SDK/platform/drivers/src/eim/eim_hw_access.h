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
 * @file eim_hw_access.h
 */

#ifndef EIM_HW_ACCESS_H
#define EIM_HW_ACCESS_H

#include "eim_driver.h"

/*!
 * @brief Error Injection Module Hardware Access.
 * EIM HW ACCESS provides low level APIs for reading and writing register bit-fields
 * belonging to the EIM module.
 * @{
 */

/*******************************************************************************
 * Definitions
 *****************************************************************************/

/*! @brief The position of the most significant bit in Error Injection Channel Enable register */
#define POS_MSB_EIM_EICHEN    (31U)

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name EIM HW ACCESS API
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Resets for the registers of EIM descriptor.
 *
 * This function disables all channels and clears check-bit
 * and data masks of all the channels.
 *
 * @param[in] base EIM peripheral base address
 */
void EIM_InitChannel(EIM_Type * const base);

/*!
 * @brief Enables EIM module.
 *
 * This function enables the error injection function of the EIM.
 *
 * @param[in] base EIM peripheral base address
 */
static inline void EIM_Enable(EIM_Type * const base)
{
    base->EIMCR |= EIM_EIMCR_GEIEN_MASK;
}

/*!
 * @brief Disables the EIM module.
 *
 * This function disables the error injection function of the EIM.
 *
 * @param[in] base EIM peripheral base address
 */
static inline void EIM_Disable(EIM_Type * const base)
{
    base->EIMCR &= ~EIM_EIMCR_GEIEN_MASK;
}

/*!
 * @brief Enables or disables EIM channel operation.
 *
 * This function enables the EIM channel given as argument.
 *
 * @param[in] base EIM peripheral base address
 * @param[in] channel EIM channel number
 * @param[in] enable EIM channel operation
 *                  - true : enables EIM channel
 *                  - false: disables EIM channel
 */
static inline void EIM_EnableChannelCmd(EIM_Type * const base,
                                        uint8_t channel,
                                        bool enable)
{
    uint32_t temp;

    temp = base->EICHEN;
    temp &= ~(1UL << (POS_MSB_EIM_EICHEN - channel));
    temp |= (enable ? 1UL : 0UL) << (POS_MSB_EIM_EICHEN - channel);
    base->EICHEN = temp;
}

/*!
 * @brief Checks whether EIM channel is enabled.
 *
 * This function check whether the EIM channel given as argument is enabled.
 *
 * @param[in] base EIM peripheral base address
 * @param[in] channel EIM channel number
 * @return EIM channel operation status
 *         -true: EIM channel is enabled
 *         -false: EIM channel is disabled
 */
static inline bool EIM_IsChannelEnabled(const EIM_Type * const base,
                                        uint8_t channel)
{
    return ((base->EICHEN & (1UL << (POS_MSB_EIM_EICHEN - channel))) != 0UL);
}

/*!
 * @brief Sets check bit mask for EIM channel.
 *
 * This function sets the check bit mask of the EIM channel given as argument.
 *
 * @param[in] base EIM peripheral base address
 * @param[in] channel EIM channel number
 * @param[in] checkBitMask Check-bit mask
 */
static inline void EIM_SetCheckBitMask(EIM_Type * const base,
                                       uint8_t channel,
                                       uint8_t checkBitMask)
{
    base->EICHDn[channel].WORD0 = EIM_EICHDn_WORD0_CHKBIT_MASK(checkBitMask);
}

/*!
 * @brief Gets check bit mask of EIM channel.
 *
 * This function gets check bit mask of EIM channel given as argument.
 *
 * @param[in] base EIM peripheral base address
 * @param[in] channel EIM channel number
 * @return Check-bit mask
 */
static inline uint8_t EIM_GetCheckBitMask(const EIM_Type * const base,
                                          uint8_t channel)
{
    return ((uint8_t)((base->EICHDn[channel].WORD0) >> EIM_EICHDn_WORD0_CHKBIT_MASK_SHIFT));
}

/*!
 * @brief Sets data mask for EIM channel.
 *
 * This function sets data mask of the EIM channel given as argument.
 *
 * @param[in] base EIM peripheral base address
 * @param[in] channel EIM channel number
 * @param[in] dataMask Data mask
 */
static inline void EIM_SetDataMask(EIM_Type * const base,
                                   uint8_t channel,
                                   uint32_t dataMask)
{
    base->EICHDn[channel].WORD1 = dataMask;
}

/*!
 * @brief Gets data mask of EIM channel.
 *
 * This function gets data mask of EIM channel given as argument.
 *
 * @param[in] base EIM peripheral base address
 * @param[in] channel EIM channel number
 * @return Data mask
 */
static inline uint32_t EIM_GetDataMask(const EIM_Type * const base,
                                       uint8_t channel)
{
    return (base->EICHDn[channel].WORD1);
}

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* EIM_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 *******************************************************************************/
