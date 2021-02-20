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
/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 */

#include <stddef.h>
#include "erm_hw_access.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Table of base addresses for ERM instances */
static ERM_Type * const s_ermBase[] = ERM_BASE_PTRS;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : ERM_DRV_Init
 * Description   : This function initializes ERM driver based on user configuration input,
 * channelCnt takes values between 1 and the maximum channel count supported by the hardware.
 *
 * Implements    : ERM_DRV_Init_Activity
 *END**************************************************************************/
void ERM_DRV_Init(uint32_t instance,
                  uint8_t channelCnt,
                  const erm_user_config_t * userConfigArr)
{
    DEV_ASSERT(instance < ERM_INSTANCE_COUNT);
    DEV_ASSERT(userConfigArr != NULL);
    DEV_ASSERT(channelCnt > 0U);
    DEV_ASSERT(channelCnt <= ERM_EARn_COUNT);
    ERM_Type * base = s_ermBase[instance];
    uint8_t i;

    /* Initializes the module */
    ERM_Init(base);

    /* Sets interrupt notification from user configuration input */
    for (i = 0U; i < channelCnt; i++)
    {
        ERM_DRV_SetInterruptConfig(instance, userConfigArr[i].channel, *userConfigArr[i].interruptCfg);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ERM_DRV_Deinit
 * Description   : This function sets the default configuration.
 *
 * Implements    : ERM_DRV_Deinit_Activity
 *END**************************************************************************/
void ERM_DRV_Deinit(uint32_t instance)
{
    DEV_ASSERT(instance < ERM_INSTANCE_COUNT);
    ERM_Type * base = s_ermBase[instance];

    /* Set the default configuration */
    ERM_Init(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ERM_DRV_SetInterruptConfig
 * Description   : This function sets interrupt notification based on interrupt
 * notification configuration input.
 *
 * Implements    : ERM_DRV_SetInterruptConfig_Activity
 *END**************************************************************************/
void ERM_DRV_SetInterruptConfig(uint32_t instance,
                                uint8_t channel,
                                erm_interrupt_config_t interruptCfg)
{
    DEV_ASSERT(instance < ERM_INSTANCE_COUNT);
    DEV_ASSERT(channel < ERM_EARn_COUNT);
    ERM_Type * base = s_ermBase[instance];

    /* Set interrupt notification base on interrupt notification configuration input */
    ERM_EnableEventInterrupt(base, channel, ERM_EVENT_SINGLE_BIT, interruptCfg.enableSingleCorrection);
    ERM_EnableEventInterrupt(base, channel, ERM_EVENT_NON_CORRECTABLE, interruptCfg.enableNonCorrectable);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ERM_DRV_GetInterruptConfig
 * Description   : This function gets the current interrupt configuration of the available events
 * (which interrupts are enabled/disabled).
 *
 * Implements    : ERM_DRV_GetInterruptConfig_Activity
 *END**************************************************************************/
void ERM_DRV_GetInterruptConfig(uint32_t instance,
                                uint8_t channel,
                                erm_interrupt_config_t * const interruptPtr)
{
    DEV_ASSERT(instance < ERM_INSTANCE_COUNT);
    DEV_ASSERT(channel < ERM_EARn_COUNT);
    DEV_ASSERT(interruptPtr != NULL);
    const ERM_Type * base = s_ermBase[instance];

    /* Get interrupt notification into interrupt notification configuration input */
    interruptPtr->enableSingleCorrection = ERM_IsEventInterruptEnabled(base, channel, ERM_EVENT_SINGLE_BIT);
    interruptPtr->enableNonCorrectable = ERM_IsEventInterruptEnabled(base, channel, ERM_EVENT_NON_CORRECTABLE);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ERM_DRV_ClearEvent
 * Description   : This function clears the record of an event. If the corresponding interrupt is enabled,
 * the interrupt notification will be cleared.
 *
 * Implements    : ERM_DRV_ClearEvent_Activity
 *END**************************************************************************/
void ERM_DRV_ClearEvent(uint32_t instance,
                        uint8_t channel,
                        erm_ecc_event_t eccEvent)
{
    DEV_ASSERT(instance < ERM_INSTANCE_COUNT);
    DEV_ASSERT(channel < ERM_EARn_COUNT);
    ERM_Type * base = s_ermBase[instance];

    /* Single-bit correction */
    if (eccEvent == ERM_EVENT_SINGLE_BIT)
    {
        ERM_ClearEventSingle(base, channel);
    }
    /* Non-correctable */
    else if (eccEvent == ERM_EVENT_NON_CORRECTABLE)
    {
        ERM_ClearEventDouble(base, channel);
    }
    else
    {
        /* Do nothing */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ERM_DRV_GetErrorDetail
 * Description   : This function gets the address of the last ECC event in Memory n
 * and the types of the event.
 *
 * Implements    : ERM_DRV_GetErrorDetail_Activity
 *END**************************************************************************/
erm_ecc_event_t ERM_DRV_GetErrorDetail(uint32_t instance,
                                       uint8_t channel,
                                       uint32_t * addressPtr)
{
    DEV_ASSERT(instance < ERM_INSTANCE_COUNT);
    DEV_ASSERT(channel < ERM_EARn_COUNT);
    DEV_ASSERT(addressPtr != NULL);
    const ERM_Type * base = s_ermBase[instance];
    erm_ecc_event_t eccEvent;

    /* Single-bit correction ECC events */
    if (ERM_IsEventDetected(base, channel, ERM_EVENT_SINGLE_BIT) != false)
    {
        eccEvent = ERM_EVENT_SINGLE_BIT;
    }
    /* Non-correctable ECC events */
    else if (ERM_IsEventDetected(base, channel, ERM_EVENT_NON_CORRECTABLE) != false)
    {
        eccEvent = ERM_EVENT_NON_CORRECTABLE;
    }
    /* None ECC events */
    else
    {
        eccEvent = ERM_EVENT_NONE;
    }

    /* Gets address */
    *addressPtr = ERM_GetLastErrorAddress(base, channel);

    return eccEvent;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
