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
/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, There shall be no occurrence of
 * undefined or critical unspecified behaviour.
 * The addresses of the stack variables are only used at local scope.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or different essential type.
 * The cast is required to perform a conversion between an unsigned integer and an enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, A cast shall not be performed
 * between pointer to void and an arithmetic type.
 * The base address parameter from HAL functions is provided as integer so
 * it needs to be cast to pointer.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, A conversion should not be performed
 * between a pointer to object and an integer type.
 * The base address parameter from HAL functions is provided as integer so
 * a conversion between a pointer and an integer has to be performed
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * It's temporary until CLOCK_DRV_Init function will be declared
 */

#include "device_registers.h"
#include "sim_hw_access.h"
#include "scg_hw_access.h"
#include "pcc_hw_access.h"
#include "pmc_hw_access.h"
#include "smc_hw_access.h"
#include "clock_manager.h"
#include <stddef.h>   /* This header is included for bool type */
/*
 * README:
 * This file provides these APIs:
 * 1. APIs to get the frequency of output clocks in Reference Manual ->
 * Chapter Clock Distribution -> Figure Clocking diagram.
 * 2. APIs for IP modules listed in Reference Manual -> Chapter Clock Distribution
 * -> Module clocks.
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* This frequency values should be set by different boards. */
/* SIM */
uint32_t g_TClkFreq[NUMBER_OF_TCLK_INPUTS];      /* TCLKx clocks    */

/* RTC */
uint32_t g_RtcClkInFreq;                         /* RTC CLKIN clock */

/* SCG */
uint32_t g_xtal0ClkFreq;                         /* EXTAL0 clock    */


/* @brief System PLL base multiplier value, it is the multiplier value when SCG_SPLLCFG[MULT]=0. */
#define SCG_SPLL_MULT_BASE 16U

/*
 * @brief System PLL base divider value, it is the PLL reference clock divider value when
 * SCG_SPLLCFG[PREDIV]=0.
 */
#define SCG_SPLL_PREDIV_BASE 1U

/*
 * @brief System PLL reference clock after SCG_SPLLCFG[PREDIV] should be in the range of
 * SCG_SPLL_REF_MIN to SCG_SPLL_REF_MAX.
 */
#define SCG_SPLL_REF_MIN 8000000U

/*
 * @brief System PLL reference clock after SCG_SPLLCFG[PREDIV] should be in the range of
 * SCG_SPLL_REF_MIN to SCG_SPLL_REF_MAX.
 */
#define SCG_SPLL_REF_MAX 32000000U

/*
 * @brief LPO 128K fixed clock frequency.
 */
#define LPO_128K_FREQUENCY 128000UL

/*
 * @brief LPO 32K fixed clock frequency.
 */
#define LPO_32K_FREQUENCY 32000UL

/*
 * @brief LPO 1K fixed clock frequency.
 */
#define LPO_1K_FREQUENCY 1000UL

/*
 * @brief Running modes.
 */
#define HIGH_SPEED_RUNNING_MODE (1UL << 7U)
#define RUN_SPEED_RUNNING_MODE  (1UL << 0U)
#define VLPR_SPEED_RUNNING_MODE (1UL << 2U)

/*! @brief Clock name mappings
 *         Constant array storing the mappings between clock names and peripheral clock control indexes.
 *         If there is no peripheral clock control index for a clock name, then the corresponding value is
 *         PCC_INVALID_INDEX.
 */
const uint16_t clockNameMappings[] = PCC_CLOCK_NAME_MAPPINGS;

/*! @brief Peripheral features list
 *         Constant array storing the mappings between clock names of the peripherals and feature lists.
 */
const uint8_t peripheralFeaturesList[] = PERIPHERAL_FEATURES;

/*!
 * @brief SCG system clock type.
 * Implements scg_system_clock_type_t_Class
 */
typedef enum
{
    SCG_SYSTEM_CLOCK_CORE,  /*!< Core clock.        */
    SCG_SYSTEM_CLOCK_BUS,   /*!< BUS clock.         */
    SCG_SYSTEM_CLOCK_SLOW,  /*!< System slow clock. */
    SCG_SYSTEM_CLOCK_MAX,   /*!< Max value.         */
} scg_system_clock_type_t;

/*!
 * @brief SCG asynchronous clock type.
 * Implements scg_async_clock_type_t_Class
 */
typedef enum
{
    SCG_ASYNC_CLOCK_DIV1   = 0U,   /*!< Clock divider 1  */
    SCG_ASYNC_CLOCK_DIV2   = 1U,   /*!< Clock divider 2  */
    SCG_ASYNC_CLOCK_MAX    = 2U,   /*!< Max value.       */
} scg_async_clock_type_t;

/*!
 * @brief SCG system clock modes.
 * Implements scg_system_clock_mode_t_Class
 */
typedef enum
{
    SCG_SYSTEM_CLOCK_MODE_CURRENT = 0U,  /*!< Current mode.            */
    SCG_SYSTEM_CLOCK_MODE_RUN     = 1U,  /*!< Run mode.                */
    SCG_SYSTEM_CLOCK_MODE_VLPR    = 2U,  /*!< Very Low Power Run mode. */
    SCG_SYSTEM_CLOCK_MODE_HSRUN   = 3U,  /*!< High Speed Run mode.     */
    SCG_SYSTEM_CLOCK_MODE_NONE           /*!< MAX value.               */
} scg_system_clock_mode_t;


/*******************************************************************************
 * INTERNAL FUNCTIONS
 ******************************************************************************/

static status_t CLOCK_SYS_GetScgClockFreq(clock_names_t clockName,
                                          uint32_t * frequency);

static status_t CLOCK_SYS_GetSimClockFreq(clock_names_t clockName,
                                          uint32_t * frequency);

static status_t CLOCK_SYS_GetPccClockFreq(clock_names_t clockName,
                                          uint32_t * frequency);

static uint32_t CLOCK_SYS_GetPeripheralClock(clock_names_t clockName,
                                             scg_async_clock_type_t divider);

static scg_system_clock_mode_t CLOCK_SYS_GetCurrentRunMode(void);

static status_t CLOCK_SYS_TransitionSystemClock(const scg_system_clock_config_t * to_clk);

static uint32_t CLOCK_SYS_GetSimClkOutFreq(void);

static uint32_t CLOCK_SYS_GetScgClkOutFreq(void);

static uint32_t CLOCK_SYS_GetSimRtcClkFreq(void);

static status_t CLOCK_SYS_ConfigureTemporarySystemClock(void);

static status_t CLOCK_SYS_ConfigureModulesFromScg(const scg_config_t * scgConfig);

static status_t CLOCK_SYS_ConfigureSIRC(bool enable, const scg_sirc_config_t * sircConfig);

static status_t CLOCK_SYS_ConfigureFIRC(bool enable, const scg_firc_config_t * fircConfig);

static status_t CLOCK_SYS_ConfigureSOSC(bool enable, const scg_sosc_config_t * soscConfig);

static status_t CLOCK_SYS_ConfigureSPLL(bool enable, const scg_spll_config_t * spllConfig);

static uint32_t CLOCK_SYS_GetSystemClockFreq(scg_system_clock_type_t type);

static status_t CLOCK_SYS_SetSystemClockConfig(scg_system_clock_mode_t mode,
                                               scg_system_clock_config_t const * config);

static uint32_t CLOCK_SYS_GetSysAsyncFreq(clock_names_t clockSource,
                                          scg_async_clock_type_t type);

static uint32_t CLOCK_SYS_GetSircFreq(void);

static uint32_t CLOCK_SYS_GetFircFreq(void);

static uint32_t CLOCK_SYS_GetSysOscFreq(void);

static uint32_t CLOCK_SYS_GetSysPllFreq(void);

static uint32_t CLOCK_SYS_GetLpoFreq(void);

static status_t CLOCK_SYS_TransitionToTmpSysClk(scg_system_clock_src_t currentSysClkSrc);

static void CLOCK_SYS_GetCurrentSysClkConfig(scg_system_clock_config_t * sysClockConfig);

/*******************************************************************************
 * Code
 ******************************************************************************/


/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_SetConfiguration
 * Description   : This function sets the system to target configuration, it
 * only sets the clock modules registers for clock mode change, but not send
 * notifications to drivers.
 *
 * Implements CLOCK_SYS_SetConfiguration_Activity
 * END**************************************************************************/
