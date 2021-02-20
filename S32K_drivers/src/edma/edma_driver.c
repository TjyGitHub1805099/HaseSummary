/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
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

/*!
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in
 * writing dynamic code is that the stack segment may be different from the data
 * segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, Could define variable at block scope
 * The variable is used in all flexio drivers so it must remain global.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to perform a conversion between a pointer and an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned long, Cast from unsigned long to pointer.
 * The cast is required to perform a conversion between a pointer and an unsigned long define,
 * representing an address.
 */

#include "edma_irq.h"
#include "clock_manager.h"
#include "interrupt_manager.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Array of base addresses for DMA instances. */
static DMA_Type * const s_edmaBase[DMA_INSTANCE_COUNT] = DMA_BASE_PTRS;

/*! @brief Array of base addresses for DMAMUX instances. */
static DMAMUX_Type * const  s_dmamuxBase[DMAMUX_INSTANCE_COUNT] = DMAMUX_BASE_PTRS;

/*! @brief Array of default DMA channel interrupt handlers. */
static const IRQn_Type s_edmaIrqId[FEATURE_CHANNEL_INTERRUPT_LINES] = DMA_CHN_IRQS;

/*! @brief Array of default DMA error interrupt handlers. */
#if defined FEATURE_EDMA_HAS_ERROR_IRQ
static const IRQn_Type s_edmaErrIrqId[FEATURE_ERROR_INTERRUPT_LINES] = DMA_ERROR_IRQS;
#endif

/*! @brief Array for eDMA & DMAMUX clock sources. */
static const clock_names_t s_edmaClockNames[DMA_INSTANCE_COUNT] = EDMA_CLOCK_NAMES;
static const clock_names_t s_dmamuxClockNames[DMAMUX_INSTANCE_COUNT] = DMAMUX_CLOCK_NAMES;

/*! @brief EDMA global structure to maintain eDMA state */
static edma_state_t *s_edma = NULL;

/*******************************************************************************
 * PROTOTYPES
 ******************************************************************************/
static status_t EDMA_DRV_RequestChannel(uint8_t requestedChannel, dma_request_source_t source,
                                        edma_chn_state_t *chn);
static void EDMA_DRV_ClearIntStatus(uint8_t channel);
static void EDMA_DRV_ClearSoftwareTCD(edma_software_tcd_t * stcd);
#ifdef DEV_ERROR_DETECT
static bool EDMA_DRV_ValidTransferSize(edma_transfer_size_t size);
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_Init
 * Description   : Initializes the eDMA module.
 *
 * Implements    : EDMA_DRV_Init_Activity
 *END**************************************************************************/
