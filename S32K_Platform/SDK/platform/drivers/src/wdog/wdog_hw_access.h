/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
 * @file wdog_hw_access.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro,
 * 'WDOG_UNLOCK', defined.
 * This macro is needed in order to unlock the WDOG prior to any update. The
 * reason for not using a function is the limited number of bus clocks in which
 * the WDOG can be reconfigured after an unlock sequence.
 *
 */

#ifndef WDOG_HW_ACCESS_H
#define WDOG_HW_ACCESS_H

#include <stddef.h>
#include "wdog_driver.h"

/*!
 * @brief Watchdog Timer Hardware Access.
 *
 * This hardware access provides low-level access to all hardware features of the WDOG.
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* The dummy read is used in order to make sure that any write to the
 * WDOG registers will be started only after the write of the unlock value was
 * completed.
 */
#define WDOG_UNLOCK32(base)  \
{ \
    (base)->CNT = FEATURE_WDOG_UNLOCK_VALUE; \
    (void)(base)->CNT; \
}

#define WDOG_UNLOCK16(base)  \
{ \
    (base)->CNT = FEATURE_WDOG_UNLOCK16_FIRST_VALUE; \
    (void)(base)->CNT; \
    (base)->CNT = FEATURE_WDOG_UNLOCK16_SECOND_VALUE; \
    (void)(base)->CNT; \
}

#define WDOG_UNLOCK(base)    \
{ \
    if (((base)->CS & WDOG_CS_CMD32EN_MASK) != 0U) \
    { \
        WDOG_UNLOCK32(base); \
    } \
    else \
    { \
        WDOG_UNLOCK16(base); \
    } \
}

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name WDOG Common Configurations
 * @{
 */

/*!
 * @brief Configures the WDOG.
 * This function configures the WDOG by user configuration
 *
 * @param[in] base WDOG base pointer.
 * @param[in] wdogUserConfig pointer to user configuration structure
 * @return the state of the WDOG.
 */
status_t WDOG_Config(WDOG_Type * const base,
                     const wdog_user_config_t * wdogUserConfig);

/*!
 * @brief Gets the current WDOG configuration.
 *
 * This function gets the current WDOG configuration
 *
 *
 * @param[in] base WDOG base pointer.
 * @return the current WDOG configuration
 */
wdog_user_config_t WDOG_GetConfig(const WDOG_Type *base);

/*!
 * @brief Enables the WDOG.
 *
 * This function enables the WDOG.
 *
 * @param[in] base WDOG base pointer.
 */
static inline void WDOG_Enable(WDOG_Type *base)
{
    WDOG_UNLOCK(base);

    base->CS |= WDOG_CS_EN_MASK;
}

/*!
 * @brief Disables the WDOG.
 *
 * This function disables the WDOG.
 *
 * @param[in] base WDOG base pointer.
 */
static inline void WDOG_Disable(WDOG_Type *base)
{
    WDOG_UNLOCK(base);

    base->CS &=  ~WDOG_CS_EN_MASK;
}

/*!
 * @brief Verifies if the WDOG is enabled.
 *
 * This function verifies the state of the WDOG.
 *
 * @param[in] base WDOG base pointer.
 * @return the state of the WDOG.
 */
static inline bool WDOG_IsEnabled(const WDOG_Type * base)
{
    return ((base->CS & WDOG_CS_EN_MASK) >> WDOG_CS_EN_SHIFT) != 0U;
}

/*!
 * @brief Refreshes the WDOG counter
 *
 * @param[in] base WDOG base pointer.
 */
static inline void WDOG_Trigger(WDOG_Type *base)
{
    if ((base->CS & WDOG_CS_CMD32EN_MASK) != 0U)
    {
        base->CNT = FEATURE_WDOG_TRIGGER_VALUE;
    }
    else
    {
        base->CNT = FEATURE_WDOG_TRIGGER16_FIRST_VALUE;
        (void)base->CNT;
        base->CNT = FEATURE_WDOG_TRIGGER16_SECOND_VALUE;
    }
}

/*!
 * @brief Enables/Disables window mode.
 *
 * This function enables/disables window mode for the WDOG.
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable window mode
 */
static inline void WDOG_SetWindowMode(WDOG_Type * base,
                                      bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_WIN_MASK) | WDOG_CS_WIN(enable ? 1UL : 0UL);
}

