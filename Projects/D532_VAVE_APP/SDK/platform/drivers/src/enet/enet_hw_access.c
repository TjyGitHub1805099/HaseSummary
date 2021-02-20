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
 * @file enet_hw_access.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned long,
 * cast from unsigned long to pointer and cast from unsigned int to pointer.
 * The cast is required to perform a conversion between a pointer
 * and an unsigned long define, representing an address or vice versa.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 */

#include "enet_hw_access.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief MDC frequency. */
#define FEATURE_ENET_MDC_MAX_FREQUENCY 2500000U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void ENET_SetTxBufferDescriptors(const enet_config_t *config,
                                        enet_buffer_descriptor_t *txBdStartAlign,
                                        uint32_t txRingSize);

static void ENET_SetRxBufferDescriptors(const enet_config_t *config,
                                        enet_buffer_descriptor_t *rxBdStartAlign,
                                        uint32_t rxRingSize,
                                        uint8_t *rxBuffStartAlign,
                                        uint16_t buffSize);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Pointers to enet handles for each instance. */
extern enet_state_t *g_enetState[ENET_INSTANCE_COUNT];

/*******************************************************************************
 * Private functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_SetTxBufferDescriptors
 * Description   : Configures the software transmit buffer descriptors.
 *
 *END**************************************************************************/