status_t CLOCK_SYS_SetConfiguration(clock_manager_user_config_t const * config)
{
    status_t result;
    DEV_ASSERT(config != NULL);

    /* Set SCG settings. */
    result = CLOCK_SYS_SetScgConfiguration(&config->scgConfig);

    if (STATUS_SUCCESS == result)
    {
        /* Set PCC settings. */
        CLOCK_SYS_SetPccConfiguration(&config->pccConfig);

        /* Set SIM settings. */
        CLOCK_SYS_SetSimConfiguration(&config->simConfig);

        /* Set PMC settings. */
        CLOCK_SYS_SetPmcConfiguration(&config->pmcConfig);
    }

    return result;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_SetScgConfiguration
 * Description   : This function configures the SCG blocks
 *
 * Implements CLOCK_SYS_SetScgConfiguration_Activity
 * END**************************************************************************/
status_t CLOCK_SYS_SetScgConfiguration(const scg_config_t * scgConfig)
{
    status_t status = STATUS_SUCCESS;
    DEV_ASSERT(scgConfig != NULL);

    if (scgConfig != NULL)
    {
        /* Configure a temporary system clock source: FIRC */
        status = CLOCK_SYS_ConfigureTemporarySystemClock();

        if (status == STATUS_SUCCESS)
        {
            /* Configure clock sources from SCG */
            status = CLOCK_SYS_ConfigureModulesFromScg(scgConfig);
        }

        if (status == STATUS_SUCCESS)
        {
            /* Configure RTC. */
            if (scgConfig->rtcConfig.initialize)
            {
                /* RTC Clock settings. */
                g_RtcClkInFreq = scgConfig->rtcConfig.rtcClkInFreq;
            }

            /* Configure SCG ClockOut. */
            if (scgConfig->clockOutConfig.initialize)
            {
                /* ClockOut settings. */
                SCG_SetClockoutSourceSel(SCG, (uint32_t)scgConfig->clockOutConfig.source);
            }

            /* Configure SCG clock modes. */
            if (scgConfig->clockModeConfig.initialize)
            {
                /* Configure SCG clock modes */
                status = CLOCK_SYS_SetSystemClockConfig(SCG_SYSTEM_CLOCK_MODE_RUN, &(scgConfig->clockModeConfig.rccrConfig));
                if (status == STATUS_SUCCESS)
                {
                    status = CLOCK_SYS_SetSystemClockConfig(SCG_SYSTEM_CLOCK_MODE_VLPR, &(scgConfig->clockModeConfig.vccrConfig));
                }

                if (status == STATUS_SUCCESS)
                {
                    status = CLOCK_SYS_SetSystemClockConfig(SCG_SYSTEM_CLOCK_MODE_HSRUN, &(scgConfig->clockModeConfig.hccrConfig));
                }
            }
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_SetPccConfiguration
 * Description   : This function configures the PCC block
 *
 * Implements CLOCK_SYS_SetPccConfiguration_Activity
 * END**************************************************************************/
void CLOCK_SYS_SetPccConfiguration(const pcc_config_t * peripheralClockConfig)
{
    DEV_ASSERT(peripheralClockConfig != NULL);

    uint32_t i;

    if ((peripheralClockConfig != NULL) && (peripheralClockConfig->peripheralClocks != NULL))
    {
        for (i = 0U; i < peripheralClockConfig->count; i++)
        {
            /* Disable the peripheral clock */
            PCC_SetClockMode(PCC, peripheralClockConfig->peripheralClocks[i].clockName, false);

            /* Set peripheral clock control */
            PCC_SetPeripheralClockControl(PCC,
                                          peripheralClockConfig->peripheralClocks[i].clockName,
                                          peripheralClockConfig->peripheralClocks[i].clkGate,
                                          (uint32_t)peripheralClockConfig->peripheralClocks[i].clkSrc,
                                          (uint32_t)peripheralClockConfig->peripheralClocks[i].divider,
                                          (uint32_t)peripheralClockConfig->peripheralClocks[i].frac);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_SetSimConfiguration
 * Description   : This function configures the SIM block
 *
 * Implements CLOCK_SYS_SetSimConfiguration_Activity
 * END**************************************************************************/
void CLOCK_SYS_SetSimConfiguration(const sim_clock_config_t * simClockConfig)
{
    DEV_ASSERT(simClockConfig != NULL);
    uint8_t i;

    /* ClockOut settings. */
    if (simClockConfig->clockOutConfig.initialize)
    {
        SIM_SetClockout(SIM, simClockConfig->clockOutConfig.enable, simClockConfig->clockOutConfig.source, simClockConfig->clockOutConfig.divider);
    }

    /* Low Power Clock settings from SIM. */
    if (simClockConfig->lpoClockConfig.initialize)
    {
        SIM_SetLpoClocks(SIM,
                         simClockConfig->lpoClockConfig.enableLpo1k,
                         simClockConfig->lpoClockConfig.enableLpo32k,
                         simClockConfig->lpoClockConfig.sourceLpoClk,
                         simClockConfig->lpoClockConfig.sourceRtcClk);
    }

    /* Platform Gate Clock settings. */
    if (simClockConfig->platGateConfig.initialize)
    {
        SIM_SetMscmClockGate(SIM, simClockConfig->platGateConfig.enableMscm);
        SIM_SetMpuClockGate(SIM, simClockConfig->platGateConfig.enableMpu);
        SIM_SetDmaClockGate(SIM, simClockConfig->platGateConfig.enableDma);
        SIM_SetErmClockGate(SIM, simClockConfig->platGateConfig.enableErm);
        SIM_SetEimClockGate(SIM, simClockConfig->platGateConfig.enableEim);
#if defined (QuadSPI_INSTANCE_COUNT)
        SIM_SetQspiIntRefClockGate(SIM, simClockConfig->qspiRefClkGating.enableQspiRefClk);
#endif
    }

    /* TCLK Clock settings. */
    if (simClockConfig->tclkConfig.initialize)
    {
        for (i = 0; i < NUMBER_OF_TCLK_INPUTS; i++)
        {
            if (i < NUMBER_OF_TCLK_INPUTS)
            {
                g_TClkFreq[i] = simClockConfig->tclkConfig.tclkFreq[i];
            }
        }
    }

    /* Debug trace Clock settings. */
    if (simClockConfig->traceClockConfig.initialize)
    {
        SIM_ClearTraceClockConfig(SIM);
        SIM_SetTraceClockSource(SIM, simClockConfig->traceClockConfig.source);
        SIM_SetTraceClockConfig(SIM,
                                simClockConfig->traceClockConfig.divEnable,
                                simClockConfig->traceClockConfig.divider,
                                simClockConfig->traceClockConfig.divFraction);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_SetPmcConfiguration
 * Description   : This function configures the PMC block
 *
 * Implements CLOCK_SYS_SetPmcConfiguration_Activity
 * END**************************************************************************/
void CLOCK_SYS_SetPmcConfiguration(const pmc_config_t * pmcConfig)
{
    DEV_ASSERT(pmcConfig != NULL);

    /* Low Power Clock settings from PMC. */
    if (pmcConfig->lpoClockConfig.initialize)
    {
        /* Enable/disable the low power oscillator. */
        PMC_SetLpoMode(PMC, pmcConfig->lpoClockConfig.enable);

        /* Write trimming value. */
        PMC_SetLpoTrimValue(PMC, pmcConfig->lpoClockConfig.trimValue);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_GetScgClockFreq
 * Description   : This function returns the frequency of a given clock from SCG
 *
 * END**************************************************************************/
static status_t CLOCK_SYS_GetScgClockFreq(clock_names_t clockName,
                                          uint32_t * frequency)
{
    status_t returnCode = STATUS_SUCCESS;
    uint32_t freq = 0U;

    switch (clockName)
    {
        /* Main clocks */
        case CORE_CLOCK:
            freq = CLOCK_SYS_GetSystemClockFreq(SCG_SYSTEM_CLOCK_CORE);
            break;
        case BUS_CLOCK:
            freq = CLOCK_SYS_GetSystemClockFreq(SCG_SYSTEM_CLOCK_BUS);
            break;
        case SLOW_CLOCK:
            freq = CLOCK_SYS_GetSystemClockFreq(SCG_SYSTEM_CLOCK_SLOW);
            break;
        case CLKOUT_CLOCK:
            freq = CLOCK_SYS_GetSimClkOutFreq();
            break;

        /* Other internal clocks used by peripherals. */
        case SIRC_CLOCK:
            freq = CLOCK_SYS_GetSircFreq();
            break;
        case FIRC_CLOCK:
            freq = CLOCK_SYS_GetFircFreq();
            break;
        case SOSC_CLOCK:
            freq = CLOCK_SYS_GetSysOscFreq();
            break;
        case SPLL_CLOCK:
            freq = CLOCK_SYS_GetSysPllFreq();
            break;
        case RTC_CLKIN_CLOCK:
            freq = g_RtcClkInFreq;
            break;
        case SCG_CLKOUT_CLOCK:
            freq = CLOCK_SYS_GetScgClkOutFreq();
            break;
        case SIRCDIV1_CLK:
            freq = CLOCK_SYS_GetSysAsyncFreq(SIRC_CLOCK, SCG_ASYNC_CLOCK_DIV1);
            break;
        case SIRCDIV2_CLK:
            freq = CLOCK_SYS_GetSysAsyncFreq(SIRC_CLOCK, SCG_ASYNC_CLOCK_DIV2);
            break;
        case FIRCDIV1_CLK:
            freq = CLOCK_SYS_GetSysAsyncFreq(FIRC_CLOCK, SCG_ASYNC_CLOCK_DIV1);
            break;
        case FIRCDIV2_CLK:
            freq = CLOCK_SYS_GetSysAsyncFreq(FIRC_CLOCK, SCG_ASYNC_CLOCK_DIV2);
            break;
        case SOSCDIV1_CLK:
            freq = CLOCK_SYS_GetSysAsyncFreq(SOSC_CLOCK, SCG_ASYNC_CLOCK_DIV1);
            break;
        case SOSCDIV2_CLK:
            freq = CLOCK_SYS_GetSysAsyncFreq(SOSC_CLOCK, SCG_ASYNC_CLOCK_DIV2);
            break;
        case SPLLDIV1_CLK:
            freq = CLOCK_SYS_GetSysAsyncFreq(SPLL_CLOCK, SCG_ASYNC_CLOCK_DIV1);
            break;
        case SPLLDIV2_CLK:
            freq = CLOCK_SYS_GetSysAsyncFreq(SPLL_CLOCK, SCG_ASYNC_CLOCK_DIV2);
            break;
        default:
            returnCode = STATUS_UNSUPPORTED;
            break;
    }

    if (frequency != NULL)
    {
        *frequency = freq;
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_GetSimClockFreq
 * Description   : This function returns the frequency of a given clock from SIM
 *
 * END**************************************************************************/
static status_t CLOCK_SYS_GetSimClockFreq(clock_names_t clockName,
                                          uint32_t * frequency)
{
    status_t returnCode = STATUS_SUCCESS;
    uint32_t clockPinSelect;
    uint32_t freq = 0U;

    switch (clockName)
    {
        /* SIM clocks */
        case SIM_FTM0_CLOCKSEL:
            clockPinSelect = SIM_GetFtm0ExternalClkPinMode(SIM);
            if (clockPinSelect < NUMBER_OF_TCLK_INPUTS)
            {
                freq = g_TClkFreq[clockPinSelect];
            }

            break;
        case SIM_FTM1_CLOCKSEL:
            clockPinSelect = SIM_GetFtm1ExternalClkPinMode(SIM);
            if (clockPinSelect < NUMBER_OF_TCLK_INPUTS)
            {
                freq = g_TClkFreq[clockPinSelect];
            }

            break;
        case SIM_FTM2_CLOCKSEL:
            clockPinSelect = SIM_GetFtm2ExternalClkPinMode(SIM);
            if (clockPinSelect < NUMBER_OF_TCLK_INPUTS)
            {
                freq = g_TClkFreq[clockPinSelect];
            }

            break;
        case SIM_FTM3_CLOCKSEL:
            clockPinSelect = SIM_GetFtm3ExternalClkPinMode(SIM);
            if (clockPinSelect < NUMBER_OF_TCLK_INPUTS)
            {
                freq = g_TClkFreq[clockPinSelect];
            }

            break;
#if FTM_INSTANCE_COUNT > 4U
        case SIM_FTM4_CLOCKSEL:
            clockPinSelect = SIM_GetFtm4ExternalClkPinMode(SIM);
            if (clockPinSelect < NUMBER_OF_TCLK_INPUTS)
            {
                freq = g_TClkFreq[clockPinSelect];
            }

            break;
#endif
#if FTM_INSTANCE_COUNT > 5U
        case SIM_FTM5_CLOCKSEL:
            clockPinSelect = SIM_GetFtm5ExternalClkPinMode(SIM);
            if (clockPinSelect < NUMBER_OF_TCLK_INPUTS)
            {
                freq = g_TClkFreq[clockPinSelect];
            }

            break;
#endif
#if FTM_INSTANCE_COUNT > 6U
        case SIM_FTM6_CLOCKSEL:
            clockPinSelect = SIM_GetFtm6ExternalClkPinMode(SIM);
            if (clockPinSelect < NUMBER_OF_TCLK_INPUTS)
            {
                freq = g_TClkFreq[clockPinSelect];
            }

            break;
#endif
#if FTM_INSTANCE_COUNT > 7U
        case SIM_FTM7_CLOCKSEL:
            clockPinSelect = SIM_GetFtm7ExternalClkPinMode(SIM);
            if (clockPinSelect < NUMBER_OF_TCLK_INPUTS)
            {
                freq = g_TClkFreq[clockPinSelect];
            }

            break;
#endif
        case SIM_CLKOUTSELL:
            freq = CLOCK_SYS_GetSimClkOutFreq();
            break;
        case SIM_RTCCLK_CLOCK:
            freq = CLOCK_SYS_GetSimRtcClkFreq();
            break;
        case SIM_LPO_CLOCK:
            if (PMC_GetLpoMode(PMC))
            {
                freq = CLOCK_SYS_GetLpoFreq();
            }

            break;
        case SIM_LPO_1K_CLOCK:
            if (PMC_GetLpoMode(PMC))
            {
                freq = SIM_GetLpo1KStatus(SIM) ? LPO_1K_FREQUENCY : 0UL;
            }

            break;
        case SIM_LPO_32K_CLOCK:
            if (PMC_GetLpoMode(PMC))
            {
                freq = SIM_GetLpo32KStatus(SIM) ? LPO_32K_FREQUENCY : 0UL;
            }

            break;
        case SIM_LPO_128K_CLOCK:
            if (PMC_GetLpoMode(PMC))
            {
                freq = LPO_128K_FREQUENCY;
            }

            break;
        case SIM_EIM_CLOCK:
            if (!SIM_GetEimClockGate(SIM))
            {
                /* EIM is not clocked. */
                returnCode = STATUS_MCU_GATED_OFF;
            }

            break;
        case SIM_ERM_CLOCK:
            if (!SIM_GetErmClockGate(SIM))
            {
                /* ERM is not clocked. */
                returnCode = STATUS_MCU_GATED_OFF;
            }

            break;
        case SIM_DMA_CLOCK:
            if (!SIM_GetDmaClockGate(SIM))
            {
                /* DMA is not clocked. */
                returnCode = STATUS_MCU_GATED_OFF;
            }

            break;
        case SIM_MPU_CLOCK:
            if (!SIM_GetMpuClockGate(SIM))
            {
                /* MPU is not clocked. */
                returnCode = STATUS_MCU_GATED_OFF;
            }

            break;
        case SIM_MSCM_CLOCK:
            if (!SIM_GetMscmClockGate(SIM))
            {
                /* MSCM is not clocked. */
                returnCode = STATUS_MCU_GATED_OFF;
            }

            break;
        default:
            returnCode = STATUS_UNSUPPORTED;
            break;
    }

    if (frequency != NULL)
    {
        *frequency = freq;
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_GetPccClockFreq
 * Description   : This function returns the clock frequency of peripheral functional clock.
 * END**************************************************************************/
static status_t CLOCK_SYS_GetPccClockFreq(clock_names_t clockName,
                                          uint32_t * frequency)
{
    status_t returnCode = STATUS_SUCCESS;
    uint32_t freq = 0U;

    /* Invalid PCC clock names. */
    if ((clockName <= SIM_END_OF_CLOCKS) ||
        (clockName == PCC_END_OF_BUS_CLOCKS) ||
        (clockName == PCC_END_OF_SYS_CLOCKS) ||
        (clockName == PCC_END_OF_SLOW_CLOCKS) ||
        (clockName == PCC_END_OF_ASYNCH_DIV1_CLOCKS) ||
        (clockName == PCC_END_OF_ASYNCH_DIV2_CLOCKS))
    {
        returnCode = STATUS_UNSUPPORTED;
    }
    else if (PCC_GetClockMode(PCC, clockName) == false)
    {
        /* Module is not clocked. */
        returnCode = STATUS_MCU_GATED_OFF;
    }
    else
    {
        if ((peripheralFeaturesList[clockName] & HAS_INT_CLOCK_FROM_BUS_CLOCK) != 0U)
        {
            /* Check whether BUS CLOCK is clocked. */
            returnCode = (status_t)((CLOCK_SYS_GetSystemClockFreq(SCG_SYSTEM_CLOCK_BUS) == 0U) ? STATUS_MCU_GATED_OFF : STATUS_SUCCESS);
        }
        else if ((peripheralFeaturesList[clockName] & HAS_INT_CLOCK_FROM_SYS_CLOCK) != 0U)
        {
            /* Check whether SYS CLOCK is clocked. */
            returnCode = (status_t)((CLOCK_SYS_GetSystemClockFreq(SCG_SYSTEM_CLOCK_CORE) == 0U) ? STATUS_MCU_GATED_OFF : STATUS_SUCCESS);
        }
        else if ((peripheralFeaturesList[clockName] & HAS_INT_CLOCK_FROM_SLOW_CLOCK) != 0U)
        {
            /* Check whether SLOW CLOCK is clocked. */
            returnCode = (status_t)((CLOCK_SYS_GetSystemClockFreq(SCG_SYSTEM_CLOCK_SLOW) == 0U) ? STATUS_MCU_GATED_OFF : STATUS_SUCCESS);
        }
        else
        {   /* It's an issue in peripheral features list, each peripheral must have one interface clock. */
            DEV_ASSERT(false);
        }

        if (returnCode == STATUS_SUCCESS)
        {
            /* Check whether peripheral has protocol clock (functional clock) */
            if ((peripheralFeaturesList[clockName] & (HAS_PROTOCOL_CLOCK_FROM_ASYNC1 | HAS_PROTOCOL_CLOCK_FROM_ASYNC2)) != 0U)
            {
                if ((peripheralFeaturesList[clockName] & HAS_PROTOCOL_CLOCK_FROM_ASYNC1) != 0U)
                {
                    /* Check whether the functional clock is clocked */
                    freq = CLOCK_SYS_GetPeripheralClock(clockName, SCG_ASYNC_CLOCK_DIV1);
                }

                if ((peripheralFeaturesList[clockName] & HAS_PROTOCOL_CLOCK_FROM_ASYNC2) != 0U)
                {
                    /* Check whether the functional clock is clocked */
                    freq = CLOCK_SYS_GetPeripheralClock(clockName, SCG_ASYNC_CLOCK_DIV2);
                }

                if (freq == 0U)
                {
                    returnCode = STATUS_MCU_GATED_OFF;
                }

            }
        }



    }

    /* If frequency reference is provided, write this value */
    if (frequency != NULL)
    {
        *frequency = freq;
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_GetFreq
 * Description   : This function returns the frequency of a given clock
 *
 * Implements CLOCK_SYS_GetFreq_Activity
 * END**************************************************************************/
status_t CLOCK_SYS_GetFreq(clock_names_t clockName,
                           uint32_t * frequency)
{
    status_t returnCode;

    /* Frequency of the clock name from SCG */
    if (clockName < SCG_END_OF_CLOCKS)
    {
        returnCode = CLOCK_SYS_GetScgClockFreq(clockName, frequency);
    }
    /* Frequency of the clock name from SIM */
    else if (clockName < SIM_END_OF_CLOCKS)
    {
        returnCode = CLOCK_SYS_GetSimClockFreq(clockName, frequency);
    }
    /* Frequency of the clock name from PCC */
    else if (clockName < PCC_END_OF_CLOCKS)
    {
        returnCode = CLOCK_SYS_GetPccClockFreq(clockName, frequency);
    }
    /* Invalid clock name */
    else
    {
        returnCode = STATUS_UNSUPPORTED;
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_GetPeripheralClock
 * Description   : Internal function used by CLOCK_SYS_GetFreq function
 * END**************************************************************************/

static uint32_t CLOCK_SYS_GetPeripheralClock(clock_names_t clockName,
                                             scg_async_clock_type_t divider)
{
    uint32_t frequency = 0;
    uint32_t fracValue = PCC_GetFracValueSel(PCC, clockName);
    uint32_t divValue = PCC_GetDividerSel(PCC, clockName);

    /* Check division factor */
    if (((uint32_t)fracValue) <= ((uint32_t)divValue))
    {
        /* Check clock gate */
        if (PCC_GetClockMode(PCC, clockName))
        {
            /* Check clock source */
            switch (PCC_GetClockSourceSel(PCC, clockName))
            {
                case (uint32_t)CLK_SRC_SOSC:
                    frequency = CLOCK_SYS_GetSysAsyncFreq(SOSC_CLOCK, divider);
                    break;
                case (uint32_t)CLK_SRC_SIRC:
                    frequency = CLOCK_SYS_GetSysAsyncFreq(SIRC_CLOCK, divider);
                    break;
                case (uint32_t)CLK_SRC_FIRC:
                    frequency = CLOCK_SYS_GetSysAsyncFreq(FIRC_CLOCK, divider);
                    break;
                case (uint32_t)CLK_SRC_SPLL:
                    frequency = CLOCK_SYS_GetSysAsyncFreq(SPLL_CLOCK, divider);
                    break;
                default:
                    frequency = 0;
                    break;
            }

            frequency = frequency / (divValue + 1U);
            frequency = frequency * (fracValue + 1U);
        }
    }

    return frequency;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_GetCurrentRunMode
 * Description   : Internal function used by CLOCK_SYS_SetScgConfiguration function
 * END**************************************************************************/
static scg_system_clock_mode_t CLOCK_SYS_GetCurrentRunMode(void)
{
    scg_system_clock_mode_t mode;

    /* Get the current running mode */
    switch (SMC_GetCurrentRunningMode(SMC))
    {
        /* High speed run mode */
        case HIGH_SPEED_RUNNING_MODE:
            mode = SCG_SYSTEM_CLOCK_MODE_HSRUN;
            break;
        /* Run mode */
        case RUN_SPEED_RUNNING_MODE:
            mode = SCG_SYSTEM_CLOCK_MODE_RUN;
            break;
        /* Very low power run mode */
        case VLPR_SPEED_RUNNING_MODE:
            mode = SCG_SYSTEM_CLOCK_MODE_VLPR;
            break;
        /* This should never happen - core has to be in some run mode to execute code */
        default:
            mode = SCG_SYSTEM_CLOCK_MODE_NONE;
            break;
    }

    return mode;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_TransitionSystemClock
 * Description   : Internal function used by CLOCK_SYS_ConfigureTemporarySystemClock and
 * CLOCK_SYS_ConfigureModulesFromScg functions
 * END**************************************************************************/
static status_t CLOCK_SYS_TransitionSystemClock(const scg_system_clock_config_t * to_clk)
{
    scg_system_clock_mode_t run_mode;
    status_t retValue = STATUS_SUCCESS;
    uint32_t timeout;

    /* Check destination clock */
    DEV_ASSERT(to_clk != NULL);
    DEV_ASSERT(to_clk->src != SCG_SYSTEM_CLOCK_SRC_NONE);

    /* Get & Convert Run mode from SMC to SCG defines*/
    run_mode = CLOCK_SYS_GetCurrentRunMode();

    /* Check the current mode */
    DEV_ASSERT(run_mode != SCG_SYSTEM_CLOCK_MODE_NONE);

    /* Update run mode configuration */
    retValue = CLOCK_SYS_SetSystemClockConfig(run_mode, to_clk);

    if (retValue == STATUS_SUCCESS)
    {
        /* Wait for system clock to transition. */
#ifdef ERRATA_E10777
        timeout = 10U;
#else
        timeout = 1U;
#endif

        do
        {
            timeout--;
        }
        while ((SCG_GetCurrentSystemClockSource(SCG) != ((uint32_t)to_clk->src)) && (timeout > 0U));

        if (timeout == 0U)
        {
            retValue = STATUS_TIMEOUT;
        }
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_GetSimClkOutFreq
 * Description   : Internal function used by CLOCK_SYS_GetFreq function
 * END**************************************************************************/
static uint32_t CLOCK_SYS_GetSimClkOutFreq(void)
{
    uint32_t frequency;

    if (SIM_GetClockoutStatus(SIM))
    {
        switch (SIM_GetClockoutSelectorValue(SIM))
        {
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_SCG_CLKOUT):
                frequency = CLOCK_SYS_GetScgClkOutFreq();
                break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_SOSC_DIV2_CLK):
                frequency = CLOCK_SYS_GetSysAsyncFreq(SOSC_CLOCK, SCG_ASYNC_CLOCK_DIV2);
                break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_SIRC_DIV2_CLK):
                frequency = CLOCK_SYS_GetSysAsyncFreq(SIRC_CLOCK, SCG_ASYNC_CLOCK_DIV2);
                break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_FIRC_DIV2_CLK):
                frequency = CLOCK_SYS_GetSysAsyncFreq(FIRC_CLOCK, SCG_ASYNC_CLOCK_DIV2);
                break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_SPLL_DIV2_CLK):
                frequency = CLOCK_SYS_GetSysAsyncFreq(SPLL_CLOCK, SCG_ASYNC_CLOCK_DIV2);
                break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_LPO_128K_CLK):
                frequency = LPO_128K_FREQUENCY;
                break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_LPO_CLK):
                frequency = CLOCK_SYS_GetLpoFreq();
                break;
            case ((uint32_t)SIM_CLKOUT_SEL_SYSTEM_RTC_CLK):
                frequency = CLOCK_SYS_GetSimRtcClkFreq();
                break;
            default:
                /* Invalid SIM CLKOUT selection.*/
                frequency = 0U;
                break;
        }

        /* Apply Divide Ratio */
        frequency /= (SIM_GetClockoutDividerValue(SIM) + 1U);
    }
    else
    {
        /* Output disabled. */
        frequency = 0U;
    }

    return frequency;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_GetScgClkOutFreq
 * Description   : Internal function used by CLOCK_SYS_GetFreq function
 * END**************************************************************************/
static uint32_t CLOCK_SYS_GetScgClkOutFreq(void)
{
    uint32_t frequency;

    switch (SCG_GetClockoutSourceSel(SCG))
    {
        case ((uint32_t)SCG_CLOCKOUT_SRC_SCG_SLOW):
            frequency = CLOCK_SYS_GetSystemClockFreq(SCG_SYSTEM_CLOCK_SLOW);
            break;
        case ((uint32_t)SCG_CLOCKOUT_SRC_SOSC):
            frequency = CLOCK_SYS_GetSysOscFreq();
            break;
        case ((uint32_t)SCG_CLOCKOUT_SRC_SIRC):
            frequency = CLOCK_SYS_GetSircFreq();
            break;
        case ((uint32_t)SCG_CLOCKOUT_SRC_FIRC):
            frequency = CLOCK_SYS_GetFircFreq();
            break;
        case ((uint32_t)SCG_CLOCKOUT_SRC_SPLL):
            frequency = CLOCK_SYS_GetSysPllFreq();
            break;
        default:
            /* Invalid SCG CLKOUT selection.*/
            frequency = 0U;
            break;
    }

    return frequency;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_GetSimRtcClkFreq
 * Description   : Internal function used by CLOCK_SYS_GetFreq function
 * END**************************************************************************/
static uint32_t CLOCK_SYS_GetSimRtcClkFreq()
{
    uint32_t frequency;

    /* Check RTCCLK Select */
    switch (SIM_GetRtcClkSrc(SIM))
    {
        case ((uint32_t)SIM_RTCCLK_SEL_SOSCDIV1_CLK):
            frequency = CLOCK_SYS_GetSysAsyncFreq(SOSC_CLOCK, SCG_ASYNC_CLOCK_DIV1);
            break;
        case ((uint32_t)SIM_RTCCLK_SEL_LPO_32K):
            frequency = SIM_GetLpo32KStatus(SIM) ? LPO_32K_FREQUENCY : 0UL;
            break;
        case ((uint32_t)SIM_RTCCLK_SEL_RTC_CLKIN):
            frequency = g_RtcClkInFreq;
            break;
        case ((uint32_t)SIM_RTCCLK_SEL_FIRCDIV1_CLK):
            frequency = CLOCK_SYS_GetSysAsyncFreq(FIRC_CLOCK, SCG_ASYNC_CLOCK_DIV1);
            break;
        default:
            /* Invalid RTCCLK selection.*/
            frequency = 0U;
            break;
    }

    return frequency;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ConfigureSIRC
 * Description   : Configures SIRC module based on provided configuration.
 * END**************************************************************************/
static status_t CLOCK_SYS_ConfigureSIRC(bool enable, const scg_sirc_config_t * sircConfig)
{
    status_t status = STATUS_SUCCESS;
    scg_sirc_config_t sircDefaultConfig;
    const scg_sirc_config_t * sircCfg;
    uint32_t timeout;

    if (sircConfig == NULL)
    {
        sircDefaultConfig.enableInStop      = false;
        sircDefaultConfig.enableInLowPower  = true;
        sircDefaultConfig.locked            = false;

        sircDefaultConfig.div1              = SCG_ASYNC_CLOCK_DIV_BY_1;
        sircDefaultConfig.div2              = SCG_ASYNC_CLOCK_DIV_BY_1;

        sircDefaultConfig.range             = SCG_SIRC_RANGE_HIGH;

        sircCfg = &sircDefaultConfig;
    }
    else
    {
        sircCfg = sircConfig;
    }

    /* If clock is used by system, return error. */
    if (SCG_GetSircSystemClockMode(SCG))
    {
        status = STATUS_BUSY;
    }
    /* Disable SIRC */
    else
    {
        /* Clear LK bit field */
        SCG_ClearSircLock(SCG);

        /* Disable monitor, disable clock and clear error. */
        SCG_ClearSircControl(SCG);
    }

    /* Configure SIRC. */
    if (enable  && (status == STATUS_SUCCESS))
    {
        /* Now start to set up SIRC clock. */
        /* Step 1. Setup dividers. */
        SCG_SetSircAsyncConfig(SCG, sircCfg->div1, sircCfg->div2);

        /* Step 2. Set SIRC configuration: frequency range. */
        SCG_SetSircConfiguration(SCG, sircCfg->range);

        /* Step 3. Set SIRC control: enable clock, configure source in STOP and VLP modes, configure lock feature. */
        SCG_SetSircControl(SCG, sircCfg->enableInStop, sircCfg->enableInLowPower, sircCfg->locked);

        /* Wait for SIRC to initialize */
        timeout = SIRC_STABILIZATION_TIMEOUT;
        while ((CLOCK_SYS_GetSircFreq() == 0U) && (timeout > 0U))
        {
            timeout--;
        }

        if (timeout == 0U)
        {
            status = STATUS_TIMEOUT;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ConfigureFIRC
 * Description   : Configures FIRC module based on provided configuration.
 * END**************************************************************************/
static status_t CLOCK_SYS_ConfigureFIRC(bool enable, const scg_firc_config_t * fircConfig)
{
    status_t status = STATUS_SUCCESS;
    scg_firc_config_t fircDefaultConfig;
    const scg_firc_config_t * fircCfg;
    uint32_t timeout;

    if (fircConfig == NULL)
    {
        fircDefaultConfig.enableInStop      = false;
        fircDefaultConfig.enableInLowPower  = false;
        fircDefaultConfig.regulator         = true;
        fircDefaultConfig.locked            = false;

        fircDefaultConfig.div1              = SCG_ASYNC_CLOCK_DIV_BY_1;
        fircDefaultConfig.div2              = SCG_ASYNC_CLOCK_DIV_BY_1;

        fircDefaultConfig.range             = SCG_FIRC_RANGE_48M;

        fircCfg = &fircDefaultConfig;
    }
    else
    {
        fircCfg = fircConfig;
    }

    /* If clock is used by system, return error. */
    if (SCG_GetFircSystemClockMode(SCG))
    {
        status = STATUS_BUSY;
    }
    /* Disable FIRC */
    else
    {
        /* Clear LK bit field */
        SCG_ClearFircLock(SCG);

        /* Disable monitor, disable clock and clear error. */
        SCG_ClearFircControl(SCG);
    }

    /* Configure FIRC. */
    if (enable && (status == STATUS_SUCCESS))
    {
        /* Now start to set up FIRC clock. */
        /* Step 1. Setup dividers. */
        SCG_SetFircAsyncConfig(SCG, fircCfg->div1, fircCfg->div2);

        /* Step 2. Set FIRC configuration. */
        SCG_SetFircConfiguration(SCG, fircCfg->range);

        /* Step 3. Enable clock, config regulator and locking feature. */
        SCG_SetFircControl(SCG, fircCfg->regulator, fircCfg->locked);

        /* Wait for FIRC to initialize */
        timeout = FIRC_STABILIZATION_TIMEOUT;
        while ((CLOCK_SYS_GetFircFreq() == 0U) && (timeout > 0U))
        {
            timeout--;
        }

        if (timeout == 0U)
        {
            status = STATUS_TIMEOUT;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ConfigureSOSC
 * Description   : Configures SOSC module based on provided configuration.
 * END**************************************************************************/
static status_t CLOCK_SYS_ConfigureSOSC(bool enable, const scg_sosc_config_t * soscConfig)
{
    status_t status = STATUS_SUCCESS;
    scg_sosc_config_t soscDefaultConfig;
    const scg_sosc_config_t * soscCfg;
    uint32_t timeout;

    if (soscConfig == NULL)
    {
        soscDefaultConfig.enableInStop      = false;
        soscDefaultConfig.enableInLowPower  = false;

        soscDefaultConfig.monitorMode       = SCG_SOSC_MONITOR_DISABLE;
        soscDefaultConfig.locked            = false;

        soscDefaultConfig.div1              = SCG_ASYNC_CLOCK_DIV_BY_1;
        soscDefaultConfig.div2              = SCG_ASYNC_CLOCK_DIV_BY_1;

        soscDefaultConfig.extRef            = SCG_SOSC_REF_EXT;
        soscDefaultConfig.gain              = SCG_SOSC_GAIN_LOW;
        soscDefaultConfig.range             = SCG_SOSC_RANGE_LOW;

        soscCfg = &soscDefaultConfig;
    }
    else
    {
        soscCfg = soscConfig;
    }

    /* If clock is used by system, return error. */
    if (SCG_GetSoscSystemClockMode(SCG))
    {
        status = STATUS_BUSY;
    }
    /* Disable SOSC */
    else
    {
        /* Clear LK bit field */
        SCG_ClearSoscLock(SCG);

        /* Disable monitor, disable clock and clear error. */
        SCG_ClearSoscControl(SCG);

        g_xtal0ClkFreq = 0U;
    }

    /* Configure SOSC. */
    if (enable && (status == STATUS_SUCCESS))
    {
        /* Now start to set up OSC clock. */
        /* Step 1. Setup dividers. */
        SCG_SetSoscAsyncConfig(SCG, soscCfg->div1, soscCfg->div2);

        /* Step 2. Set OSC configuration. */
        SCG_SetSoscConfiguration(SCG, soscCfg->range, soscCfg->gain, soscCfg->extRef);

        /* Step 3. Enable clock, configure monitor, lock register. */
        switch (soscCfg->monitorMode)
        {
            case SCG_SOSC_MONITOR_DISABLE:
            {
                SCG_SetSoscControl(SCG, false, false, soscCfg->locked);
            }
            break;
            case SCG_SOSC_MONITOR_INT:
            {
                SCG_SetSoscControl(SCG, true, false, soscCfg->locked);
            }
            break;
            case SCG_SOSC_MONITOR_RESET:
            {
                SCG_SetSoscControl(SCG, true, true, soscCfg->locked);
            }
            break;
            default:
                /* Invalid monitor mode */
                DEV_ASSERT(false);
                break;
        }

        g_xtal0ClkFreq = soscCfg->freq;

        /* Wait for System OSC to initialize */
        timeout = SOSC_STABILIZATION_TIMEOUT;
        while ((CLOCK_SYS_GetSysOscFreq() == 0U) && (timeout > 0U))
        {
            timeout--;
        }

        if (timeout == 0U)
        {
            status = STATUS_TIMEOUT;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ConfigureSPLL
 * Description   : Configures SPLL module based on provided configuration.
 * END**************************************************************************/
static status_t CLOCK_SYS_ConfigureSPLL(bool enable, const scg_spll_config_t * spllConfig)
{
    status_t status = STATUS_SUCCESS;
    scg_spll_config_t spllDefaultConfig;
    const scg_spll_config_t * spllCfg;
    uint32_t srcFreq, timeout;

    if (spllConfig == NULL)
    {
        spllDefaultConfig.enableInStop   = false;
        spllDefaultConfig.monitorMode    = SCG_SPLL_MONITOR_DISABLE;
        spllDefaultConfig.locked         = false;

        spllDefaultConfig.div1           = SCG_ASYNC_CLOCK_DIV_BY_1;
        spllDefaultConfig.div2           = SCG_ASYNC_CLOCK_DIV_BY_1;

        spllDefaultConfig.prediv         = 0;
        spllDefaultConfig.mult           = 0;
        spllDefaultConfig.src            = 0;

        spllCfg = &spllDefaultConfig;
    }
    else
    {
        spllCfg = spllConfig;
    }

    /* If clock is used by system, return error. */
    if (SCG_GetSpllSystemClockMode(SCG))
    {
        status = STATUS_BUSY;
    }
    /* Disable the SPLL. */
    else
    {
        /* Clear LK bit field */
        SCG_ClearSpllLock(SCG);

        /* Disable monitor, disable clock and clear error. */
        SCG_ClearSpllControl(SCG);
    }

    /* Configure SPLL. */
    if (enable && (status == STATUS_SUCCESS))
    {
        /* Get clock source frequency. */
        srcFreq = CLOCK_SYS_GetSysOscFreq();
        DEV_ASSERT(srcFreq != 0U);

        /* Pre-divider checking. */
        srcFreq /= (((uint32_t)spllCfg->prediv) + SCG_SPLL_PREDIV_BASE);
        DEV_ASSERT((srcFreq >= SCG_SPLL_REF_MIN) && (srcFreq <= SCG_SPLL_REF_MAX));

        /* Now start to set up PLL clock. */
        SCG_SetSpllAsyncConfig(SCG, spllCfg->div1, spllCfg->div2);

        /* Step 2. Set PLL configuration. */
        SCG_SetSpllConfiguration(SCG, spllCfg->prediv, spllCfg->mult);

        /* Step 3. Enable clock, configure monitor, lock register. */
        switch (spllCfg->monitorMode)
        {
            case SCG_SPLL_MONITOR_DISABLE:
            {
                SCG_SetSpllControl(SCG, false, false, spllCfg->locked);
            }
            break;
            case SCG_SPLL_MONITOR_INT:
            {
                SCG_SetSpllControl(SCG, true, false, spllCfg->locked);
            }
            break;
            case SCG_SPLL_MONITOR_RESET:
            {
                SCG_SetSpllControl(SCG, true, true, spllCfg->locked);
            }
            break;
            default:
                /* Invalid monitor mode */
                DEV_ASSERT(false);
                break;
        }

        /* Wait for System PLL to initialize */
        timeout = SPLL_STABILIZATION_TIMEOUT;
        while ((CLOCK_SYS_GetSysPllFreq() == 0U) && (timeout > 0U))
        {
            timeout--;
        }

        if (timeout == 0U)
        {
            status = STATUS_TIMEOUT;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ConfigureTemporarySystemClock
 * Description   : Configures and transitions to a temporary system clock source: FIRC
 * END**************************************************************************/
static status_t CLOCK_SYS_ConfigureTemporarySystemClock(void)
{
    status_t status = STATUS_SUCCESS;
    scg_system_clock_config_t sysClockConfig;
    static const scg_system_clock_div_t tmpSysClk[TMP_SYS_CLK_NO][TMP_SYS_DIV_NO] = TMP_SYSTEM_CLOCK_CONFIGS;

    /* If the current system clock source is not FIRC:
     * 1. Enable FIRC (if it's not enabled)
     * 2. Switch to FIRC.
     */
    if (SCG_GetCurrentSystemClockSource(SCG) != ((uint32_t)SCG_SYSTEM_CLOCK_SRC_FIRC))
    {
        /* If FIRC is not on, then FIRC is configured
         * with the default configuration */
        if (CLOCK_SYS_GetFircFreq() == 0UL)
        {
            status = CLOCK_SYS_ConfigureFIRC(true, NULL);
        }

        /* FIRC is enabled, transition the system clock source to FIRC. */
        if (status == STATUS_SUCCESS)
        {
            sysClockConfig.src     = SCG_SYSTEM_CLOCK_SRC_FIRC;
            sysClockConfig.divCore = tmpSysClk[TMP_FIRC_CLK][TMP_SYS_DIV];
            sysClockConfig.divBus  = tmpSysClk[TMP_FIRC_CLK][TMP_BUS_DIV];
            sysClockConfig.divSlow = tmpSysClk[TMP_FIRC_CLK][TMP_SLOW_DIV];
            status = CLOCK_SYS_TransitionSystemClock(&sysClockConfig);
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ConfigureModulesFromScg
 * Description   : Configures all modules from SCG (SIRC, FIRC, SOSC and SPLL)
 * END**************************************************************************/
static status_t CLOCK_SYS_ConfigureModulesFromScg(const scg_config_t * scgConfig)
{
    status_t status = STATUS_SUCCESS;
    scg_system_clock_config_t sysClockConfig;
    const scg_system_clock_config_t * nextSysClockConfig;
    static const scg_system_clock_div_t tmpSysClk[TMP_SYS_CLK_NO][TMP_SYS_DIV_NO] = TMP_SYSTEM_CLOCK_CONFIGS;

    /* Configure all clock sources that are different from the
     * current system clock source FIRC (SIRC, SOSC, SPLL). */
    status = CLOCK_SYS_ConfigureSIRC(scgConfig->sircConfig.initialize, &scgConfig->sircConfig);
    if (status == STATUS_SUCCESS)
    {
        status = CLOCK_SYS_ConfigureSOSC(scgConfig->soscConfig.initialize, &scgConfig->soscConfig);
        if (status == STATUS_SUCCESS)
        {
            status = CLOCK_SYS_ConfigureSPLL(scgConfig->spllConfig.initialize,&scgConfig->spllConfig);
        }
    }

    /* Get the next system clock source */
    switch (CLOCK_SYS_GetCurrentRunMode())
    {
        case SCG_SYSTEM_CLOCK_MODE_RUN:
        {
            nextSysClockConfig = &scgConfig->clockModeConfig.rccrConfig;
        }
        break;
        case SCG_SYSTEM_CLOCK_MODE_VLPR:
        {
            nextSysClockConfig = &scgConfig->clockModeConfig.vccrConfig;
        }
        break;
        case SCG_SYSTEM_CLOCK_MODE_HSRUN:
        {
            nextSysClockConfig = &scgConfig->clockModeConfig.hccrConfig;
        }
        break;
        default:
            DEV_ASSERT(false);
            nextSysClockConfig = NULL;
            break;
    }

    if (status == STATUS_SUCCESS)
    {
        /* The current system clock source is FIRC.
         * Verify whether the next system clock source is FIRC. */
        if (nextSysClockConfig->src == SCG_SYSTEM_CLOCK_SRC_FIRC)
        {
            /* If they are the same, search for a temporary system clock source
             * (use one of the following sources: SPLL, SOSC, SIRC)
             * Assume that a temporary clock is not found status = ERROR. */
            status = STATUS_ERROR;

            /* SPLL is enabled */
            if (scgConfig->spllConfig.initialize /* && (status == STATUS_ERROR) */)
            {
                sysClockConfig.src     = SCG_SYSTEM_CLOCK_SRC_SYS_PLL;
                sysClockConfig.divCore = tmpSysClk[TMP_SPLL_CLK][TMP_SYS_DIV];
                sysClockConfig.divBus  = tmpSysClk[TMP_SPLL_CLK][TMP_BUS_DIV];
                sysClockConfig.divSlow = tmpSysClk[TMP_SPLL_CLK][TMP_SLOW_DIV];
                status = CLOCK_SYS_TransitionSystemClock(&sysClockConfig);
            }

            /* SOSC is enabled and SPLL configuration for system clock source is not valid */
            if (scgConfig->soscConfig.initialize && (status == STATUS_ERROR))
            {
                sysClockConfig.src     = SCG_SYSTEM_CLOCK_SRC_SYS_OSC;
                sysClockConfig.divCore = tmpSysClk[TMP_SOSC_CLK][TMP_SYS_DIV];
                sysClockConfig.divBus  = tmpSysClk[TMP_SOSC_CLK][TMP_BUS_DIV];
                sysClockConfig.divSlow = tmpSysClk[TMP_SOSC_CLK][TMP_SLOW_DIV];
                status = CLOCK_SYS_TransitionSystemClock(&sysClockConfig);
            }

            /* SIRC is enabled and SOSC configuration for system clock source is not valid */
            if (scgConfig->sircConfig.initialize && (status == STATUS_ERROR))
            {
                sysClockConfig.src     = SCG_SYSTEM_CLOCK_SRC_SIRC;
                sysClockConfig.divCore = tmpSysClk[TMP_SIRC_CLK][TMP_SYS_DIV];
                sysClockConfig.divBus  = tmpSysClk[TMP_SIRC_CLK][TMP_BUS_DIV];
                sysClockConfig.divSlow = tmpSysClk[TMP_SIRC_CLK][TMP_SLOW_DIV];
                status = CLOCK_SYS_TransitionSystemClock(&sysClockConfig);
            }

            /* Transitioned to a temporary system clock source. */
            if (status == STATUS_SUCCESS)
            {
                /* Configure the remaining clock source (FIRC). */
                status = CLOCK_SYS_ConfigureFIRC(scgConfig->fircConfig.initialize, &scgConfig->fircConfig);

                if (status == STATUS_SUCCESS)
                {
                    /* Transition to the next system clock source. */
                    sysClockConfig.src     = nextSysClockConfig->src;
                    sysClockConfig.divCore = nextSysClockConfig->divCore;
                    sysClockConfig.divBus  = nextSysClockConfig->divBus;
                    sysClockConfig.divSlow = nextSysClockConfig->divSlow;
                    status = CLOCK_SYS_TransitionSystemClock(&sysClockConfig);
                }
            }
        }
        else
        {   /* Transition to the next system clock source. */
            sysClockConfig.src     = nextSysClockConfig->src;
            sysClockConfig.divCore = nextSysClockConfig->divCore;
            sysClockConfig.divBus  = nextSysClockConfig->divBus;
            sysClockConfig.divSlow = nextSysClockConfig->divSlow;
            status = CLOCK_SYS_TransitionSystemClock(&sysClockConfig);

            if (status == STATUS_SUCCESS)
            {
                /* Configure the remaining clock source (FIRC) */
                status = CLOCK_SYS_ConfigureFIRC(scgConfig->fircConfig.initialize, &scgConfig->fircConfig);
            }
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_GetSystemClockFreq
 * Description   : This function gets the SCG system clock frequency, these
 * clocks are used for core, platform, external and bus clock domains.
 * END**************************************************************************/
static uint32_t CLOCK_SYS_GetSystemClockFreq(scg_system_clock_type_t type)
{
    uint32_t freq;


    DEV_ASSERT(type < SCG_SYSTEM_CLOCK_MAX);

    switch (SCG_GetCurrentSystemClockSource(SCG))
    {
        case ((uint32_t)SCG_SYSTEM_CLOCK_SRC_SYS_OSC):
            freq = CLOCK_SYS_GetSysOscFreq();
            break;
        case ((uint32_t)SCG_SYSTEM_CLOCK_SRC_SIRC):
            freq = CLOCK_SYS_GetSircFreq();
            break;
        case ((uint32_t)SCG_SYSTEM_CLOCK_SRC_FIRC):
            freq = CLOCK_SYS_GetFircFreq();
            break;
        case ((uint32_t)SCG_SYSTEM_CLOCK_SRC_SYS_PLL):
            freq = CLOCK_SYS_GetSysPllFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    freq /= (SCG_GetCurrentCoreClockDividerRatio(SCG) + 1U);

    switch (type)
    {
        case SCG_SYSTEM_CLOCK_CORE:
            /* Intentionally left blank */
            break;
        case SCG_SYSTEM_CLOCK_BUS:
            freq /= (SCG_GetCurrentBusClockDividerRatio(SCG) + 1U);
            break;
        case SCG_SYSTEM_CLOCK_SLOW:
            freq /= (SCG_GetCurrentSlowClockDividerRatio(SCG) + 1U);
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_SetSystemClockConfig
 * Description   : This function sets the system configuration for the specified mode.
 * END**************************************************************************/
static status_t CLOCK_SYS_SetSystemClockConfig(scg_system_clock_mode_t mode,
                                               scg_system_clock_config_t const * config)
{
    status_t status = STATUS_SUCCESS;
    uint32_t srcFreq = 0U;

    /* The maximum clock frequencies in all power modes */
    static const uint32_t maxClocksFreq[MAX_FREQ_MODES_NO][MAX_FREQ_CLK_NO] = CLOCK_MAX_FREQUENCIES;
    uint32_t maxFreqRunMode = 0U;
    const uint32_t sysFreqMul = ((uint32_t)config->divCore) + 1UL;
    const uint32_t busFreqMul = (((uint32_t)config->divCore) + 1UL) * (((uint32_t)config->divBus) + 1UL);
    const uint32_t slowFreqMul = (((uint32_t)config->divCore) + 1UL) * (((uint32_t)config->divSlow) + 1UL);


    DEV_ASSERT(mode != SCG_SYSTEM_CLOCK_MODE_CURRENT);

    switch (config->src)
    {
        case SCG_SYSTEM_CLOCK_SRC_SYS_OSC:
            srcFreq = CLOCK_SYS_GetSysOscFreq();
            break;
        case SCG_SYSTEM_CLOCK_SRC_SIRC:
            srcFreq = CLOCK_SYS_GetSircFreq();
            break;
        case SCG_SYSTEM_CLOCK_SRC_FIRC:
            srcFreq = CLOCK_SYS_GetFircFreq();
            break;
        case SCG_SYSTEM_CLOCK_SRC_SYS_PLL:
            srcFreq = CLOCK_SYS_GetSysPllFreq();
            break;
        default:
            srcFreq = 0U;
            break;
    }

    switch (mode)
    {
        case SCG_SYSTEM_CLOCK_MODE_RUN:       /*!< Run mode.                */
            maxFreqRunMode = MAX_FREQ_RUN;
            break;
        case SCG_SYSTEM_CLOCK_MODE_VLPR:      /*!< Very Low Power Run mode. */
            DEV_ASSERT((SCG_SYSTEM_CLOCK_SRC_SYS_OSC == config->src) || (SCG_SYSTEM_CLOCK_SRC_SIRC    == config->src));
            maxFreqRunMode = MAX_FREQ_VLPR;
            break;
        case SCG_SYSTEM_CLOCK_MODE_HSRUN:     /*!< High Speed Run mode.     */
            maxFreqRunMode = MAX_FREQ_HSRUN;
            break;
        default:
            /* Invalid mode */
            DEV_ASSERT(false);
            break;
    }

    /* Verify the frequencies of sys, bus and slow clocks. */
    if ((srcFreq > (sysFreqMul * maxClocksFreq[maxFreqRunMode][MAX_FREQ_SYS_CLK])) ||    /* Sys(core) clock */
        (srcFreq > (busFreqMul * maxClocksFreq[maxFreqRunMode][MAX_FREQ_BUS_CLK])) ||    /* Bus clock */
        (srcFreq > (slowFreqMul * maxClocksFreq[maxFreqRunMode][MAX_FREQ_SLOW_CLK])))     /* Slow clock */
    {
        /* Configuration for the next system clock source is not valid. */
        status = STATUS_ERROR;
    }

    if (status == STATUS_SUCCESS)
    {
        switch (mode)
        {
            case SCG_SYSTEM_CLOCK_MODE_RUN:       /*!< Run mode.                */
                SCG_SetRunClockControl(SCG, (uint32_t)config->src, (uint32_t)config->divCore, (uint32_t)config->divBus, (uint32_t)config->divSlow);
                break;
            case SCG_SYSTEM_CLOCK_MODE_VLPR:      /*!< Very Low Power Run mode. */
                DEV_ASSERT((SCG_SYSTEM_CLOCK_SRC_SYS_OSC == config->src) || (SCG_SYSTEM_CLOCK_SRC_SIRC    == config->src));
                SCG_SetVlprClockControl(SCG, (uint32_t)config->src, (uint32_t)config->divCore, (uint32_t)config->divBus, (uint32_t)config->divSlow);
                break;
            case SCG_SYSTEM_CLOCK_MODE_HSRUN:     /*!< High Speed Run mode.     */
                SCG_SetHsrunClockControl(SCG, (uint32_t)config->src, (uint32_t)config->divCore, (uint32_t)config->divBus, (uint32_t)config->divSlow);
                break;
            default:
                /* Invalid mode */
                DEV_ASSERT(false);
                break;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_GetSysAsyncFreq
 * Description   : Gets SCG asynchronous clock frequency from a clock source.
 * END**************************************************************************/
static uint32_t CLOCK_SYS_GetSysAsyncFreq(clock_names_t clockSource,
                                          scg_async_clock_type_t type)
{
    uint32_t freq, div = 0U;

    switch (type)
    {
        case SCG_ASYNC_CLOCK_DIV1:
        {
            switch (clockSource)
            {
                case FIRC_CLOCK:
                {
                    freq = CLOCK_SYS_GetFircFreq();
                    div = SCG_GetFircFirstAsyncDivider(SCG);
                }
                break;
                case SIRC_CLOCK:
                {
                    freq = CLOCK_SYS_GetSircFreq();
                    div = SCG_GetSircFirstAsyncDivider(SCG);
                }
                break;
                case SOSC_CLOCK:
                {
                    freq = CLOCK_SYS_GetSysOscFreq();
                    div = SCG_GetSoscFirstAsyncDivider(SCG);
                }
                break;
                case SPLL_CLOCK:
                {
                    freq = CLOCK_SYS_GetSysPllFreq();
                    div = SCG_GetSpllFirstAsyncDivider(SCG);
                }
                break;
                default:
                {
                    /* Invalid clock source type */
                    freq = 0U;
                    DEV_ASSERT(false);
                }
                break;
            }
        }
        break;
        case SCG_ASYNC_CLOCK_DIV2:
        {
            switch (clockSource)
            {
                case FIRC_CLOCK:
                {
                    freq = CLOCK_SYS_GetFircFreq();
                    div = SCG_GetFircSecondAsyncDivider(SCG);
                }
                break;
                case SIRC_CLOCK:
                {
                    freq = CLOCK_SYS_GetSircFreq();
                    div = SCG_GetSircSecondAsyncDivider(SCG);
                }
                break;
                case SOSC_CLOCK:
                {
                    freq = CLOCK_SYS_GetSysOscFreq();
                    div = SCG_GetSoscSecondAsyncDivider(SCG);
                }
                break;
                case SPLL_CLOCK:
                {
                    freq = CLOCK_SYS_GetSysPllFreq();
                    div = SCG_GetSpllSecondAsyncDivider(SCG);
                }
                break;
                default:
                {
                    /* Invalid clock source type */
                    freq = 0U;
                    DEV_ASSERT(false);
                }
                break;
            }
        }
        break;
        default:
            /* Invalid async clock source */
            freq = 0U;
            DEV_ASSERT(false);
            break;
    }


    if (div != 0U)
    {
        freq = (freq >> (div - 1U));
    }
    else  /* Output disabled. */
    {
        freq = 0U;
    }

    return freq;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_GetSysOscFreq
 * Description   : Gets SCG System OSC clock frequency (SYSOSC).
 * END**************************************************************************/
static uint32_t CLOCK_SYS_GetSysOscFreq(void)
{
    uint32_t retValue;
    if (SCG_GetSoscStatus(SCG)) /* System OSC clock is valid. */
    {
        retValue = g_xtal0ClkFreq;
    }
    else
    {
        retValue = 0U;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_GetSircFreq
 * Description   : Gets SCG Slow IRC clock frequency (SIRC).
 * END**************************************************************************/
static uint32_t CLOCK_SYS_GetSircFreq(void)
{
    uint32_t retValue;

    if (SCG_GetSircStatus(SCG)) /* SIRC is valid. */
    {
        retValue =  (SCG_GetSircRange(SCG) != 0U) ? FEATURE_SCG_SIRC_HIGH_RANGE_FREQ : FEATURE_SCG_SIRC_LOW_RANGE_FREQ;
    }
    else
    {
        retValue = 0U;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_GetFircFreq
 * Description   : Gets SCG Fast IRC clock frequency (FIRC).
 * END**************************************************************************/
static uint32_t CLOCK_SYS_GetFircFreq(void)
{
    uint32_t retValue;

    static const uint32_t fircFreq[] = {
        FEATURE_SCG_FIRC_FREQ0, FEATURE_SCG_FIRC_FREQ1, FEATURE_SCG_FIRC_FREQ2, FEATURE_SCG_FIRC_FREQ3,
    };

    if (SCG_GetFircStatus(SCG)) /* FIRC is valid. */
    {
        retValue = fircFreq[SCG_GetFircRange(SCG)];
    }
    else
    {
        retValue = 0U;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_GetSysPllFreq
 * Description   : Gets SCG System PLL clock frequency (SYSPLL).
 * END**************************************************************************/
static uint32_t CLOCK_SYS_GetSysPllFreq(void)
{
    uint32_t freq, retValue;

    if (SCG_GetSpllStatus(SCG)) /* System PLL is valid. */
    {
        /* Get System OSC. frequency. */
        freq = CLOCK_SYS_GetSysOscFreq();

        if (freq != 0U) /* If source is valid. */
        {
            freq /= (SCG_GetSpllPredivider(SCG) + SCG_SPLL_PREDIV_BASE);    /* Pre-divider. */
            freq *= (SCG_GetSpllMultiplier(SCG) + SCG_SPLL_MULT_BASE);      /* Multiplier. */
            freq = freq >> 1U;  /* Divide VCO by 2. */
        }

        retValue = freq;
    }
    else
    {
        retValue = 0U;
    }

    return retValue;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_GetLpoFreq
 * Description   : Gets SIM LPO clock frequency (LPO).
 * END**************************************************************************/
static uint32_t CLOCK_SYS_GetLpoFreq(void)
{
    uint32_t freq = 0U;

    switch (SIM_GetLpoClkSelectorValue(SIM))
    {
        case 0U:  /* SIM_LPO_CLK_SEL_LPO_128K */
            freq = LPO_128K_FREQUENCY;
            break;
        case 1U:  /* SIM_LPO_CLK_SEL_NO_CLOCK: */
            freq = 0U;
            break;
        case 2U:  /* SIM_LPO_CLK_SEL_LPO_32K: */
            freq = SIM_GetLpo32KStatus(SIM) ? LPO_32K_FREQUENCY : 0UL;
            break;
        case 3U:  /* SIM_LPO_CLK_SEL_LPO_1K:  */
            freq = SIM_GetLpo32KStatus(SIM) ? LPO_32K_FREQUENCY : 0UL;
            break;
        default:
            /* Invalid LPOCLKSEL selection.*/
            DEV_ASSERT(false);
            break;
    }

    return freq;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_TransitionToTmpSysClk
 * Description   : Transition to a temporary system clock
 * END**************************************************************************/
static status_t CLOCK_SYS_TransitionToTmpSysClk(scg_system_clock_src_t currentSysClkSrc)
{
    scg_system_clock_config_t sysClockConfig;

    static const scg_system_clock_div_t tmpSysClk[TMP_SYS_CLK_NO][TMP_SYS_DIV_NO] = TMP_SYSTEM_CLOCK_CONFIGS;

    status_t status = STATUS_ERROR;

    /* SPLL is functional, and is not the current system clock source */
    if ((CLOCK_SYS_GetSysPllFreq() != 0U) && (currentSysClkSrc != SCG_SYSTEM_CLOCK_SRC_SYS_PLL) /* && (status == STATUS_ERROR) */)
    {
        sysClockConfig.src     = SCG_SYSTEM_CLOCK_SRC_SYS_PLL;
        sysClockConfig.divCore = tmpSysClk[TMP_SPLL_CLK][TMP_SYS_DIV];
        sysClockConfig.divBus  = tmpSysClk[TMP_SPLL_CLK][TMP_BUS_DIV];
        sysClockConfig.divSlow = tmpSysClk[TMP_SPLL_CLK][TMP_SLOW_DIV];
        status = CLOCK_SYS_TransitionSystemClock(&sysClockConfig);
    }

    /* FIRC is functional, it is not the current system clock source, no valid source has been found yet. */
    if ((CLOCK_SYS_GetFircFreq() != 0U) && (currentSysClkSrc != SCG_SYSTEM_CLOCK_SRC_FIRC) && (status != STATUS_SUCCESS))
    {
        sysClockConfig.src     = SCG_SYSTEM_CLOCK_SRC_FIRC;
        sysClockConfig.divCore = tmpSysClk[TMP_FIRC_CLK][TMP_SYS_DIV];
        sysClockConfig.divBus  = tmpSysClk[TMP_FIRC_CLK][TMP_BUS_DIV];
        sysClockConfig.divSlow = tmpSysClk[TMP_FIRC_CLK][TMP_SLOW_DIV];
        status = CLOCK_SYS_TransitionSystemClock(&sysClockConfig);
    }


    /* SOSC is functional, it is not the current system clock source, no valid source has been found yet. */
    if ((CLOCK_SYS_GetSysOscFreq() != 0U) && (currentSysClkSrc != SCG_SYSTEM_CLOCK_SRC_SYS_OSC) && (status != STATUS_SUCCESS))
    {
        sysClockConfig.src     = SCG_SYSTEM_CLOCK_SRC_SYS_OSC;
        sysClockConfig.divCore = tmpSysClk[TMP_SOSC_CLK][TMP_SYS_DIV];
        sysClockConfig.divBus  = tmpSysClk[TMP_SOSC_CLK][TMP_BUS_DIV];
        sysClockConfig.divSlow = tmpSysClk[TMP_SOSC_CLK][TMP_SLOW_DIV];
        status = CLOCK_SYS_TransitionSystemClock(&sysClockConfig);
    }

    /* SIRC is functional, it is not the current system clock source, no valid source has been found yet. */
    if ((CLOCK_SYS_GetSircFreq() != 0U) && (currentSysClkSrc != SCG_SYSTEM_CLOCK_SRC_SIRC) && (status != STATUS_SUCCESS))
    {
        sysClockConfig.src     = SCG_SYSTEM_CLOCK_SRC_SIRC;
        sysClockConfig.divCore = tmpSysClk[TMP_SIRC_CLK][TMP_SYS_DIV];
        sysClockConfig.divBus  = tmpSysClk[TMP_SIRC_CLK][TMP_BUS_DIV];
        sysClockConfig.divSlow = tmpSysClk[TMP_SIRC_CLK][TMP_SLOW_DIV];
        status = CLOCK_SYS_TransitionSystemClock(&sysClockConfig);
    }

    return status;
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_TransitionToTmpSysClk
 * Description   : Transition to a temporary system clock
 * END**************************************************************************/
static void CLOCK_SYS_GetCurrentSysClkConfig(scg_system_clock_config_t * sysClockConfig)
{
    static const scg_system_clock_src_t sysClkSrcMappings[((uint32_t)SCG_SYSTEM_CLOCK_SRC_SYS_PLL) + 1U] = {SCG_SYSTEM_CLOCK_SRC_NONE,SCG_SYSTEM_CLOCK_SRC_SYS_OSC,SCG_SYSTEM_CLOCK_SRC_SIRC,SCG_SYSTEM_CLOCK_SRC_FIRC,SCG_SYSTEM_CLOCK_SRC_NONE,SCG_SYSTEM_CLOCK_SRC_NONE,SCG_SYSTEM_CLOCK_SRC_SYS_PLL};
    static const scg_system_clock_div_t sysClkDivMappings[((uint32_t) SCG_SYSTEM_CLOCK_DIV_BY_16)+ 1U] = {
            SCG_SYSTEM_CLOCK_DIV_BY_1, SCG_SYSTEM_CLOCK_DIV_BY_2,
            SCG_SYSTEM_CLOCK_DIV_BY_3, SCG_SYSTEM_CLOCK_DIV_BY_4,
            SCG_SYSTEM_CLOCK_DIV_BY_5, SCG_SYSTEM_CLOCK_DIV_BY_6,
            SCG_SYSTEM_CLOCK_DIV_BY_7, SCG_SYSTEM_CLOCK_DIV_BY_8,
            SCG_SYSTEM_CLOCK_DIV_BY_9, SCG_SYSTEM_CLOCK_DIV_BY_10,
            SCG_SYSTEM_CLOCK_DIV_BY_11, SCG_SYSTEM_CLOCK_DIV_BY_12,
            SCG_SYSTEM_CLOCK_DIV_BY_13, SCG_SYSTEM_CLOCK_DIV_BY_14,
            SCG_SYSTEM_CLOCK_DIV_BY_15, SCG_SYSTEM_CLOCK_DIV_BY_16 };

    /* Save the current system clock source configuration */
    sysClockConfig->src     = sysClkSrcMappings[SCG_GetCurrentSystemClockSource(SCG)];
    sysClockConfig->divBus  = sysClkDivMappings[SCG_GetCurrentBusClockDividerRatio(SCG)];
    sysClockConfig->divCore = sysClkDivMappings[SCG_GetCurrentCoreClockDividerRatio(SCG)];
    sysClockConfig->divSlow = sysClkDivMappings[SCG_GetCurrentSlowClockDividerRatio(SCG)];
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_Init
 * Description   : This function initializes clocking modules according to a provided
 * configuration.
 *
 * Implements CLOCK_DRV_Init_Activity
 * END**************************************************************************/
status_t CLOCK_DRV_Init(clock_manager_user_config_t const * config)
{
    return CLOCK_SYS_SetConfiguration(config);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_EnablePeripheralClock
 * Description   : This function enables a peripheral clock
 *
 * Implements CLOCK_DRV_EnablePeripheralClock_Activity
 * END**************************************************************************/
void CLOCK_DRV_EnablePeripheralClock(clock_names_t clockName,
                                     const periph_clk_config_t * periphClkConfig)
{
    uint32_t source, divider, multiplier;
    periph_clk_config_t defaultPeriphClkCfg;
    const periph_clk_config_t * periphClkCfg;

    /* Configuration is not provided, a default one will be used. */
    if (periphClkConfig == NULL)
    {
        defaultPeriphClkCfg.source = FIRC_CLK_SRC;
        defaultPeriphClkCfg.divider = DO_NOT_DIVIDE;
        defaultPeriphClkCfg.multiplier = DO_NOT_MULTIPLY;
        periphClkCfg = &defaultPeriphClkCfg;
    }
    else
    {
        periphClkCfg = periphClkConfig;
    }

    /* Check that clock gating is configurable from PCC */
    if ((peripheralFeaturesList[clockName] & HAS_CLOCK_GATING_IN_SIM) == 0U)
    {
        /* Check that protocol clock is supported by the peripheral corresponding to the clock name */
        source = ((peripheralFeaturesList[clockName] & (HAS_PROTOCOL_CLOCK_FROM_ASYNC1 | HAS_PROTOCOL_CLOCK_FROM_ASYNC2)) != 0U) ?
                        ((uint32_t) periphClkCfg->source) : 0U;

        /* Check that multiplier clock is supported. */
        multiplier = ((peripheralFeaturesList[clockName] & HAS_MULTIPLIER) != 0U) ?
                ((uint32_t) periphClkCfg->multiplier) : 0U;

        /* Check that divider clock is supported. */
        divider = ((peripheralFeaturesList[clockName] & HAS_DIVIDER) != 0U) ?
                ((uint32_t) periphClkCfg->divider) : 0U;

        /* Disable the peripheral clock */
        PCC_SetClockMode(PCC, clockName, false);

        /* Set peripheral clock control */
        PCC_SetPeripheralClockControl(PCC, clockName, true, source, divider, multiplier);
    }
    else
    {
        switch (clockName)
        {
            case SIM_MSCM_CLOCK:
                SIM_SetMscmClockGate(SIM, true);
                break;

            case SIM_MPU_CLOCK:
                SIM_SetMpuClockGate(SIM, true);
                break;

            case SIM_DMA_CLOCK:
                SIM_SetDmaClockGate(SIM, true);
                break;

            case SIM_ERM_CLOCK:
                SIM_SetErmClockGate(SIM, true);
                break;

            case SIM_EIM_CLOCK:
                SIM_SetEimClockGate(SIM, true);
                break;
            default:
                /* Invalid clock name */
                DEV_ASSERT(false);
                break;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_DisablePeripheralClock
 * Description   : This function disables a peripheral clock
 *
 * Implements CLOCK_DRV_DisablePeripheralClock_Activity
 * END**************************************************************************/
void CLOCK_DRV_DisablePeripheralClock(clock_names_t clockName)
{
    /* Check that clock gating is configurable from PCC */
    if ((peripheralFeaturesList[clockName] & HAS_CLOCK_GATING_IN_SIM) == 0U)
    {
        /* Disable the peripheral clock from PCC */
        PCC_SetClockMode(PCC, clockName, false);
    }
    /* Clock gating is implemented in PCC */
    else
    {
        switch (clockName)
        {
            case SIM_MSCM_CLOCK:
                SIM_SetMscmClockGate(SIM, false);
                break;

            case SIM_MPU_CLOCK:
                SIM_SetMpuClockGate(SIM, false);
                break;

            case SIM_DMA_CLOCK:
                SIM_SetDmaClockGate(SIM, false);
                break;

            case SIM_ERM_CLOCK:
                SIM_SetErmClockGate(SIM, false);
                break;

            case SIM_EIM_CLOCK:
                SIM_SetEimClockGate(SIM, false);
                break;
            default:
                /* Invalid clock name */
                DEV_ASSERT(false);
                break;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_SetSystemClock
 * Description   : This function configures the system clocks (core, bus and flash clocks).
 *
 * Implements CLOCK_DRV_SetSystemClock_Activity
 * END**************************************************************************/
status_t CLOCK_DRV_SetSystemClock(const pwr_modes_t * mode,
                                  const sys_clk_config_t * sysClkConfig)
{
    status_t retCode = STATUS_SUCCESS;

    scg_system_clock_mode_t sysClockMode, currentSysClockMode = CLOCK_SYS_GetCurrentRunMode();
    scg_system_clock_config_t sysClockConfig;

    static const scg_system_clock_src_t sysClkSrcMappings[] = {SCG_SYSTEM_CLOCK_SRC_SIRC,SCG_SYSTEM_CLOCK_SRC_FIRC,SCG_SYSTEM_CLOCK_SRC_SYS_OSC,SCG_SYSTEM_CLOCK_SRC_SYS_PLL};
    static const scg_system_clock_div_t sysClkDivMappings[] = {SCG_SYSTEM_CLOCK_DIV_BY_1,SCG_SYSTEM_CLOCK_DIV_BY_1,SCG_SYSTEM_CLOCK_DIV_BY_2,SCG_SYSTEM_CLOCK_DIV_BY_3,SCG_SYSTEM_CLOCK_DIV_BY_4,SCG_SYSTEM_CLOCK_DIV_BY_5,SCG_SYSTEM_CLOCK_DIV_BY_6,SCG_SYSTEM_CLOCK_DIV_BY_7,SCG_SYSTEM_CLOCK_DIV_BY_8,SCG_SYSTEM_CLOCK_DIV_BY_9,SCG_SYSTEM_CLOCK_DIV_BY_10,SCG_SYSTEM_CLOCK_DIV_BY_11,SCG_SYSTEM_CLOCK_DIV_BY_12,SCG_SYSTEM_CLOCK_DIV_BY_13,SCG_SYSTEM_CLOCK_DIV_BY_14,SCG_SYSTEM_CLOCK_DIV_BY_15,SCG_SYSTEM_CLOCK_DIV_BY_16};
    static const scg_system_clock_div_t defSysClk[TMP_SYS_CLK_NO][TMP_SYS_DIV_NO] = TMP_SYSTEM_CLOCK_CONFIGS;

    /* The power mode is not provided, the current one will be used by default. */
    if (mode == NULL)
    {
        sysClockMode = currentSysClockMode;
    }
    else
    {
        switch (*mode)
        {
            /* High speed run mode */
            case HSRUN_MODE:
                sysClockMode = SCG_SYSTEM_CLOCK_MODE_HSRUN;
                break;
            /* Run mode */
            case RUN_MODE:
                sysClockMode = SCG_SYSTEM_CLOCK_MODE_RUN;
                break;
            /* Very low power run mode */
            case VLPR_MODE:
                sysClockMode = SCG_SYSTEM_CLOCK_MODE_VLPR;
                break;
            /* This should never happen - input power mode is invalid */
            default:
                sysClockMode = SCG_SYSTEM_CLOCK_MODE_NONE;
                DEV_ASSERT(false);
                break;
        }
    }

    /* System clock configuration is not provided, a default one will be used. */
    if (sysClkConfig == NULL)
    {
        /* Find a valid clock source. */
        if (CLOCK_SYS_GetFircFreq() != 0U)
        {
            sysClockConfig.src     = SCG_SYSTEM_CLOCK_SRC_FIRC;
            sysClockConfig.divCore = defSysClk[TMP_FIRC_CLK][TMP_SYS_DIV];
            sysClockConfig.divBus  = defSysClk[TMP_FIRC_CLK][TMP_BUS_DIV];
            sysClockConfig.divSlow = defSysClk[TMP_FIRC_CLK][TMP_SLOW_DIV];
        }
        else if (CLOCK_SYS_GetSysPllFreq() != 0U)
        {
            sysClockConfig.src     = SCG_SYSTEM_CLOCK_SRC_SYS_PLL;
            sysClockConfig.divCore = defSysClk[TMP_SPLL_CLK][TMP_SYS_DIV];
            sysClockConfig.divBus  = defSysClk[TMP_SPLL_CLK][TMP_BUS_DIV];
            sysClockConfig.divSlow = defSysClk[TMP_SPLL_CLK][TMP_SLOW_DIV];
        }
        else if (CLOCK_SYS_GetSysOscFreq() != 0U)
        {
            sysClockConfig.src     = SCG_SYSTEM_CLOCK_SRC_SYS_OSC;
            sysClockConfig.divCore = defSysClk[TMP_SOSC_CLK][TMP_SYS_DIV];
            sysClockConfig.divBus  = defSysClk[TMP_SOSC_CLK][TMP_BUS_DIV];
            sysClockConfig.divSlow = defSysClk[TMP_SOSC_CLK][TMP_SLOW_DIV];
        }
        else if (CLOCK_SYS_GetSircFreq() != 0U)
        {
            sysClockConfig.src     = SCG_SYSTEM_CLOCK_SRC_SIRC;
            sysClockConfig.divCore = defSysClk[TMP_SIRC_CLK][TMP_SYS_DIV];
            sysClockConfig.divBus  = defSysClk[TMP_SIRC_CLK][TMP_BUS_DIV];
            sysClockConfig.divSlow = defSysClk[TMP_SIRC_CLK][TMP_SLOW_DIV];
        }
        else
        {
            /* Can't reach this point, at least one clock source is functional.
             * This code is written to avoid MISRA 15.7 (no 'else' at end of 'if ... else if' chain) */
            DEV_ASSERT(false);
        }
    }
    else
    {
        /* The system clock source from input configuration structure must be valid. */
        DEV_ASSERT((CLOCK_SYS_GetSircFreq() != 0U) || (sysClkConfig->src != SYS_CLK_SIRC));
        DEV_ASSERT((CLOCK_SYS_GetFircFreq() != 0U) || (sysClkConfig->src != SYS_CLK_FIRC));
        DEV_ASSERT((CLOCK_SYS_GetSysOscFreq() != 0U) || (sysClkConfig->src != SYS_CLK_SOSC));
        DEV_ASSERT((CLOCK_SYS_GetSysPllFreq() != 0U) || (sysClkConfig->src != SYS_CLK_SPLL));

        sysClockConfig.src     = sysClkSrcMappings[sysClkConfig->src];
        sysClockConfig.divCore = sysClkDivMappings[sysClkConfig->core];
        sysClockConfig.divBus  = sysClkDivMappings[sysClkConfig->bus];
        sysClockConfig.divSlow = sysClkDivMappings[sysClkConfig->slow];
    }

    /* System clock is configured in the current mode,
     * set configuration and wait until the system clock is changed. */
    if (sysClockMode == currentSysClockMode)
    {
        retCode = CLOCK_SYS_TransitionSystemClock(&sysClockConfig);
    }
    /* System clock is not configured in the current mode, just set configuration. */
    else
    {
        retCode = CLOCK_SYS_SetSystemClockConfig(sysClockMode,&sysClockConfig);
    }
    return retCode;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_GetSystemClockSource
 * Description   : This function gets the current system clock source
 *
 * Implements CLOCK_DRV_GetSystemClockSource_Activity
 * END**************************************************************************/
void CLOCK_DRV_GetSystemClockSource(sys_clk_config_t *sysClkConfig)
{
    sys_clk_div_t sysClkDivValues[] = {SYS_CLK_DO_NOT_DIVIDE, SYS_CLK_DIV_BY_2, SYS_CLK_DIV_BY_3, SYS_CLK_DIV_BY_4,
            SYS_CLK_DIV_BY_5, SYS_CLK_DIV_BY_6, SYS_CLK_DIV_BY_7, SYS_CLK_DIV_BY_8,
            SYS_CLK_DIV_BY_9, SYS_CLK_DIV_BY_10, SYS_CLK_DIV_BY_11, SYS_CLK_DIV_BY_12,
            SYS_CLK_DIV_BY_13, SYS_CLK_DIV_BY_14, SYS_CLK_DIV_BY_15, SYS_CLK_DIV_BY_16};

    switch(SCG_GetCurrentSystemClockSource(SCG))
    {
        case SCG_SYSTEM_CLOCK_SRC_SYS_OSC:
        {
            sysClkConfig->src = SYS_CLK_SOSC;
        }
        break;
        case SCG_SYSTEM_CLOCK_SRC_SIRC:
        {
            sysClkConfig->src = SYS_CLK_SIRC;
        }
        break;
        case SCG_SYSTEM_CLOCK_SRC_FIRC:
        {
            sysClkConfig->src = SYS_CLK_FIRC;
        }
        break;
        case SCG_SYSTEM_CLOCK_SRC_SYS_PLL:
        {
            sysClkConfig->src = SYS_CLK_SPLL;
        }
        break;
        default:
        {
            /* Invalid system clock source. */
            DEV_ASSERT(false);
            sysClkConfig->src = SYS_CLK_SIRC;
        }
        break;
    }

    DEV_ASSERT(SCG_GetCurrentBusClockDividerRatio(SCG) < 16U);
    sysClkConfig->bus = sysClkDivValues[SCG_GetCurrentBusClockDividerRatio(SCG)];

    DEV_ASSERT(SCG_GetCurrentCoreClockDividerRatio(SCG) < 16U);
    sysClkConfig->core = sysClkDivValues[SCG_GetCurrentCoreClockDividerRatio(SCG)];

    DEV_ASSERT(SCG_GetCurrentSlowClockDividerRatio(SCG) < 8U);
    sysClkConfig->slow = sysClkDivValues[SCG_GetCurrentSlowClockDividerRatio(SCG)];
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_SetSirc
 * Description   : This function configures SIRC clock source
 *
 * Implements CLOCK_DRV_SetSirc_Activity
 * END**************************************************************************/
status_t CLOCK_DRV_SetSirc(bool enable,
                           const sirc_config_t * sircConfig)
{
    status_t retCode = STATUS_SUCCESS;
    scg_sirc_config_t scgSircConfig;
    bool revertTmpSysClkTransition = false;
    scg_system_clock_config_t currentSysClockConfig;

    static const scg_sirc_range_t sircRangeMappings[(uint32_t)SIRC_RANGES_MAX] = {SCG_SIRC_RANGE_LOW,SCG_SIRC_RANGE_HIGH};

    /* Check whether the current system clock source is SIRC */
    if (SCG_GetCurrentSystemClockSource(SCG) == ((uint32_t)SCG_SYSTEM_CLOCK_SRC_SIRC))
    {
        if (enable == false)
        {
            /* Can't disable SIRC, it's used as system clock source */
            retCode = STATUS_ERROR;
        }
        else
        {
            /* Save the current system clock source configuration */
            CLOCK_SYS_GetCurrentSysClkConfig(&currentSysClockConfig);

            /* Transition to a temporary system clock source */
            retCode = CLOCK_SYS_TransitionToTmpSysClk(SCG_SYSTEM_CLOCK_SRC_SIRC);

            revertTmpSysClkTransition = (retCode == STATUS_SUCCESS) ? true : false;
        }
    }

    if (retCode == STATUS_SUCCESS)
    {
        if (sircConfig == NULL)
        {
            retCode = CLOCK_SYS_ConfigureSIRC(enable, NULL);
        }
        else
        {
            /* Check VLP modes */
            if (((((uint32_t)sircConfig->modes) & ((uint32_t)VLPR_MODE)) != 0U) || ((((uint32_t)sircConfig->modes) & ((uint32_t)VLPS_MODE)) != 0U))
            {
                scgSircConfig.enableInLowPower  = true;
            }
            else
            {
                scgSircConfig.enableInLowPower  = false;
            }

            /* Check STOP mode. */
            if (((((uint32_t)sircConfig->modes) & ((uint32_t)STOP_MODE)) != 0U))
            {
                scgSircConfig.enableInStop  = true;
            }
            else
            {
                scgSircConfig.enableInStop  = false;
            }

            scgSircConfig.locked            = false;

            scgSircConfig.div1              = SCG_ASYNC_CLOCK_DIV_BY_1;
            scgSircConfig.div2              = SCG_ASYNC_CLOCK_DIV_BY_1;

            scgSircConfig.range             = sircRangeMappings[sircConfig->range];

            retCode = CLOCK_SYS_ConfigureSIRC(enable, &scgSircConfig);
        }

        /* If system clock source was SIRC and SIRC has been set successfully,
         * then transition back to SIRC */
        if ((retCode == STATUS_SUCCESS) && (revertTmpSysClkTransition == true))
        {
            retCode = CLOCK_SYS_TransitionSystemClock(&currentSysClockConfig);
        }
    }

    return retCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_SetFirc
 * Description   : This function configures FIRC clock source
 *
 * Implements CLOCK_DRV_SetFirc_Activity
 * END**************************************************************************/
status_t CLOCK_DRV_SetFirc(bool enable,
                           const firc_config_t * fircConfig)
{
    status_t retCode = STATUS_SUCCESS;
    scg_firc_config_t scgFircConfig;
    bool revertTmpSysClkTransition = false;
    scg_system_clock_config_t currentSysClockConfig;

    static const scg_firc_range_t fircRangeMappings[(uint32_t)FIRC_RANGES_MAX] = {SCG_FIRC_RANGE_48M, SCG_FIRC_RANGE_52M, SCG_FIRC_RANGE_56M, SCG_FIRC_RANGE_60M};

    /* Check whether the current system clock source is FIRC */
    if (SCG_GetCurrentSystemClockSource(SCG) == ((uint32_t)SCG_SYSTEM_CLOCK_SRC_FIRC))
    {
        if (enable == false)
        {
            /* Can't disable FIRC, it's used as system clock source. */
            retCode = STATUS_ERROR;
        }
        else
        {
            /* Save the current system clock source configuration. */
            CLOCK_SYS_GetCurrentSysClkConfig(&currentSysClockConfig);

            /* Transition to a temporary system clock source. */
            retCode = CLOCK_SYS_TransitionToTmpSysClk(SCG_SYSTEM_CLOCK_SRC_FIRC);

            revertTmpSysClkTransition = (retCode == STATUS_SUCCESS) ? true : false;
        }
    }

    if (retCode == STATUS_SUCCESS)
    {
        if (fircConfig == NULL)
        {
            retCode = CLOCK_SYS_ConfigureFIRC(enable, NULL);
        }
        else
        {
            /* Check VLP modes */
            if (((((uint32_t)fircConfig->modes) & ((uint32_t)VLPR_MODE)) != 0U) || ((((uint32_t)fircConfig->modes) & ((uint32_t)VLPS_MODE)) != 0U))
            {
                scgFircConfig.enableInLowPower  = true;
            }
            else
            {
                scgFircConfig.enableInLowPower  = false;
            }

            /* Check STOP mode. */
            if (((((uint32_t)fircConfig->modes) & ((uint32_t)STOP_MODE)) != 0U))
            {
                scgFircConfig.enableInStop  = true;
            }
            else
            {
                scgFircConfig.enableInStop  = false;
            }

            scgFircConfig.locked            = false;

            scgFircConfig.div1              = SCG_ASYNC_CLOCK_DIV_BY_1;
            scgFircConfig.div2              = SCG_ASYNC_CLOCK_DIV_BY_1;

            scgFircConfig.range             = fircRangeMappings[fircConfig->range];
            scgFircConfig.regulator         = fircConfig->regulator;

            retCode = CLOCK_SYS_ConfigureFIRC(enable, &scgFircConfig);
        }

        /* If system clock source was FIRC and FIRC has been set successfully,
         * then transition back to FIRC */
        if ((retCode == STATUS_SUCCESS) && (revertTmpSysClkTransition == true))
        {
            retCode = CLOCK_SYS_TransitionSystemClock(&currentSysClockConfig);
        }
    }

    return retCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_SetSosc
 * Description   : This function configures SOSC clock source
 *
 * Implements CLOCK_DRV_SetSosc_Activity
 * END**************************************************************************/
status_t CLOCK_DRV_SetSosc(bool enable,
                           const sosc_config_t * soscConfig)
{
    status_t retCode = STATUS_SUCCESS;
    scg_sosc_config_t scgSoscConfig;
    bool revertTmpSysClkTransition = false;
    scg_system_clock_config_t currentSysClockConfig;

    static const scg_sosc_range_t soscRangeMappings[(uint32_t)SOSC_RANGES_MAX] = {SCG_SOSC_RANGE_LOW, SCG_SOSC_RANGE_MID, SCG_SOSC_RANGE_HIGH};
    static const scg_sosc_ext_ref_t soscRefMappings[(uint32_t)SOSC_REFS_MAX] = {SCG_SOSC_REF_EXT, SCG_SOSC_REF_OSC};

    /* Check whether the current system clock source is SOSC */
    if (SCG_GetCurrentSystemClockSource(SCG) == ((uint32_t)SCG_SYSTEM_CLOCK_SRC_SYS_OSC))
    {
        if (enable == false)
        {
            /* Can't disable SOSC, it's used as system clock source. */
            retCode = STATUS_ERROR;
        }
        else
        {
            /* Save the current system clock source configuration. */
            CLOCK_SYS_GetCurrentSysClkConfig(&currentSysClockConfig);

            /* Transition to a temporary system clock source. */
            retCode = CLOCK_SYS_TransitionToTmpSysClk(SCG_SYSTEM_CLOCK_SRC_SYS_OSC);

            revertTmpSysClkTransition = (retCode == STATUS_SUCCESS) ? true : false;
        }
    }

    if (retCode == STATUS_SUCCESS)
    {
        if (soscConfig == NULL)
        {
            retCode = CLOCK_SYS_ConfigureSOSC(enable, NULL);
        }
        else
        {
            /* Check VLP modes */
            if (((((uint32_t)soscConfig->modes) & ((uint32_t)VLPR_MODE)) != 0U) || ((((uint32_t)soscConfig->modes) & ((uint32_t)VLPS_MODE)) != 0U))
            {
                scgSoscConfig.enableInLowPower  = true;
            }
            else
            {
                scgSoscConfig.enableInLowPower  = false;
            }

            /* Check STOP mode. */
            if (((((uint32_t)soscConfig->modes) & ((uint32_t)STOP_MODE)) != 0U))
            {
                scgSoscConfig.enableInStop  = true;
            }
            else
            {
                scgSoscConfig.enableInStop  = false;
            }

            scgSoscConfig.freq              = soscConfig->freq;

            scgSoscConfig.monitorMode       = SCG_SOSC_MONITOR_DISABLE;
            scgSoscConfig.locked            = false;

            scgSoscConfig.div1              = SCG_ASYNC_CLOCK_DIV_BY_1;
            scgSoscConfig.div2              = SCG_ASYNC_CLOCK_DIV_BY_1;

            scgSoscConfig.extRef            = soscRefMappings[soscConfig->ref];
            scgSoscConfig.gain              = SCG_SOSC_GAIN_LOW;
            scgSoscConfig.range             = soscRangeMappings[soscConfig->range];

            retCode = CLOCK_SYS_ConfigureSOSC(enable, &scgSoscConfig);
        }

        /* If system clock source was SOSC and SOSC has been set successfully,
         * then transition back to SOSC */
        if ((retCode == STATUS_SUCCESS) && (revertTmpSysClkTransition == true))
        {
            retCode = CLOCK_SYS_TransitionSystemClock(&currentSysClockConfig);
        }
    }

    return retCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_SetSpll
 * Description   : This function configures SPLL clock source
 *
 * Implements CLOCK_DRV_SetSpll_Activity
 * END**************************************************************************/
status_t CLOCK_DRV_SetSpll(bool enable,
                           const spll_config_t * spllConfig)
{
    status_t retCode = STATUS_SUCCESS;
    scg_spll_config_t scgSpllConfig;
    bool revertTmpSysClkTransition = false;
    scg_system_clock_config_t currentSysClockConfig;

    /* Check whether the current system clock source is SPLL */
    if (SCG_GetCurrentSystemClockSource(SCG) == ((uint32_t)SCG_SYSTEM_CLOCK_SRC_SYS_PLL))
    {
        if (enable == false)
        {
            /* Can't disable SPLL, it's used as system clock source. */
            retCode = STATUS_ERROR;
        }
        else
        {
            /* Save the current system clock source configuration. */
            CLOCK_SYS_GetCurrentSysClkConfig(&currentSysClockConfig);

            /* Transition to a temporary system clock source. */
            retCode = CLOCK_SYS_TransitionToTmpSysClk(SCG_SYSTEM_CLOCK_SRC_SYS_PLL);

            revertTmpSysClkTransition = (retCode == STATUS_SUCCESS) ? true : false;
        }
    }

    if (retCode == STATUS_SUCCESS)
    {
        if (spllConfig == NULL)
        {
            retCode = CLOCK_SYS_ConfigureSPLL(enable, NULL);
        }
        else
        {
            /* Check STOP mode. */
            if (((((uint32_t)spllConfig->modes) & ((uint32_t)STOP_MODE)) != 0U))
            {
                scgSpllConfig.enableInStop  = true;
            }
            else
            {
                scgSpllConfig.enableInStop  = false;
            }

            scgSpllConfig.monitorMode       = SCG_SPLL_MONITOR_DISABLE;
            scgSpllConfig.locked            = false;

            scgSpllConfig.div1              = SCG_ASYNC_CLOCK_DIV_BY_1;
            scgSpllConfig.div2              = SCG_ASYNC_CLOCK_DIV_BY_1;

            scgSpllConfig.mult              = (uint8_t) spllConfig->mult;
            scgSpllConfig.prediv            = (uint8_t) spllConfig->prediv;

            retCode = CLOCK_SYS_ConfigureSPLL(enable, &scgSpllConfig);
        }

        /* If system clock source was SPLL and SPLL has been set successfully,
         * then transition back to SPLL */
        if ((retCode == STATUS_SUCCESS) && (revertTmpSysClkTransition == true))
        {
            retCode = CLOCK_SYS_TransitionSystemClock(&currentSysClockConfig);
        }
    }

    return retCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_DRV_SetLpo
 * Description   : This function configures LPO clock source
 *
 * Implements CLOCK_DRV_SetLpo_Activity
 * END**************************************************************************/
status_t CLOCK_DRV_SetLpo(bool enable)
{
    if (enable)
    {
        SIM_SetLpoClocks(SIM,true,true,SIM_LPO_CLK_SEL_LPO_128K,SIM_RTCCLK_SEL_SOSCDIV1_CLK);
        PMC_SetLpoMode(PMC, true);
    }
    else
    {
        PMC_SetLpoMode(PMC, false);
    }

    return STATUS_SUCCESS;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
