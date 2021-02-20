/*
 * Copyright (c) 2013-2014, Freescale Semiconductor, Inc.
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
#ifndef POWER_PMC_HW_ACCESS_H
#define POWER_PMC_HW_ACCESS_H

#include "device_registers.h"
#include "power_manager_S32K1xx.h"

/*!
 * @file power_pmc_hw_access.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * pmc_config_t is referenced from clock manager.
 */

/*!
 * @ingroup power_pmc_hw_access
 * @defgroup power_pmc_hw_access
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*! @name Power Management Controller Control APIs*/
/*@{*/

/*!
 * @brief Enables/Disables the low voltage-related interrupts.
 *
 * This function enables the low voltage detection, warning,
 * etc. interrupts.
 *
 * @param[in] baseAddr  Base address for current PMC instance.
 * @param[in] intSelect Interrupt select
 * @param[in] enable    Enable/disable the interrupt
 */
void PMC_SetLowVoltIntCmd(PMC_Type * const baseAddr,
                          const pmc_int_select_t intSelect,
                          const bool enable);

/*!
 * @brief Acknowledges the low voltage-related interrupts.
 *
 * This function acknowledges the low voltage detection, warning,
 * etc. interrupts
 *
 * @param[in] baseAddr  Base address for current PMC instance.
 * @param[in] intSelect Interrupt select
 */
void PMC_SetLowVoltIntAckCmd(PMC_Type * const baseAddr,
                             const pmc_int_select_t intSelect);

/*!
 * @brief Gets the flag for the low voltage-related interrupts.
 *
 * This function gets the flag for the low voltage detection, warning,
 * etc. interrupts
 *
 * @param[in] baseAddr  Base address for current PMC instance.
 * @param[in] intSelect Interrupt select
 * @return status Current low voltage interrupt flag
 *                - true: Low-Voltage Interrupt flag is set
 *                - false: Low-Voltage Interrupt flag is not set
 */
bool PMC_GetLowVoltIntFlag(const PMC_Type * const baseAddr,
                           const pmc_int_select_t intSelect);

/*!
 * @brief Low-Voltage Detect Hardware Reset Enable/Disable (write once)
 *
 * This function enables/disables the hardware reset for the low voltage
 * detection. When enabled, if the LVDF (Low Voltage Detect Flag) is set, a
 * hardware reset occurs. This setting is a write-once-only. Any additional writes
 * are ignored.
 *
 * @param[in] baseAddr  Base address for current PMC instance.
 * @param[in] enable    Enable/disable the LVD hardware reset
 */
static inline void PMC_SetLowVoltDetectResetCmd(PMC_Type * const baseAddr,
                                                const bool enable)
{
    uint8_t regValue = baseAddr->LVDSC1;
    regValue &= (uint8_t)(~(PMC_LVDSC1_LVDRE_MASK));
    regValue |= (uint8_t)PMC_LVDSC1_LVDRE(enable ? 1U : 0U);
    baseAddr->LVDSC1 = regValue;
}

/*!
 * @brief Enables/Disables the Bias.
 *
 * This function enables/disables the Bias.
 *
 * @param[in] baseAddr  Base address for current PMC instance.
 * @param[in] enable    Enable/disable the Bias.
 */
static inline void PMC_SetBiasMode(PMC_Type * const baseAddr,
                                   const bool enable)
{
    uint8_t regValue = baseAddr->REGSC;
    regValue &= (uint8_t)(~(PMC_REGSC_BIASEN_MASK));
    regValue |= (uint8_t)PMC_REGSC_BIASEN(enable ? 1U : 0U);
    baseAddr->REGSC = regValue;
}

/*!
 * @brief Gets the Regulator regulation status.
 *
 * This function provides the current status of
 * the internal voltage regulator.
 *
 * @param[in] baseAddr  Base address for current PMC instance.
 * @return value Regulation status
 *               0 - Regulator is in low power mode or transition to/from.
 *               1 - Regulator is in full performance mode.
 */
static inline uint8_t PMC_GetRegulatorStatus(const PMC_Type * const baseAddr)
{
    uint8_t regValue = baseAddr->REGSC;
    regValue = (uint8_t)((regValue & PMC_REGSC_REGFPM_MASK) >> PMC_REGSC_REGFPM_SHIFT);

    return regValue;
}

/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* POWER_PMC_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
