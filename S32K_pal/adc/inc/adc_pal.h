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

#ifndef ADC_PAL_H
#define ADC_PAL_H

#include "adc_pal_cfg.h"
#include "adc_pal_mapping.h"
#include "status.h"
#include "callbacks.h"

/*! @file adc_hal.h */

/*!
 * @ingroup adc_pal
 * @addtogroup adc_pal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if defined(ADC_PAL_S32K1xx)

/*!
 * @brief Defines the enumeration with ADC PAL input channels
 *
 * Implements : adc_inputchannel_t_Class
 */
typedef adc_inputchannel_t adc_input_chan_t;

/*!
 * @brief Defines the enumeration with ADC PAL hardware trigger sources
 *
 * Implements : trgmux_trigger_source_t_Class
 */
typedef trgmux_trigger_source_t adc_trigger_source_t;

#elif defined(ADC_PAL_MPC574x)

typedef uint32_t adc_input_chan_t;      /* TODO: should be made enum inside ADC C55 driver - however PAL needs to expose the reunion of all ADC chans */

typedef uint32_t adc_trigger_source_t;  /* TODO: should be made enum inside BCTU driver */

#endif /* defined(ADC_PAL_MPC574x) */


/*!
 * @brief Defines the configuration structure for an ADC PAL conversion group
 *
 * Implements : adc_group_config_t_Class
 */
typedef struct
{
    const adc_input_chan_t * inputChannelArray;   /*!< Pointer to the array of ADC input channels. Each entry in this array corresponds to an individual conversion in the group.
                                                       The same input channel may appear multiple times. E.g.: InputChan0,InputChan1,InputChan0,InputChan2 */
    uint16_t * resultBuffer;                      /*!< Pointer to the array for conversion results */
    uint8_t numChannels;                          /*!< Number of input channels in the array */
    uint8_t numSetsResultBuffer;                  /*!< Number of sets of results which can be stored in result buffer:
                                                       length of the result buffer = numChannels x numSetsResultBuffer */
    bool hwTriggerSupport;                        /*!< Conversion group is HW triggered (true) or SW triggered (false).  */
    adc_trigger_source_t triggerSource;           /*!< HW trigger source associated with the conversion group. Will be ignored if (hwTriggerSupport == false) */
    bool continuousConvEn;                        /*!< Flag for enabling continuous conversions of a group - used only for SW triggered groups i.e. hwTriggerSupport==false. */
    adc_callback_t callback;                      /*!< Callback function associated with group conversion complete */
    void * callbackUserData;                      /*!< Pointer to additional user data to be passed by the callback */
} adc_group_config_t;


/*!
 * @brief Defines the configuration structure for ADC PAL
 *
 * Implements : adc_config_t_Class
 */
typedef struct
{
    const adc_group_config_t * groupConfigArray;  /*!< Array of group configurations */
    uint16_t numGroups;                           /*!< Number of elements in groupConfigArray */

    uint8_t sampleTicks;                          /*!< Duration of sample time expressed in ADC clock ticks */
    void * extension;                             /*!< This field is used to add extra IP specific settings to the basic configuration. */
} adc_config_t;


#if defined(ADC_PAL_S32K1xx)
/*!
 * @brief Defines the extension structure for ADC S32K1xx
 *
 * Implements : extension_adc_s32k1xx_t_Class
 */
typedef struct
{
    adc_clk_divide_t clockDivide;        /*!< Divider of the input clock for the ADC */
    adc_resolution_t resolution;         /*!< ADC resolution (8,10,12 bit) */
    adc_input_clock_t inputClock;        /*!< Input clock source */
    adc_voltage_reference_t voltageRef;  /*!< Voltage reference used */
} extension_adc_s32k1xx_t;
#endif /* defined(ADC_PAL_S32K1xx) */


#if defined(ADC_PAL_MPC574x)
/*!
 * @brief Defines the extension structure for ADC MPC574x
 *
 * Implements : extension_adc_mpc574x_t
 */
typedef struct
{
    adc_clk_sel_t clkSelect;            /*!< Clock input */
    adc_ref_sel_t refSelect;            /*!< Reference selection  */
    bool autoClockOff;                  /*!< Enable Auto Clock Off */
} extension_adc_mpc574x_t;
#endif /* defined(ADC_PAL_MPC574x) */



/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the ADC PAL instance
 *
 * This function initializes the ADC PAL instance, including the other platform specific HW units used together with ADC.
 * Notifications are default enabled after init.
 *
 * @param[in] instance The ADC PAL instance
 * @param[in] config The ADC PAL configuration structure
 * @return status:
 * \n - STATUS_ERROR: error initializing one of the HW modules used by ADC PAL
 * \n - STATUS_SUCCESS: ADC PAL initialized successfully
 */
status_t ADC_Init(const adc_pal_instance_t instance, const adc_config_t * const config);

/*!
 * @brief Deinitializes the ADC PAL instance
 *
 * This function resets the ADC PAL instance, including the other platform specific HW units used together with ADC,
 * if there are no active conversions.
 *
 * @param[in] instance The ADC PAL instance
 * @return status:
 * \n - STATUS_BUSY: there is already a HW triggered group enabled or executing, or a SW triggered group executing
 * \n - STATUS_SUCCESS: ADC PAL initialized successfully
 */
