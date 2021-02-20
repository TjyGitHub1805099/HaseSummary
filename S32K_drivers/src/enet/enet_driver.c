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
 * @file enet_driver.c
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
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * The symbol is declared in the hardware access file as external; it is needed
 * for accessing the installed callback, but is not a part of the public API.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro
 * Function-like macros are used instead of inline functions in order to ensure
 * that the performance will not be decreased if the functions will not be
 * inlined by the compiler.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.13, Pointer parameter could be declared as
 * pointing to const
 * This is a pointer to a buffer which is used by the internal Rx mechanism, and
 * the application must make no assumption that the content of the buffer will not
 * be modified.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable
 * The code is not dynamically linked. An absolute stack address is obtained when
 * taking the address of the near auto variable. A source of error in writing
 * dynamic code is that the stack segment may be different from the data segment.
 *
 */

#include "enet_driver.h"
#include "enet_hw_access.h"
#include "clock_manager.h"
#include "interrupt_manager.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ENET_ROUNDED_UP_DIV(a, b)   (((uint32_t)(a) + (uint32_t)(b) - 1UL) / (uint32_t)(b))
#define ENET_NS_IN_SECOND           (1000000000U)

#define ENET_ALL_INTERRUPTS_MASK    (0xFFFFFFFFU)

#define ENET_BYTE_0_IN_ADDR_SHIFT   (24U)
#define ENET_BYTE_1_IN_ADDR_SHIFT   (16U)
#define ENET_BYTE_2_IN_ADDR_SHIFT   (8U)
#define ENET_BYTE_3_IN_ADDR_SHIFT   (0U)
#define ENET_BYTE_4_IN_ADDR_SHIFT   (8U)
#define ENET_BYTE_5_IN_ADDR_SHIFT   (0U)

#define ENET_BYTE_MASK  (0xFFU)

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Pointers to ENET internal driver state for each instance. */
enet_state_t *g_enetState[ENET_INSTANCE_COUNT] = {NULL};

/*! @brief ENET transmit IRQ number for each instance. */
static const IRQn_Type s_enetTxIrqId[] = FEATURE_ENET_TX_IRQS;
/*! @brief ENET receive IRQ number for each instance. */
static const IRQn_Type s_enetRxIrqId[] = FEATURE_ENET_RX_IRQS;
/*! @brief ENET error IRQ number for each instance. */
static const IRQn_Type s_enetErrIrqId[] = FEATURE_ENET_ERR_IRQS;
#ifdef FEATURE_ENET_WAKEUP_IRQS
/*! @brief ENET wakeup IRQ number for each instance. */
static const IRQn_Type s_enetWakeupIrqId[] = FEATURE_ENET_WAKE_IRQS;
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static uint32_t ENET_DRV_ComputeCRC32(const uint8_t *mac);

/*******************************************************************************
 * Private functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_ComputeCRC32
 * Description   : Computes the CRC32 of a given MAC address.
 *
 *END**************************************************************************/
static uint32_t ENET_DRV_ComputeCRC32(const uint8_t *mac)
{
    uint32_t crc = 0xFFFFFFFFUL;
    uint8_t i, j;

    for (i = 0; i < 6U; i++)
    {
        crc = crc ^ mac[i];
        for (j = 0; j < 8U; j++)
        {
            if ((crc & 0x1U) != 0U)
            {
                crc = (crc >> 1U) ^ 0xEDB88320U;
            }
            else
            {
                crc = (crc >> 1U);
            }
        }
    }

    return crc;
}

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_GetDefaultConfig
 * Description   : Gets the default configuration structure
 *
 * Implements    : ENET_DRV_GetDefaultConfig_Activity
 *END**************************************************************************/
