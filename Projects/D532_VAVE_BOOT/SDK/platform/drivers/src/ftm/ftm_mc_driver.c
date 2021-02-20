/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED  WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*!
 * @file ftm_mc_driver.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * The symbols are declared in the driver common file as external; they are needed
 * at driver initialization to install the correct interrupt handler, but are not
 * a part of the public API.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 */

#include "ftm_mc_driver.h"
#include "ftm_hw_access.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_InitCounter
 * Description   : Initializes the FTM counter. This function provides access to the
 * FTM counter settings. The counter can be run in Up counting or Up-down counting modes.
 * To run the counter in Free running mode, choose Up counting option and provide
 * 0x0 for the countStartVal and 0xFFFF for countFinalVal. Please call this
 * function only when FTM is used as timer/counter.
 *
 * Implements    : FTM_DRV_InitCounter_Activity
 *END**************************************************************************/
status_t FTM_DRV_InitCounter(uint32_t instance,
                             const ftm_timer_param_t * timer)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    DEV_ASSERT(timer != NULL);
    DEV_ASSERT((FTM_MODE_UP_TIMER == timer->mode) || (FTM_MODE_UP_DOWN_TIMER == timer->mode));
    FTM_Type * ftmBase = g_ftmBase[instance];
    ftm_state_t * state = ftmStatePtr[instance];
    status_t retStatus = STATUS_SUCCESS;
    uint8_t channel = 0U;

    if ((NULL != state) && (FTM_MODE_NOT_INITIALIZED == state->ftmMode))
    {
        /* Disable counter clock */
        FTM_DRV_SetClockSource(ftmBase, FTM_CLOCK_SOURCE_NONE);
        /* Clear the overflow flag */
        FTM_DRV_ClearTimerOverflow(ftmBase);
        /* Set counter initial and maximum values */
        FTM_DRV_SetCounterInitVal(ftmBase, timer->initialValue);
        FTM_DRV_SetMod(ftmBase, timer->finalValue);
        /* Disable the quadrature decoder mode */
        FTM_DRV_SetQuadDecoderCmd(ftmBase, false);
        /* Use FTM as counter, disable all the channels */
        for (channel = 0U; channel < FEATURE_FTM_CHANNEL_COUNT; channel++)
        {
            FTM_DRV_SetChnEdgeLevel(ftmBase, channel, 0U);
        }

        /* Check the FTM counter modes */
        if (FTM_MODE_UP_TIMER == timer->mode)
        {
            FTM_DRV_SetCpwms(ftmBase, false);
        }
        else if (FTM_MODE_UP_DOWN_TIMER == timer->mode)
        {
            FTM_DRV_SetCpwms(ftmBase, true);
        }
        else
        {
            /* Do nothing */
        }

        state->ftmMode = timer->mode;
    }
    else
    {
        retStatus = STATUS_ERROR;
    }

    return retStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_CounterStart
 * Description   : Starts the FTM counter.
 *
 * Implements    : FTM_DRV_CounterStart_Activity
 *END**************************************************************************/
status_t FTM_DRV_CounterStart(uint32_t instance)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    FTM_Type * ftmBase = g_ftmBase[instance];

    const ftm_state_t * state = ftmStatePtr[instance];
    /* Check the clock source is available for FTM counter */
    DEV_ASSERT(state->ftmSourceClockFrequency > 0U);
    /* Enable counter clock */
    FTM_DRV_SetClockSource(ftmBase, state->ftmClockSource);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_CounterStop
 * Description   : Stops the FTM counter.
 *
 * Implements    : FTM_DRV_CounterStop_Activity
 *END**************************************************************************/
status_t FTM_DRV_CounterStop(uint32_t instance)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    FTM_Type * ftmBase = g_ftmBase[instance];
    ftm_state_t * state = ftmStatePtr[instance];

    /* Stop the FTM counter */
    FTM_DRV_SetClockSource(ftmBase, FTM_CLOCK_SOURCE_NONE);
    state->ftmMode = FTM_MODE_NOT_INITIALIZED;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : FTM_DRV_CounterRead
 * Description   : Reads back the current value of the FTM counter.
 *
 * Implements    : FTM_DRV_CounterRead_Activity
 *END**************************************************************************/
uint32_t FTM_DRV_CounterRead(uint32_t instance)
{
    DEV_ASSERT(instance < FTM_INSTANCE_COUNT);
    FTM_Type const * ftmBase = g_ftmBase[instance];

    return FTM_DRV_GetCounter(ftmBase);
}

/*******************************************************************************
* EOF
******************************************************************************/