/*!
 * @brief Sets the value of the WDOG window.
 *
 * This sets the value of the WDOG window.
 *
 * @param[in] base WDOG base pointer.
 * @param[in] window the value of the WDOG window.
 */
static inline void WDOG_SetWindowValue(WDOG_Type * base,
                                       uint16_t window)
{
    WDOG_UNLOCK(base);

    base->WIN = window;
}

/*!
 * @brief Enables/Disables WDOG interrupt.
 *
 * This function enables/disables the interrupts from the WDOG.
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable interrupts.
 */
static inline void WDOG_SetInt(WDOG_Type *base,
                               bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_INT_MASK) | WDOG_CS_INT(enable? 1UL : 0UL);
}

/*!
 * @brief Clears the Interrupt Flag.
 *
 * This function clears the Interrupt Flag (FLG).
 *
 * @param[in] base WDOG base pointer.
 */
static inline void WDOG_ClearIntFlag(WDOG_Type * base)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_FLG_MASK) | WDOG_CS_FLG(1U);

#ifdef ERRATA_E9005
    /* Read-after-write sequence to guarantee required serialization of memory operations */
    (void)base->CS;
#endif
}

/*!
 * @brief Enables/Disables prescaler.
 *
 * This function enables/disables the prescaler for the WDOG.
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable prescaler.
 */
static inline void WDOG_SetPrescaler(WDOG_Type * base,
                                     bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_PRES_MASK) | WDOG_CS_PRES(enable ? 1UL : 0UL);
}

/*!
 * @brief Selects the clock source used by the WDOG.
 *
 * This function selects the clock source used by the WDOG.
 *
 * @param[in] base WDOG base pointer.
 * @param[in] clkSource the clock source to be used by the WDOG:
 *         - 0x00U: Bus clock
 *         - 0x01U: LPO clock
 *         - 0x02U: System oscillator clock (SOSC, from SCG)
 *         - 0x03U: Slow internal reference clock (SIRC, from SCG)
 */
static inline void WDOG_SetClockSource(WDOG_Type * base,
                                       wdog_clk_source_t clkSource)
{
    uint32_t regValue = base->CS;

    regValue &= ~(WDOG_CS_CLK_MASK);
    regValue |= WDOG_CS_CLK(clkSource);

    WDOG_UNLOCK(base);

    base->CS = regValue;
}

/*!
 * @brief Gets the clock source of the WDOG.
 *
 * This function gets the  mode clock source of the WDOG.
 *
 * @param[in] base WDOG base pointer.
 * @return mode clock source of the WDOG.
 */
static inline wdog_clk_source_t WDOG_GetClockSource(const WDOG_Type * base)
{
    uint32_t cs = base->CS;
    wdog_clk_source_t getclocksource;

    switch((cs & WDOG_CS_CLK_MASK) >> WDOG_CS_CLK_SHIFT)
    {
    case 0U:
        getclocksource = WDOG_BUS_CLOCK;
        break;
    case 1U:
        getclocksource = WDOG_LPO_CLOCK;
        break;
    case 2U:
        getclocksource = WDOG_SOSC_CLOCK;
        break;
    case 3U:
        getclocksource = WDOG_SIRC_CLOCK;
        break;
    default:
        getclocksource = WDOG_BUS_CLOCK;
        break;
    }

    return getclocksource;
}

/*!
 * @brief Verifies if the WDOG updates are allowed.
 *
 * This function verifies if software is allowed to reconfigure the WDOG without
 * a reset.
 *
 * @param[in] base WDOG base pointer.
 * @return the state of the WDOG updates:
 *         - false: updates are not allowed
 *         - true: updates are allowed
 */
