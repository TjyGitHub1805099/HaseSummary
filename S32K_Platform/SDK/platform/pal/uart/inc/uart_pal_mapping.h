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
 * @file uart_pal_mapping.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced
 * This macro is used by user.
 */

#ifndef UART_PAL_MAPPING_H
#define UART_PAL_MAPPING_H
#include "device_registers.h"

#if (defined(CPU_S32K144HFT0VLLT) || defined(CPU_S32K144LFT0MLLT) || defined(CPU_S32K146) || defined(CPU_S32K148))

/*!
 * @brief Defines instances for S32K144 and S32K148 (LPUART and FlexIO)
 *
 * Implements : uart_instance_t_Class
 */
typedef enum
{
    UART_OVER_LPUART00_INSTANCE = 0U,
    UART_OVER_LPUART01_INSTANCE = 1U,
    UART_OVER_LPUART02_INSTANCE = 2U,
    UART_OVER_FLEXIO0_INSTANCE = 3U,
    UART_OVER_FLEXIO1_INSTANCE = 4U,
} uart_instance_t;

/* On S32K144, S32K146 and S32K148 are 5 available instances which support UART */
#define NUMBER_OF_UART_PAL_INSTANCES 5U

/* Defines the index limits for all UARTs */
#define LPUART_HIGH_INDEX      2U
#define FLEXIO_UART_LOW_INDEX  3U
#define FLEXIO_UART_HIGH_INDEX 4U

#elif (defined(CPU_S32K142))

/*!
 * @brief Defines instances for S32K142 (LPUART and FlexIO)
 *
 * Implements : uart_instance_t_Class
 */
typedef enum
{
    UART_OVER_LPUART00_INSTANCE = 0U,
    UART_OVER_LPUART01_INSTANCE = 1U,
    UART_OVER_FLEXIO0_INSTANCE = 2U,
    UART_OVER_FLEXIO1_INSTANCE = 3U,
} uart_instance_t;

/* On S32K142 are 4 available instances which support UART */
#define NUMBER_OF_UART_PAL_INSTANCES 4U

/* Defines the index limits for all UARTs */
#define LPUART_HIGH_INDEX      1U
#define FLEXIO_UART_LOW_INDEX  2U
#define FLEXIO_UART_HIGH_INDEX 3U

#elif (defined(CPU_S32V234))

/*!
 * @brief Defines instances for S32V234 (LinFlexD)
 *
 * Implements : uart_instance_t_Class
 */
typedef enum
{
    UART_OVER_LINFLEXD00_INSTANCE = 0U,
    UART_OVER_LINFLEXD01_INSTANCE = 1U,
} uart_instance_t;

/* On S32V234 are 2 available instances which support UART */
#define NUMBER_OF_UART_PAL_INSTANCES 2U

/* Defines the index limits for all UARTs */
#define LINFLEXD_UART_LOW_INDEX   0U
#define LINFLEXD_UART_HIGH_INDEX  1U

#elif (defined(CPU_MPC5748G))

/*!
 * @brief Defines instances for MPC5748G (LinFlexD)
 *
 * Implements : uart_instance_t_Class
 */
typedef enum
{
    UART_OVER_LINFLEXD00_INSTANCE = 0U,
    UART_OVER_LINFLEXD01_INSTANCE = 1U,
    UART_OVER_LINFLEXD02_INSTANCE = 2U,
    UART_OVER_LINFLEXD03_INSTANCE = 3U,
    UART_OVER_LINFLEXD04_INSTANCE = 4U,
    UART_OVER_LINFLEXD05_INSTANCE = 5U,
    UART_OVER_LINFLEXD06_INSTANCE = 6U,
    UART_OVER_LINFLEXD07_INSTANCE = 7U,
    UART_OVER_LINFLEXD08_INSTANCE = 8U,
    UART_OVER_LINFLEXD09_INSTANCE = 9U,
    UART_OVER_LINFLEXD10_INSTANCE = 10U,
    UART_OVER_LINFLEXD11_INSTANCE = 11U,
    UART_OVER_LINFLEXD12_INSTANCE = 12U,
    UART_OVER_LINFLEXD13_INSTANCE = 13U,
    UART_OVER_LINFLEXD14_INSTANCE = 14U,
    UART_OVER_LINFLEXD15_INSTANCE = 15U,
    UART_OVER_LINFLEXD16_INSTANCE = 16U,
    UART_OVER_LINFLEXD17_INSTANCE = 17U,
} uart_instance_t;

/* On MPC5748G are 18 available instances which support UART */
#define NUMBER_OF_UART_PAL_INSTANCES 18U

/* Defines the index limits for all UARTs */
#define LINFLEXD_UART_LOW_INDEX   0U
#define LINFLEXD_UART_HIGH_INDEX 17U

#elif (defined(CPU_MPC5746C))

/*!
 * @brief Defines instances for MPC5746C (LinFlexD)
 *
 * Implements : uart_instance_t_Class
 */
typedef enum
{
    UART_OVER_LINFLEXD00_INSTANCE = 0U,
    UART_OVER_LINFLEXD01_INSTANCE = 1U,
    UART_OVER_LINFLEXD02_INSTANCE = 2U,
    UART_OVER_LINFLEXD03_INSTANCE = 3U,
    UART_OVER_LINFLEXD04_INSTANCE = 4U,
    UART_OVER_LINFLEXD05_INSTANCE = 5U,
    UART_OVER_LINFLEXD06_INSTANCE = 6U,
    UART_OVER_LINFLEXD07_INSTANCE = 7U,
    UART_OVER_LINFLEXD08_INSTANCE = 8U,
    UART_OVER_LINFLEXD09_INSTANCE = 9U,
    UART_OVER_LINFLEXD10_INSTANCE = 10U,
    UART_OVER_LINFLEXD11_INSTANCE = 11U,
    UART_OVER_LINFLEXD12_INSTANCE = 12U,
    UART_OVER_LINFLEXD13_INSTANCE = 13U,
    UART_OVER_LINFLEXD14_INSTANCE = 14U,
    UART_OVER_LINFLEXD15_INSTANCE = 15U,
} uart_instance_t;

/* On MPC5746C are 16 available instances which support UART */
#define NUMBER_OF_UART_PAL_INSTANCES 16U

/* Defines the index limits for all UARTs */
#define LINFLEXD_UART_LOW_INDEX   0U
#define LINFLEXD_UART_HIGH_INDEX 15U

#endif

#endif /* UART_PAL_MAPPING_H */