status_t ADC_Deinit(const adc_pal_instance_t instance);


/*!
 * @brief Enables the selected HW trigger for a conversion group, if the conversion group has support for HW trigger.
 *
 * Enables the selected HW trigger for a conversion group, if the conversion group has support for HW trigger.
 * The function will return an error code if there is a conversion group already active.
 * If the function succeeds, the conversion group will be triggered for execution when the
 * selected HW trigger occurs.
 *
 * @param[in] instance The ADC PAL instance
 * @param[in] groupIdx Index of the selected group configured via groupConfigArray in adc_config_t
 * @return status:
 * \n - STATUS_BUSY: there is already a HW triggered group enabled or executing, or a SW triggered group executing
 * \n - STATUS_SUCCESS: HW trigger enabled successfully for the selected conversion group
 */
status_t ADC_EnableHardwareTrigger(const adc_pal_instance_t instance, const uint32_t groupIdx);


/*!
 * @brief Disables the selected HW trigger for a conversion group, if the conversion group has support for HW trigger.
 *
 * This function disables HW trigger for a configured conversion group and also stops its execution,
 * if called when a conversion group is executing. The execution shall be stopped according to device specific procedures.
 * The function shall wait for the procedures to complete within the given timeout interval and return error code if they do not succeed.
 * E.g. on calypso a conversion already started for execution cannot be stopped, so the function shall wait until it finishes
 * or timeout occurs.
 *
 * @param[in] instance The ADC PAL instance
 * @param[in] groupIdx Index of the selected group configured via groupConfigArray in adc_config_t
 * @param[in] timeout Timeout interval in milliseconds
 * @return status:
 * \n - STATUS_TIMEOUT: the operation did not complete successfully within the provided timeout interval
 * \n - STATUS_SUCCESS: the operation completed successfully within the provided timeout interval
 */
status_t ADC_DisableHardwareTrigger(const adc_pal_instance_t instance, const uint32_t groupIdx, const uint32_t timeout);


/*!
 * @brief Starts the execution of a selected SW triggered ADC conversion group.
 *
 * This function starts execution of a selected ADC conversion group, if there is no other conversion group active.
 * Conversion groups started by ADC_StartGroupConversion shall not be preempted by HW triggered conversion groups.
 *
 * @param[in] instance The ADC PAL instance
 * @param[in] groupIdx Index of the selected group configured via groupConfigArray in adc_config_t
 * @return status:
 * \n - STATUS_BUSY: there is already a HW triggered group enabled or executing, or a SW triggered group executing
 * \n - STATUS_SUCCESS: group conversion successfully triggered
 */
status_t ADC_StartGroupConversion(const adc_pal_instance_t instance, const uint32_t groupIdx);


/*!
 * @brief Stops the selected SW triggered ADC conversion group execution.
 *
 * This function stops the execution of a SW triggered conversion group.
 * The execution shall be stopped according to device specific procedures.
 * The function shall wait for the procedures to complete within the given timeout interval and return error code if they do not succeed.
 * E.g. on calypso a conversion already started for execution cannot be stopped, so the function shall wait until it finishes
 * or timeout occurs.
 *
 * @param[in] instance The ADC PAL instance
 * @param[in] groupIdx Index of the selected group configured via groupConfigArray in adc_config_t
 * @param[in] timeout Timeout interval in milliseconds
 * @return status:
 * \n - STATUS_TIMEOUT: the operation did not complete successfully within the provided timeout interval
 * \n - STATUS_SUCCESS: the operation completed successfully within the provided timeout interval
 */
status_t ADC_StopGroupConversion(const adc_pal_instance_t instance, const uint32_t groupIdx, const uint32_t timeout);


/*!
 * @brief Enables the notification callback for a configured group
 *
 * This function enables the notification callback for a selected group of ADC conversions.
 *
 * @param[in] instance The ADC PAL instance
 * @param[in] groupIdx Index of the selected group configured via groupConfigArray in adc_config_t
 * @return status:
 * \n - STATUS_ERROR: the selected group is not active (SW triggered running or HW triggered running or enabled)
 * \n - STATUS_SUCCESS: the notification has been enabled successfully
 */
status_t ADC_EnableNotification(const adc_pal_instance_t instance, const uint32_t groupIdx);


/*!
 * @brief Disables the notification callback for a configured group
 *
 * This function disables the notification callback for a selected group of ADC conversions.
 *
 * @param[in] instance The ADC PAL instance
 * @param[in] groupIdx Index of the selected group configured via groupConfigArray in adc_config_t
 * @return status:
 * \n - STATUS_ERROR: the selected group is not active (SW triggered running or HW triggered running or enabled)
 * \n - STATUS_SUCCESS: the notification has been disabled successfully
 */
status_t ADC_DisableNotification(const adc_pal_instance_t instance, const uint32_t groupIdx);


#if defined(__cplusplus)
}
#endif

/*! @}*/
#endif /* ADC_PAL_H */


/*******************************************************************************
 * EOF
 ******************************************************************************/