void ENET_DRV_GetDefaultConfig(enet_config_t *config)
{
    /* Checks input parameter. */
    DEV_ASSERT(config != NULL);

    /* No interrupt is enabled by default. */
    config->interrupts = 0;
    /* Maximum receive frame length. */
    config->maxFrameLen = ENET_FRAME_MAX_FRAMELEN;
    /* No special receive/transmit control configuration. */
    config->rxConfig = 0;
    config->txConfig = 0;
    /* No acceleration function enabled. */
    config->rxAccelerConfig = 0;
    config->txAccelerConfig = 0;
    /* RMII/MII mode, full duplex, 100Mbps for MAC and PHY data interface. */
    config->miiMode = FEATURE_ENET_DEFAULT_PHY_IF;
    config->miiSpeed = ENET_MII_SPEED_100M;
    config->miiDuplex = ENET_MII_FULL_DUPLEX;
    /* No callback. */
    config->callback = NULL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_Init
 * Description   : Initializes the ENET module
 *
 * This function initializes and enables the ENET module, configuring receive
 * and transmit control settings, the receive and transmit descriptors rings,
 * and the MAC physical address.
 *
 * Implements    : ENET_DRV_Init_Activity
 *END**************************************************************************/
void ENET_DRV_Init(uint8_t instance,
                   enet_state_t *state,
                   const enet_config_t *config,
                   const enet_buffer_config_t *bufferConfig,
                   const uint8_t *macAddr)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(state != NULL);
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(config->maxFrameLen >= ENET_MIN_BUFFERSIZE);
    DEV_ASSERT(bufferConfig != NULL);
    DEV_ASSERT(bufferConfig->rxRingAligned != NULL);
    DEV_ASSERT(ENET_BUFFDESCR_IS_ALIGNED(bufferConfig->rxRingAligned));
    DEV_ASSERT(bufferConfig->rxBufferAligned != NULL);
    DEV_ASSERT(ENET_BUFF_IS_ALIGNED(bufferConfig->rxBufferAligned));
    DEV_ASSERT(bufferConfig->rxRingSize > 0U);
    DEV_ASSERT(bufferConfig->txRingAligned != NULL);
    DEV_ASSERT(ENET_BUFFDESCR_IS_ALIGNED(bufferConfig->txRingAligned));
    DEV_ASSERT(bufferConfig->txRingSize > 0U);

    base = s_enetBases[instance];

    /* Reset ENET module. */
    ENET_Reset(base);

    /* Configure MAC address. */
    ENET_DRV_SetMacAddr(instance, macAddr);

    /* Configure receive and transmit control. */
    ENET_ConfigReceiveControl(base, config);
    ENET_ConfigTransmitControl(base, config);

    /* Configure receive and transmit accelerators. */
    ENET_ConfigReceiveAccelerator(base, config->rxAccelerConfig);
    ENET_ConfigTransmitAccelerator(base, config->txAccelerConfig);

    /* Enable Ethernet interrupts. */
    INT_SYS_EnableIRQ(s_enetRxIrqId[instance]);
    INT_SYS_EnableIRQ(s_enetTxIrqId[instance]);
    INT_SYS_EnableIRQ(s_enetErrIrqId[instance]);
#ifdef FEATURE_ENET_WAKEUP_IRQS
    INT_SYS_EnableIRQ(s_enetWakeupIrqId[instance]);
#endif

    ENET_EnableInterrupts(base, config->interrupts);

    /* Update state. */
    state->rxBdBase = bufferConfig->rxRingAligned;
    state->rxBdCurrent = bufferConfig->rxRingAligned;
    state->rxBdAlloc = bufferConfig->rxRingAligned;
    state->txBdBase = bufferConfig->txRingAligned;
    state->txBdCurrent = bufferConfig->txRingAligned;
    state->callback = config->callback;

    g_enetState[instance] = state;

    /* Configure buffer descriptors and indicate that the receive descriptor has been updated. */
    ENET_ConfigBufferDescriptors(base, config, bufferConfig, (uint16_t)ENET_BUFF_ALIGN(config->maxFrameLen));

    ENET_DRV_ConfigCounters(instance, true);

    /* Enable the module. */
    ENET_Enable(base);

    ENET_ActivateReceive(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_Deinit
 * Description   : Deinitializes the ENET module
 *
 * This function disables the interrupts and then disables the ENET module.
 *
 * Implements    : ENET_DRV_Deinit_Activity
 *END**************************************************************************/
void ENET_DRV_Deinit(uint8_t instance)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    /* Disable interrupts. */
    ENET_DisableInterrupts(base, ENET_ALL_INTERRUPTS_MASK);

    INT_SYS_DisableIRQ(s_enetRxIrqId[instance]);
    INT_SYS_DisableIRQ(s_enetTxIrqId[instance]);
    INT_SYS_DisableIRQ(s_enetErrIrqId[instance]);
#ifdef FEATURE_ENET_WAKEUP_IRQS
    INT_SYS_DisableIRQ(s_enetWakeupIrqId[instance]);
#endif

    /* Disable module. */
    ENET_Disable(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_ReadFrame
 * Description   : Reads a received Ethernet frame
 *
 * This function reads the first received Ethernet frame in the Rx queue. The
 * buffer received as parameter will be updated by the driver and the .data field
 * will point to a memory area containing the frame data.
 *
 * Important: Once the application finished processing the buffer, it needs to be
 * released using the ENET_DRV_ProvideRxBuff function.
 *
 * Implements    : ENET_DRV_ReadFrame_Activity
 *END**************************************************************************/
status_t ENET_DRV_ReadFrame(uint8_t instance,
                            enet_buffer_t * buff,
                            enet_rx_enh_info_t * info)
{
    enet_buffer_descriptor_t *bd;
    status_t status = STATUS_SUCCESS;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(g_enetState[instance] != NULL);
    DEV_ASSERT(buff != NULL);

    bd = g_enetState[instance]->rxBdCurrent;

    if ((bd->control & (ENET_BUFFDESCR_RX_EMPTY_MASK | ENET_BUFFDESCR_RX_INUSE_MASK)) != 0U)
    {
        status = STATUS_ENET_RX_QUEUE_EMPTY;
    }
    else
    {
        buff->data = bd->buffer;
        buff->length = bd->length;

        bd->control |= ENET_BUFFDESCR_RX_INUSE_MASK;

#if FEATURE_ENET_HAS_ENHANCED_BD
        if (info != NULL)
        {
            info->errMask = bd->enh1 & ENET_RX_ENH_ERR_MASK;
            info->ipv4Frag = ((bd->enh1 & ENET_RX_ENH1_FRAG_MASK) != 0UL);
            info->ipv6 = ((bd->enh1 & ENET_RX_ENH1_IPV6_MASK) != 0UL);
            info->unicast = ((bd->enh1 & ENET_RX_ENH1_UNICAST_MASK) != 0UL);
            info->vlanPrio = (uint8_t)((bd->enh1 & ENET_RX_ENH1_VPCP_MASK) >> ENET_RX_ENH1_VPCP_SHIFT);
            info->checksum = (uint16_t)(bd->enh2 & ENET_RX_ENH2_CSUM_MASK);
            info->headerLen = (uint8_t)((bd->enh2 & ENET_RX_ENH2_HLEN_MASK) >> ENET_RX_ENH2_HLEN_SHIFT);
            info->proto = (uint8_t)((bd->enh2 & ENET_RX_ENH2_PROTO_MASK) >> ENET_RX_ENH2_PROTO_SHIFT);
            info->timestamp = bd->timestamp;
        }
#else
        DEV_ASSERT(info == NULL);
        (void)info;
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */

        /* Update the current buffer descriptor. */
        if ((bd->control & ENET_BUFFDESCR_RX_WRAP_MASK) != 0U)
        {
           g_enetState[instance]->rxBdCurrent = g_enetState[instance]->rxBdBase;
        }
        else
        {
            g_enetState[instance]->rxBdCurrent++;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SendFrame
 * Description   : Sends an Ethernet frame
 *
 * This function sends an Ethernet frame, represented by the buffer received as
 * parameter.
 *
 * Implements    : ENET_DRV_SendFrame_Activity
 *END**************************************************************************/
status_t ENET_DRV_SendFrame(uint8_t instance,
                            const enet_buffer_t * buff)
{
    ENET_Type *base;
    enet_buffer_descriptor_t *bd;
    status_t status = STATUS_SUCCESS;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(g_enetState[instance] != NULL);
    DEV_ASSERT(buff != NULL);

    base = s_enetBases[instance];

    bd = g_enetState[instance]->txBdCurrent;

    if ((bd->control & ENET_BUFFDESCR_TX_READY_MASK) != 0U)
    {
        status = STATUS_ENET_TX_QUEUE_FULL;
    }
    else
    {
        /* Configure buffer descriptor. */
        bd->length = buff->length;
        bd->buffer = buff->data;
        bd->control |= (uint16_t)(ENET_BUFFDESCR_TX_READY_MASK | ENET_BUFFDESCR_TX_LAST_MASK);

        /* Activate the transmit buffer descriptor. */
        ENET_ActivateTransmit(base);

        /* Update the current buffer descriptor pointer. */
        if ((bd->control & ENET_BUFFDESCR_TX_WRAP_MASK) != 0U)
        {
            g_enetState[instance]->txBdCurrent = g_enetState[instance]->txBdBase;
        }
        else
        {
            g_enetState[instance]->txBdCurrent++;
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_GetTransmitStatus
 * Description   : Checks if the transmission of a buffer is complete
 *
 * This function checks if the transmission of the given buffer is complete.
 *
 * Implements    : ENET_DRV_GetTransmitStatus_Activity
 *END**************************************************************************/
status_t ENET_DRV_GetTransmitStatus(uint8_t instance,
                                    const enet_buffer_t * buff,
                                    enet_tx_enh_info_t * info)
{
    const enet_buffer_descriptor_t *bd;
    status_t status = STATUS_ENET_BUFF_NOT_FOUND;
    bool finished = false;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(g_enetState[instance] != NULL);
    DEV_ASSERT(buff != NULL);
#if (FEATURE_ENET_HAS_ENHANCED_BD == 0)
    DEV_ASSERT(info == NULL);
#endif /* !FEATURE_ENET_HAS_ENHANCED_BD */

    bd = g_enetState[instance]->txBdBase;

    /* Search for the buffer descriptor associated with the given buffer. */
    while (!finished)
    {
        if (bd->buffer == buff->data)
        {
            /* Check if the buffer descriptor is still in use. */
            if ((bd->control & ENET_BUFFDESCR_TX_READY_MASK) != 0U)
            {
                status = STATUS_BUSY;
            }
            else
            {
                status = STATUS_SUCCESS;
#if FEATURE_ENET_HAS_ENHANCED_BD
                if (info != NULL)
                {
                    info->errMask = bd->enh1 & ENET_TX_ENH_ERR_MASK;
                    info->timestamp = bd->timestamp;
                }
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */
            }

            break;
        }

        /* This was the last buffer in the ring. */
        if ((bd->control & ENET_BUFFDESCR_TX_WRAP_MASK) != 0U)
        {
            finished = true;
        }

        bd++;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_ProvideRxBuff
 * Description   : Provides a receive buffer to be used by the driver for reception.
 *
 * This function provides a buffer which can further be used by the reception
 * mechanism in order to store the received data.
 *
 * Implements    : ENET_DRV_ProvideRxBuff_Activity
 *END**************************************************************************/
void ENET_DRV_ProvideRxBuff(uint8_t instance,
                            enet_buffer_t * buff)
{
    ENET_Type *base;
    enet_buffer_descriptor_t *bd;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(g_enetState[instance] != NULL);
    DEV_ASSERT(buff != NULL);

    base = s_enetBases[instance];

    /* This is the left-most allocated buffer descriptor. */
    bd = g_enetState[instance]->rxBdAlloc;

    DEV_ASSERT((bd->control & ENET_BUFFDESCR_RX_EMPTY_MASK) == 0U);

    bd->buffer = buff->data;

    /* Cleanup the left-most allocated buffer descriptor. */
    bd->control &= ENET_BUFFDESCR_RX_WRAP_MASK;
    bd->control |= ENET_BUFFDESCR_RX_EMPTY_MASK;

    /* Update the pointer to the first allocated buffer descriptor. */
    if ((bd->control & ENET_BUFFDESCR_RX_WRAP_MASK) != 0U)
    {
        g_enetState[instance]->rxBdAlloc = g_enetState[instance]->rxBdBase;
    }
    else
    {
        g_enetState[instance]->rxBdAlloc++;
    }

    /* New empty buffers have been produced */
    ENET_ActivateReceive(base);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_EnableMDIO
 * Description   : Enables the MDIO interface
 *
 * Implements    : ENET_DRV_EnableMDIO_Activity
 *END**************************************************************************/
void ENET_DRV_EnableMDIO(uint8_t instance,
                         bool miiPreambleDisabled)
{
    ENET_Type *base;
    uint32_t moduleClk, holdTime, miiSpeed;
    uint32_t config;
    const clock_names_t s_enetClkNames[] = FEATURE_ENET_CLOCK_NAMES;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    (void)CLOCK_SYS_GetFreq(s_enetClkNames[instance], &moduleClk);

    DEV_ASSERT(moduleClk != 0U);

    holdTime = ENET_ROUNDED_UP_DIV(FEATURE_ENET_MDIO_MIN_HOLD_TIME_NS * moduleClk, ENET_NS_IN_SECOND) - 1U;
    miiSpeed = ENET_ROUNDED_UP_DIV(moduleClk, (2U * FEATURE_ENET_MDC_MAX_FREQUENCY)) - 1U;

    config = ENET_MSCR_MII_SPEED(miiSpeed) | ENET_MSCR_DIS_PRE(miiPreambleDisabled ? 1UL : 0UL) | ENET_MSCR_HOLDTIME(holdTime);
    ENET_WriteManagementConfig(base, config);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_MDIOWrite
 * Description   : Writes the selected register of the PHY
 *
 * Implements    : ENET_DRV_MDIOWrite_Activity
 *END**************************************************************************/
status_t ENET_DRV_MDIOWrite(uint8_t instance,
                            uint8_t phyAddr,
                            uint8_t phyReg,
                            uint16_t data,
                            uint32_t timeoutMs)
{
    ENET_Type *base;
    uint32_t startTime, crtTime;
    status_t status = STATUS_TIMEOUT;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    /* Write management frame */
    ENET_WriteManagementFrame(base, phyAddr, phyReg, ENET_MMFR_OP_WRITE, data);

    /* Wait for completion */
    startTime = OSIF_GetMilliseconds();
    do
    {
        if ((ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK) != 0U)
        {
            status = STATUS_SUCCESS;
            /* Clear the flag */
            ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);
            break;
        }
        crtTime = OSIF_GetMilliseconds();
    }
    while (crtTime < (startTime + timeoutMs));

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_MDIORead
 * Description   : Reads the selected register of the PHY
 *
 * Implements    : ENET_DRV_MDIORead_Activity
 *END**************************************************************************/
status_t ENET_DRV_MDIORead(uint8_t instance,
                           uint8_t phyAddr,
                           uint8_t phyReg,
                           uint16_t *data,
                           uint32_t timeoutMs)
{
    ENET_Type *base;
    uint32_t startTime, crtTime;
    status_t status = STATUS_TIMEOUT;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    /* Write management frame */
    ENET_WriteManagementFrame(base, phyAddr, phyReg, ENET_MMFR_OP_READ, 0);

    /* Wait for completion */
    startTime = OSIF_GetMilliseconds();
    do
    {
        if ((ENET_GetInterruptStatus(base) & ENET_EIR_MII_MASK) != 0U)
        {
            *data = ENET_ReadManagementFrameData(base);
            status = STATUS_SUCCESS;
            /* Clear the flag */
            ENET_ClearInterruptStatus(base, ENET_EIR_MII_MASK);
            break;
        }
        crtTime = OSIF_GetMilliseconds();
    }
    while (crtTime < (startTime + timeoutMs));

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetMacAddr
 * Description   : Configures the physical address of the MAC
 *
 * Implements    : ENET_DRV_SetMacAddr_Activity
 *END**************************************************************************/
void ENET_DRV_SetMacAddr(uint8_t instance,
                         const uint8_t *macAddr)
{
    uint32_t address;
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    /* Set physical address lower register. */
    address = ((uint32_t)macAddr[0] << ENET_BYTE_0_IN_ADDR_SHIFT) |
              ((uint32_t)macAddr[1] << ENET_BYTE_1_IN_ADDR_SHIFT) |
              ((uint32_t)macAddr[2] << ENET_BYTE_2_IN_ADDR_SHIFT) |
              ((uint32_t)macAddr[3] << ENET_BYTE_3_IN_ADDR_SHIFT);
    ENET_SetPhyAddrLower(base, address);

    /* Set physical address high register. */
    address = ((uint32_t)macAddr[4] << ENET_BYTE_4_IN_ADDR_SHIFT) |
              ((uint32_t)macAddr[5] << ENET_BYTE_5_IN_ADDR_SHIFT);

    ENET_SetPhyAddrUpper(base, address);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_GetMacAddr
 * Description   : Gets the physical address of the MAC
 *
 * Implements    : ENET_DRV_GetMacAddr_Activity
 *END**************************************************************************/
void ENET_DRV_GetMacAddr(uint8_t instance,
                         uint8_t *macAddr)
{
    const ENET_Type *base;
    uint32_t address;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(macAddr != NULL);

    base = s_enetBases[instance];

    /* Get from physical address lower register. */
    address = ENET_GetPhyAddrLower(base);
    macAddr[0] = (uint8_t)(ENET_BYTE_MASK & (address >> ENET_BYTE_0_IN_ADDR_SHIFT));
    macAddr[1] = (uint8_t)(ENET_BYTE_MASK & (address >> ENET_BYTE_1_IN_ADDR_SHIFT));
    macAddr[2] = (uint8_t)(ENET_BYTE_MASK & (address >> ENET_BYTE_2_IN_ADDR_SHIFT));
    macAddr[3] = (uint8_t)(ENET_BYTE_MASK & (address >> ENET_BYTE_3_IN_ADDR_SHIFT));

    /* Get from physical address high register. */
    address = ENET_GetPhyAddrUpper(base);
    macAddr[4] = (uint8_t)(ENET_BYTE_MASK & (address >> ENET_BYTE_4_IN_ADDR_SHIFT));
    macAddr[5] = (uint8_t)(ENET_BYTE_MASK & (address >> ENET_BYTE_5_IN_ADDR_SHIFT));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetUnicastForward
 * Description   : Enables/Disables forwarding of unicast traffic having a
 * specific MAC address as destination.
 *
 * Implements    : ENET_DRV_SetUnicastForward_Activity
 *END**************************************************************************/
void ENET_DRV_SetUnicastForward(uint8_t instance,
                                const uint8_t *macAddr,
                                bool enable)
{
    ENET_Type *base;
    uint32_t crc;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(macAddr != NULL);

    base = s_enetBases[instance];
    crc = ENET_DRV_ComputeCRC32(macAddr);

    if (enable)
    {
        ENET_AddToIndividualHashTable(base, crc);
    }
    else
    {
        ENET_RemoveFromIndividualHashTable(base, crc);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetMulticastForward
 * Description   : Enables/Disables forwarding of multicast traffic having a
 * specific MAC address as destination.
 *
 * Implements    : ENET_DRV_SetMulticastForward_Activity
 *END**************************************************************************/
void ENET_DRV_SetMulticastForward(uint8_t instance,
                                  const uint8_t *macAddr,
                                  bool enable)
{
    ENET_Type *base;
    uint32_t crc;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);
    DEV_ASSERT(macAddr != NULL);

    base = s_enetBases[instance];
    crc = ENET_DRV_ComputeCRC32(macAddr);

    if (enable)
    {
        ENET_AddToGroupHashTable(base, crc);
    }
    else
    {
        ENET_RemoveFromGroupHashTable(base, crc);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetMulticastForwardAll
 * Description   : Enables/Disables forwarding of the multicast traffic, irrespective
 * of the destination MAC address.
 *
 * Implements    : ENET_DRV_SetMulticastForwardAll_Activity
 *END**************************************************************************/
void ENET_DRV_SetMulticastForwardAll(uint8_t instance,
                                     bool enable)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    if (enable)
    {
        base->GAUR = 0xFFFFFFFFU;
        base->GALR = 0xFFFFFFFFU;
    }
    else
    {
        base->GAUR = 0x0U;
        base->GALR = 0x0U;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_SetSleepMode
 * Description   : Sets the MAC in sleep mode or normal mode
 *
 * Implements    : ENET_DRV_SetSleepMode_Activity
 *END**************************************************************************/
void ENET_DRV_SetSleepMode(uint8_t instance,
                           bool enable)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base  = s_enetBases[instance];

    if (enable)
    {
        base->ECR |= ENET_ECR_SLEEP_MASK | ENET_ECR_MAGICEN_MASK;
    }
    else
    {
        base->ECR &= ~(ENET_ECR_SLEEP_MASK | ENET_ECR_MAGICEN_MASK);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_ConfigCounters
 * Description   : Enables/Disables the MIB counters
 *
 * Implements    : ENET_DRV_ConfigCounters_Activity
 *END**************************************************************************/
void ENET_DRV_ConfigCounters(uint8_t instance,
                             bool enable)
{
    ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    if (enable)
    {
        /* Clear counters - MIB_CLEAR bit needs to be set, then cleared */
        base->MIBC |= ENET_MIBC_MIB_CLEAR_MASK;
        (void) base->MIBC;
        base->MIBC &= ~ENET_MIBC_MIB_CLEAR_MASK;

        base->MIBC &= ~ENET_MIBC_MIB_DIS_MASK;
    }
    else
    {
    	base->MIBC |= ENET_MIBC_MIB_DIS_MASK;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : ENET_DRV_GetCounter
 * Description   : Gets statistics from the specified counter
 *
 * Implements    : ENET_DRV_GetCounter_Activity
 *END**************************************************************************/
uint32_t ENET_DRV_GetCounter(uint8_t instance,
                             enet_counter_t counter)
{
	const ENET_Type *base;

    DEV_ASSERT(instance <  ENET_INSTANCE_COUNT);

    base = s_enetBases[instance];

    return (&(base->RMON_T_DROP))[(uint32_t)counter];
}

/*! @} */

/*******************************************************************************
 * EOF
 ******************************************************************************/
