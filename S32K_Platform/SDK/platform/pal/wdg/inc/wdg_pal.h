/*
 * Copyright 2017 NXP
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
 * @file wdg_pal.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * Type used only in some modules of the SDK.
 *
 */

#ifndef WDG_PAL_H
#define WDG_PAL_H

#include "status.h"
#include "wdg_pal_mapping.h"
#include "wdg_pal_cfg.h"

/*!
 * @defgroup wdg_pal_code WDG PAL
 * @ingroup wdg_pal
 * @brief Watchdog Peripheral Abstraction Layer.
 * @{
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Clock sources for the WDG PAL.
 * Implements : wdg_clock_source_t_Class
 */
typedef enum
{
    WDG_PAL_BUS_CLOCK     = 0x00U, /*!< Bus clock */
    WDG_PAL_LPO_CLOCK     = 0x01U, /*!< LPO clock */
    WDG_PAL_SOSC_CLOCK    = 0x02U, /*!< SOSC clock */
    WDG_PAL_SIRC_CLOCK    = 0x03U  /*!< SIRC clock */
} wdg_clock_source_t;

/*!
 * @brief WDG PAL option mode configuration structure
 * Implements : wdg_option_mode_t_Class
 */
typedef struct
{
    bool    wait;  /*!< Wait mode */
    bool    stop;  /*!< Stop mode */
    bool    debug; /*!< Debug mode */
} wdg_option_mode_t;

/*!
 * @brief WDG PAL input pin configuration
 * Configures if the input pin is enabled and when is asserted
 * Implements : wdg_in_assert_logic_t_Class
 */
typedef enum
{
    WDG_IN_ASSERT_DISABLED      = 0x00U, /*!< Input pin disabled                    */
    WDG_IN_ASSERT_ON_LOGIC_ZERO = 0x01U, /*!< Input pin asserts EWM when on logic 0 */
    WDG_IN_ASSERT_ON_LOGIC_ONE  = 0x02U  /*!< Input pin asserts EWM when on logic 1 */
} wdg_in_assert_logic_t;

/*!
 * @brief The extension structure for the EWM over WDOG peripheral
 * Implements : extension_ewm_for_wdg_t_Class
 */
typedef struct
{
    wdg_in_assert_logic_t    assertLogic;    /*!< Assert logic for EWM input pin */
    uint8_t                  prescalerValue; /*!< EWM clock prescaler */
} extension_ewm_for_wdg_t;

/*!
 * @brief WDG PAL configuration structure
 * Implements : wdg_config_t_Class
 */
typedef struct
{
    wdg_clock_source_t    clkSource;        /*!< The clock source of the WDOG */
    wdg_option_mode_t     opMode;           /*!< The modes in which the WDOG is functional */
    uint32_t              timeoutValue;     /*!< The timeout value */
    uint8_t               percentWindow;    /*!< The window value compares to timeout value */
    bool                  intEnable;        /*!< If true, an interrupt request is generated before reset */
    bool                  winEnable;        /*!< If true, window mode is enabled */
    bool                  prescalerEnable;  /*!< If true, prescaler is enabled( default prescaler = 256) */
    void                  *extension;       /*!< This field will be add extra settings to EWM's configuration */
} wdg_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name WDG PAL API
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the WDG PAL.
 *
 * This function initializes the WDG instance by user configuration
 *
 * @param[in] instance The name of the instance.
 * @param[in] configPtr Pointer to the WDG PAL user configuration structure
 * @return operation status
 *        - STATUS_SUCCESS : Operation was successful.
 *        - STATUS_ERROR   : Operation failed. Possible causes: previous
 *        clock source or the one specified in the configuration structure is
 *        disabled; WDG PAL configuration updates are not allowed.
 */
status_t WDG_Init(wdg_instance_t instance,
                  const wdg_config_t * configPtr);

/*!
 * @brief De-initializes the WDG PAL
 *
 * This function resets all configuration to default and disable the WDG PAL instance.
 *
 * @param[in] instance  The name of the instance.
 * @return operation status
 *         - STATUS_SUCCESS     : Operation was successful.
 *         - STATUS_ERROR       : Operation failed due to WDG PAL was locked.
 *         - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t WDG_Deinit(wdg_instance_t instance);

/*!
 * @brief Gets default configuration of the WDG PAL.
 *
 * This function gets the default configuration of the WDG PAL.
 *
 * @param[out] configures the default configuration
 */
void WDG_GetDefaultConfig(wdg_config_t * const config);

/*!
 * @brief Refreshes the WDG PAL counter.
 *
 * This function resets the WDG PAL counter
 *
 * @param[in] instance The name of the instance.
 */
void WDG_Refresh(wdg_instance_t instance);

/*!
 * @brief Set interrupt for WDG PAL.
 *
 * This function enables/disables the WDG PAL timeout interrupt and sets a function to be
 * called when a timeout interrupt is received, before reset.
 *
 * @param[in] instance The name of the instance.
 * @param[in] enable
 *            - true  : Enable interrupt
 *            - false : Disable interrupt
 * @return operation status
 *        - STATUS_SUCCESS    : Operation was successful.
 *        - STATUS_ERROR      : Operation failed. Possible causes: failed to
 *          WDG PAL configuration updates not allowed.
 *        - STATUS_UNSUPPORTED: Operation was unsupported.
 */
status_t WDG_SetInt(wdg_instance_t instance,
                    bool enable);

/*!
 * @brief Sets the value of the WDG PAL timeout.
 *
 * This function sets the value of the WDG PAL timeout.
 *
 * @param[in] instance The name of the instance.
 * @param[in] value The value of the WDG PAL timeout.
 * @return operation status
 *         - STATUS_SUCCESS     : Operation was successful.
 *         - STATUS_ERROR       : Operation failed due to WDG PAL was locked.
 *         - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t WDG_SetTimeout(wdg_instance_t instance,
                         uint32_t value);

/*!
 * @brief Set window mode and window value of the WDG PAL.
 *
 * This function set window mode, window value is set when window mode enabled.
 *
 * @param[in] instance The name of the instance.
 * @param[in] enable
 *            - true  : Enable interrupt
 *            - false : Disable interrupt
 * @param[in] value The value of the WDG PAL window.
 * @return operation status
 *         - STATUS_SUCCESS     : Operation was successful.
 *         - STATUS_ERROR       : Operation failed due to WDG PAL was locked.
 *         - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t WDG_SetWindow(wdg_instance_t instance,
                        bool enable,
                        uint32_t value);

/*!
 * @brief Gets the value of the WDG PAL counter.
 *
 * This function gets counter of WDG PAL module.
 *
 * @param[in] instance The name of the instance.
 * @param[out] value Pointer to the counter value
 * @return operation status
 *         - STATUS_SUCCESS     : The value of the WDG PAL counter.
 *         - STATUS_ERROR       : Operation failed due to WDG PAL was enabled.
 *         - STATUS_UNSUPPORTED : Operation was unsupported.
 */
status_t WDG_GetCounter(wdg_instance_t instance,
                        uint32_t * value);

/*!
 * @brief Clears the Timeout Interrupt Flag.
 *
 * This function clears the Timeout Interrupt Flag.
 *
 * @param[in] instance The name of the instance.
 */
void WDG_ClearIntFlag(wdg_instance_t instance);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* WDG_PAL_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
