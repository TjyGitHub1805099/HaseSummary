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
 * @file wdog_hw_access.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 */

#include "wdog_hw_access.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : WDOG_Config
 * Description   : Configures all WDOG registers.
 *
 *END**************************************************************************/
status_t WDOG_Config(WDOG_Type * const base,
                     const wdog_user_config_t * wdogUserConfig)
{
    DEV_ASSERT(wdogUserConfig != NULL);
    status_t status = STATUS_SUCCESS;
    uint32_t cs = base->CS;
    bool tmp1 = WDOG_IsEnabled(base);
    bool tmp2 = WDOG_IsUpdateEnabled(base);

    INT_SYS_DisableIRQGlobal();

    if ((tmp1 == false) && (tmp2 == true) && (wdogUserConfig->timeoutValue > FEATURE_WDOG_MINIMUM_TIMEOUT_VALUE))
    {
        /* Clear the bits used for configuration */
        cs &= ~(WDOG_CS_WIN_MASK | WDOG_CS_PRES_MASK | WDOG_CS_CLK_MASK | WDOG_CS_INT_MASK |
                WDOG_CS_UPDATE_MASK | WDOG_CS_DBG_MASK | WDOG_CS_WAIT_MASK | WDOG_CS_STOP_MASK);
        /* Construct CS register new value */
        cs |= WDOG_CS_WIN(wdogUserConfig->winEnable ? 1UL : 0UL);
        cs |= WDOG_CS_PRES(wdogUserConfig->prescalerEnable ? 1UL : 0UL);
        cs |= WDOG_CS_CLK(wdogUserConfig->clkSource);
        cs |= WDOG_CS_INT(wdogUserConfig->intEnable ? 1UL : 0UL);
        cs |= WDOG_CS_UPDATE(wdogUserConfig->updateEnable ? 1UL : 0UL);
        if (wdogUserConfig->opMode.debug)
        {
            cs |= WDOG_CS_DBG_MASK;
        }
        if (wdogUserConfig->opMode.wait)
        {
            cs |= WDOG_CS_WAIT_MASK;
        }
        if (wdogUserConfig->opMode.stop)
        {
            cs |= WDOG_CS_STOP_MASK;
        }
        /* Reset interrupt flags */
        cs |= WDOG_CS_FLG_MASK;
        /* Enable WDOG in 32-bit mode */
        cs |= WDOG_CS_EN_MASK | WDOG_CS_CMD32EN_MASK;

        WDOG_UNLOCK(base);

        while (!WDOG_IsUnlocked(base))
        {
          /* Wait until registers are unlocked */
        }

        base->CS = cs;
        base->TOVAL = wdogUserConfig->timeoutValue;
        if (wdogUserConfig->winEnable)
        {
            base->WIN = wdogUserConfig->windowValue;
        }

        while (WDOG_IsUnlocked(base))
        {
          /* Wait until the unlock window closes */
        }
    }
    else
    {
        status = STATUS_ERROR;
    }

    INT_SYS_EnableIRQGlobal();

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WDOG_GetConfig
 * Description   : Gets the current WDOG configuration.
 *
 *END**************************************************************************/
wdog_user_config_t WDOG_GetConfig(const WDOG_Type *base)
{
    wdog_user_config_t config;
    uint32_t cs = base->CS;

    /* Construct CS register new value */
    config.winEnable       = ((cs & WDOG_CS_WIN_MASK) != 0U);
    config.prescalerEnable = ((cs & WDOG_CS_PRES_MASK) != 0U);
    config.intEnable       = ((cs & WDOG_CS_INT_MASK) != 0U);
    config.updateEnable    = ((cs & WDOG_CS_UPDATE_MASK) != 0U);
    config.opMode.debug    = ((cs & WDOG_CS_DBG_MASK) != 0U);
    config.opMode.wait     = ((cs & WDOG_CS_WAIT_MASK) != 0U);
    config.opMode.stop     = ((cs & WDOG_CS_STOP_MASK) != 0U);
    config.timeoutValue    = (uint16_t)base->TOVAL;
    config.windowValue     = (uint16_t)base->WIN;

    switch((cs & WDOG_CS_CLK_MASK) >> WDOG_CS_CLK_SHIFT)
    {
    case 0U:
        config.clkSource = WDOG_BUS_CLOCK;
        break;
    case 1U:
        config.clkSource = WDOG_LPO_CLOCK;
        break;
    case 2U:
        config.clkSource = WDOG_SOSC_CLOCK;
        break;
    case 3U:
        config.clkSource = WDOG_SIRC_CLOCK;
        break;
    default:
        config.clkSource = WDOG_BUS_CLOCK;
        break;
    }

    return config;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

