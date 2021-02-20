/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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
 * @file edma_irq.c
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * These are symbols weak symbols defined in platform startup files (.s).
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, external could be made static.
 * The functions are called by the interrupt controller when the appropriate event
 * occurs.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * This is required for initializing pointers to the module's memory map, which
 * is located at a fixed address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 */

#include "edma_irq.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#ifdef FEATURE_EDMA_ORED_IRQ_LINES_16_CHN
/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA0_15_IRQHandler(void)
{
    /* Read the status flags register */
    uint32_t mask = 0xFFFF;
    uint32_t flags = DMA->INT;
    uint8_t i = 0U;
    flags &= mask;
    /* Check all the flags from 0 to 15 and call the handler for the appropriate channel */
    while (flags > 0U)
    {
       if ((flags & 1U) > 0U)
       {
           EDMA_DRV_IRQHandler(i);
       }
       i++;
       flags >>= 1U;
    }
}

/*! @brief DMA16_31_IRQn IRQ handler with the same name in the startup code*/
void DMA16_31_IRQHandler(void)
{
    /* Read the status flags register */
    uint32_t flags = DMA->INT;
    uint8_t i = 16U;
    flags >>= 16U;
    /* Check all the flags from 16 to 31 and call the handler for the appropriate channel */
    while (flags > 0U)
    {
       if ((flags & 1U) > 0U)
       {
           EDMA_DRV_IRQHandler(i);
       }
       i++;
       flags >>= 1U;
    }
}
#endif

#ifdef FEATURE_EDMA_SEPARATE_IRQ_LINES_PER_CHN
/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA0_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(0U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA1_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(1U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA2_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(2U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA3_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(3U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA4_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(4U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA5_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(5U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA6_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(6U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA7_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(7U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA8_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(8U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA9_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(9U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA10_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(10U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA11_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(11U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA12_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(12U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA13_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(13U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA14_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(14U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA15_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(15U);
}
#if (FEATURE_EDMA_MODULE_CHANNELS > 16U)
void DMA16_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(16U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA17_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(17U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA18_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(18U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA19_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(19U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA20_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(20U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA21_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(21U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA22_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(22U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA23_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(23U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA24_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(24U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA25_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(25U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA26_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(26U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA27_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(27U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA28_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(28U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA29_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(29U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA30_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(30U);
}

/*! @brief EDMA IRQ handler with the same name in the startup code*/
void DMA31_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(31U);
}
#endif
#endif

#ifdef FEATURE_EDMA_HAS_ERROR_IRQ
/*! @brief EDMA ERROR IRQ handler with the same name in the startup code*/
void DMA_Error_IRQHandler(void)
{
    EDMA_DRV_ErrorIRQHandler();
}
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/