static inline bool WDOG_IsUpdateEnabled(const WDOG_Type * base)
{
    return ((base->CS & WDOG_CS_UPDATE_MASK) >> WDOG_CS_UPDATE_SHIFT) != 0U;
}

/*!
 * @brief Enables/Disables WDOG in debug mode.
 *
 * This function enables/disables the WDOG in debug mode.
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable WDOG in debug mode
 */
static inline void WDOG_SetDebug(WDOG_Type * base,
                                 bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_DBG_MASK) | WDOG_CS_DBG(enable ? 1UL : 0UL);
}

/*!
 * @brief Enables/Disables WDOG in wait mode.
 *
 * This function enables/disables the WDOG in wait mode.
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable WDOG in wait mode.
 */
static inline void WDOG_SetWait(WDOG_Type * base,
                                bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_WAIT_MASK) | WDOG_CS_WAIT(enable ? 1UL : 0UL);
}

/*!
 * @brief Enables/Disables WDOG in stop mode.
 *
 * This function enables/disables the WDOG in stop mode.
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable WDOG in stop mode.
 */
static inline void WDOG_SetStop(WDOG_Type * base,
                                bool enable)
{
    WDOG_UNLOCK(base);

    if (enable)
    {
        base->CS |= WDOG_CS_STOP_MASK;
    }
    else
    {
        base->CS &= ~WDOG_CS_STOP_MASK;
    }
}

/*!
 * @brief Sets the value of the WDOG timeout.
 *
 * This function sets the value of the WDOG timeout.
 *
 * @param[in] base WDOG base pointer.
 * @param[in] timeout the value of the WDOG timeout.
 */
static inline void WDOG_SetTimeout(WDOG_Type * base,
                                   uint16_t timeout)
{
    WDOG_UNLOCK(base);

    base->TOVAL = timeout;
}

/*!
 * @brief Checks if the WDOG is unlocked.
 *
 * This function checks if the WDOG is unlocked. If the module is unlocked,
 * reconfiguration of the registers is allowed.
 *
 * @param[in] base WDOG base pointer.
 * @return true if the module is unlocked, false if the module is locked.
 */
static inline bool WDOG_IsUnlocked(const WDOG_Type * base)
{
    return ((base->CS & WDOG_CS_ULK_MASK) >> WDOG_CS_ULK_SHIFT) != 0U;
}

/*!
 * @brief Checks if the last configuration of the WDOG was successful.
 *
 * This function checks if the last configuration of the WDOG was successful.
 *
 * @param[in] base WDOG base pointer.
 * @return true if the configuration was successful, false if not.
 */
static inline bool WDOG_IsReconfigurationComplete(const WDOG_Type * base)
{
    return ((base->CS & WDOG_CS_RCS_MASK) >> WDOG_CS_RCS_SHIFT) != 0U;
}

/*!
 * @brief Enables/Disables support for 32-bit refresh/unlock command write words.
 *
 * This function enables/disables support for 32-bit refresh/unlock command write words.
 *
 * @param[in] base WDOG base pointer.
 * @param[in] enable enable/disable 32-bit refresh/unlock command write words.
 */
static inline void WDOG_SetCmd32(WDOG_Type * base,
                                 bool enable)
{
    WDOG_UNLOCK(base);

    base->CS = (base->CS & ~WDOG_CS_CMD32EN_MASK) | WDOG_CS_CMD32EN(enable ? 1UL : 0UL);

    WDOG_UNLOCK(base);
}

/*!
 * @brief Gets test mode of the WDOG.
 *
 * This function gets test mode of the WDOG.
 *
 * @param[in] base WDOG base pointer.
 * @return test mode of the WDOG.
 */
static inline uint32_t WDOG_GetTestMode(const WDOG_Type * base)
{
    return ((base->CS & WDOG_CS_TST_MASK) >> WDOG_CS_TST_SHIFT);
}

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* WDOG_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
