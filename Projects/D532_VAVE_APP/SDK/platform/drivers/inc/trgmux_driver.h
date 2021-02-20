/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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

#ifndef TRGMUX_DRIVER_H
#define TRGMUX_DRIVER_H

/*! @file trgmux_driver.h */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.1, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.2, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.4, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 5.5, identifier clash
 * The supported compilers use more than 31 significant characters for identifiers.
 */

#include <stdint.h>
#include <stdbool.h>
#include "status.h"
#include "device_registers.h"

/*!
 * @addtogroup trgmux_driver
 * @{
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Describes all possible inputs (trigger sources) of the TRGMUX IP
 * This enumeration depends on the supported instances in device which is defined in
 * <i>"device_name"_features.h</i> file
 *
 * Implements : trgmux_trigger_source_t_Class
 */
typedef enum
{
    TRGMUX_TRIG_SOURCE_DISABLED             = 0U,
    TRGMUX_TRIG_SOURCE_VDD                  = 1U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN0           = 2U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN1           = 3U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN2           = 4U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN3           = 5U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN4           = 6U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN5           = 7U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN6           = 8U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN7           = 9U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN8           = 10U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN9           = 11U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN10          = 12U,
    TRGMUX_TRIG_SOURCE_TRGMUX_IN11          = 13U,
    TRGMUX_TRIG_SOURCE_CMP0_OUT             = 14U,
    TRGMUX_TRIG_SOURCE_LPIT_CH0             = 17U,
    TRGMUX_TRIG_SOURCE_LPIT_CH1             = 18U,
    TRGMUX_TRIG_SOURCE_LPIT_CH2             = 19U,
    TRGMUX_TRIG_SOURCE_LPIT_CH3             = 20U,
    TRGMUX_TRIG_SOURCE_LPTMR0               = 21U,
    TRGMUX_TRIG_SOURCE_FTM0_INIT_TRIG       = 22U,
    TRGMUX_TRIG_SOURCE_FTM0_EXT_TRIG        = 23U,
    TRGMUX_TRIG_SOURCE_FTM1_INIT_TRIG       = 24U,
    TRGMUX_TRIG_SOURCE_FTM1_EXT_TRIG        = 25U,
    TRGMUX_TRIG_SOURCE_FTM2_INIT_TRIG       = 26U,
    TRGMUX_TRIG_SOURCE_FTM2_EXT_TRIG        = 27U,
    TRGMUX_TRIG_SOURCE_FTM3_INIT_TRIG       = 28U,
    TRGMUX_TRIG_SOURCE_FTM3_EXT_TRIG        = 29U,
    TRGMUX_TRIG_SOURCE_ADC0_SC1A_COCO       = 30U,
    TRGMUX_TRIG_SOURCE_ADC0_SC1B_COCO       = 31U,
    TRGMUX_TRIG_SOURCE_ADC1_SC1A_COCO       = 32U,
    TRGMUX_TRIG_SOURCE_ADC1_SC1B_COCO       = 33U,
    TRGMUX_TRIG_SOURCE_PDB0_CH0_TRIG        = 34U,
    TRGMUX_TRIG_SOURCE_PDB0_PULSE_OUT       = 36U,
    TRGMUX_TRIG_SOURCE_PDB1_CH0_TRIG        = 37U,
    TRGMUX_TRIG_SOURCE_PDB1_PULSE_OUT       = 39U,
    TRGMUX_TRIG_SOURCE_RTC_ALARM            = 43U,
    TRGMUX_TRIG_SOURCE_RTC_SECOND           = 44U,
    TRGMUX_TRIG_SOURCE_FLEXIO_TRIG0         = 45U,
    TRGMUX_TRIG_SOURCE_FLEXIO_TRIG1         = 46U,
    TRGMUX_TRIG_SOURCE_FLEXIO_TRIG2         = 47U,
    TRGMUX_TRIG_SOURCE_FLEXIO_TRIG3         = 48U,
    TRGMUX_TRIG_SOURCE_LPUART0_RX_DATA      = 49U,
    TRGMUX_TRIG_SOURCE_LPUART0_TX_DATA      = 50U,
    TRGMUX_TRIG_SOURCE_LPUART0_RX_IDLE      = 51U,
    TRGMUX_TRIG_SOURCE_LPUART1_RX_DATA      = 52U,
    TRGMUX_TRIG_SOURCE_LPUART1_TX_DATA      = 53U,
    TRGMUX_TRIG_SOURCE_LPUART1_RX_IDLE      = 54U,
    TRGMUX_TRIG_SOURCE_LPI2C0_MASTER_TRIG   = 55U,
    TRGMUX_TRIG_SOURCE_LPI2C0_SLAVE_TRIG    = 56U,
    TRGMUX_TRIG_SOURCE_LPSPI0_FRAME         = 59U,
    TRGMUX_TRIG_SOURCE_LPSPI0_RX_DATA       = 60U,
    TRGMUX_TRIG_SOURCE_LPSPI1_FRAME         = 61U,
    TRGMUX_TRIG_SOURCE_LPSPI1_RX_DATA       = 62U,
    TRGMUX_TRIG_SOURCE_SIM_SW_TRIG          = 63U,
#if FEATURE_TRGMUX_SUPPORT_LPI2C1
    TRGMUX_TRIG_SOURCE_LPI2C1_MASTER_TRIG   = 67U,
    TRGMUX_TRIG_SOURCE_LPI2C1_SLAVE_TRIG    = 68U,
#endif /* FEATURE_TRGMUX_SUPPORT_LPI2C1 */
#if FEATURE_TRGMUX_SUPPORT_FTM_4_5
    TRGMUX_TRIG_SOURCE_FTM4_INIT_TRIG       = 69U,
    TRGMUX_TRIG_SOURCE_FTM4_EXT_TRIG        = 70U,
    TRGMUX_TRIG_SOURCE_FTM5_INIT_TRIG       = 71U,
    TRGMUX_TRIG_SOURCE_FTM5_EXT_TRIG        = 72U,
#endif /* FEATURE_TRGMUX_SUPPORT_FTM_4_5 */
#if FEATURE_TRGMUX_SUPPORT_FTM_6_7
    TRGMUX_TRIG_SOURCE_FTM6_INIT_TRIG       = 73U,
    TRGMUX_TRIG_SOURCE_FTM6_EXT_TRIG        = 74U,
    TRGMUX_TRIG_SOURCE_FTM7_INIT_TRIG       = 75U,
    TRGMUX_TRIG_SOURCE_FTM7_EXT_TRIG        = 76U
#endif /* FEATURE_TRGMUX_SUPPORT_FTM_6_7 */
} trgmux_trigger_source_t;

