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
 * @file enet_irq.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, external could be made static.
 * The functions are called by the interrupt controller when the appropriate event
 * occurs.
 *
 */

#include "enet_hw_access.h"

#if (defined(CPU_MPC5748G) || defined(CPU_MPC5746C))

/*******************************************************************************
 * Default interrupt handlers signatures
 ******************************************************************************/
#if (ENET_INSTANCE_COUNT > 0U)
void ENET0_Tx_IRQHandler(void);
void ENET0_Rx_IRQHandler(void);
void ENET0_Err_IRQHandler(void);
#endif /* (ENET_INSTANCE_COUNT > 0U) */

#if (ENET_INSTANCE_COUNT > 1U)
void ENET1_Tx_IRQHandler(void);
void ENET1_Rx_IRQHandler(void);
void ENET1_Err_IRQHandler(void);
#endif /* (ENET_INSTANCE_COUNT > 1U) */

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (ENET_INSTANCE_COUNT > 0U)

/* Implementation of ENET0 IRQ handler for transmit interrupts. */
void ENET0_Tx_IRQHandler(void)
{
    ENET_TransmitIRQHandler(0);
}

/* Implementation of ENET0 IRQ handler for receive interrupts. */
void ENET0_Rx_IRQHandler(void)
{
    ENET_ReceiveIRQHandler(0);
}

/* Implementation of ENET0 IRQ handler for error interrupts. */
void ENET0_Err_IRQHandler(void)
{
    ENET_WakeIRQHandler(0);
    ENET_ErrorIRQHandler(0);
}

#endif /* (ENET_INSTANCE_COUNT > 0U) */

#if (ENET_INSTANCE_COUNT > 1U)

/* Implementation of ENET1 IRQ handler for transmit interrupts. */
void ENET1_Tx_IRQHandler(void)
{
    ENET_TransmitIRQHandler(1);
}

/* Implementation of ENET1 IRQ handler for receive interrupts. */
void ENET1_Rx_IRQHandler(void)
{
    ENET_ReceiveIRQHandler(1);
}

/* Implementation of ENET1 IRQ handler for error interrupts. */
void ENET1_Err_IRQHandler(void)
{
    ENET_WakeIRQHandler(1);
    ENET_ErrorIRQHandler(1);
}

#endif /* (ENET_INSTANCE_COUNT > 1U) */

#elif (defined(CPU_S32K148))

/*******************************************************************************
 * Default interrupt handlers signatures
 ******************************************************************************/
#if (ENET_INSTANCE_COUNT > 0U)
void ENET_TX_IRQHandler(void);
void ENET_RX_IRQHandler(void);
void ENET_ERR_IRQHandler(void);
void ENET_WAKE_IRQHandler(void);
#endif /* (ENET_INSTANCE_COUNT > 0U) */

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (ENET_INSTANCE_COUNT > 0U)

/* Implementation of ENET0 IRQ handler for transmit interrupts. */
void ENET_TX_IRQHandler(void)
{
    ENET_TransmitIRQHandler(0);
}

/* Implementation of ENET0 IRQ handler for receive interrupts. */
void ENET_RX_IRQHandler(void)
{
    ENET_ReceiveIRQHandler(0);
}

/* Implementation of ENET0 IRQ handler for error interrupts. */
void ENET_ERR_IRQHandler(void)
{
    ENET_ErrorIRQHandler(0);
}

/* Implementation of ENET0 IRQ handler for wakeup interrupts. */
void ENET_WAKE_IRQHandler(void)
{
    ENET_WakeIRQHandler(0);
}

#endif /* (ENET_INSTANCE_COUNT > 0U) */

#else
    #error "No valid CPU defined!"
#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