status_t EDMA_DRV_Init(edma_state_t *edmaState, const edma_user_config_t *userConfig,
                       edma_chn_state_t * const chnStateArray[],
                       const edma_channel_config_t * const chnConfigArray[],
                       uint8_t chnCount)
{
    uint32_t i;
    DMA_Type * edmaRegBase;
    IRQn_Type irqNumber;
    status_t edmaStatus = STATUS_SUCCESS;
    status_t chnInitStatus;
#ifdef DEV_ERROR_DETECT
    uint32_t freq;
    status_t clockManagerStatus;
#endif

    /* Check the state and configuration structure pointers are valid */
    DEV_ASSERT((edmaState != NULL) && (userConfig != NULL));

    /* Check the module has not already been initialized */
    DEV_ASSERT(s_edma == NULL);

#ifdef DEV_ERROR_DETECT
    /* Check that eDMA and DMAMUX modules are clock gated on */
    clockManagerStatus = CLOCK_SYS_GetFreq(s_edmaClockNames[0U], &freq);
    DEV_ASSERT(clockManagerStatus == STATUS_SUCCESS);

    for (i = 0U; i < DMAMUX_INSTANCE_COUNT; i++)
    {
        clockManagerStatus = CLOCK_SYS_GetFreq(s_dmamuxClockNames[i], &freq);
        DEV_ASSERT(clockManagerStatus == STATUS_SUCCESS);
    }
#endif

    /* Save the runtime state structure for the driver */
    s_edma = edmaState;

    /* Clear the state structure. */
    volatile uint8_t *clearStructPtr = (volatile uint8_t *)s_edma;
    size_t clearSize = sizeof(edma_state_t);
    while (clearSize > 0U)
    {
        *clearStructPtr = 0;
        clearStructPtr ++;
        clearSize --;
    }

    edmaRegBase = s_edmaBase[0U];

    /* Init eDMA module on hardware level. */
    EDMA_Init(edmaRegBase);

    /* Set arbitration mode */
    EDMA_SetChannelArbitrationMode(edmaRegBase, userConfig->chnArbitration);
#if (FEATURE_EDMA_CHANNEL_GROUP_COUNT > 0x1U)
    EDMA_SetGroupArbitrationMode(edmaRegBase, userConfig->groupArbitration);
    EDMA_SetGroupPriority(edmaRegBase, userConfig->groupPriority);
#endif

    /* Set 'Halt on error' configuration */
    EDMA_SetHaltOnErrorCmd(edmaRegBase, !userConfig->notHaltOnError);

#if defined FEATURE_EDMA_HAS_ERROR_IRQ
    /* Enable the error interrupts for eDMA module. */
    for (i = 0U; i < FEATURE_ERROR_INTERRUPT_LINES; i++)
    {
        /* Enable channel interrupt ID. */
        irqNumber = s_edmaErrIrqId[i];
        INT_SYS_EnableIRQ(irqNumber);
    }
#endif

    /* Register all edma channel interrupt handler into vector table. */
    for (i = 0U; i < FEATURE_CHANNEL_INTERRUPT_LINES; i++)
    {
        /* Enable channel interrupt ID. */
        irqNumber = s_edmaIrqId[i];
        INT_SYS_EnableIRQ(irqNumber);
    }

    /* Initialize all DMAMUX instances */
    for (i = 0U; i < DMAMUX_INSTANCE_COUNT; i++)
    {
        DMAMUX_Init(s_dmamuxBase[i]);
    }

    /* Initialize the channels based on configuration list */
    if ((chnStateArray != NULL) && (chnConfigArray != NULL))
    {
        for (i = 0U; i < chnCount; i++)
        {
            chnInitStatus = EDMA_DRV_ChannelInit(chnStateArray[i], chnConfigArray[i]);
            if (chnInitStatus != STATUS_SUCCESS)
            {
                edmaStatus = chnInitStatus;
            }
        }
    }

    return edmaStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_Deinit
 * Description   : Deinitialize EDMA.
 *
 * Implements    : EDMA_DRV_Deinit_Activity
 *END**************************************************************************/
status_t EDMA_DRV_Deinit(void)
{
    uint32_t i;
    IRQn_Type irqNumber;
    const edma_chn_state_t *chn;

    /* Release all edma channel. */
#if defined FEATURE_EDMA_HAS_ERROR_IRQ
    /* Disable the error interrupts for eDMA module. */
    for (i = 0U; i < FEATURE_ERROR_INTERRUPT_LINES; i++)
    {
        /* Enable channel interrupt ID. */
        irqNumber = s_edmaErrIrqId[i];
        INT_SYS_DisableIRQ(irqNumber);
    }
#endif
    if (s_edma != NULL)
    {
        for (i = 0U; i < FEATURE_EDMA_MODULE_CHANNELS; i++)
        {
            /* Release all channels. */
            chn = s_edma->chn[i];
            if (chn != NULL)
            {
                (void) EDMA_DRV_ReleaseChannel(chn->channel);
            }
        }
        for (i = 0U; i < FEATURE_CHANNEL_INTERRUPT_LINES; i++)
        {
            /* Disable channel interrupts. */
            irqNumber = s_edmaIrqId[i];
            INT_SYS_DisableIRQ(irqNumber);
        }

        s_edma = NULL;
    }

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_ChannelInit
 * Description   : Initialize EDMA channel.
 *
 * Implements    : EDMA_DRV_ChannelInit_Activity
 *END**************************************************************************/
status_t EDMA_DRV_ChannelInit(edma_chn_state_t *edmaChannelState, const edma_channel_config_t *edmaChannelConfig)
{
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    status_t status;

    /* Check the state and configuration structure pointers are valid */
    DEV_ASSERT((edmaChannelState != NULL) && (edmaChannelConfig != NULL));
    /* Check if the module is initialized */
    DEV_ASSERT(s_edma != NULL);
    /* Check if the channel defined by user in the channel configuration structure is valid */
    DEV_ASSERT(edmaChannelConfig->channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Request the channel */
    status = EDMA_DRV_RequestChannel(edmaChannelConfig->channel, edmaChannelConfig->source, edmaChannelState);

    if (status == STATUS_SUCCESS)
    {
        /* Set the channel priority, as defined in the configuration, only if fixed arbitration mode is selected */
        if ((EDMA_GetChannelArbitrationMode(edmaRegBase) == EDMA_ARBITRATION_FIXED_PRIORITY) &&
            (edmaChannelConfig->priority != EDMA_CHN_DEFAULT_PRIORITY))
        {
            EDMA_SetChannelPriority(edmaRegBase, edmaChannelConfig->channel, edmaChannelConfig->priority);
        }

        /* Install the user callback */
        status = EDMA_DRV_InstallCallback(edmaChannelConfig->channel, edmaChannelConfig->callback,
                                          edmaChannelConfig->callbackParam);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_InstallCallback
 * Description   : Register callback function and parameter.
 *
 * Implements    : EDMA_DRV_InstallCallback_Activity
 *END**************************************************************************/
status_t EDMA_DRV_InstallCallback(uint8_t channel, edma_callback_t callback, void *parameter)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    edma_chn_state_t * chnState = s_edma->chn[channel];
    /* Check the channel is allocated */
    DEV_ASSERT(chnState != NULL);

    chnState->callback = callback;
    chnState->parameter = parameter;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_RequestChannel
 * Description   : Request an eDMA channel.
 *
 *END**************************************************************************/
static status_t EDMA_DRV_RequestChannel(uint8_t requestedChannel, dma_request_source_t source,
                                        edma_chn_state_t *chn)
{
    /* Retrieve the DMAMUX instance serving this channel */
    uint8_t dmamuxInstance = (uint8_t)FEATURE_DMAMUX_REQ_SRC_TO_INSTANCE(source);
    /* Retrieve the appropriate request source index for the corresponding DMAMUX instance */
    uint8_t sourceDmamuxChannel = (uint8_t)FEATURE_DMAMUX_REQ_SRC_TO_CHN((uint8_t)source);
    /* Retrieve the appropriate DMAMUX channel for the selected request source */
    uint8_t dmamuxChannel = (uint8_t)FEATURE_DMAMUX_CHN_FOR_EDMA_CHN(requestedChannel);
    /* Base addresses for eDMA and DMAMUX */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    DMAMUX_Type * dmamuxRegBase = s_dmamuxBase[dmamuxInstance];

    /* Check the channel has not already been allocated */
    DEV_ASSERT(s_edma->chn[requestedChannel] == NULL);

    /* Channel allocation */
    s_edma->chn[requestedChannel] = chn;

    /* Reset the channel state structure to default value. */
    uint8_t *clearStructPtr = (uint8_t *)chn;
    size_t clearSize = sizeof(edma_chn_state_t);
    while (clearSize > 0U)
    {
        *clearStructPtr = 0;
        clearStructPtr ++;
        clearSize --;
    }

    /* Init the channel state structure to the allocated channel number. */
    chn->channel = requestedChannel;

    /* Enable error interrupt for this channel */
    EDMA_SetErrorIntCmd(edmaRegBase, requestedChannel, true);

    /* Configure the DMAMUX for edma channel */
    DMAMUX_SetChannelCmd(dmamuxRegBase, dmamuxChannel, false);
    DMAMUX_SetChannelSource(dmamuxRegBase, dmamuxChannel, sourceDmamuxChannel);
    DMAMUX_SetChannelCmd(dmamuxRegBase, dmamuxChannel, true);

    /* Clear the TCD registers for this channel */
    EDMA_TCDClearReg(edmaRegBase, requestedChannel);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_ReleaseChannel
 * Description   : Free eDMA channel's hardware and software resource.
 *
 * Implements    : EDMA_DRV_ReleaseChannel_Activity
 *END**************************************************************************/
status_t EDMA_DRV_ReleaseChannel(uint8_t channel)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the DMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    edma_chn_state_t * chnState = s_edma->chn[channel];

    /* Check the channel is initialized */
    DEV_ASSERT(chnState != NULL);

    DMA_Type * edmaRegBase = s_edmaBase[0U];

    /* Stop edma channel. */
    EDMA_SetDmaRequestCmd(edmaRegBase, channel, false);

    /* Reset the channel state structure to default value. */
    uint8_t *clearStructPtr = (uint8_t *)chnState;
    size_t clearSize = sizeof(edma_chn_state_t);
    while (clearSize > 0U)
    {
        *clearStructPtr = 0;
        clearStructPtr ++;
        clearSize --;
    }

    s_edma->chn[channel] = NULL;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_ClearIntStatus
 * Description   : Clear done and interrupt status.
 *
 *END**************************************************************************/
static void EDMA_DRV_ClearIntStatus(uint8_t channel)
{
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_ClearDoneStatusFlag(edmaRegBase, channel);
    EDMA_ClearIntStatusFlag(edmaRegBase, channel);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_ClearSoftwareTCD
 * Description   : Clear the software tcd structure.
 *
 *END**************************************************************************/
static void EDMA_DRV_ClearSoftwareTCD(edma_software_tcd_t * stcd)
{
    uint8_t *byteAccess = (uint8_t *)stcd;
    size_t clearSize = sizeof(edma_software_tcd_t);
    while (clearSize > 0U)
    {
        *byteAccess = 0;
        byteAccess ++;
        clearSize --;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_IRQHandler
 * Description   : EDMA IRQ handler.
 *END**************************************************************************/
void EDMA_DRV_IRQHandler(uint8_t channel)
{
    const edma_chn_state_t *chn = s_edma->chn[channel];

    EDMA_DRV_ClearIntStatus(channel);

    if (chn != NULL)
    {
        if (chn->callback != NULL)
        {
            chn->callback(chn->parameter, chn->status);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_ErrorIRQHandler
 * Description   : EDMA error IRQ handler
 *END**************************************************************************/
void EDMA_DRV_ErrorIRQHandler(void)
{
    uint8_t channel = 0U;
    uint32_t error;
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    edma_chn_state_t *chn;

    error = EDMA_GetErrorIntStatusFlag(edmaRegBase);

    while ((error != 0UL) && (channel < (uint8_t) FEATURE_EDMA_MODULE_CHANNELS))
    {
        if ((error & EDMA_ERR_LSB_MASK) != 0UL)
        {
            EDMA_SetDmaRequestCmd(edmaRegBase, channel, false);
            chn = s_edma->chn[channel];
            if (chn != NULL)
            {
                EDMA_DRV_ClearIntStatus(channel);
                EDMA_ClearErrorIntStatusFlag(edmaRegBase, channel);
                chn->status = EDMA_CHN_ERROR;
                if (chn->callback != NULL)
                {
                    chn->callback(chn->parameter, chn->status);
                }
            }
        }
        error = error >> 1U;
        channel++;
    }
    EDMA_SetHaltCmd(edmaRegBase, false);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_ConfigSingleBlockTransfer
 * Description   : Configures a DMA single block transfer.
 *
 * Implements    : EDMA_DRV_ConfigSingleBlockTransfer_Activity
 *END**************************************************************************/
status_t EDMA_DRV_ConfigSingleBlockTransfer(uint8_t channel, edma_transfer_type_t type,
                                            uint32_t srcAddr, uint32_t destAddr,
                                            edma_transfer_size_t transferSize, uint32_t dataBufferSize)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

#ifdef DEV_ERROR_DETECT
    /* Check if the value passed for 'transferSize' is valid */
    DEV_ASSERT(EDMA_DRV_ValidTransferSize(transferSize));
#endif

    DMA_Type * edmaRegBase = s_edmaBase[0U];
    uint8_t transferOffset;
    status_t status = STATUS_SUCCESS;

    /* Compute the transfer offset, based on transfer size.
     * The number of bytes transferred in each source read/destination write
     * is obtained with the following formula:
     *    source_read_size = 2^SSIZE
     *    destination_write_size = 2^DSIZE
     */
    transferOffset = (uint8_t) (1U << ((uint8_t)transferSize));

    /* The number of bytes to be transferred (buffer size) must
     * be a multiple of the source read/destination write size
     */
    if ((dataBufferSize % transferOffset) != 0U)
    {
        status = STATUS_ERROR;
    }

    if (status == STATUS_SUCCESS)
    {
        /* Clear transfer control descriptor for the current channel */
        EDMA_TCDClearReg(edmaRegBase, channel);

        /* Configure source and destination addresses */
        EDMA_TCDSetSrcAddr(edmaRegBase, channel, srcAddr);
        EDMA_TCDSetDestAddr(edmaRegBase, channel, destAddr);

        /* Set transfer size (1B/2B/4B/16B/32B) */
        EDMA_TCDSetAttribute(edmaRegBase, channel, EDMA_MODULO_OFF, EDMA_MODULO_OFF, transferSize, transferSize);

        /* Configure source/destination offset. */
        switch (type)
        {
            case EDMA_TRANSFER_PERIPH2MEM:
                EDMA_TCDSetSrcOffset(edmaRegBase, channel, 0);
                EDMA_TCDSetDestOffset(edmaRegBase, channel, (int8_t) transferOffset);
                break;
            case EDMA_TRANSFER_MEM2PERIPH:
                EDMA_TCDSetSrcOffset(edmaRegBase, channel, (int8_t) transferOffset);
                EDMA_TCDSetDestOffset(edmaRegBase, channel, 0);
                break;
            case EDMA_TRANSFER_MEM2MEM:
                EDMA_TCDSetSrcOffset(edmaRegBase, channel, (int8_t) transferOffset);
                EDMA_TCDSetDestOffset(edmaRegBase, channel, (int8_t) transferOffset);
                break;
            case EDMA_TRANSFER_PERIPH2PERIPH:
                EDMA_TCDSetSrcOffset(edmaRegBase, channel, 0);
                EDMA_TCDSetDestOffset(edmaRegBase, channel, 0);
                break;
            default:
                /* This should never be reached - all the possible values have been handled. */
                break;
        }

        /* Set the total number of bytes to be transfered */
        EDMA_TCDSetNbytes(edmaRegBase, channel, dataBufferSize);

        /* Set major iteration count to 1 (single block mode) */
        EDMA_TCDSetMajorCount(edmaRegBase, channel, 1U);

        /* Enable interrupt when the transfer completes */
        EDMA_TCDSetMajorCompleteIntCmd(edmaRegBase, channel, true);
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_ConfigMultiBlockTransfer
 * Description   : Configures a DMA single block transfer.
 *
 * Implements    : EDMA_DRV_ConfigMultiBlockTransfer_Activity
 *END**************************************************************************/
status_t EDMA_DRV_ConfigMultiBlockTransfer(uint8_t channel, edma_transfer_type_t type,
                                           uint32_t srcAddr, uint32_t destAddr,
                                           edma_transfer_size_t transferSize, uint32_t blockSize,
                                           uint32_t blockCount, bool disableReqOnCompletion)
{
    status_t status = STATUS_SUCCESS;

    /* Configure the transfer for one data block */
    status = EDMA_DRV_ConfigSingleBlockTransfer(channel, type, srcAddr, destAddr, transferSize, blockSize);

    if (status == STATUS_SUCCESS)
    {
        DMA_Type * edmaRegBase = s_edmaBase[0U];

        /* Set the number of data blocks */
        EDMA_TCDSetMajorCount(edmaRegBase, channel, blockCount);

        /* Enable/disable requests upon completion */
        EDMA_TCDSetDisableDmaRequestAfterTCDDoneCmd(edmaRegBase, channel, disableReqOnCompletion);
    }

    return status;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_ConfigLoopTransfer
 * Description   : Configures the DMA transfer in a loop.
 *
 * Implements    : EDMA_DRV_ConfigLoopTransfer_Activity
 *END**************************************************************************/
status_t EDMA_DRV_ConfigLoopTransfer(uint8_t channel, const edma_transfer_config_t *transferConfig)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Check the transfer configuration structure is valid */
    DEV_ASSERT(transferConfig != NULL);

    /* Check the minor/major loop properties are defined */
    DEV_ASSERT(transferConfig->loopTransferConfig != NULL);

    /* If the modulo feature is enabled, check alignment of addresses */
    DEV_ASSERT((transferConfig->srcModulo == EDMA_MODULO_OFF) ||
               ((transferConfig->srcAddr % (((uint32_t)1U) << (uint32_t)transferConfig->srcModulo)) == 0U));
    DEV_ASSERT((transferConfig->destModulo == EDMA_MODULO_OFF) ||
               ((transferConfig->destAddr % (((uint32_t)1U) << (uint32_t)transferConfig->destModulo)) == 0U));

    /* Enable minor loop mapping */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_SetMinorLoopMappingCmd(edmaRegBase, true);

    /* Write the configuration in the transfer control descriptor registers */
    EDMA_DRV_PushConfigToReg(channel, transferConfig);

    /* Enable interrupt when major loop iteration completes */
    EDMA_TCDSetMajorCompleteIntCmd(edmaRegBase, channel, true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_ConfigScatterGatherTransfer
 * Description   : Configure eDMA for scatter/gather operation
 *
 * Implements    : EDMA_DRV_ConfigScatterGatherTransfer_Activity
 *END**************************************************************************/
status_t EDMA_DRV_ConfigScatterGatherTransfer(uint8_t channel, edma_software_tcd_t *stcd,
                                              edma_transfer_size_t transferSize, uint32_t bytesOnEachRequest,
                                              const edma_scatter_gather_list_t *srcList,
                                              const edma_scatter_gather_list_t *destList, uint8_t tcdCount)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Check the input arrays for scatter/gather operation are valid */
    DEV_ASSERT((stcd != NULL) && (srcList != NULL) && (destList != NULL));

#ifdef DEV_ERROR_DETECT
    /* Check if the value passed for 'transferSize' is valid */
    DEV_ASSERT(EDMA_DRV_ValidTransferSize(transferSize));
#endif

    uint8_t i;
    uint16_t transferOffset;
    uint32_t stcdAlignedAddr = STCD_ADDR(stcd);
    edma_software_tcd_t *stcdAddr = (edma_software_tcd_t *)stcdAlignedAddr;
    edma_loop_transfer_config_t loopConfig;
    edma_transfer_config_t config;
    status_t status = STATUS_SUCCESS;

    /* Compute the transfer offset, based on transfer size.
     * The number of bytes transferred in each source read/destination write
     * is obtained with the following formula:
     *    source_read_size = 2^SSIZE
     *    destination_write_size = 2^DSIZE
     */
    transferOffset = (uint16_t) (1UL << ((uint16_t)transferSize));

    /* The number of bytes to be transferred on each request must
     * be a multiple of the source read/destination write size
     */
    if ((bytesOnEachRequest % transferOffset) != 0U)
    {
        status = STATUS_ERROR;
    }

    /* Clear the configuration structures before initializing them. */
    uint8_t *clearStructPtr = (uint8_t *)(&config);
    size_t clearSize = sizeof(edma_transfer_config_t);
    while (clearSize > 0U)
    {
        *clearStructPtr = 0;
        clearStructPtr ++;
        clearSize --;
    }

    clearStructPtr = (uint8_t *)(&loopConfig);
    clearSize = sizeof(edma_loop_transfer_config_t);
    while (clearSize > 0U)
    {
        *clearStructPtr = 0;
        clearStructPtr ++;
        clearSize --;
    }

    /* Configure the transfer for scatter/gather mode. */
    config.srcLastAddrAdjust = 0;
    config.destLastAddrAdjust = 0;
    config.srcModulo = EDMA_MODULO_OFF;
    config.destModulo = EDMA_MODULO_OFF;
    config.srcTransferSize = transferSize;
    config.destTransferSize = transferSize;
    config.minorByteTransferCount = bytesOnEachRequest;
    config.interruptEnable = true;
    config.scatterGatherEnable = true;
    config.loopTransferConfig = &loopConfig;
    config.loopTransferConfig->srcOffsetEnable = false;
    config.loopTransferConfig->dstOffsetEnable = false;
    config.loopTransferConfig->minorLoopChnLinkEnable = false;
    config.loopTransferConfig->majorLoopChnLinkEnable = false;

    /* Copy scatter/gather lists to transfer configuration*/
    for (i = 0U; (i < tcdCount) && (status == STATUS_SUCCESS); i++)
    {
        config.srcAddr = srcList[i].address;
        config.destAddr = destList[i].address;
        if ((srcList[i].length != destList[i].length) || (srcList[i].type != destList[i].type))
        {
            status = STATUS_ERROR;
            break;
        }
        config.loopTransferConfig->majorLoopIterationCount = srcList[i].length/bytesOnEachRequest;

        switch (srcList[i].type)
        {
            case EDMA_TRANSFER_PERIPH2MEM:
                /* Configure Source Read. */
                config.srcOffset = 0;
                /* Configure Dest Write. */
                config.destOffset = (int16_t) transferOffset;
                break;
            case EDMA_TRANSFER_MEM2PERIPH:
                /* Configure Source Read. */
                config.srcOffset = (int16_t) transferOffset;
                /* Configure Dest Write. */
                config.destOffset = 0;
                break;
            case EDMA_TRANSFER_MEM2MEM:
                /* Configure Source Read. */
                config.srcOffset = (int16_t) transferOffset;
                /* Configure Dest Write. */
                config.destOffset = (int16_t) transferOffset;
                break;
            case EDMA_TRANSFER_PERIPH2PERIPH:
                /* Configure Source Read. */
                config.srcOffset = 0;
                /* Configure Dest Write. */
                config.destOffset = 0;
                break;
            default:
                /* This should never be reached - all the possible values have been handled. */
                break;
        }

        /* Configure the pointer to next software TCD structure; for the last one, this address should be 0 */
        if (i == ((uint8_t)(tcdCount - 1U)))
        {
            config.scatterGatherNextDescAddr = 0U;
        }
        else
        {
            edma_software_tcd_t *nextAddr = &stcdAddr[i];
            config.scatterGatherNextDescAddr = ((uint32_t) nextAddr);
        }

        if (i == 0U)
        {
            /* Push the configuration for the first descriptor to registers */
            EDMA_DRV_PushConfigToReg(channel, &config);
        }
        else
        {
            /* Copy configuration to software TCD structure */
            EDMA_DRV_PushConfigToSTCD(&config, &stcdAddr[i - 1U]);
        }
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_StartChannel
 * Description   : Starts an eDMA channel.
 *
 * Implements    : EDMA_DRV_StartChannel_Activity
 *END**************************************************************************/
status_t EDMA_DRV_StartChannel(uint8_t channel)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Enable requests for current channel */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_SetDmaRequestCmd(edmaRegBase, channel, true);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_StopChannel
 * Description   : Stops an eDMA channel.
 *
 * Implements    : EDMA_DRV_StopChannel_Activity
 *END**************************************************************************/
status_t EDMA_DRV_StopChannel(uint8_t channel)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Disable requests for current channel */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_SetDmaRequestCmd(edmaRegBase, channel, false);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_SetChannelRequest
 * Description   : Configures the DMA request for the eDMA channel.
 *
 * Implements    : EDMA_DRV_SetChannelRequest_Activity
 *END**************************************************************************/
status_t EDMA_DRV_SetChannelRequest(uint8_t channel, uint8_t req)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Retrieve the DMAMUX instance serving this request */
    uint8_t dmamuxInstance = (uint8_t)FEATURE_DMAMUX_REQ_SRC_TO_INSTANCE(req);
    /* Retrieve the appropriate request source index for the corresponding DMAMUX instance */
    uint8_t dmamuxRequest = (uint8_t)FEATURE_DMAMUX_REQ_SRC_TO_CHN(req);
    /* Retrieve the appropriate DMAMUX channel for the selected request source */
    uint8_t dmamuxChannel = (uint8_t)FEATURE_DMAMUX_CHN_FOR_EDMA_CHN(channel);
    /* Retrieve the appropriate DMAMUX instance */
    DMAMUX_Type * dmamuxRegBase = s_dmamuxBase[dmamuxInstance];
    /* Set the request */
    DMAMUX_SetChannelSource(dmamuxRegBase, dmamuxChannel, dmamuxRequest);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_ClearTCD
 * Description   : Clears all registers to 0 for the hardware TCD.
 *
 * Implements    : EDMA_DRV_ClearTCD_Activity
 *END**************************************************************************/
void EDMA_DRV_ClearTCD(uint8_t channel)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Clear the TCD memory */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TCDClearReg(edmaRegBase, channel);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_SetSrcAddr
 * Description   : Configures the source address for the eDMA channel.
 *
 * Implements    : EDMA_DRV_SetSrcAddr_Activity
 *END**************************************************************************/
void EDMA_DRV_SetSrcAddr(uint8_t channel, uint32_t address)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Set channel TCD source address */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TCDSetSrcAddr(edmaRegBase, channel, address);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_SetSrcOffset
 * Description   : Configures the source address signed offset for the eDMA channel.
 *
 * Implements    : EDMA_DRV_SetSrcOffset_Activity
 *END**************************************************************************/
void EDMA_DRV_SetSrcOffset(uint8_t channel, int16_t offset)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Set channel TCD source offset */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TCDSetSrcOffset(edmaRegBase, channel, offset);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_SetSrcReadChunkSize
 * Description   : Configures the source read data chunk size (transferred in a read sequence).
 *
 * Implements    : EDMA_DRV_SetSrcReadChunkSize_Activity
 *END**************************************************************************/
void EDMA_DRV_SetSrcReadChunkSize(uint8_t channel, edma_transfer_size_t size)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Set channel TCD source transfer size */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TCDSetSrcTransferSize(edmaRegBase, channel, size);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_SetSrcLastAddrAdjustment
 * Description   : Configures the source address last adjustment.
 *
 * Implements    : EDMA_DRV_SetSrcLastAddrAdjustment_Activity
 *END**************************************************************************/
void EDMA_DRV_SetSrcLastAddrAdjustment(uint8_t channel, int32_t adjust)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Set channel TCD source last adjustment */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TCDSetSrcLastAdjust(edmaRegBase, channel, adjust);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_SetDestLastAddrAdjustment
 * Description   : Configures the source address last adjustment.
 *
 * Implements    : EDMA_DRV_SetDestLastAddrAdjustment_Activity
 *END**************************************************************************/
void EDMA_DRV_SetDestLastAddrAdjustment(uint8_t channel, int32_t adjust)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Set channel TCD source last adjustment */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TCDSetDestLastAdjust(edmaRegBase, channel, adjust);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_SetDestAddr
 * Description   : Configures the destination address for the eDMA channel.
 *
 * Implements    : EDMA_DRV_SetDestAddr_Activity
 *END**************************************************************************/
void EDMA_DRV_SetDestAddr(uint8_t channel, uint32_t address)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Set channel TCD destination address */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TCDSetDestAddr(edmaRegBase, channel, address);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_SetDestOffset
 * Description   : Configures the destination address signed offset for the eDMA channel.
 *
 * Implements    : EDMA_DRV_SetDestOffset_Activity
 *END**************************************************************************/
void EDMA_DRV_SetDestOffset(uint8_t channel, int16_t offset)
{
        /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Set channel TCD destination offset */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TCDSetDestOffset(edmaRegBase, channel, offset);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_SetDestWriteChunkSize
 * Description   : Configures the destination data chunk size (transferred in a write sequence).
 *
 * Implements    : EDMA_DRV_SetDestWriteChunkSize_Activity
 *END**************************************************************************/
void EDMA_DRV_SetDestWriteChunkSize(uint8_t channel, edma_transfer_size_t size)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Set channel TCD source transfer size */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TCDSetDestTransferSize(edmaRegBase, channel, size);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_SetMinorLoopBlockSize
 * Description   : Configures the number of bytes to be transferred in each service request of the channel.
 *
 * Implements    : EDMA_DRV_SetMinorLoopBlockSize_Activity
 *END**************************************************************************/
void EDMA_DRV_SetMinorLoopBlockSize(uint8_t channel, uint32_t nbytes)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Set channel TCD minor loop block size */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TCDSetNbytes(edmaRegBase, channel, nbytes);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_SetMajorLoopIterationCount
 * Description   : Configures the number of major loop iterations.
 *
 * Implements    : EDMA_DRV_SetMajorLoopIterationCount_Activity
 *END**************************************************************************/
void EDMA_DRV_SetMajorLoopIterationCount(uint8_t channel, uint32_t majorLoopCount)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Set the major loop iteration count */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TCDSetMajorCount(edmaRegBase, channel, majorLoopCount);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_GetRemainingMajorIterationsCount
 * Description   : Returns the remaining major loop iteration count.
 *
 * Implements    : EDMA_DRV_GetRemainingMajorIterationsCount_Activity
 *END**************************************************************************/
uint32_t EDMA_DRV_GetRemainingMajorIterationsCount(uint8_t channel)
{
    uint32_t count;

    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Retrieve the number of minor loops yet to be triggered */
    const DMA_Type * edmaRegBase = s_edmaBase[0U];
    count = EDMA_TCDGetCurrentMajorCount(edmaRegBase, channel);

    return count;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_SetScatterGatherLink
 * Description   : Configures the memory address of the next TCD, in scatter/gather mode.
 *
 * Implements    : EDMA_DRV_SetScatterGatherLink_Activity
 *END**************************************************************************/
void EDMA_DRV_SetScatterGatherLink(uint8_t channel, uint32_t nextTCDAddr)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Configures the memory address of the next TCD */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TCDSetScatterGatherLink(edmaRegBase, channel, nextTCDAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_DisableRequestsOnTransferComplete
 * Description   : Disables/Enables the DMA request after the major loop completes for the TCD.
 *
 * Implements    : EDMA_DRV_DisableRequestsOnTransferComplete_Activity
 *END**************************************************************************/
void EDMA_DRV_DisableRequestsOnTransferComplete(uint8_t channel, bool disable)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Disables/Enables the DMA request upon TCD completion */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TCDSetDisableDmaRequestAfterTCDDoneCmd(edmaRegBase, channel, disable);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_ConfigureInterrupt
 * Description   : Disables/Enables the channel interrupt requests.
 *
 * Implements    : EDMA_DRV_ConfigureInterrupt_Activity
 *END**************************************************************************/
void EDMA_DRV_ConfigureInterrupt(uint8_t channel, edma_channel_interrupt_t intSrc, bool enable)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Disables/Enables the channel interrupt requests. */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    switch (intSrc)
    {
        case EDMA_CHN_ERR_INT:
            /* Enable channel interrupt request when error conditions occur */
            EDMA_SetErrorIntCmd(edmaRegBase, channel, enable);
            break;
        case EDMA_CHN_HALF_MAJOR_LOOP_INT:
            /* Enable channel interrupt request when major iteration count reaches halfway point */
            EDMA_TCDSetMajorHalfCompleteIntCmd(edmaRegBase, channel, enable);
            break;
        case EDMA_CHN_MAJOR_LOOP_INT:
            /* Enable channel interrupt request when major iteration count reaches zero */
            EDMA_TCDSetMajorCompleteIntCmd(edmaRegBase, channel, enable);
            break;
        default:
            /* This branch should never be reached if driver API is used properly */
            break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_CancelTransfer
 * Description   : Cancels the running transfer for this channel.
 *
 * Implements    : EDMA_DRV_CancelTransfer_Activity
 *END**************************************************************************/
void EDMA_DRV_CancelTransfer(bool error)
{
    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Cancel the running transfer. */
    DMA_Type * edmaRegBase = s_edmaBase[0U];

    if (error)
    {
        EDMA_CancelTransferWithError(edmaRegBase);
    }
    else
    {
        EDMA_CancelTransfer(edmaRegBase);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_TriggerSwRequest
 * Description   : Triggers a sw request for the current channel.
 *
 * Implements    : EDMA_DRV_TriggerSwRequest_Activity
 *END**************************************************************************/
void EDMA_DRV_TriggerSwRequest(uint8_t channel)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Trigger the channel transfer. */
    DMA_Type * edmaRegBase = s_edmaBase[0U];
    EDMA_TriggerChannelStart(edmaRegBase, channel);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_PushConfigToSTCD
 * Description   : Copy the configuration to the software TCD structure.
 *
 * Implements    : EDMA_DRV_PushConfigToSTCD_Activity
 *END**************************************************************************/
void EDMA_DRV_PushConfigToSTCD(const edma_transfer_config_t *config, edma_software_tcd_t *stcd)
{
    if ((config != NULL) && (stcd != NULL))
    {
        /* Clear the array of software TCDs passed by the user */
        EDMA_DRV_ClearSoftwareTCD(stcd);

        /* Set the software TCD fields */
        stcd->ATTR = (uint16_t)((((uint16_t)config->srcModulo) << DMA_TCD_ATTR_SMOD_SHIFT) | (((uint16_t)config->srcTransferSize) << DMA_TCD_ATTR_SSIZE_SHIFT) |
                                (((uint16_t)config->destModulo) << DMA_TCD_ATTR_DMOD_SHIFT) | (((uint16_t)config->destTransferSize) << DMA_TCD_ATTR_DSIZE_SHIFT));
        stcd->SADDR = config->srcAddr;
        stcd->SOFF = config->srcOffset;
        stcd->NBYTES = config->minorByteTransferCount;
        stcd->SLAST = config->srcLastAddrAdjust;
        stcd->DADDR = config->destAddr;
        stcd->DOFF = config->destOffset;
        stcd->CITER = (uint16_t) config->loopTransferConfig->majorLoopIterationCount;
        if (config->scatterGatherEnable)
        {
            stcd->DLAST_SGA = (int32_t) config->scatterGatherNextDescAddr;
        }
        else
        {
            stcd->DLAST_SGA = config->destLastAddrAdjust;
        }
        stcd->CSR = (uint16_t) (((config->interruptEnable ? 1UL : 0UL) << DMA_TCD_CSR_INTMAJOR_SHIFT) |
                                ((config->scatterGatherEnable ? 1UL : 0UL) << DMA_TCD_CSR_ESG_SHIFT));
        stcd->BITER = (uint16_t) config->loopTransferConfig->majorLoopIterationCount;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_PushConfigToReg
 * Description   : Copy the configuration to the TCD registers.
 *
 * Implements    : EDMA_DRV_PushConfigToReg_Activity
 *END**************************************************************************/
void EDMA_DRV_PushConfigToReg(uint8_t channel, const edma_transfer_config_t *tcd)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    /* Check the transfer configuration structure is valid */
    DEV_ASSERT(tcd != NULL);

    DMA_Type * edmaRegBase = s_edmaBase[0U];
    /* Clear TCD registers */
    EDMA_TCDClearReg(edmaRegBase, channel);

    /* Set source and destination addresses */
    EDMA_TCDSetSrcAddr(edmaRegBase, channel, tcd->srcAddr);
    EDMA_TCDSetDestAddr(edmaRegBase, channel, tcd->destAddr);

    /* Set source/destination modulo feature and transfer size */
    EDMA_TCDSetAttribute(edmaRegBase, channel, tcd->srcModulo, tcd->destModulo,
                         tcd->srcTransferSize, tcd->destTransferSize);

    /* Set source/destination offset and last adjustment; for scatter/gather operation, destination
     * last adjustment is the address of the next TCD structure to be loaded by the eDMA engine */
    EDMA_TCDSetSrcOffset(edmaRegBase, channel, tcd->srcOffset);
    EDMA_TCDSetDestOffset(edmaRegBase, channel, tcd->destOffset);
    EDMA_TCDSetSrcLastAdjust(edmaRegBase, channel, tcd->srcLastAddrAdjust);
    if (tcd->scatterGatherEnable)
    {
        EDMA_TCDSetScatterGatherCmd(edmaRegBase, channel, true);
        EDMA_TCDSetScatterGatherLink(edmaRegBase, channel, tcd->scatterGatherNextDescAddr);
    }
    else
    {
        EDMA_TCDSetScatterGatherCmd(edmaRegBase, channel, false);
        EDMA_TCDSetDestLastAdjust(edmaRegBase, channel, tcd->destLastAddrAdjust);
    }

    /* Configure channel interrupt */
    EDMA_TCDSetMajorCompleteIntCmd(edmaRegBase, channel, tcd->interruptEnable);

    /* If loop configuration is available, copy minor/major loop setup to registers */
    if (tcd->loopTransferConfig != NULL)
    {
        EDMA_TCDSetSrcMinorLoopOffsetCmd(edmaRegBase, channel, tcd->loopTransferConfig->srcOffsetEnable);
        EDMA_TCDSetDestMinorLoopOffsetCmd(edmaRegBase, channel, tcd->loopTransferConfig->dstOffsetEnable);
        EDMA_TCDSetMinorLoopOffset(edmaRegBase, channel, tcd->loopTransferConfig->minorLoopOffset);
        EDMA_TCDSetNbytes(edmaRegBase, channel, tcd->minorByteTransferCount);

        EDMA_TCDSetChannelMinorLink(edmaRegBase, channel, tcd->loopTransferConfig->minorLoopChnLinkNumber,
                                        tcd->loopTransferConfig->minorLoopChnLinkEnable);
        EDMA_TCDSetChannelMajorLink(edmaRegBase, channel, tcd->loopTransferConfig->majorLoopChnLinkNumber,
                                        tcd->loopTransferConfig->majorLoopChnLinkEnable);

        EDMA_TCDSetMajorCount(edmaRegBase, channel, tcd->loopTransferConfig->majorLoopIterationCount);
    }
    else
    {
        EDMA_TCDSetNbytes(edmaRegBase, channel, tcd->minorByteTransferCount);
    }
}

#ifdef DEV_ERROR_DETECT
/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_ValidTransferSize
 * Description   : Check if the transfer size value is legal (0/1/2/4/5).
 *
 *END**************************************************************************/
static bool EDMA_DRV_ValidTransferSize(edma_transfer_size_t size)
{
    bool isValid;
    switch (size)
    {
        case EDMA_TRANSFER_SIZE_1B:
        case EDMA_TRANSFER_SIZE_2B:
        case EDMA_TRANSFER_SIZE_4B:
        case EDMA_TRANSFER_SIZE_16B:
        case EDMA_TRANSFER_SIZE_32B:
            isValid = true;
            break;
        default:
            isValid = false;
            break;
    }
    return isValid;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : EDMA_DRV_GetChannelStatus
 * Description   : Returns the eDMA channel status.
 *
 * Implements    : EDMA_DRV_GetChannelStatus_Activity
 *END**************************************************************************/
edma_chn_status_t EDMA_DRV_GetChannelStatus(uint8_t channel)
{
    /* Check the channel number is valid */
    DEV_ASSERT(channel < FEATURE_EDMA_MODULE_CHANNELS);

    /* Check the eDMA module is initialized */
    DEV_ASSERT(s_edma != NULL);

    /* Check the channel is initialized */
    DEV_ASSERT(s_edma->chn[channel] != NULL);

    return s_edma->chn[channel]->status;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/