/*! @brief Describes all possible outputs (target modules) of the TRGMUX IP
 * This enumeration depends on the supported instances in device which is defined in
 * <i>"device_name"_features.h</i> file
 *
 * Implements : trgmux_target_module_t_Class
 */
typedef enum
{
    TRGMUX_TARGET_MODULE_DMA_CH0            = 0U,
    TRGMUX_TARGET_MODULE_DMA_CH1            = 1U,
    TRGMUX_TARGET_MODULE_DMA_CH2            = 2U,
    TRGMUX_TARGET_MODULE_DMA_CH3            = 3U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT0        = 4U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT1        = 5U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT2        = 6U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT3        = 7U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT4        = 8U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT5        = 9U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT6        = 10U,
    TRGMUX_TARGET_MODULE_TRGMUX_OUT7        = 11U,
    TRGMUX_TARGET_MODULE_ADC0_ADHWT_TLA0    = 12U,
    TRGMUX_TARGET_MODULE_ADC0_ADHWT_TLA1    = 13U,
    TRGMUX_TARGET_MODULE_ADC0_ADHWT_TLA2    = 14U,
    TRGMUX_TARGET_MODULE_ADC0_ADHWT_TLA3    = 15U,
    TRGMUX_TARGET_MODULE_ADC1_ADHWT_TLA0    = 16U,
    TRGMUX_TARGET_MODULE_ADC1_ADHWT_TLA1    = 17U,
    TRGMUX_TARGET_MODULE_ADC1_ADHWT_TLA2    = 18U,
    TRGMUX_TARGET_MODULE_ADC1_ADHWT_TLA3    = 19U,
    TRGMUX_TARGET_MODULE_CMP0_SAMPLE_INPUT  = 28U,
    TRGMUX_TARGET_MODULE_FTM0_HWTRIG0       = 40U,
    TRGMUX_TARGET_MODULE_FTM0_FAULT0        = 41U,
    TRGMUX_TARGET_MODULE_FTM0_FAULT1        = 42U,
    TRGMUX_TARGET_MODULE_FTM0_FAULT2        = 43U,
    TRGMUX_TARGET_MODULE_FTM1_HWTRIG0       = 44U,
    TRGMUX_TARGET_MODULE_FTM1_FAULT0        = 45U,
    TRGMUX_TARGET_MODULE_FTM1_FAULT1        = 46U,
    TRGMUX_TARGET_MODULE_FTM1_FAULT2        = 47U,
    TRGMUX_TARGET_MODULE_FTM2_HWTRIG0       = 48U,
    TRGMUX_TARGET_MODULE_FTM2_FAULT0        = 49U,
    TRGMUX_TARGET_MODULE_FTM2_FAULT1        = 50U,
    TRGMUX_TARGET_MODULE_FTM2_FAULT2        = 51U,
    TRGMUX_TARGET_MODULE_FTM3_HWTRIG0       = 52U,
    TRGMUX_TARGET_MODULE_FTM3_FAULT0        = 53U,
    TRGMUX_TARGET_MODULE_FTM3_FAULT1        = 54U,
    TRGMUX_TARGET_MODULE_FTM3_FAULT2        = 55U,
    TRGMUX_TARGET_MODULE_PDB0_TRG_IN        = 56U,
    TRGMUX_TARGET_MODULE_PDB1_TRG_IN        = 60U,
    TRGMUX_TARGET_MODULE_FLEXIO_TRG_TIM0    = 68U,
    TRGMUX_TARGET_MODULE_FLEXIO_TRG_TIM1    = 69U,
    TRGMUX_TARGET_MODULE_FLEXIO_TRG_TIM2    = 70U,
    TRGMUX_TARGET_MODULE_FLEXIO_TRG_TIM3    = 71U,
    TRGMUX_TARGET_MODULE_LPIT_TRG_CH0       = 72U,
    TRGMUX_TARGET_MODULE_LPIT_TRG_CH1       = 73U,
    TRGMUX_TARGET_MODULE_LPIT_TRG_CH2       = 74U,
    TRGMUX_TARGET_MODULE_LPIT_TRG_CH3       = 75U,
    TRGMUX_TARGET_MODULE_LPUART0_TRG        = 76U,
    TRGMUX_TARGET_MODULE_LPUART1_TRG        = 80U,
    TRGMUX_TARGET_MODULE_LPI2C0_TRG         = 84U,
    TRGMUX_TARGET_MODULE_LPSPI0_TRG         = 92U,
    TRGMUX_TARGET_MODULE_LPSPI1_TRG         = 96U,
    TRGMUX_TARGET_MODULE_LPTMR0_ALT0        = 100U,
#if FEATURE_TRGMUX_SUPPORT_LPI2C1
    TRGMUX_TARGET_MODULE_LPI2C1_TRG         = 108U,
#endif /* FEATURE_TRGMUX_SUPPORT_LPI2C1 */
#if FEATURE_TRGMUX_SUPPORT_FTM_4_5
    TRGMUX_TARGET_MODULE_FTM4_HWTRIG0       = 112U,
    TRGMUX_TARGET_MODULE_FTM5_HWTRIG0       = 116U,
#endif /* FEATURE_TRGMUX_SUPPORT_FTM_4_5 */
#if FEATURE_TRGMUX_SUPPORT_FTM_6_7
    TRGMUX_TARGET_MODULE_FTM6_HWTRIG0       = 120U,
    TRGMUX_TARGET_MODULE_FTM7_HWTRIG0       = 124U
#endif /* FEATURE_TRGMUX_SUPPORT_FTM_6_7 */
} trgmux_target_module_t;