static void ENET_SetTxBufferDescriptors(const enet_config_t *config,
                                        enet_buffer_descriptor_t *txBdStartAlign,
                                        uint32_t txRingSize)
{
    uint32_t count;
    enet_buffer_descriptor_t *curBuffDescrip = txBdStartAlign;

    for (count = 0; count < txRingSize; count++)
    {
        curBuffDescrip->control = ENET_BUFFDESCR_TX_TRANSMITCRC_MASK;

#if FEATURE_ENET_HAS_ENHANCED_BD
        curBuffDescrip->enh1 = 0UL;
        curBuffDescrip->enh2 = 0UL;
        curBuffDescrip->enh3 = 0UL;
        if ((config->interrupts & ENET_TX_INTERRUPTS_MASK) != 0UL)
        {
            curBuffDescrip->enh1 |= ENET_TX_ENH1_INT_MASK;
        }
        if ((config->txAccelerConfig & ENET_TACC_IPCHK_MASK) != 0U)
        {
            curBuffDescrip->enh1 |= ENET_TX_ENH1_IINS_MASK;
        }
        if ((config->txAccelerConfig & ENET_TACC_PROCHK_MASK) != 0U)
        {
            curBuffDescrip->enh1 |= ENET_TX_ENH1_PINS_MASK;
        }
        curBuffDescrip->reserved1 = 0UL;
        curBuffDescrip->reserved2 = 0UL;
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */

        /* Sets the last buffer descriptor with the wrap flag. */
        if (count == (txRingSize - 1U))
        {
            curBuffDescrip->control |= ENET_BUFFDESCR_TX_WRAP_MASK;
        }

        /* Increase the index. */
        curBuffDescrip++;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_SetRxBufferDescriptors
 * Description   : Configures the software receive buffer descriptors.
 *
 *END**************************************************************************/
static void ENET_SetRxBufferDescriptors(const enet_config_t *config,
                                        enet_buffer_descriptor_t *rxBdStartAlign,
                                        uint32_t rxRingSize,
                                        uint8_t *rxBuffStartAlign,
                                        uint16_t buffSize)
{
    enet_buffer_descriptor_t *curBuffDescrip = rxBdStartAlign;
    uint32_t count = 0;

    /* Initializes receive buffer descriptors. */
    for (count = 0; count < rxRingSize; count++)
    {
        /* Set data buffer and the length. */
        curBuffDescrip->buffer = &rxBuffStartAlign[count * buffSize];
        curBuffDescrip->length = 0;

        /* Initializes the buffer descriptors with empty bit. */
        curBuffDescrip->control = ENET_BUFFDESCR_RX_EMPTY_MASK;

#if FEATURE_ENET_HAS_ENHANCED_BD
        curBuffDescrip->enh1 = 0UL;
        curBuffDescrip->enh2 = 0UL;
        curBuffDescrip->enh3 = 0UL;
        if ((config->interrupts & ENET_RX_INTERRUPTS_MASK) != 0UL)
        {
            curBuffDescrip->enh1 |= ENET_RX_ENH1_INT_MASK;
        }
        curBuffDescrip->reserved1 = 0UL;
        curBuffDescrip->reserved2 = 0UL;
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */

        /* Sets the last buffer descriptor with the wrap flag. */
        if (count == (rxRingSize - 1U))
        {
            curBuffDescrip->control |= ENET_BUFFDESCR_RX_WRAP_MASK;
        }

        /* Increase the index. */
        curBuffDescrip++;
    }
}

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_ConfigBufferDescriptors
 * Description   : Configures the receive and transmit buffer descriptors.
 *
 *END**************************************************************************/
void ENET_ConfigBufferDescriptors(ENET_Type * base,
                                  const enet_config_t *config,
                                  const enet_buffer_config_t *bufferConfig,
                                  uint16_t buffSize)
{
    /* Initializes the ENET transmit buffer descriptors. */
    ENET_SetTxBufferDescriptors(config, bufferConfig->txRingAligned, bufferConfig->txRingSize);

    /* Initializes the ENET receive buffer descriptors. */
    ENET_SetRxBufferDescriptors(config, bufferConfig->rxRingAligned, bufferConfig->rxRingSize,
                                bufferConfig->rxBufferAligned, buffSize);

    /* Initializes transmit buffer descriptor rings start address, two start address should be aligned. */
    base->TDSR = (uint32_t)bufferConfig->txRingAligned;
    base->RDSR = (uint32_t)bufferConfig->rxRingAligned;
    /* Initializes the maximum buffer size, the buffer size should be aligned. */
    base->MRBR = buffSize;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_ConfigReceiveControl
 * Description   : Configures the receive block.
 *
 *END**************************************************************************/
void ENET_ConfigReceiveControl(ENET_Type *base,
                               const enet_config_t *config)
{
    uint32_t rcr = 0;

    /* Configures MAC receive controller with user configure structure. */
    rcr = ENET_RCR_NLC(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_ENABLE_PAYLOAD_LEN_CHECK) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_CFEN(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_ENABLE_FLOW_CONTROL) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_CRCFWD(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_STRIP_CRC_FIELD) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_FCE(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_ENABLE_FLOW_CONTROL) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_PAUFWD(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_FORWARD_PAUSE_FRAMES) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_PADEN(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_REMOVE_PADDING) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_BC_REJ(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_REJECT_BROADCAST_FRAMES) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_PROM(((config->rxConfig & (uint32_t)ENET_RX_CONFIG_ENABLE_PROMISCUOUS_MODE) != 0UL) ? 1UL : 0UL) |
          ENET_RCR_MII_MODE(1U) |
          ENET_RCR_RMII_MODE(config->miiMode) |
          ENET_RCR_RMII_10T(config->miiSpeed) |
          ENET_RCR_MAX_FL(config->maxFrameLen);

    /* Receive setting for half duplex. */
    if (config->miiDuplex == ENET_MII_HALF_DUPLEX)
    {
        rcr |= ENET_RCR_DRT(1);
    }
    /* Sets internal loopback. */
    if ((config->rxConfig & (uint32_t)ENET_RX_CONFIG_ENABLE_MII_LOOPBACK) != 0U)
    {
        rcr |= ENET_RCR_LOOP(1);
        rcr &= ~ENET_RCR_DRT_MASK;
    }
    base->RCR = rcr;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_ConfigTransmitControl
 * Description   : Configures the transmit block.
 *
 *END**************************************************************************/
void ENET_ConfigTransmitControl(ENET_Type *base,
                                const enet_config_t *config)
{
    uint32_t tcr = base->TCR;

    /* Configures MAC transmit controller: duplex mode, mac address insertion. */
    tcr &= ~(ENET_TCR_FDEN_MASK | ENET_TCR_CRCFWD_MASK | ENET_TCR_ADDINS_MASK);
    tcr |= ENET_TCR_FDEN(config->miiDuplex) |
           ENET_TCR_CRCFWD(((config->txConfig & (uint32_t)ENET_TX_CONFIG_DISABLE_CRC_APPEND) != 0UL) ? 1UL : 0UL) |
           ENET_TCR_ADDINS(((config->txConfig & (uint32_t)ENET_TX_CONFIG_ENABLE_MAC_ADDR_INSERTION) != 0UL) ? 1UL : 0UL);

    base->TCR = tcr;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_TransmitIRQHandler
 * Description   : Handler for ENET transmission interrupts.
 * This handler clears the interrupt flags and invokes the installed callback,
 * if available.
 *
 *END**************************************************************************/
void ENET_TransmitIRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    /* Check if the transmit interrupt happen. */
    if ((((uint32_t)ENET_TX_BUFFER_INTERRUPT | (uint32_t)ENET_TX_FRAME_INTERRUPT) & base->EIR) != 0U)
    {
        /* Clear the transmit interrupt event. */
        base->EIR = (uint32_t)ENET_TX_FRAME_INTERRUPT | (uint32_t)ENET_TX_BUFFER_INTERRUPT;
        /* Callback function. */
        if (g_enetState[instance]->callback != NULL)
        {
            g_enetState[instance]->callback(instance, ENET_TX_EVENT);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_ReceiveIRQHandler
 * Description   : Handler for ENET reception interrupts.
 * This handler clears the interrupt flags, extracts the received frame and
 * invokes the installed callback, if available. After the callback completes,
 * the buffers will be marked as empty.
 *
 *END**************************************************************************/
void ENET_ReceiveIRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    /* Check if the receive interrupt happen. */
    if ((((uint32_t)ENET_RX_BUFFER_INTERRUPT | (uint32_t)ENET_RX_FRAME_INTERRUPT) & base->EIR) != 0U)
    {
        /* Clear the transmit interrupt event. */
        base->EIR = (uint32_t)ENET_RX_FRAME_INTERRUPT | (uint32_t)ENET_RX_BUFFER_INTERRUPT;

        /* Callback function. */
        if (g_enetState[instance]->callback != NULL)
        {
            g_enetState[instance]->callback(instance, ENET_RX_EVENT);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_ErrorIRQHandler
 * Description   : Handler for ENET error interrupts.
 * This handler clears the interrupt flags and invokes the installed callback,
 * if available.
 *
 *END**************************************************************************/
void ENET_ErrorIRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    uint32_t errMask = (uint32_t)ENET_BABR_INTERRUPT | (uint32_t)ENET_BABT_INTERRUPT | (uint32_t)ENET_EBERR_INTERRUPT |
                       (uint32_t)ENET_PAYLOAD_RX_INTERRUPT | (uint32_t)ENET_LATE_COLLISION_INTERRUPT |
                       (uint32_t)ENET_RETRY_LIMIT_INTERRUPT | (uint32_t)ENET_UNDERRUN_INTERRUPT;

    if ((errMask & base->EIR) != 0U)
    {
        errMask &= base->EIR;
        base->EIR = errMask;
        /* Callback function. */
        if (g_enetState[instance]->callback != NULL)
        {
            g_enetState[instance]->callback(instance, ENET_ERR_EVENT);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_WakeIRQHandler
 * Description   : Handler for ENET wakeup interrupts.
 * This handler clears the interrupt flags and invokes the installed callback,
 * if available. Also, the ENET module is set to normal operation mode.
 *
 *END**************************************************************************/
void ENET_WakeIRQHandler(uint8_t instance)
{
    ENET_Type *base = s_enetBases[instance];

    if (((uint32_t)ENET_WAKEUP_INTERRUPT & base->EIR) != 0U)
    {
        /* Clear the wakeup interrupt. */
        base->EIR = (uint32_t)ENET_WAKEUP_INTERRUPT;
        ENET_DRV_SetSleepMode(instance, false);
        /* Callback function. */
        if (g_enetState[instance]->callback != NULL)
        {
            g_enetState[instance]->callback(instance, ENET_WAKE_UP_EVENT);
        }
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
