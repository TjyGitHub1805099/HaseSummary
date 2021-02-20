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

#include <stddef.h>
#include "adc_pal.h"
#include "adc_irq.h"
#include "device_registers.h"
#include "interrupt_manager.h"
#include "osif.h" 


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(ADC_PAL_S32K1xx)

/*! @cond DRIVER_INTERNAL_USE_ONLY */
typedef struct
{
    uint16_t bufferLength;            /*!< Length of the buffer associated with the current active conversion group */
    uint16_t currentBufferOffset;     /*!< Offset (in elements) of the next position to be written in the result buffer */
    bool notificationEn;              /*!< Flag for enabling/disabling notification */
} adc_group_state_t;

/*!
 * @brief Runtime state of the ADC PAL
 *
 * This structure is used by the ADC PAL for storing internal information, needed at runtime.
 */
typedef struct
{
    const adc_group_config_t * groupArray;              /*!< Pointer to the array of group configurations */
    uint16_t numGroups;                                 /*!< Number of group configurations available in the input array */
    uint32_t activeGroupIdx;                            /*!< Index of the active group (HW trigger group enabled or executing, or SW triggered group executing) */
    adc_group_state_t activeGroupState;                 /*!< State of the active group (HW trigger group enabled or executing, or SW triggered group executing) */
    bool activeGroupFlag;                               /*!< True/False - group conversion active/not active */
} adc_pal_state_t;


#define ADC_PAL_PDB_CHAN            (0u)                /*!< PDB channel used for triggering ADC */
#define ADC_PAL_TRGMUX_IDX          (0u)                /*!< TRGMUX instance used by ADC PAL */
#define ADC_PAL_MAX_CONVS_IN_GROUP  (PDB_DLY_COUNT)     /*!< Maximum number of ADC conversions in a group of conversions. */

const trgmux_target_module_t adcPalTrgmuxTarget[NUMBER_OF_ADC_PAL_INSTANCES] = {TRGMUX_TARGET_MODULE_PDB0_TRG_IN, TRGMUX_TARGET_MODULE_PDB1_TRG_IN};


static inline void ADC_ConfigPdbAndPretriggers(const uint32_t instance, const pdb_trigger_src_t trgSrc,  const adc_group_config_t * currentGroupCfg);
static void ADC_ConfigGroup(const uint32_t instance, const uint32_t groupIdx, const bool hwTriggerFlag);
static status_t ADC_StopGroupBlocking(const uint32_t instance, const uint32_t timeout);

/*! @endcond */
#endif /* defined(ADC_PAL_MPC574x) */

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Static variable to store the PAL state information */
static adc_pal_state_t adcPalState[NUMBER_OF_ADC_PAL_INSTANCES];

/*******************************************************************************
 * Public Functions
 ******************************************************************************/


/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_Init
 * Description   : This function initializes the ADC PAL instance, including the
 * other platform specific HW units used together with ADC.
 *
 * Implements : ADC_Init_Activity
 *END**************************************************************************/