/*!
 * @brief Configuration structure for pairing source triggers with target modules.
 *
 * Use an instance of this structure to define a TRGMUX link between a trigger source and a target module.
 * This structure is used by the user configuration structure.
 *
 * Implements : trgmux_inout_mapping_config_t_Class
 */
typedef struct
{
    trgmux_trigger_source_t triggerSource; /*!< selects one of the TRGMUX trigger sources */
    trgmux_target_module_t targetModule;   /*!< selects one of the TRGMUX target modules  */
    bool lockTargetModuleReg;              /*!< if true, the LOCK bit of the target module register will be
                                                set by TRGMUX_DRV_INIT(), after the current mapping is configured */
} trgmux_inout_mapping_config_t;

/*!
 * @brief User configuration structure for the TRGMUX driver.
 *
 * Use an instance of this structure with the TRGMUX_DRV_Init() function. This enables configuration of TRGMUX with the user
 * defined mappings between inputs (source triggers) and outputs (target modules), via a single function call.
 *
 * Implements : trgmux_user_config_t_Class
 */
typedef struct
{
    uint8_t numInOutMappingConfigs;                           /*!< number of in-out mappings defined in TRGMUX configuration */
    const trgmux_inout_mapping_config_t * inOutMappingConfig; /*!< pointer to array of in-out mapping structures */
} trgmux_user_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif


/*!
 * @brief Initialize a TRGMUX instance for operation.
 *
 * This function first resets the source triggers of all TRGMUX target modules to their default values,
 * then configures the TRGMUX with all the user defined in-out mappings.
 * If at least one of the target modules is locked, the function will not change any of the TRGMUX target modules
 * and return error code.
 * This example shows how to set up the trgmux_user_config_t parameters and how to call the
 * TRGMUX_DRV_Init() function with the required parameters:
 *  @code
 *   trgmux_user_config_t             trgmuxConfig;
 *   trgmux_inout_mapping_config_t    trgmuxInoutMappingConfig[] =
 *   {
 *       {TRGMUX_TRIG_SOURCE_TRGMUX_IN9,     TRGMUX_TARGET_MODULE_DMA_CH0,     false},
 *       {TRGMUX_TRIG_SOURCE_FTM1_EXT_TRIG,  TRGMUX_TARGET_MODULE_TRGMUX_OUT4, true}
 *   };
 *
 *   trgmuxConfig.numInOutMappingConfigs = 2;
 *   trgmuxConfig.inOutMappingConfig     = trgmuxInoutMappingConfig;
 *
 *   TRGMUX_DRV_Init(instance, &trgmuxConfig);
 *   @endcode
 *
 * @param[in] instance          The TRGMUX instance number.
 * @param[in] trgmuxUserConfig  Pointer to the user configuration structure.
 * @return                      Execution status: \n
 *   STATUS_SUCCESS \n
 *   STATUS_ERROR    - if at least one of the target module register is locked.  */
status_t TRGMUX_DRV_Init(const uint32_t instance,
                         const trgmux_user_config_t * const trgmuxUserConfig);

/*!
 * @brief Reset to default values the source triggers corresponding to all target modules,
 * if none of the target modules is locked.
 *
 * @param[in] instance          The TRGMUX instance number.
 * @return                      Execution status: \n
 *   STATUS_SUCCESS \n
 *   STATUS_ERROR    - if at least one of the target module register is locked.
 */
status_t TRGMUX_DRV_Deinit(const uint32_t instance);

/*!
 * @brief Configure a source trigger for a selected target module.
 *
 * This function configures a TRGMUX link between a source trigger and a target module,
 * if the requested target module is not locked.
 *
 * @param[in] instance          The TRGMUX instance number.
 * @param[in] triggerSource     One of the values in the trgmux_trigger_source_t enumeration
 * @param[in] targetModule      One of the values in the trgmux_target_module_t enumeration
 * @return                      Execution status: \n
 *   STATUS_SUCCESS \n
 *   STATUS_ERROR    - if requested target module is locked  */
status_t TRGMUX_DRV_SetTrigSourceForTargetModule(const uint32_t instance,
                                                 const trgmux_trigger_source_t triggerSource,
                                                 const trgmux_target_module_t targetModule);

/*!
 * @brief Get the source trigger configured for a target module.
 *
 * This function returns the TRGMUX source trigger linked to a selected target module.
 *
 * @param[in] instance      The TRGMUX instance number.
 * @param[in] targetModule  One of the values in the trgmux_target_module_t enumeration.
 * @return                  Enum value corresponding to the trigger source configured
 *                          for the selected target module.
 */
trgmux_trigger_source_t TRGMUX_DRV_GetTrigSourceForTargetModule(const uint32_t instance,
                                                                const trgmux_target_module_t targetModule);

/*!
 * @brief Locks the TRGMUX register of a target module.
 *
 * This function sets the LK bit of the TRGMUX register corresponding to
 * the selected target module. Please note that some TRGMUX registers can contain up to 4
 * SEL bitfields, meaning that these registers can be used to configure up to 4 target
 * modules independently. Because the LK bit is only one per register, the configuration
 * of all target modules referred from that register will be locked.
 *
 * @param[in] instance          The TRGMUX instance number.
 * @param[in] targetModule      One of the values in the trgmux_target_module_t enumeration
 */
void TRGMUX_DRV_SetLockForTargetModule(const uint32_t instance,
                                       const trgmux_target_module_t targetModule);

/*!
 * @brief Get the Lock bit status of the TRGMUX register of a target module.
 *
 * This function gets the value of the LK bit from the TRGMUX register corresponding to
 * the selected target module.
 *
 * @param[in] instance          The TRGMUX instance number.
 * @param[in] targetModule      One of the values in the trgmux_target_module_t enumeration
 * @return                      true - if the selected targetModule register is locked \n
 *                              false - if the selected targetModule register is not locked
 */
bool TRGMUX_DRV_GetLockForTargetModule(const uint32_t instance,
                                       const trgmux_target_module_t targetModule);

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* TRGMUX_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
