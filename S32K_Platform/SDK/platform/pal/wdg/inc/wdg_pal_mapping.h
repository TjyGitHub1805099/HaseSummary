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
 * @file wdg_pal_mapping.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macros defined are used to define features for each driver, so this might be reported
 * when the analysis is made only on one driver.
 *
 */

#ifndef WDG_PAL_MAPPING_H
#define WDG_PAL_MAPPING_H

#include "device_registers.h"

/*!
 * @ingroup wdg_pal_code
 */

#if (defined(CPU_S32K144HFT0VLLT) || defined(CPU_S32K144LFT0MLLT) || defined(CPU_S32K148) || defined(CPU_S32K142) \
     || defined(CPU_S32K146) || defined(CPU_S32MTV))
/*!
 * @brief Define instances for S32K14x and S32M (WDOG and EWM).
 * Implements : wdg_instance_t_Class
 */
typedef enum
{
    WDG_OVER_WDOG_INSTANCE = 0U,
    WDG_OVER_EWM_INSTANCE  = 1U
} wdg_instance_t;

/* On S32K14x and S32M are 2 available instances which support WDG PAL */
#define NUMBER_OF_WDG_PAL_INSTANCES          (2U)
/* Defines the index limits for all WDG PAL */
#define WDG_OVER_WDOG_LOW_INDEX              (0U)
#define WDG_OVER_WDOG_HIGH_INDEX             (0U)
#define WDG_OVER_EWM_LOW_INDEX               (1U)
#define WDG_OVER_EWM_HIGH_INDEX              (1U)

#elif (defined(CPU_MPC5746C))
/*!
 * @brief Define instances for MPC5746C (SWT).
 * Implements : wdg_instance_t_Class
 */
typedef enum
{
    WDG_OVER_SWT0_INSTANCE  = 0U,
    WDG_OVER_SWT1_INSTANCE  = 1U
} wdg_instance_t;

/* On MPC5746C are 2 available instances which support WDG PAL*/
#define NUMBER_OF_WDG_PAL_INSTANCES          (2U)
/* Defines the index limits for all WDG PAL */
#define WDG_OVER_SWT_LOW_INDEX               (0U)
#define WDG_OVER_SWT_HIGH_INDEX              (1U)

#elif (defined(CPU_MPC5748G))
/*!
 * @brief Define instances for MPC5748G (SWT).
 * Implements : wdg_instance_t_Class
 */
typedef enum
{
    WDG_OVER_SWT0_INSTANCE  = 0U,
    WDG_OVER_SWT1_INSTANCE  = 1U,
    WDG_OVER_SWT2_INSTANCE  = 2U
} wdg_instance_t;

/* On MPC5748G are 3 available instances which support WDG PAL */
#define NUMBER_OF_WDG_PAL_INSTANCES          (3U)
/* Defines the index limits for all WDG PAL */
#define WDG_OVER_SWT_LOW_INDEX               (0U)
#define WDG_OVER_SWT_HIGH_INDEX              (2U)

#elif (defined(CPU_MPC5744P))
/*!
 * @brief Define instances for MPC5744P (SWT).
 * Implements : wdg_instance_t_Class
 */
typedef enum
{
    WDG_OVER_SWT0_INSTANCE  = 0U
} wdg_instance_t;

/* On MPC5744P are 1 available instances which support WDG PAL*/
#define NUMBER_OF_WDG_PAL_INSTANCES          (1U)
/* Defines the index limits for all WDG PAL */
#define WDG_OVER_SWT_LOW_INDEX               (0U)
#define WDG_OVER_SWT_HIGH_INDEX              (0U)

#endif

#endif /* WDG_PAL_MAPPING_H */