status_t ADC_Init(const adc_pal_instance_t instance, const adc_config_t * const config)
{
    DEV_ASSERT(instance < NUMBER_OF_ADC_PAL_INSTANCES);
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(config->extension != NULL);
    DEV_ASSERT(config->groupConfigArray != NULL);

    status_t status = STATUS_SUCCESS;

#if defined(ADC_PAL_S32K1xx)
    /* Initialize PAL state structure */
    adcPalState[instance].groupArray        = config->groupConfigArray;
    adcPalState[instance].activeGroupIdx    = 0u;
    adcPalState[instance].numGroups         = config->numGroups;

#if defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
    {
        /* Iterate over all conversion groups and DEV_ASSERT if number of conversions is larger than max supported */
        uint16_t idx;

        for(idx = 0u; idx < adcPalState[instance].numGroups; idx++)
        {
            DEV_ASSERT(adcPalState[instance].groupArray[idx].numChannels <= ADC_PAL_MAX_CONVS_IN_GROUP);
        }
    }
#endif /* defined (CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */

    extension_adc_s32k1xx_t * extension = (extension_adc_s32k1xx_t *)(config->extension);
    adc_converter_config_t adcCfg;

    /* ADC configuration */
    ADC_DRV_Reset(instance);

    ADC_DRV_InitConverterStruct(&adcCfg);
    adcCfg.trigger     = ADC_TRIGGER_HARDWARE; /* ADC in hw trigger mode - sw triggering shall be done via PDB */
    adcCfg.sampleTime  = config->sampleTicks;
    adcCfg.clockDivide = extension->clockDivide;
    adcCfg.resolution  = extension->resolution;
    adcCfg.inputClock  = extension->inputClock;
    adcCfg.voltageRef  = extension->voltageRef;
    adcCfg.pretriggerSel = ADC_PRETRIGGER_SEL_PDB; /* configure pretriggers 0->3 to be routed from PDB */

    ADC_DRV_ConfigConverter(instance, &adcCfg);

    ADC_DRV_AutoCalibration(instance);

    /* PDB init shall only be called from StartConversion() & EnableHardwareTrigger()
     * because PDB input source and continuous conversion enable need to be configured for each call. */

    /* Only reset the trgmux target register corresponding to PDB instance.
     * Calling TRGMUX init would reset all TRGMUX target registers - affecting other modules. */
    status = TRGMUX_DRV_SetTrigSourceForTargetModule(ADC_PAL_TRGMUX_IDX, TRGMUX_TRIG_SOURCE_DISABLED, adcPalTrgmuxTarget[instance]);
    if(status != STATUS_SUCCESS)
    {
        status = STATUS_ERROR;
    }
    DEV_ASSERT(status == STATUS_SUCCESS);

#elif defined(ADC_PAL_MPC574x)
    // on C55 all HW triggered conversions shall be configured in HW
    // check if HW triggered groups fit in BCTU LIST DRV
#endif /* defined(ADC_PAL_MPC574x) */

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_Deinit
 * Description   : This function resets the ADC PAL instance, including
 * the other platform specific HW units used together with ADC.
 *
 * Implements : ADC_Deinit_Activity
 *END**************************************************************************/
status_t ADC_Deinit(const adc_pal_instance_t instance)
{
    DEV_ASSERT(instance < NUMBER_OF_ADC_PAL_INSTANCES);

    adc_pal_state_t * palState     = &(adcPalState[instance]);
    adc_group_state_t * groupState = &(palState->activeGroupState);
    status_t status;

    if(palState->activeGroupFlag == true)
    {
        status = STATUS_BUSY;
    }
    else
    {
#if defined(ADC_PAL_S32K1xx)
        /* Reset PAL state structure */
        palState->groupArray              = NULL;
        palState->numGroups               = 0u;
        groupState->bufferLength          = 0u;
        groupState->currentBufferOffset   = 0u;
        groupState->notificationEn        = false;

        PDB_DRV_Deinit(instance);

        status = TRGMUX_DRV_SetTrigSourceForTargetModule(ADC_PAL_TRGMUX_IDX, TRGMUX_TRIG_SOURCE_DISABLED, adcPalTrgmuxTarget[instance]);
        DEV_ASSERT(status == STATUS_SUCCESS);

        ADC_DRV_Reset(instance);

        /* Disable interrupt from INT manager */
        IRQn_Type adcIrqId;
        adcIrqId = ADC_DRV_GetInterruptNumber(instance);
        INT_SYS_DisableIRQ(adcIrqId);

        status = STATUS_SUCCESS;

#elif defined(ADC_PAL_MPC574x)

        /* Deinit BCTU and ADCs */

#endif /* defined(ADC_PAL_MPC574x) */
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_EnableHardwareTrigger
 * Description   : Enables the selected HW trigger for a conversion group,
 * if the conversion group has support for HW trigger.
 *
 * Implements : ADC_EnableHardwareTrigger_Activity
 *END**************************************************************************/
status_t ADC_EnableHardwareTrigger(const adc_pal_instance_t instance, const uint32_t groupIdx)
{
    DEV_ASSERT(instance < NUMBER_OF_ADC_PAL_INSTANCES);
    DEV_ASSERT(groupIdx < adcPalState[instance].numGroups);
    DEV_ASSERT(adcPalState[instance].groupArray != NULL);

    adc_pal_state_t * palState                  = &(adcPalState[instance]);
    const adc_group_config_t * currentGroupCfg  = &(palState->groupArray[groupIdx]);
    status_t status       = STATUS_SUCCESS;
    status_t statusTrgmux = STATUS_SUCCESS;
    (void) statusTrgmux;

    DEV_ASSERT(currentGroupCfg->numChannels <= ADC_PAL_MAX_CONVS_IN_GROUP);
    DEV_ASSERT(currentGroupCfg->hwTriggerSupport == true);

    if(palState->activeGroupFlag == true)
    {
        /* A conversion group is already active */
        status = STATUS_BUSY;
    }
    else
    {
#if defined(ADC_PAL_S32K1xx)

        ADC_ConfigGroup(instance, groupIdx, true);

        /* Enable in TRGMUX the selected HW trigger source for PDB.
         * Must be called after all PDB pre-triggers have been configured, to make sure no triggers occur during configuration. */
        statusTrgmux = TRGMUX_DRV_SetTrigSourceForTargetModule(ADC_PAL_TRGMUX_IDX, currentGroupCfg->triggerSource, adcPalTrgmuxTarget[instance]);
        DEV_ASSERT(statusTrgmux == STATUS_SUCCESS);

        status = STATUS_SUCCESS;

#elif defined(ADC_PAL_MPC574x)

#endif /* defined(ADC_PAL_MPC574x) */

    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_DisableHardwareTrigger
 * Description   : Disables the selected HW trigger for a conversion group,
 * if the conversion group has support for HW trigger.
 *
 * Implements : ADC_DisableHardwareTrigger_Activity
 *END**************************************************************************/
status_t ADC_DisableHardwareTrigger(const adc_pal_instance_t instance, const uint32_t groupIdx, const uint32_t timeout)
{
    DEV_ASSERT(instance < NUMBER_OF_ADC_PAL_INSTANCES);
    DEV_ASSERT(groupIdx < adcPalState[instance].numGroups);
    DEV_ASSERT(adcPalState[instance].groupArray != NULL);
    DEV_ASSERT(adcPalState[instance].groupArray[groupIdx].hwTriggerSupport == true);

    status_t status = STATUS_SUCCESS;

    if(adcPalState[instance].activeGroupIdx == groupIdx)
    {
#if defined(ADC_PAL_S32K1xx)
        /* Disable from TRGMUX the trigger source for PDB */
        status = TRGMUX_DRV_SetTrigSourceForTargetModule(ADC_PAL_TRGMUX_IDX, TRGMUX_TRIG_SOURCE_DISABLED, adcPalTrgmuxTarget[instance]);
        DEV_ASSERT(status == STATUS_SUCCESS);

        status = ADC_StopGroupBlocking(instance, timeout);
#elif defined(ADC_PAL_MPC574x)


#endif /* defined(ADC_PAL_MPC574x) */
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_StartGroupConversion
 * Description   : Starts the execution of a selected ADC conversion group.
 *
 * Implements : ADC_StartGroupConversion_Activity
 *END**************************************************************************/
status_t ADC_StartGroupConversion(const adc_pal_instance_t instance, const uint32_t groupIdx)
{
    DEV_ASSERT(instance < NUMBER_OF_ADC_PAL_INSTANCES);

    adc_pal_state_t * palState = &(adcPalState[instance]);

    DEV_ASSERT(groupIdx < palState->numGroups);
    DEV_ASSERT(palState->groupArray != NULL);
    DEV_ASSERT(palState->groupArray[groupIdx].hwTriggerSupport == false);

    status_t status;
    bool hwTriggerEnabled = false;

    if(palState->activeGroupFlag == true)
    {
        /* A conversion group is already active */
        status = STATUS_BUSY;
    }
    else
    {
#if defined(ADC_PAL_S32K1xx)
        DEV_ASSERT(palState->groupArray[groupIdx].numChannels <= ADC_PAL_MAX_CONVS_IN_GROUP);

        ADC_ConfigGroup(instance, groupIdx, hwTriggerEnabled);

        /* Sw trigger PDB */
        PDB_DRV_SoftTriggerCmd(instance);

        status = STATUS_SUCCESS;

#elif defined(ADC_PAL_MPC574x)

#endif /* defined(ADC_PAL_MPC574x) */
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_StopGroupConversion
 * Description   : Stops the execution of the ADC group currently executing.
 *
 * Implements : ADC_StopGroupConversion_Activity
 *END**************************************************************************/
status_t ADC_StopGroupConversion(const adc_pal_instance_t instance, const uint32_t groupIdx, const uint32_t timeout)
{
    DEV_ASSERT(instance < NUMBER_OF_ADC_PAL_INSTANCES);
    DEV_ASSERT(groupIdx < adcPalState[instance].numGroups);
    DEV_ASSERT(adcPalState[instance].groupArray != NULL);
    DEV_ASSERT(adcPalState[instance].groupArray[groupIdx].hwTriggerSupport == false);

    status_t status = STATUS_SUCCESS;

    if(adcPalState[instance].activeGroupIdx == groupIdx)
    {
#if defined(ADC_PAL_S32K1xx)

        status = ADC_StopGroupBlocking(instance, timeout);

#elif defined(ADC_PAL_MPC574x)


#endif /* defined(ADC_PAL_MPC574x) */
    }

    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_EnableNotification
 * Description   : Enables the notification callback for a configured group.
 *
 * Implements : ADC_EnableNotification_Activity
 *END**************************************************************************/
status_t ADC_EnableNotification(const adc_pal_instance_t instance, const uint32_t groupIdx)
{
    DEV_ASSERT(instance < NUMBER_OF_ADC_PAL_INSTANCES);

    adc_pal_state_t * state = &(adcPalState[instance]);

    DEV_ASSERT(groupIdx < state->numGroups);

    status_t status = STATUS_SUCCESS;

    if(groupIdx != state->activeGroupIdx)
    {
        status = STATUS_ERROR;
    }
    else
    {
#if defined(ADC_PAL_S32K1xx)

        state->activeGroupState.notificationEn = true;

#elif defined(ADC_PAL_MPC574x)


#endif /* defined(ADC_PAL_MPC574x) */

    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_DisableNotification
 * Description   : Disables the notification callback for a configured group.
 *
 * Implements : ADC_DisableNotification_Activity
 *END**************************************************************************/
status_t ADC_DisableNotification(const adc_pal_instance_t instance, const uint32_t groupIdx)
{
    DEV_ASSERT(instance < NUMBER_OF_ADC_PAL_INSTANCES);

    adc_pal_state_t * state = &(adcPalState[instance]);

    DEV_ASSERT(groupIdx < state->numGroups);

    status_t status = STATUS_SUCCESS;

    if(groupIdx != state->activeGroupIdx)
    {
        status = STATUS_ERROR;
    }
    else
    {
#if defined(ADC_PAL_S32K1xx)

        state->activeGroupState.notificationEn = false;

#elif defined(ADC_PAL_MPC574x)


#endif /* defined(ADC_PAL_MPC574x) */

    }

    return status;
}


/*******************************************************************************
 * Private Functions
 ******************************************************************************/

#if defined(ADC_PAL_S32K1xx)

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_S32K1xx_IrqHandler
 * Description   : Implements interrupt handler functionality to be called from
 * interrupt handlers for each ADC.
 *
 *END**************************************************************************/
void ADC_S32K1xx_IrqHandler(adc_pal_instance_t instance)
{
    adc_pal_state_t * palState                  = &(adcPalState[instance]);
    adc_group_state_t * groupState              = &(palState->activeGroupState);
    const adc_group_config_t * activeGroupCfg   = &(palState->groupArray[palState->activeGroupIdx]);
    uint8_t controlChanIdx                      = 0u;

    uint16_t * result = &(activeGroupCfg->resultBuffer[groupState->currentBufferOffset]);

    /* Read all conversion results */
    for(controlChanIdx = 0u; controlChanIdx < activeGroupCfg->numChannels; controlChanIdx++)
    {
        ADC_DRV_GetChanResult(instance, controlChanIdx, result);    /* interrupt flag is cleared when reading the result */
        result++;
    }
    /* Increment offset in result buffer */
    groupState->currentBufferOffset = (uint16_t)((groupState->currentBufferOffset + activeGroupCfg->numChannels) % groupState->bufferLength);


    if(activeGroupCfg->hwTriggerSupport == false) /* Continuous mode currently supported only for SW triggered groups */
    {
        if(activeGroupCfg->continuousConvEn == true)
        {
            /* Sw trigger PDB */
            PDB_DRV_SoftTriggerCmd(instance);
        }
        else
        {
            palState->activeGroupFlag = false;
        }
    }

    /* Call notification callback, if it is enabled */
    if(groupState->notificationEn)
    {
        adc_callback_info_t cbInfo;
        cbInfo.groupIndex = palState->activeGroupIdx;

        if(groupState->currentBufferOffset == 0u)
        {
            cbInfo.resultBufferTail = (uint16_t)(groupState->bufferLength - 1u); /* set tail to the last position in buffer */
        }
        else
        {
            cbInfo.resultBufferTail = (uint16_t)(groupState->currentBufferOffset - 1u); /* set tail to last written position  */
        }

        //(*(activeGroupCfg->callback))(&cbInfo, activeGroupCfg->callbackUserData);
        (*(activeGroupCfg->callback))(&cbInfo, (void*)instance);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_ConfigPdbAndPretriggers
 * Description   : Configures PDB and PDB pretriggers
 *
 *END**************************************************************************/
static inline void ADC_ConfigPdbAndPretriggers(const uint32_t instance, const pdb_trigger_src_t trgSrc, const adc_group_config_t * currentGroupCfg)
{
    pdb_timer_config_t pdbCfg;
    pdb_adc_pretrigger_config_t pdbPretrigCfg;
    uint8_t idx;

    /* PDB driver instance configuration */
    pdbCfg.loadValueMode        = PDB_LOAD_VAL_IMMEDIATELY;
    pdbCfg.seqErrIntEnable      = false;
    pdbCfg.clkPreDiv            = PDB_CLK_PREDIV_BY_1;
    pdbCfg.clkPreMultFactor     = PDB_CLK_PREMULT_FACT_AS_1;
    pdbCfg.dmaEnable            = false;
    pdbCfg.intEnable            = false;
    pdbCfg.continuousModeEnable = false; /* Continuous mode refers to Counter being reset at zero - not used in ADC PAL */

    pdbCfg.triggerInput         = trgSrc;
    PDB_DRV_Init(instance, &pdbCfg);

    /* PDB pre-triggers configuration */
    pdbPretrigCfg.preTriggerEnable           = true;
    pdbPretrigCfg.preTriggerOutputEnable     = false;

    pdbPretrigCfg.preTriggerBackToBackEnable = false; /* the first pretrigger in the group must not have BB enabled */
    pdbPretrigCfg.adcPreTriggerIdx           = 0u;
    PDB_DRV_ConfigAdcPreTrigger(instance, ADC_PAL_PDB_CHAN, &pdbPretrigCfg);

    pdbPretrigCfg.preTriggerBackToBackEnable = true; /* the rest of pretriggers in the group must have BB enabled */
    for(idx = 1u; idx < currentGroupCfg->numChannels; idx++)
    {
        pdbPretrigCfg.adcPreTriggerIdx = idx;
        PDB_DRV_ConfigAdcPreTrigger(instance, ADC_PAL_PDB_CHAN, &pdbPretrigCfg);
    }
}


/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_ConfigGroup
 * Description   : Configures ADC input channels and PDB for a conversion group
 *
 *END**************************************************************************/
static void ADC_ConfigGroup(const uint32_t instance, const uint32_t groupIdx, const bool hwTriggerFlag)
{
    adc_pal_state_t * palState                  = &(adcPalState[instance]);
    adc_group_state_t * groupState              = &(palState->activeGroupState);
    const adc_group_config_t * currentGroupCfg  = &(palState->groupArray[groupIdx]);

    pdb_trigger_src_t pdbTrigSrc;
    if(hwTriggerFlag == true)
    {
        pdbTrigSrc = PDB_TRIGGER_0;
    }
    else
    {
        pdbTrigSrc = PDB_SOFTWARE_TRIGGER;
    }

    /* Configure PDB instance and pre-triggers */
    ADC_ConfigPdbAndPretriggers(instance, pdbTrigSrc, currentGroupCfg);

    /* Configure ADC channels */
    adc_chan_config_t adcChanCfg;
    uint8_t idx = 0u;

    adcChanCfg.interruptEnable = false; /* interrupt is disabled for all conversion, except the last one in the group */
    for(idx = 0u; idx < currentGroupCfg->numChannels - 1u; idx++)
    {
        adcChanCfg.channel = currentGroupCfg->inputChannelArray[idx]; /* set the ADC input channel */

        ADC_DRV_ConfigChan(instance, idx, &adcChanCfg); /* conversion complete flag is cleared implicitly when writing a new configuration */
    }
    adcChanCfg.interruptEnable = true; /* enable interrupt for last conversion in the group */
    adcChanCfg.channel         = currentGroupCfg->inputChannelArray[idx]; /* set the ADC input channel */
    ADC_DRV_ConfigChan(instance, idx, &adcChanCfg); /* configure the last conversion in the group */

    /* Update ADC PAL and group state structures */
    palState->activeGroupFlag           = true;
    palState->activeGroupIdx            = groupIdx;
    groupState->currentBufferOffset     = 0u;
    groupState->bufferLength            = (uint16_t)(currentGroupCfg->numChannels * currentGroupCfg->numSetsResultBuffer);
    if(currentGroupCfg->callback != NULL)
    {
        groupState->notificationEn = true; /* enable notification by default if callback is available */
    }

    /* Enable interrupt in INT manager */
    IRQn_Type adcIrqId;
    adcIrqId = ADC_DRV_GetInterruptNumber(instance);
    INT_SYS_EnableIRQ(adcIrqId);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ADC_StopGroupBlocking
 * Description   : Stops a group of conversions within a given timeout interval
 *
 *END**************************************************************************/
static status_t ADC_StopGroupBlocking(const uint32_t instance, const uint32_t timeout)
{
    adc_pal_state_t * palState = &(adcPalState[instance]);
    uint32_t startTime, deltaTime;
    status_t status;
    pdb_adc_pretrigger_config_t pdbPretrigCfg;
    uint8_t idx;

    /* Disable the ADC interrupt from Interrupt Manager */
    IRQn_Type adcIrqId;
    adcIrqId = ADC_DRV_GetInterruptNumber(instance);
    INT_SYS_DisableIRQ(adcIrqId);

    /* Make sure OSIF timer is initialized. */
    OSIF_TimeDelay(0u);

    startTime = OSIF_GetMilliseconds();
    deltaTime = 0u;

    /* Reset PDB pre-trigger configurations to stop PDB from triggering other conversions in the group */
    pdbPretrigCfg.preTriggerEnable           = false;
    pdbPretrigCfg.preTriggerOutputEnable     = false;
    pdbPretrigCfg.preTriggerBackToBackEnable = false;
    for(idx = 0u; idx < palState->groupArray[palState->activeGroupIdx].numChannels; idx++)
    {
        pdbPretrigCfg.adcPreTriggerIdx = idx;
        PDB_DRV_ConfigAdcPreTrigger(instance, ADC_PAL_PDB_CHAN, &pdbPretrigCfg);
    }
    /* Completely stop PDB */
    PDB_DRV_Deinit((uint32_t)instance);

    /* Wait for current ADC active conversion to finish execution */
    ADC_Type * const adcBase[ADC_INSTANCE_COUNT] = ADC_BASE_PTRS;
    const ADC_Type * const base                  = adcBase[instance];
    while((ADC_GetConvActiveFlag(base) == true) && (deltaTime < timeout))
    {
        deltaTime = OSIF_GetMilliseconds() - startTime;
    }

    if(deltaTime >= timeout)
    {
        status = STATUS_TIMEOUT;
    }
    else
    {
        status = STATUS_SUCCESS;

        palState->activeGroupFlag = false;
    }

    return status;
}

#elif defined(ADC_PAL_MPC574x)

#endif /* defined(ADC_PAL_MPC574x) */


/*******************************************************************************
 * EOF
 ******************************************************************************/
