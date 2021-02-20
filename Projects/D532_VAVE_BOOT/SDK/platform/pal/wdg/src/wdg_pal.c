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
 * @file wdg_pal.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3,  Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a different essential type.
 * The cast is required expression assigned to a narrower or different essential type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast;
 * cannot cast from 'essentially enum<i>' to 'essentially enum<i>'.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, The cast performed between a pointer to
 * object type and a pointer to a different object type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, conversion from pointer to void to pointer to other type
 * This is required for working with the common initialize function in initialize function.
 *
 */

#include "wdg_pal.h"
#include "stddef.h"

/* Include PD files */
#if defined(WDG_OVER_WDOG)
    #include "wdog_driver.h"
#endif

#if defined(WDG_OVER_EWM)
    #include "ewm_driver.h"
#endif

#if defined(WDG_OVER_SWT)
    #include "swt_driver.h"
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @name Watchdog Peripheral Abstraction Layer
 * @{
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_Init
 * Description   : Initialize the WDG PAL
 *
 * Implements    : WDG_Init_Activity
 *END**************************************************************************/
status_t WDG_Init(wdg_instance_t instance,
                   const wdg_config_t * configPtr)
{
    DEV_ASSERT(configPtr != NULL);

    uint32_t windowValue = 0U;
    status_t status = STATUS_ERROR;
    uint32_t valInstance = (uint32_t)instance;

    /* Calculate window value */
    if ((configPtr->winEnable) && (configPtr->percentWindow <= 100U))
    {
        windowValue = (configPtr->percentWindow * configPtr->timeoutValue) / 100U;
    }

    /* Define WDG PAL over WDOG */
#if defined (WDG_OVER_WDOG)
    if (valInstance <= WDG_OVER_WDOG_HIGH_INDEX)
    {
        /* Define structures for WDG PAL over WDOG */
        wdog_user_config_t wdogConfig =
        {
            .clkSource        = (wdog_clk_source_t)configPtr->clkSource,
            .opMode           =
            {
                .wait         = configPtr->opMode.wait,
                .stop         = configPtr->opMode.stop,
                .debug        = configPtr->opMode.debug
            },
            .updateEnable     = true,
            .intEnable        = configPtr->intEnable,
            .winEnable        = configPtr->winEnable,
            .windowValue      = (uint16_t)((configPtr->timeoutValue - windowValue) & 0xFFFFU),
            .timeoutValue     = (uint16_t)(configPtr->timeoutValue & 0xFFFFU),
            .prescalerEnable  = configPtr->prescalerEnable
        };

        /* Initialize WDOG instance */
        status = WDOG_DRV_Init(valInstance, &wdogConfig);
    }
#endif

    /* Define WDG PAL over EWM */
#if defined (WDG_OVER_EWM)
    if ((valInstance >= WDG_OVER_EWM_LOW_INDEX) && (valInstance <= WDG_OVER_EWM_HIGH_INDEX))
    {
        /* Define structures for WDG PAL over EWM */
        ewm_init_config_t ewmConfig;

        /* Set value for EWM structure */
        ewmConfig.interruptEnable = configPtr->intEnable;

        /* Set value for input pin asserts and clock prescaler */
        if (NULL == configPtr->extension)
        {
            ewmConfig.assertLogic = EWM_IN_ASSERT_ON_LOGIC_ZERO;
            ewmConfig.prescaler   = 0xFFU;
        }
        else
        {
            ewmConfig.assertLogic = (ewm_in_assert_logic_t)((extension_ewm_for_wdg_t*)(configPtr->extension))->assertLogic;
            ewmConfig.prescaler   = ((extension_ewm_for_wdg_t*)(configPtr->extension))->prescalerValue;
        }

        /* Set value for compare low register */
        if (true == configPtr->winEnable)
        {
            ewmConfig.compareLow  = (uint8_t)((configPtr->timeoutValue - windowValue) & 0xFFU);
        }
        else
        {
            ewmConfig.compareLow  = 0U;
        }

        /* Set value for compare high register */
        if (configPtr->timeoutValue > 0xFEU)
        {
            ewmConfig.compareHigh  = 0xFEU;
        }
        else
        {
            ewmConfig.compareHigh = (uint8_t)(configPtr->timeoutValue & 0xFFU);
        }

        /* Allocate instance for EWM */
        valInstance -= WDG_OVER_EWM_LOW_INDEX;

        /* Initialize EWM instance */
        status = EWM_DRV_Init(valInstance, &ewmConfig);
    }
#endif

    /* Define WDG PAL over SWT */
#if defined (WDG_OVER_SWT)
    if (valInstance <= WDG_OVER_SWT_HIGH_INDEX)
    {
        /* Define structures for WDG PAL over SWT */
        swt_user_config_t swtConfig =
        {
        .mapConfig    = 0xFFU,
        .invalidReset = false,
        .winEnable    = configPtr->winEnable,
        .intEnable    = configPtr->intEnable,
        .stop         = configPtr->opMode.stop,
        .debug        = configPtr->opMode.debug,
        .serviceMode  = SWT_FS_SEQ_MODE,
        .lockConfig   = SWT_UNLOCK,
        .timeoutValue = configPtr->timeoutValue,
        .windowValue  = windowValue,
        .initKey      = 0U
        };

        /* Initialize SWT instance */
        status = SWT_DRV_Init(valInstance, &swtConfig);
    }

#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_Deinit
 * Description   : De-initializes the WDG PAL
 *
 * Implements    : WDG_Deinit_Activity
 *END**************************************************************************/
status_t WDG_Deinit(wdg_instance_t instance)
{
    uint32_t valInstance = (uint32_t)instance;
    status_t status = STATUS_UNSUPPORTED;

    /* Define WDG PAL over WDOG */
#if defined(WDG_OVER_WDOG)
    if (valInstance <= WDG_OVER_WDOG_HIGH_INDEX)
    {
        WDOG_DRV_Deinit(valInstance);
        status = STATUS_SUCCESS;
    }
#endif

    /* Define WDG PAL over SWT */
#if defined(WDG_OVER_SWT)
    if (valInstance <= WDG_OVER_SWT_HIGH_INDEX)
    {
        status = SWT_DRV_Deinit(valInstance);
    }
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_GetDefaultConfig
 * Description   : Gets default configuration of the WDG PAL
 *
 * Implements    : WDG_GetDefaultConfig_Activity
 *END**************************************************************************/
void WDG_GetDefaultConfig(wdg_config_t * const config)
{
    DEV_ASSERT(config != NULL);

    config->opMode.debug    = false;
    config->opMode.wait     = false;
    config->opMode.stop     = false;
    config->percentWindow   = 0U;
    config->intEnable       = false;
    config->winEnable       = false;
    config->prescalerEnable = false;
    config->extension       = NULL;

    /* Define WDG PAL over WDOG and EWM */
#if (defined(WDG_OVER_WDOG) || defined(WDG_OVER_EWM))
    config->clkSource       = WDG_PAL_LPO_CLOCK;
    config->timeoutValue    = 0x4FEU;
#endif

    /* Define WDG PAL over SWT */
#if defined(WDG_OVER_SWT)
    config->clkSource       = WDG_PAL_SIRC_CLOCK;
    config->timeoutValue    = FEATURE_SWT_TO_RESET_VALUE;
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_Refresh
 * Description   : Refreshes the WDG PAL counter.
 *
 * Implements    : WDG_Refresh_Activity
 *END**************************************************************************/
void WDG_Refresh(wdg_instance_t instance)
{
    uint32_t valInstance = (uint32_t)instance;

    /* Define WDG PAL over WDOG */
#if defined(WDG_OVER_WDOG)
    if (valInstance <= WDG_OVER_WDOG_HIGH_INDEX)
    {
        WDOG_DRV_Trigger(valInstance);
    }
#endif

    /* Define WDG PAL over EWM */
#if defined(WDG_OVER_EWM)
    if ((valInstance >= WDG_OVER_EWM_LOW_INDEX) && (valInstance <= WDG_OVER_EWM_HIGH_INDEX))
    {
        /* Allocate instance for EWM */
        valInstance -= WDG_OVER_EWM_LOW_INDEX;
        /* Initialize EWM instance */
        EWM_DRV_Refresh(valInstance);
    }
#endif

    /* Define WDG PAL over SWT */
#if defined(WDG_OVER_SWT)
    if (valInstance <= WDG_OVER_SWT_HIGH_INDEX)
    {
        SWT_DRV_Service(valInstance);
    }
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_SetInt
 * Description   : Set interrupt for WDG PAL module
 *
 * Implements    : WDG_SetInt_Activity
 *END**************************************************************************/
status_t WDG_SetInt(wdg_instance_t instance,
                     bool enable)
{
    uint32_t valInstance = (uint32_t)instance;
    status_t status = STATUS_UNSUPPORTED;

    /* Define WDG PAL over WDOG */
#if defined(WDG_OVER_WDOG)
    if (valInstance <= WDG_OVER_WDOG_HIGH_INDEX)
    {
        status = WDOG_DRV_SetInt(valInstance, enable);
    }
#endif

    /* Define  PAL PAL over SWT */
#if defined(WDG_OVER_SWT)
    if (valInstance <= WDG_OVER_SWT_HIGH_INDEX)
    {
        status = SWT_DRV_SetIntConfig(valInstance, enable);
    }
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_SetTimeout
 * Description   : Sets the value of the WDG PAL timeout.
 *
 * Implements    : WDG_SetTimeout_Activity
 *END**************************************************************************/
status_t WDG_SetTimeout(wdg_instance_t instance,
                         uint32_t value)
{
    uint32_t valInstance = (uint32_t)instance;
    status_t status = STATUS_UNSUPPORTED;

    /* Define WDG PAL over WDOG */
#if defined(WDG_OVER_WDOG)
    if (valInstance <= WDG_OVER_WDOG_HIGH_INDEX)
    {
        WDOG_DRV_SetTimeout(valInstance, (uint16_t) value);
        status = STATUS_SUCCESS;
    }
#endif

    /* Define WDG PAL over SWT */
#if defined(WDG_OVER_SWT)
    if (valInstance <= WDG_OVER_SWT_HIGH_INDEX)
    {
        status = SWT_DRV_SetTimeoutValue(valInstance, value);
    }
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_SetWindow
 * Description   : Set window mode and window value of the WDG PAL.
 *
 * Implements    : WDG_SetWindow_Activity
 *END**************************************************************************/
status_t WDG_SetWindow(wdg_instance_t instance,
                        bool enable,
                        uint32_t value)
{
    uint32_t valInstance = (uint32_t)instance;
    status_t status = STATUS_UNSUPPORTED;

    /* Define WDG PAL over WDOG */
#if defined(WDG_OVER_WDOG)
    if (valInstance <= WDG_OVER_WDOG_HIGH_INDEX)
    {
        WDOG_DRV_SetWindow(valInstance, enable, (uint16_t)value);
        status = STATUS_SUCCESS;
    }
#endif

    /* Define WDG PAL over SWT */
#if defined(WDG_OVER_SWT)
    if (valInstance <= WDG_OVER_SWT_HIGH_INDEX)
    {
        status = SWT_DRV_SetWindowConfig(valInstance, enable, value);
    }
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_GetCounter
 * Description   : Gets the value of the WDG PAL counter.
 *
 * Implements    : WDG_GetCounter_Activity
 *END**************************************************************************/
status_t WDG_GetCounter(wdg_instance_t instance,
                         uint32_t * value)
{
    uint32_t valInstance = (uint32_t)instance;
    status_t status = STATUS_UNSUPPORTED;

    /* Define WDG PAL over WDOG */
#if defined(WDG_OVER_WDOG)
    if (valInstance <= WDG_OVER_WDOG_HIGH_INDEX)
    {
        *value = WDOG_DRV_GetCounter(valInstance);
        status = STATUS_SUCCESS;
    }
#endif

    /* Define WDG PAL over SWT */
#if defined(WDG_OVER_SWT)
    if (valInstance <= WDG_OVER_SWT_HIGH_INDEX)
    {
        status = SWT_DRV_GetCounterValue(valInstance, value);
    }
#endif

    return status;
}

#if defined(WDG_OVER_SWT)
/*FUNCTION**********************************************************************
 *
 * Function Name : WDG_ClearIntFlag
 * Description   : Clears the Timeout Interrupt Flag.
 *
 * Implements    : WDG_ClearIntFlag_Activity
 *END**************************************************************************/
void WDG_ClearIntFlag(wdg_instance_t instance)
{
    uint32_t valInstance = (uint32_t)instance;
    DEV_ASSERT(valInstance <= WDG_OVER_SWT_HIGH_INDEX);

    SWT_DRV_ClearIntFlag(valInstance);
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
