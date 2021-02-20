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
/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, cast performed between a pointer to object
 * type and a pointer to a different object type.
 * Driver support many data element sizes (1, 2, 4 byte), the cast is required to be able to
 * work on correct data size.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 18.4, pointer arithmetic other than array indexing used
 * Driver support many data element sizes (1, 2, 4 byte), pointer arithmetic is required to
 * be able to work on correct data size.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.8, attempt to cast away const/volatile
 * from a pointer or reference
 * To reduce complexity of driver, transmit and receive use the same state structure, which hold
 * user data pointer that might be read from or write to.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable
 * Local variable is required to get neccesary value from function.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.13, Pointer parameter 'data' could be declared as pointing to const
 * Memory pointed by 'data' is used for receive.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, Impermissible cast of composite expression
 * Required to cast to void* to used with edma function
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced
 * These macro are used by user.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External function could be made static
 * These functions are used by user.
 */

#include "sai_driver.h"
#include "sai_hw_access.h"

/*! @cond DRIVER_INTERNAL_USE_ONLY */

/* default values set by driver */
#define TX_WATERMARK 2U
#define RX_WATERMARK 5U
#define FIFO_SIZE 8U
#define BIT_CLK_SWAP false
#define BIT_CLK_AS_EXTERNAL false
#define WORD_FLAG_INDEX 0
#define CONT_ON_ERROR true
#define PACK_MODE 0
#define SYNC_ON_DEMAND true
#define TX_FIFO_SIZE (FIFO_SIZE-TX_WATERMARK)
#define RX_FIFO_SIZE (RX_WATERMARK+1U)
/*******************************************************************************
 * Private Functions
 ******************************************************************************/

static void SAI_DRV_TxResetVar(uint32_t instNum);
static void SAI_DRV_RxResetVar(uint32_t instNum);
static void SAI_DRV_CompleteSendDataUsingDma(void * parameter,
                                             edma_chn_status_t status);
static void SAI_DRV_CompleteReceiveDataUsingDma(void * parameter,
                                             edma_chn_status_t status);
static void SAI_DRV_TxIRQ(uint32_t instNum);
static void SAI_DRV_RxIRQ(uint32_t instNum);
static void SAI_DRV_SendInt(uint32_t instNum,
                            const uint8_t* data[],
                            uint32_t count);
static void SAI_DRV_SendDma(uint32_t instNum,
                            const uint8_t* data[],
                            uint32_t count);
static void SAI_DRV_ReceiveDma(uint32_t instNum,
                              uint8_t* data[],
                              uint32_t count);
static void SAI_DRV_ReceiveInt(uint32_t instNum,
                              uint8_t* data[],
                              uint32_t count);
static bool SAI_DRV_TxFillFifoInterrupt(uint32_t instNum);
static bool SAI_DRV_TxFillFifoDma(uint32_t instNum);

void SAI0_Tx_IRQHandler(void);
void SAI0_Rx_IRQHandler(void);
void SAI1_Tx_IRQHandler(void);
void SAI1_Rx_IRQHandler(void);
void SAI2_Tx_IRQHandler(void);
void SAI2_Rx_IRQHandler(void);

#if (SAI_INSTANCE_COUNT > 0U)
/* Implementation of SAI0 handler named in startup code. */
void SAI0_Tx_IRQHandler(void)
{
    SAI_DRV_TxIRQ(0U);
}
void SAI0_Rx_IRQHandler(void)
{
    SAI_DRV_RxIRQ(0U);
}
#endif
#if (SAI_INSTANCE_COUNT > 1U)
/* Implementation of SAI1 handler named in startup code. */
void SAI1_Tx_IRQHandler(void)
{
    SAI_DRV_TxIRQ(1U);
}
void SAI1_Rx_IRQHandler(void)
{
    SAI_DRV_RxIRQ(1U);
}
#endif
#if (SAI_INSTANCE_COUNT > 2U)
/* Implementation of SAI1 handler named in startup code. */
void SAI2_Tx_IRQHandler(void)
{
    SAI_DRV_TxIRQ(2U);
}
void SAI2_Rx_IRQHandler(void)
{
    SAI_DRV_RxIRQ(2U);
}
#endif

/* Channel count for each instance */
#if (SAI_INSTANCE_COUNT == 2U)
    static const uint8_t sai_channel_count[SAI_INSTANCE_COUNT] = {SAI0_CHANNEL_COUNT, SAI1_CHANNEL_COUNT};
#elif (SAI_INSTANCE_COUNT == 3U)
    static const uint8_t sai_channel_count[SAI_INSTANCE_COUNT] = {SAI0_CHANNEL_COUNT, SAI1_CHANNEL_COUNT, SAI2_CHANNEL_COUNT};
#endif
#if (SAI_INSTANCE_COUNT == 2U)
    static const clock_names_t SAIClkNames[SAI_INSTANCE_COUNT] = {SAI0_CLK, SAI1_CLK};
#elif (SAI_INSTANCE_COUNT == 3U)
    static const clock_names_t SAIClkNames[SAI_INSTANCE_COUNT] = {SAI0_CLK, SAI1_CLK, SAI2_CLK};
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/
static SAI_Type * const SAIBase[SAI_INSTANCE_COUNT] = SAI_BASE_PTRS;
static const IRQn_Type SAITxIrqId[SAI_INSTANCE_COUNT] = SAI_TX_IRQS;
static const IRQn_Type SAIRxIrqId[SAI_INSTANCE_COUNT] = SAI_RX_IRQS;

/* state variables */
static sai_state_t* TxState[SAI_INSTANCE_COUNT];
static sai_state_t* RxState[SAI_INSTANCE_COUNT];

/*FUNCTION**********************************************************************
 *
 * Function Name : TO_BIT
 * Description   : Convert bool to interger
 *
 *END**************************************************************************/
static inline uint32_t TO_BIT(bool x)
{
    return x ? 1UL : 0UL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxResetVar
 * Description   : Reset driver variable
 *
 *END**************************************************************************/
static void SAI_DRV_TxResetVar(uint32_t instNum)
{
    uint8_t j;
    for (j = 0; j < sai_channel_count[instNum]; j++)
    {
        TxState[instNum]->ChnState[j].data = NULL;
        TxState[instNum]->ChnState[j].count = 0U;
    }
    /* unsupported means uninitialized */
    TxState[instNum]->status = STATUS_UNSUPPORTED;
    TxState[instNum]->Blocking = false;
    TxState[instNum]->Callback = NULL;
    TxState[instNum]->NextChn = 0U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxResetVar
 * Description   : Reset driver variable
 *
 *END**************************************************************************/
static void SAI_DRV_RxResetVar(uint32_t instNum)
{
    uint8_t j;
    for (j = 0; j < sai_channel_count[instNum]; j++)
    {
        RxState[instNum]->ChnState[j].data = NULL;
        RxState[instNum]->ChnState[j].count = 0U;
    }
    /* unsupported means uninitialized */
    RxState[instNum]->status = STATUS_UNSUPPORTED;
    RxState[instNum]->Blocking = false;
    RxState[instNum]->Callback = NULL;
    RxState[instNum]->NextChn = 0U;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_CompleteSendDataUsingDma
 * Description   : Finish sending data using dma
 *
 *END**************************************************************************/
static void SAI_DRV_CompleteSendDataUsingDma(void * parameter,
                                             edma_chn_status_t status)
{
    uint32_t instNum = ((uint32_t)parameter & 0xffUL);
    uint8_t ChnNum = (uint8_t)(((uint32_t)parameter >> 16UL) & 0xffUL);
    uint8_t DmaChn = (uint8_t)(((uint32_t)parameter >> 8UL) & 0xffUL);
    uint8_t c = 0;
    SAI_Type* inst = SAIBase[instNum];
    uint8_t i;
    bool finish = true;

    /* Release the DMA channel */
    if (status != EDMA_CHN_NORMAL)
    {
        TxState[instNum]->status = STATUS_ERROR;
        /* stop all other dma channel*/
        if ((TxState[instNum]->mux == SAI_MUX_DISABLED) || (TxState[instNum]->mux == SAI_MUX_LINE))
        {
            for (i = 0; i < sai_channel_count[instNum]; i++)
            {
                if (SAI_DRV_IsTxChannelEnabled(inst, i))
                {
                    (void)EDMA_DRV_StopChannel(TxState[instNum]->DmaChannel[c]);
                    c++;
                    TxState[instNum]->ChnState[i].count = 0U;
                }
            }
        }
        else
        {
            (void)EDMA_DRV_StopChannel(TxState[instNum]->DmaChannel[0]);
            TxState[instNum]->ChnState[0].count = 0U;
        }
        finish = true;
    }
    else
    {
        (void)EDMA_DRV_StopChannel(TxState[instNum]->DmaChannel[DmaChn]);
        TxState[instNum]->ChnState[ChnNum].count = 0U;
        /* check if all channel finished */
        if ((TxState[instNum]->mux == SAI_MUX_DISABLED) || (TxState[instNum]->mux == SAI_MUX_LINE))
        {
            for (i = 0; i < sai_channel_count[instNum]; i++)
            {
                if (SAI_DRV_IsTxChannelEnabled(inst, i))
                {
                    if (TxState[instNum]->ChnState[i].count > 0U)
                    {
                        finish = false;
                        break;
                    }
                }
            }
        }
        else /* mux mem only one channel */
        {
            finish = true;
        }
    }
    if (finish)
    {
        /* Disable tx DMA requests for the current instance */
        SAI_DRV_TxDisableFifoReqDma(inst);
        if (TxState[instNum]->status != STATUS_ERROR)
        {
            TxState[instNum]->status = STATUS_SUCCESS;
        }
        if (TxState[instNum]->Blocking)
        {
            (void)OSIF_SemaPost(&TxState[instNum]->Sema);
        }
        else
        {
            /* Invoke callback if there is one */
            if (TxState[instNum]->Callback != NULL)
            {
                TxState[instNum]->Callback(ChnNum, SAI_TRANSFER_COMPLETE, TxState[instNum]->status);
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_CompleteReceiveDataUsingDma
 * Description   : Finish receiving data using dma
 *
 *END**************************************************************************/
static void SAI_DRV_CompleteReceiveDataUsingDma(void * parameter,
                                             edma_chn_status_t status)
{
    uint32_t instNum = ((uint32_t)parameter & 0xffUL);
    uint8_t ChnNum = (uint8_t)(((uint32_t)parameter >> 16U) & 0xffUL);
    uint8_t DmaChn = (uint8_t)(((uint32_t)parameter >> 8U) & 0xffUL);
    uint8_t c = 0;
    SAI_Type* inst = SAIBase[instNum];
    uint8_t i;
    bool finish = true;

    /* Release the DMA channel */
    if (status != EDMA_CHN_NORMAL)
    {
        RxState[instNum]->status = STATUS_ERROR;
        /* stop all other dma channel*/
        if ((RxState[instNum]->mux == SAI_MUX_DISABLED) || (RxState[instNum]->mux == SAI_MUX_LINE))
        {
            for (i = 0; i < sai_channel_count[instNum]; i++)
            {
                if (SAI_DRV_IsRxChannelEnabled(inst, i))
                {
                    (void)EDMA_DRV_StopChannel(RxState[instNum]->DmaChannel[c]);
                    c++;
                    RxState[instNum]->ChnState[i].count = 0U;
                }
            }
        }
        else
        {
            (void)EDMA_DRV_StopChannel(RxState[instNum]->DmaChannel[0]);
            RxState[instNum]->ChnState[0].count = 0U;
        }
        finish = true;
    }
    else
    {
        (void)EDMA_DRV_StopChannel(RxState[instNum]->DmaChannel[DmaChn]);
        RxState[instNum]->ChnState[ChnNum].count = 0U;
        /* check if all channel finished */
        if ((RxState[instNum]->mux == SAI_MUX_DISABLED) || (RxState[instNum]->mux == SAI_MUX_LINE))
        {
            for (i = 0; i < sai_channel_count[instNum]; i++)
            {
                if (SAI_DRV_IsRxChannelEnabled(inst, i))
                {
                    if (RxState[instNum]->ChnState[i].count > 0U)
                    {
                        finish = false;
                        break;
                    }
                }
            }
        }
        else /* mux mem only one channel */
        {
            finish = true;
        }
    }
    if (finish)
    {
        /* Disable Rx DMA requests for the current instance */
        SAI_DRV_RxDisableFifoReqDma(inst);
        if (RxState[instNum]->status != STATUS_ERROR)
        {
            RxState[instNum]->status = STATUS_SUCCESS;
        }
        if (RxState[instNum]->Blocking)
        {
            (void)OSIF_SemaPost(&RxState[instNum]->Sema);
        }
        else
        {
            /* Invoke callback if there is one */
            if (RxState[instNum]->Callback != NULL)
            {
                RxState[instNum]->Callback(ChnNum, SAI_TRANSFER_COMPLETE, RxState[instNum]->status);
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxIRQ
 * Description   : Manage interrupt transfer using fifo request, execute
 *                 other user interrupt func (fifo error, word start,
 *                 sync error)
 *
 *END**************************************************************************/
static void SAI_DRV_TxIRQ(uint32_t instNum)
{
    SAI_Type* inst = SAIBase[instNum];
    uint8_t i,j;
    uint32_t temp;
    bool finish;
    status_t CurStatus = TxState[instNum]->status;
    if (SAI_DRV_TxGetFifoReqFlag(inst))
    {
        if ((TxState[instNum]->XferType == SAI_INTERRUPT) && (CurStatus == STATUS_BUSY))
        {
            finish = false;
            if (TxState[instNum]->mux == SAI_MUX_LINE)
            {
                for (i = 0U; i < sai_channel_count[instNum]; i++)
                {
                    if (SAI_DRV_IsTxChannelEnabled(inst, i))
                    {
                        if (TxState[instNum]->ChnState[0].count < TX_FIFO_SIZE)
                        {
                            temp = TxState[instNum]->ChnState[0].count;
                        }
                        else
                        {
                            temp = TX_FIFO_SIZE;
                        }
                        for (j = 0U; j < temp; j++)
                        {
                            if (TxState[instNum]->ElementSize == 1U)
                            {
                                SAI_DRV_TxWrite(inst, i, (uint32_t)*TxState[instNum]->ChnState[TxState[instNum]->NextChn].data);
                            }
                            else if (TxState[instNum]->ElementSize == 2U)
                            {
                                SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) TxState[instNum]->ChnState[TxState[instNum]->NextChn].data));
                            }
                            else if (TxState[instNum]->ElementSize == 4U)
                            {
                                SAI_DRV_TxWrite(inst, i, *((uint32_t*) TxState[instNum]->ChnState[TxState[instNum]->NextChn].data));
                            }
                            else
                            {
                                DEV_ASSERT(false);
                            }
                            TxState[instNum]->ChnState[TxState[instNum]->NextChn].data += TxState[instNum]->ElementSize;
                            TxState[instNum]->NextChn++; /* alternate between data block */
                            if (TxState[instNum]->NextChn == TxState[instNum]->ChannelCount)
                            {
                                TxState[instNum]->NextChn = 0U;
                            }
                        }
                        if (TxState[instNum]->ChnState[0].count <= TX_FIFO_SIZE)
                        {
                            finish = true;
                            TxState[instNum]->ChnState[0].count = 0;
                        }
                        else
                        {
                            TxState[instNum]->ChnState[0].count -= TX_FIFO_SIZE;
                        }
                        break; /* only one channle enabled in this mode */
                    }
                }
            }
            else if (TxState[instNum]->mux == SAI_MUX_MEM)
            {
                if (TxState[instNum]->ChnState[0].count < TX_FIFO_SIZE)
                {
                    temp = TxState[instNum]->ChnState[0].count;
                }
                else
                {
                    temp = TX_FIFO_SIZE;
                }
                for (j = 0U; j < temp; j++)
                {
                    for (i = 0U; i < sai_channel_count[instNum]; i++)
                    {
                        if (SAI_DRV_IsTxChannelEnabled(inst, i))
                        {
                            if (TxState[instNum]->ElementSize == 1U)
                            {
                                SAI_DRV_TxWrite(inst, i, (uint32_t)*TxState[instNum]->ChnState[0].data);
                            }
                            else if (TxState[instNum]->ElementSize == 2U)
                            {
                                SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) TxState[instNum]->ChnState[0U].data));
                            }
                            else if (TxState[instNum]->ElementSize == 4U)
                            {
                                SAI_DRV_TxWrite(inst, i, *((uint32_t*) TxState[instNum]->ChnState[0U].data));
                            }
                            else
                            {
                                DEV_ASSERT(false);
                            }
                            TxState[instNum]->ChnState[0U].data += TxState[instNum]->ElementSize;
                        }
                    }
                }
                if (TxState[instNum]->ChnState[0].count <= TX_FIFO_SIZE)
                {
                    finish = true;
                    TxState[instNum]->ChnState[0].count = 0;
                }
                else
                {
                    TxState[instNum]->ChnState[0].count -= TX_FIFO_SIZE;
                }
            }
            else /* mux disabled */
            {
                if (TxState[instNum]->ChnState[0].count < TX_FIFO_SIZE)
                {
                    temp = TxState[instNum]->ChnState[0].count;
                }
                else
                {
                    temp = TX_FIFO_SIZE;
                }
                for (i = 0U; i < sai_channel_count[instNum]; i++)
                {
                    if (SAI_DRV_IsTxChannelEnabled(inst, i))
                    {
                        for (j = 0U; j < temp; j++)
                        {
                            if (TxState[instNum]->ElementSize == 1U)
                            {
                                SAI_DRV_TxWrite(inst, i, (uint32_t)*TxState[instNum]->ChnState[i].data);
                            }
                            else if (TxState[instNum]->ElementSize == 2U)
                            {
                                SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) TxState[instNum]->ChnState[i].data));
                            }
                            else if (TxState[instNum]->ElementSize == 4U)
                            {
                                SAI_DRV_TxWrite(inst, i, *((uint32_t*) TxState[instNum]->ChnState[i].data));
                            }
                            else
                            {
                                DEV_ASSERT(false);
                            }
                            TxState[instNum]->ChnState[i].data += TxState[instNum]->ElementSize;
                        }
                    }
                }
                if (TxState[instNum]->ChnState[0].count <= TX_FIFO_SIZE)
                {
                    finish = true;
                    TxState[instNum]->ChnState[0].count = 0;
                }
                else
                {
                    TxState[instNum]->ChnState[0].count -= TX_FIFO_SIZE;
                }
            }
            if (finish)
            {
                TxState[instNum]->status = STATUS_SUCCESS;
                SAI_DRV_TxDisableFifoReqInt(inst);
                if (TxState[instNum]->Blocking)
                {
                    (void)OSIF_SemaPost(&TxState[instNum]->Sema);
                }
                else
                {
                    if (TxState[instNum]->Callback != NULL)
                    {
                        TxState[instNum]->Callback(0U, SAI_TRANSFER_COMPLETE, STATUS_SUCCESS);
                    }
                }
            }
        }
    }
    if (TxState[instNum]->Callback != NULL)
    {
        if (SAI_DRV_TxGetFifoErrorFlag(inst))
        {
            for (i = 0; i < sai_channel_count[instNum]; i++)
            {
                if (SAI_DRV_IsTxChannelEnabled(inst, i))
                {
                    if (SAI_DRV_IsTxFifoEmpty(inst, i))
                    {
                        TxState[instNum]->Callback(i, SAI_RUN_ERROR, STATUS_ERROR);
                    }
                }
                SAI_DRV_TxClearFlag (inst, SAI_TCSR_FEF_SHIFT);
            }
        }
        if (SAI_DRV_TxGetWordStartFlag(inst))
        {
            TxState[instNum]->Callback(0U, SAI_FRAME_START, STATUS_SUCCESS);
            SAI_DRV_TxClearFlag (inst, SAI_TCSR_WSF_SHIFT);
        }
        if (SAI_DRV_TxGetSyncErrorFlag(inst))
        {
            TxState[instNum]->Callback(0U, SAI_SYNC_ERROR, STATUS_ERROR);
            SAI_DRV_TxClearFlag (inst, SAI_TCSR_SEF_SHIFT);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxIRQ
 * Description   : Manage interrupt transfer using fifo request, execute
 *                 other user interrupt func (fifo error, word start,
 *                 sync error)
 *
 *END**************************************************************************/
static void SAI_DRV_RxIRQ(uint32_t instNum)
{
    SAI_Type* inst = SAIBase[instNum];
    uint8_t i, j;
    uint32_t temp;
    bool finish;
    status_t CurStatus = RxState[instNum]->status;

    if (SAI_DRV_RxGetFifoReqFlag(inst))
    {
        if ((RxState[instNum]->XferType == SAI_INTERRUPT) && (CurStatus == STATUS_BUSY))
        {
            finish = false;
            if (RxState[instNum]->mux == SAI_MUX_LINE)
            {
                for (i = 0U; i < sai_channel_count[instNum]; i++)
                {
                    if (SAI_DRV_IsRxChannelEnabled(inst, i))
                    {
                        if (RxState[instNum]->ChnState[0].count < RX_FIFO_SIZE)
                        {
                            temp = RxState[instNum]->ChnState[0].count;
                        }
                        else
                        {
                            temp = RX_FIFO_SIZE;
                        }
                        for (j = 0U; j < temp; j++)
                        {
                            if (RxState[instNum]->ElementSize == 1U)
                            {
                                *RxState[instNum]->ChnState[RxState[instNum]->NextChn].data = (uint8_t) SAI_DRV_RxRead(inst, i);
                            }
                            else if (RxState[instNum]->ElementSize == 2U)
                            {
                                *(uint16_t*)RxState[instNum]->ChnState[RxState[instNum]->NextChn].data = (uint16_t) SAI_DRV_RxRead(inst, i);
                            }
                            else if (RxState[instNum]->ElementSize == 4U)
                            {
                                *(uint32_t*)RxState[instNum]->ChnState[RxState[instNum]->NextChn].data = SAI_DRV_RxRead(inst, i);
                            }
                            else
                            {
                                DEV_ASSERT(false);
                            }
                            RxState[instNum]->ChnState[RxState[instNum]->NextChn].data += RxState[instNum]->ElementSize;
                            RxState[instNum]->NextChn++; /* alternate between data block */
                            if (RxState[instNum]->NextChn == RxState[instNum]->ChannelCount)
                            {
                                RxState[instNum]->NextChn = 0U;
                            }
                        }
                        if (RxState[instNum]->ChnState[0].count <= RX_FIFO_SIZE)
                        {
                            finish = true;
                            RxState[instNum]->ChnState[0].count = 0;
                        }
                        else
                        {
                            RxState[instNum]->ChnState[0].count -= RX_FIFO_SIZE;
                            if (RxState[instNum]->ChnState[0].count < RX_FIFO_SIZE) /* set watermark below normal level */
                            {
                                SAI_DRV_RxSetWatermark(inst, (uint8_t)(RxState[instNum]->ChnState[0].count - 1UL));
                            }
                        }
                        break; /* only one channel is enabled in this mode */
                    }
                }
            }
            else if (RxState[instNum]->mux == SAI_MUX_MEM)
            {
                if (RxState[instNum]->ChnState[0].count < RX_FIFO_SIZE)
                {
                    temp = RxState[instNum]->ChnState[0].count;
                }
                else
                {
                    temp = RX_FIFO_SIZE;
                }
                for (j = 0U; j < temp; j++)
                {
                    for (i = 0U; i < sai_channel_count[instNum]; i++)
                    {
                        if (SAI_DRV_IsRxChannelEnabled(inst, i))
                        {
                            if (RxState[instNum]->ElementSize == 1U)
                            {
                                *RxState[instNum]->ChnState[0U].data = (uint8_t) SAI_DRV_RxRead(inst, i);
                            }
                            else if (RxState[instNum]->ElementSize == 2U)
                            {
                                *(uint16_t*)RxState[instNum]->ChnState[0U].data = (uint16_t) SAI_DRV_RxRead(inst, i);
                            }
                            else if (RxState[instNum]->ElementSize == 4U)
                            {
                                *(uint32_t*)RxState[instNum]->ChnState[0U].data = SAI_DRV_RxRead(inst, i);
                            }
                            else
                            {
                                DEV_ASSERT(false);
                            }
                            RxState[instNum]->ChnState[0U].data += RxState[instNum]->ElementSize;
                        }
                    }
                }
                if (RxState[instNum]->ChnState[0].count <= RX_FIFO_SIZE)
                {
                    finish = true;
                    RxState[instNum]->ChnState[0].count = 0;
                }
                else
                {
                    RxState[instNum]->ChnState[0].count -= RX_FIFO_SIZE;
                    if (RxState[instNum]->ChnState[0].count < RX_FIFO_SIZE) /* set watermark below normal level */
                    {
                        SAI_DRV_RxSetWatermark(inst, (uint8_t)(RxState[instNum]->ChnState[0].count - 1UL));
                    }
                }
            }
            else /* mux disabled */
            {
                if (RxState[instNum]->ChnState[0].count < RX_FIFO_SIZE)
                {
                    temp = RxState[instNum]->ChnState[0].count;
                }
                else
                {
                    temp = RX_FIFO_SIZE;
                }
                for (i = 0U; i < sai_channel_count[instNum]; i++)
                {
                    if (SAI_DRV_IsRxChannelEnabled(inst, i))
                    {
                        for (j = 0U; j < temp; j++)
                        {
                            if (RxState[instNum]->ElementSize == 1U)
                            {
                                *RxState[instNum]->ChnState[i].data = (uint8_t) SAI_DRV_RxRead(inst, i);
                            }
                            else if (RxState[instNum]->ElementSize == 2U)
                            {
                                *(uint16_t*)RxState[instNum]->ChnState[i].data = (uint16_t) SAI_DRV_RxRead(inst, i);
                            }
                            else if (RxState[instNum]->ElementSize == 4U)
                            {
                                *(uint32_t*)RxState[instNum]->ChnState[i].data = SAI_DRV_RxRead(inst, i);
                            }
                            else
                            {
                                DEV_ASSERT(false);
                            }
                            RxState[instNum]->ChnState[i].data += RxState[instNum]->ElementSize;
                        }
                    }
                }
                if (RxState[instNum]->ChnState[0].count <= RX_FIFO_SIZE)
                {
                    finish = true;
                    RxState[instNum]->ChnState[0].count = 0;
                }
                else
                {
                    RxState[instNum]->ChnState[0].count -= RX_FIFO_SIZE;
                    if (RxState[instNum]->ChnState[0].count < RX_FIFO_SIZE) /* set watermark below normal level */
                    {
                        SAI_DRV_RxSetWatermark(inst, (uint8_t)(RxState[instNum]->ChnState[0].count - 1UL));
                    }
                }
            }
            if (finish)
            {
                RxState[instNum]->status = STATUS_SUCCESS;
                SAI_DRV_RxSetWatermark(inst, RX_WATERMARK);
                SAI_DRV_RxDisableFifoReqInt(inst);
                if (RxState[instNum]->Blocking)
                {
                    (void)OSIF_SemaPost(&RxState[instNum]->Sema);
                }
                else
                {
                    if (RxState[instNum]->Callback != NULL)
                    {
                        RxState[instNum]->Callback(0U, SAI_TRANSFER_COMPLETE, STATUS_SUCCESS);
                    }
                }
            }
        }
    }
    if (RxState[instNum]->Callback != NULL)
    {
        if (SAI_DRV_RxGetFifoErrorFlag(inst))
        {
            for (i = 0; i < sai_channel_count[instNum]; i++)
            {
                if (SAI_DRV_IsRxChannelEnabled(inst, i))
                {
                    if (SAI_DRV_IsRxFifoFull(inst, i))
                    {
                        RxState[instNum]->Callback(i, SAI_RUN_ERROR, STATUS_ERROR);
                    }
                }
                SAI_DRV_RxClearFlag (inst, SAI_RCSR_FEF_SHIFT);
            }
        }
        if (SAI_DRV_RxGetWordStartFlag(inst))
        {
            RxState[instNum]->Callback(0U, SAI_FRAME_START, STATUS_SUCCESS);
            SAI_DRV_RxClearFlag (inst, SAI_RCSR_WSF_SHIFT);
        }
        if (SAI_DRV_RxGetSyncErrorFlag(inst))
        {
            RxState[instNum]->Callback(0U, SAI_SYNC_ERROR, STATUS_ERROR);
            SAI_DRV_RxClearFlag (inst, SAI_RCSR_SEF_SHIFT);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxSendInt
 * Description   : Start sending data using interrupt, return immediately
 *
 *END**************************************************************************/
static void SAI_DRV_SendInt(uint32_t instNum,
                            const uint8_t* data[],
                            uint32_t count)
{
    SAI_Type* inst = SAIBase[instNum];
    uint8_t i;
    uint8_t c = 0;
    bool ret = false;
    uint32_t tcsr;

    TxState[instNum]->status = STATUS_BUSY;
    if (TxState[instNum]->mux == SAI_MUX_LINE)
    {
        for (i = 0; i < TxState[instNum]->ChannelCount; i++)
        {
            DEV_ASSERT(data[i] != NULL);
            /* cast to non-const type to avoid compile error/warning */
            TxState[instNum]->ChnState[i].data = (uint8_t*) data[i];
        }
        TxState[instNum]->ChnState[0].count = count * TxState[instNum]->ChannelCount;
    }
    else if (TxState[instNum]->mux == SAI_MUX_MEM)
    {
        DEV_ASSERT(data[0] != NULL);
        /* cast to non-const type to avoid compile error/warning */
        TxState[instNum]->ChnState[0].data = (uint8_t*) data[0];
        TxState[instNum]->ChnState[0].count = count;
    }
    else
    {
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsTxChannelEnabled(inst, i))
            {
                DEV_ASSERT(data[c] != NULL);
                /* cast to non-const type to avoid compile error/warning */
                TxState[instNum]->ChnState[i].data = (uint8_t*) data[c];
                c++;
            }
        }
        TxState[instNum]->ChnState[0].count = count;
    }
    /* Fill fifo before enable tx */
    tcsr = inst->TCSR;
    if ((tcsr & SAI_TCSR_TE_MASK) == 0UL)
    {
        ret = SAI_DRV_TxFillFifoInterrupt(instNum);
    }
    if (((tcsr & SAI_TCSR_TE_MASK) != 0UL) || (!ret))
    {
        SAI_DRV_TxEnableFifoReqInt(inst);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_SendDMA
 * Description   : Start sending data using dma, return immediately
 *
 *END**************************************************************************/
static void SAI_DRV_SendDma(uint32_t instNum,
                            const uint8_t* data[],
                            uint32_t count)
{
    SAI_Type* inst = SAIBase[instNum];
    uint8_t i;
    uint8_t c;
    edma_transfer_size_t xferSize = EDMA_TRANSFER_SIZE_1B;
    uint8_t numChan = 0;
    uint32_t temp;
    bool ret = false;
    uint32_t tcsr;

    /* Setup state variable */
    if ((TxState[instNum]->mux == SAI_MUX_LINE) || (TxState[instNum]->mux == SAI_MUX_DISABLED))
    {
        c = 0;
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsTxChannelEnabled(inst, i))
            {
                DEV_ASSERT(data[c] != NULL);
                TxState[instNum]->ChnState[i].count = count;
                /* cast to non-const type to avoid compile error/warning */
                TxState[instNum]->ChnState[i].data = (uint8_t*) data[c];
                c++;
            }
        }
    }
    else if (TxState[instNum]->mux == SAI_MUX_MEM)
    {
        DEV_ASSERT(data[0] != NULL);
        TxState[instNum]->ChnState[0].count = count;
        /* cast to non-const type to avoid compile error/warning */
        TxState[instNum]->ChnState[0].data = (uint8_t*) data[0];
    }
    else
    {
        /* do nothing */
    }

    /* Fill fifo before enable tx */
    tcsr = inst->TCSR;
    if ((tcsr & SAI_TCSR_TE_MASK) == 0UL)
    {
        ret = SAI_DRV_TxFillFifoDma(instNum);
    }
    if (((tcsr & SAI_TCSR_TE_MASK) != 0UL) || (!ret))
    {
        if (TxState[instNum]->ElementSize == 1U)
        {
            xferSize = EDMA_TRANSFER_SIZE_1B;
        }
        else if (TxState[instNum]->ElementSize == 2U)
        {
            xferSize = EDMA_TRANSFER_SIZE_2B;
        }
        else if (TxState[instNum]->ElementSize == 4U)
        {
            xferSize = EDMA_TRANSFER_SIZE_4B;
        }
        else
        {
            DEV_ASSERT(false);
        }
        TxState[instNum]->status = STATUS_BUSY;
        if ((TxState[instNum]->mux == SAI_MUX_LINE) || (TxState[instNum]->mux == SAI_MUX_DISABLED))
        {
            c = 0;
            for (i = 0; i < sai_channel_count[instNum]; i++)
            {
                if (SAI_DRV_IsTxChannelEnabled(inst, i))
                {
                    (void)EDMA_DRV_ConfigMultiBlockTransfer(TxState[instNum]->DmaChannel[c], EDMA_TRANSFER_MEM2PERIPH,
                                                            (uint32_t)TxState[instNum]->ChnState[i].data, (uint32_t)(&(inst->TDR[i])), xferSize,
                                                            TxState[instNum]->ElementSize, TxState[instNum]->ChnState[i].count, true);
                    /* Call driver function to end the transmission when the DMA transfer is done */
                    (void)EDMA_DRV_InstallCallback(TxState[instNum]->DmaChannel[c],
                                                   (edma_callback_t)(SAI_DRV_CompleteSendDataUsingDma),
                                                   (void*)(instNum | ((uint32_t)i << 16U) | ((uint32_t)c << 8U)));
                    /* Start the DMA channel */
                    (void)EDMA_DRV_StartChannel(TxState[instNum]->DmaChannel[c]);
                    c++;
                }
            }
        }
        else if (TxState[instNum]->mux == SAI_MUX_MEM)
        {
            for (i = 0; i < sai_channel_count[instNum]; i++)
            {
                if (SAI_DRV_IsTxChannelEnabled(inst, i))
                {
                    numChan++;
                }
            }
            temp = TxState[instNum]->ChnState[0].count * numChan;
            (void)EDMA_DRV_ConfigMultiBlockTransfer(TxState[instNum]->DmaChannel[0], EDMA_TRANSFER_MEM2PERIPH,
                                                    (uint32_t)TxState[instNum]->ChnState[0].data, (uint32_t)(&(inst->TDR[0])), xferSize,
                                                    TxState[instNum]->ElementSize, temp, true);

            /* Call driver function to end the transmission when the DMA transfer is done */
            (void)EDMA_DRV_InstallCallback(TxState[instNum]->DmaChannel[0],
                                           (edma_callback_t)(SAI_DRV_CompleteSendDataUsingDma),
                                           (void*)(instNum));
            /* Start the DMA channel */
            (void)EDMA_DRV_StartChannel(TxState[instNum]->DmaChannel[0]);
        }
        else
        {
            /* do nothing */
        }
        /* Enable tx DMA requests for the current instance */
        SAI_DRV_TxEnableFifoReqDma(inst);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxReceiveInt
 * Description   : Start receiving data using interrupt, return immediately
 *
 *END**************************************************************************/
static void SAI_DRV_ReceiveInt(uint32_t instNum,
                              uint8_t* data[],
                              uint32_t count)
{
    SAI_Type* inst = SAIBase[instNum];
    uint8_t i;
    uint8_t c = 0;

    RxState[instNum]->status = STATUS_BUSY;
    if (RxState[instNum]->mux == SAI_MUX_LINE)
    {
        for (i = 0; i < RxState[instNum]->ChannelCount; i++)
        {
            DEV_ASSERT(data[i] != NULL);
            RxState[instNum]->ChnState[i].data = data[i];
        }
        RxState[instNum]->ChnState[0].count = RxState[instNum]->ChannelCount * count;
        if (RxState[instNum]->ChnState[0].count < RX_FIFO_SIZE)
        {
            SAI_DRV_RxSetWatermark(inst, (uint8_t)(RxState[instNum]->ChnState[0].count - 1UL));
        }
        else
        {
            SAI_DRV_RxSetWatermark(inst, RX_WATERMARK);
        }
    }
    else if (RxState[instNum]->mux == SAI_MUX_MEM)
    {
        DEV_ASSERT(data[0] != NULL);
        RxState[instNum]->ChnState[0].data = data[0];
        RxState[instNum]->ChnState[0].count = count;
        if (count < RX_FIFO_SIZE)
        {
            SAI_DRV_RxSetWatermark(inst, (uint8_t)(RxState[instNum]->ChnState[0].count - 1UL));
        }
        else
        {
            SAI_DRV_RxSetWatermark(inst, RX_WATERMARK);
        }
    }
    else
    {
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsRxChannelEnabled(inst, i))
            {
                DEV_ASSERT(data[c] != NULL);
                RxState[instNum]->ChnState[i].data = data[c];
                c++;
            }
        }
        RxState[instNum]->ChnState[0].count = count;
        if (count < RX_FIFO_SIZE)
        {
            SAI_DRV_RxSetWatermark(inst, (uint8_t) (count - 1UL));
        }
        else
        {
            SAI_DRV_RxSetWatermark(inst, RX_WATERMARK);
        }
    }
    SAI_DRV_RxEnableFifoReqInt(inst);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_ReceiveDMA
 * Description   : Start receiving data using dma, return immediately
 *
 *END**************************************************************************/
static void SAI_DRV_ReceiveDma(uint32_t instNum,
                               uint8_t* data[],
                               uint32_t count)
{
    SAI_Type* inst = SAIBase[instNum];
    uint8_t i;
    uint8_t c;
    edma_transfer_size_t xferSize = EDMA_TRANSFER_SIZE_1B;
    uint8_t numChan = 0;
    uint32_t temp;

    if (RxState[instNum]->ElementSize == 1U)
    {
        xferSize = EDMA_TRANSFER_SIZE_1B;
    }
    else if (RxState[instNum]->ElementSize == 2U)
    {
        xferSize = EDMA_TRANSFER_SIZE_2B;
    }
    else if (RxState[instNum]->ElementSize == 4U)
    {
        xferSize = EDMA_TRANSFER_SIZE_4B;
    }
    else
    {
        DEV_ASSERT(false);
    }
    RxState[instNum]->status = STATUS_BUSY;
    if ((RxState[instNum]->mux == SAI_MUX_LINE) || (RxState[instNum]->mux == SAI_MUX_DISABLED))
    {
        c = 0;
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsRxChannelEnabled(inst, i))
            {
                DEV_ASSERT(data[c] != NULL);
                RxState[instNum]->ChnState[i].count = count;
                (void)EDMA_DRV_ConfigMultiBlockTransfer(RxState[instNum]->DmaChannel[c], EDMA_TRANSFER_PERIPH2MEM,
                                                        (uint32_t)(&(inst->RDR[i])), (uint32_t)data[c], xferSize,
                                                        RxState[instNum]->ElementSize, count, true);

                /* Call driver function to end the transmission when the DMA transfer is done */
                (void)EDMA_DRV_InstallCallback(RxState[instNum]->DmaChannel[c],
                                               (edma_callback_t)(SAI_DRV_CompleteReceiveDataUsingDma),
                                               (void*)(instNum | ((uint32_t)i << 16U) | ((uint32_t)c << 8U)));
                /* Start the DMA channel */
                (void)EDMA_DRV_StartChannel(RxState[instNum]->DmaChannel[c]);
                c++;
            }
        }
    }
    else if (RxState[instNum]->mux == SAI_MUX_MEM)
    {
        DEV_ASSERT(data[0] != NULL);
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsRxChannelEnabled(inst, i))
            {
                numChan++;
            }
        }
        temp = count * numChan;
        RxState[instNum]->ChnState[0].count = count;

        (void)EDMA_DRV_ConfigMultiBlockTransfer(RxState[instNum]->DmaChannel[0], EDMA_TRANSFER_PERIPH2MEM,
                                                (uint32_t)(&(inst->RDR[0])), (uint32_t)data[0], xferSize,
                                                RxState[instNum]->ElementSize, temp, true);

        /* Call driver function to end the transmission when the DMA transfer is done */
        (void)EDMA_DRV_InstallCallback(RxState[instNum]->DmaChannel[0],
                                       (edma_callback_t)(SAI_DRV_CompleteReceiveDataUsingDma),
                                       (void*)(instNum));
        /* Start the DMA channel */
        (void)EDMA_DRV_StartChannel(RxState[instNum]->DmaChannel[0]);
    }
    else
    {
        DEV_ASSERT(false);
    }
    /* Enable rx DMA requests for the current instance */
    SAI_DRV_RxEnableFifoReqDma(inst);
}
/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_FCDInit
* Description   : Init fractional clock divider
*                 Only DIVIDE in register is filled for 50% duty cycle
* Implements    : SAI_DRV_FCDInit_Activity
*END*************************************************************************/
#ifdef FEATURE_SAI_MSEL_FCD
void SAI_DRV_FCDInit(uint32_t instNum,
                     sai_fcd_clock_source_t fcdSource,
                     uint32_t freqDivisor,
                     bool MclkBypassFcd)
{
    SAI_Type* inst = SAIBase[instNum];
    uint32_t SourceClock;
    clock_names_t clkName = END_OF_PERIPHERAL_CLKS;
    uint32_t divisor = 0U;

    if (!MclkBypassFcd)
    {
        DEV_ASSERT(freqDivisor > 0UL);
        if ((fcdSource == SAI_FCD_PLL) || (fcdSource == SAI_FCD_FXOSC))
        {
            if (fcdSource == SAI_FCD_PLL)
            {
                clkName = PLL_CLK;
            }
            else if (fcdSource == SAI_FCD_FXOSC)
            {
                clkName = FXOSC_CLK;
            }
            else
            {
                /* do nothing */
            }
            (void)CLOCK_SYS_GetFreq(clkName, &SourceClock);
            DEV_ASSERT(SourceClock > 0U);
            divisor = SourceClock / freqDivisor;
            if (divisor == 0UL)
            {
                divisor = 1UL;
            }
        }
        else if (fcdSource == SAI_FCD_MCLK)
        {
            DEV_ASSERT(freqDivisor > 0UL);
            divisor = freqDivisor;
        }
        else
        {
            /* do nothing */
        }
        if (divisor > 0x1000UL) /* DIVIDE field is 12 bit */
        {
            divisor = 0x1000UL;
        }
        inst->MDR = SAI_MDR_DIVIDE(divisor - 1UL);
        inst->MCR = SAI_MCR_MOE(1U) | SAI_MCR_MICS(fcdSource);
    }
    else
    {
        inst->MCR = 0;
    }
}
#endif /* FEATURE_SAI_MSEL_FCD */
/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_TxInit
* Description   : Init tx core
* These params are set by driver:
*      bool BIT_CLK_SWAP = false             < Enable or disable bit clock swap mode.>
*      bool BIT_CLK_AS_EXTERNAL = false;      < Delay internal clock as if clock is generated externally.>
*      uint8_t BitClkDiv = calculate for nearest value to user config value,
*                       only set by driver if master clock and bit clock is internal, otherwise set by user
*                                          < If bit clock is internal, it is divided from master clock by this.>
*      uint8_t WORD_FLAG_INDEX = 0;          < Index of word in frame that raise word start flag.>
*      bool CONT_ON_ERROR = true;            < Continue on fifo error.>
*      sai_combine_mode CombineMode = combine line or combine memory, user choice
*                                          < Select fifo combine mode.>
*      sai_packing_mode PACK_MODE = false;     < Select fifo pack mode.>
*      bool SYNC_ON_DEMAND; = true            < Generate frame sync only when fifo is not empty (transmit)
*                                             or not full (receive).>
* Implements    : SAI_DRV_TxInit_Activity
*END*************************************************************************/
void SAI_DRV_TxInit(uint32_t instNum,
                   const sai_user_config_t* saiUserConfig,
                   sai_state_t* StateAlloc)
{
    SAI_Type* inst = SAIBase[instNum];
    uint32_t SourceClock = 0U;

    DEV_ASSERT(saiUserConfig != NULL);
    DEV_ASSERT(StateAlloc != NULL);
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);
    DEV_ASSERT((saiUserConfig->FrameSize <= (1U << (SAI_DRV_GetParamFrameSize(inst)))) && (saiUserConfig->FrameSize > 0U));
    /* Check if current instance is clock gated off. */
    DEV_ASSERT(CLOCK_SYS_GetFreq(SAIClkNames[instNum], &SourceClock) == STATUS_SUCCESS);
#ifdef DEV_ERROR_DETECT
    uint32_t rcsr = inst->RCSR;
    /* check if sync with other and other must be enabled later */
    DEV_ASSERT((saiUserConfig->SyncMode != SAI_SYNC_WITH_OTHER) ||
              ((saiUserConfig->SyncMode == SAI_SYNC_WITH_OTHER) && ((rcsr & SAI_RCSR_RE_MASK) == 0U)));
#ifdef FEATURE_SAI_SYNC_WITH_OTHER_INST
    uint32_t sai2Tcsr;
    uint32_t sai2Rcsr;
    /* sai2 cannot sync with sai2 */
    DEV_ASSERT(!((instNum == 2UL) &&
                 ((saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_SAME) || (saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_OTHER))));
    /* sync with sai2 and sai2 same part must be enabled later */
    if (saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_SAME)
    {
        sai2Tcsr = SAIBase[2U]->TCSR;
        DEV_ASSERT((sai2Tcsr & SAI_TCSR_TE_MASK) == 0U);
    }
    /* sync with sai2 and sai2 other part must be enabled later */
    if (saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_OTHER)
    {
        sai2Rcsr = SAIBase[2U]->RCSR;
        DEV_ASSERT((sai2Rcsr & SAI_RCSR_RE_MASK) == 0U);
    }
#endif /* FEATURE_SAI_SYNC_WITH_OTHER_INST */
#endif /* DEV_ERROR_DETECT */
    DEV_ASSERT(saiUserConfig->Word0Width > 0U);
    DEV_ASSERT(saiUserConfig->WordNWidth > 0U);
    DEV_ASSERT(saiUserConfig->SyncWidth > 0U);
    DEV_ASSERT(saiUserConfig->FirstBitIndex < 32U);
    DEV_ASSERT((saiUserConfig->ElementSize == 1U) || (saiUserConfig->ElementSize == 2U) || (saiUserConfig->ElementSize == 4U));

    uint32_t divisor = 0U;
    uint8_t CombineMode = 0U;
    status_t osifError;
    uint8_t i;
    uint8_t channel = 0U;
    bool clkInternal = false;

    TxState[instNum] = StateAlloc;
    SAI_DRV_TxResetVar(instNum);
    /* calculate divisor parameter */
    /* Get clock as configured in the clock manager */
    if ((saiUserConfig->SyncMode == SAI_ASYNC) && saiUserConfig->BitClkInternal)
    {
        switch(saiUserConfig->MasterClkSrc)
        {
#ifdef FEATURE_SAI_MSEL_FCD
            case SAI_FCD_CLK:
                SAI_DRV_GetFCDFreq(instNum, &SourceClock);
                clkInternal = ((SourceClock > 0UL) ? true : false);
                break;
#endif
#ifdef FEATURE_SAI_MSEL_BUS_CLK
            case SAI_BUS_CLK:
                clkInternal = true;
    #if (defined(CPU_MPC5748G) || defined(CPU_MPC5746C))
                (void)CLOCK_SYS_GetFreq(SAIClkNames[instNum], &SourceClock);
    #elif (defined(CPU_S32K148))
                (void)CLOCK_SYS_GetFreq(BUS_CLK, &SourceClock);
    #endif
                break;
#endif
#ifdef FEATURE_SAI_MSEL_SOSC_CLK
            case SAI_SOSC_CLK:
                clkInternal = true;
    #ifdef CPU_S32K148
                (void)CLOCK_SYS_GetFreq(SOSC_CLK, &SourceClock);
    #endif
                break;
#endif
            default:
                /* do nothing */
                break;
        }
        if (clkInternal)
        {
            DEV_ASSERT(SourceClock > 0UL);
            DEV_ASSERT(saiUserConfig->BitClkFreq != 0U);
            divisor = ((SourceClock / saiUserConfig->BitClkFreq) / 2UL);
            if (divisor > 0U)
            {
                divisor -= 1UL;
            }
            if (divisor > 255U)
            {
                divisor = 255U;
            }
        }
        else
        {
            divisor = ((uint32_t)saiUserConfig->BitClkDiv / 2UL);
            if (divisor > 0U)
            {
                divisor -= 1UL;
            }
            if (divisor > 255U)
            {
                divisor = 255U;
            }
        }
    }
    TxState[instNum]->ElementSize = saiUserConfig->ElementSize;
    TxState[instNum]->XferType = saiUserConfig->TransferType;
    TxState[instNum]->mux = saiUserConfig->MuxMode;
    if (saiUserConfig->TransferType == SAI_DMA)
    {
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            TxState[instNum]->DmaChannel[i] = saiUserConfig->DmaChannel[i];
        }
        CombineMode = (uint8_t)saiUserConfig->MuxMode;
    }
    else
    {
        CombineMode = 0U; /* emulate combine mode */
        TxState[instNum]->ChannelCount = saiUserConfig->ChannelCount;
    }
    channel = saiUserConfig->ChannelEnable;
    osifError = OSIF_SemaCreate(&TxState[instNum]->Sema, 0U);
    DEV_ASSERT(osifError == STATUS_SUCCESS);
    inst->TCSR = SAI_TCSR_SR(1U);  /* call reset bit, reset fifo and logic */
    inst->TCSR   = SAI_TCSR_FEIE(TO_BIT(saiUserConfig->RunErrorReport)) |
                   SAI_TCSR_SEIE(TO_BIT(saiUserConfig->SyncErrorReport)) |
                   SAI_TCSR_WSIE(TO_BIT(saiUserConfig->FrameStartReport));
    TxState[instNum]->Callback = saiUserConfig->callback;
    inst->TCR1 = SAI_TCR1_TFW(TX_WATERMARK);
    inst->TCR2 = SAI_TCR2_SYNC(saiUserConfig->SyncMode) |
                   SAI_TCR2_BCS(TO_BIT(BIT_CLK_SWAP)) |
                   SAI_TCR2_BCI(TO_BIT(BIT_CLK_AS_EXTERNAL)) |
                   SAI_TCR2_MSEL(saiUserConfig->MasterClkSrc) |
                   SAI_TCR2_BCP(TO_BIT(saiUserConfig->BitClkNegPolar)) |
                   SAI_TCR2_BCD(TO_BIT(saiUserConfig->BitClkInternal)) |
                   SAI_TCR2_DIV((uint8_t) divisor);
    inst->TCR3 = SAI_TCR3_TCE(channel) |
                   SAI_TCR3_WDFL(WORD_FLAG_INDEX);
    inst->TCR4 = SAI_TCR4_FCONT(TO_BIT(CONT_ON_ERROR)) |
                   SAI_TCR4_FCOMB(CombineMode) |
                   SAI_TCR4_FPACK(PACK_MODE) |
                   SAI_TCR4_FRSZ((uint32_t)saiUserConfig->FrameSize - 1U) |
                   SAI_TCR4_SYWD((uint32_t)saiUserConfig->SyncWidth - 1U) |
#ifdef FEATURE_SAI_HAS_CHMOD
                   SAI_TCR4_CHMOD(saiUserConfig->MaskMode) |
#endif /* FEATURE_SAI_HAS_CHMOD */
                   SAI_TCR4_MF(TO_BIT(saiUserConfig->MsbFirst)) |
                   SAI_TCR4_FSE(TO_BIT(saiUserConfig->SyncEarly)) |
                   SAI_TCR4_ONDEM(TO_BIT(SYNC_ON_DEMAND)) |
                   SAI_TCR4_FSP(TO_BIT(saiUserConfig->SyncNegPolar)) |
                   SAI_TCR4_FSD(TO_BIT(saiUserConfig->SyncInternal));
    inst->TCR5 = SAI_TCR5_WNW((uint32_t)saiUserConfig->WordNWidth - 1U) |
                   SAI_TCR5_W0W((uint32_t)saiUserConfig->Word0Width - 1U) |
                   SAI_TCR5_FBT(saiUserConfig->MsbFirst ? ~(31U - (uint32_t)saiUserConfig->FirstBitIndex) : (uint32_t)saiUserConfig->FirstBitIndex);
    SAI_DRV_TxResetFifo(inst);
    if ((saiUserConfig->TransferType == SAI_INTERRUPT) || (saiUserConfig->RunErrorReport) ||
        (saiUserConfig->SyncErrorReport) || (saiUserConfig->FrameStartReport))
    {
        INT_SYS_EnableIRQ(SAITxIrqId[instNum]);
    };
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_RxInit
* Description   : Init rx core
* These params are set by driver:
*      Others are the same as TxInit
* Implements    : SAI_DRV_RxInit_Activity
*END*************************************************************************/
void SAI_DRV_RxInit(uint32_t instNum,
                    const sai_user_config_t* saiUserConfig,
                    sai_state_t* StateAlloc)
{
    SAI_Type* inst = SAIBase[instNum];
    uint32_t SourceClock = 0U;

    DEV_ASSERT(saiUserConfig != NULL);
    DEV_ASSERT(StateAlloc != NULL);
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);
    DEV_ASSERT((saiUserConfig->FrameSize <= (1U << (SAI_DRV_GetParamFrameSize(inst)))) && (saiUserConfig->FrameSize > 0U));
    /* Check if current instance is clock gated off. */
    DEV_ASSERT(CLOCK_SYS_GetFreq(SAIClkNames[instNum], &SourceClock) == STATUS_SUCCESS);
#ifdef DEV_ERROR_DETECT
    uint32_t tcsr = inst->TCSR;
    /* check if sync with other and other is not enabled */
    DEV_ASSERT((saiUserConfig->SyncMode != SAI_SYNC_WITH_OTHER) ||
              ((saiUserConfig->SyncMode == SAI_SYNC_WITH_OTHER) && ((tcsr & SAI_TCSR_TE_MASK) == 0U)));
#ifdef FEATURE_SAI_SYNC_WITH_OTHER_INST
    uint32_t sai2Tcsr;
    uint32_t sai2Rcsr;
    /* sai2 cannot sync with sai2 */
    DEV_ASSERT(!((instNum == 2UL) &&
                 ((saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_SAME) || (saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_OTHER))));
    /* if sync with sai2 then sai2 same part must be enabled later */
    if (saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_OTHER)
    {
        sai2Tcsr = SAIBase[2U]->TCSR;
        DEV_ASSERT((sai2Tcsr & SAI_TCSR_TE_MASK) == 0U);
    }
    /* if sync with sai2 then sai2 other part must be enabled later */
    if (saiUserConfig->SyncMode == SAI_SYNC_WITH_SAI2_SAME)
    {
        sai2Rcsr = SAIBase[2U]->RCSR;
        DEV_ASSERT((sai2Rcsr & SAI_RCSR_RE_MASK) == 0U);
    }
#endif /* FEATURE_SAI_SYNC_WITH_OTHER_INST */
#endif /* DEV_ERROR_DETECT */
    DEV_ASSERT(saiUserConfig->Word0Width > 0U);
    DEV_ASSERT(saiUserConfig->WordNWidth > 0U);
    DEV_ASSERT(saiUserConfig->SyncWidth > 0U);
    DEV_ASSERT(saiUserConfig->FirstBitIndex < 32U);
    DEV_ASSERT((saiUserConfig->ElementSize == 1U) || (saiUserConfig->ElementSize == 2U) || (saiUserConfig->ElementSize == 4U));

    uint32_t divisor = 0U;
    uint8_t CombineMode = 0U;
    status_t osifError;
    uint8_t i;
    uint8_t channel = 0U;
    bool clkInternal = false;

    RxState[instNum] = StateAlloc;
    SAI_DRV_RxResetVar(instNum);
    /* calculate divisor parameter */
    /* Get clock as configured in the clock manager */
    if ((saiUserConfig->SyncMode == SAI_ASYNC) && saiUserConfig->BitClkInternal)
    {
        switch(saiUserConfig->MasterClkSrc)
        {
#ifdef FEATURE_SAI_MSEL_FCD
            case SAI_FCD_CLK:
                SAI_DRV_GetFCDFreq(instNum, &SourceClock);
                clkInternal = ((SourceClock > 0UL) ? true : false);
                break;
#endif
#ifdef FEATURE_SAI_MSEL_BUS_CLK
            case SAI_BUS_CLK:
                clkInternal = true;
    #if (defined(CPU_MPC5748G) || defined(CPU_MPC5746C))
                (void)CLOCK_SYS_GetFreq(SAIClkNames[instNum], &SourceClock);
    #elif (defined(CPU_S32K148))
                (void)CLOCK_SYS_GetFreq(BUS_CLK, &SourceClock);
    #endif
                break;
#endif
#ifdef FEATURE_SAI_MSEL_SOSC_CLK
            case SAI_SOSC_CLK:
                clkInternal = true;
    #ifdef CPU_S32K148
                (void)CLOCK_SYS_GetFreq(SOSC_CLK, &SourceClock);
    #endif
                break;
#endif
                default:
                /* do nothing */
                break;

        }
        if (clkInternal)
        {
            DEV_ASSERT(SourceClock > 0UL);
            DEV_ASSERT(saiUserConfig->BitClkFreq != 0U);
            divisor = ((SourceClock / saiUserConfig->BitClkFreq) / 2UL);
            if (divisor > 0U)
            {
                divisor -= 1UL;
            }
            if (divisor > 255U)
            {
                divisor = 255U;
            }
        }
        else
        {
            divisor = ((uint32_t)saiUserConfig->BitClkDiv / 2UL);
            if (divisor > 0U)
            {
                divisor -= 1UL;
            }
            if (divisor > 255U)
            {
                divisor = 255U;
            }
        }
    }
    RxState[instNum]->ElementSize = saiUserConfig->ElementSize;
    RxState[instNum]->XferType = saiUserConfig->TransferType;
    RxState[instNum]->mux = saiUserConfig->MuxMode;
    if (saiUserConfig->TransferType == SAI_DMA)
    {
        for (i = 0; i < sai_channel_count[instNum]; i++)
        {
            RxState[instNum]->DmaChannel[i] = saiUserConfig->DmaChannel[i];
        }
        CombineMode = (uint8_t)saiUserConfig->MuxMode;
    }
    else
    {
        CombineMode = 0U; /* emulate combine mode */
        RxState[instNum]->ChannelCount = saiUserConfig->ChannelCount;
    }
    channel = saiUserConfig->ChannelEnable;
    osifError = OSIF_SemaCreate(&RxState[instNum]->Sema, 0U);
    DEV_ASSERT(osifError == STATUS_SUCCESS);
    inst->RCSR = SAI_RCSR_SR(1UL);  /* call reset bit, reset fifo and logic */
    inst->RCSR   = SAI_RCSR_FEIE(TO_BIT(saiUserConfig->RunErrorReport)) |
                   SAI_RCSR_SEIE(TO_BIT(saiUserConfig->SyncErrorReport)) |
                   SAI_RCSR_WSIE(TO_BIT(saiUserConfig->FrameStartReport));
    RxState[instNum]->Callback = saiUserConfig->callback;
    inst->RCR1 = SAI_RCR1_RFW((saiUserConfig->TransferType == SAI_DMA) ? 0UL : RX_WATERMARK);
    inst->RCR2 = SAI_RCR2_SYNC(saiUserConfig->SyncMode) |
                   SAI_RCR2_BCS(TO_BIT(BIT_CLK_SWAP)) |
                   SAI_RCR2_BCI(TO_BIT(BIT_CLK_AS_EXTERNAL)) |
                   SAI_RCR2_MSEL(saiUserConfig->MasterClkSrc) |
                   SAI_RCR2_BCP(TO_BIT(saiUserConfig->BitClkNegPolar)) |
                   SAI_RCR2_BCD(TO_BIT(saiUserConfig->BitClkInternal)) |
                   SAI_RCR2_DIV((uint8_t) divisor);
    inst->RCR3 = SAI_RCR3_RCE(channel) |
                   SAI_RCR3_WDFL(WORD_FLAG_INDEX);
    inst->RCR4 = SAI_RCR4_FCONT(TO_BIT(CONT_ON_ERROR)) |
                   SAI_RCR4_FCOMB(CombineMode) |
                   SAI_RCR4_FPACK(PACK_MODE) |
                   SAI_RCR4_FRSZ(saiUserConfig->FrameSize - 1UL) |
                   SAI_RCR4_SYWD(saiUserConfig->SyncWidth - 1UL) |
                   SAI_RCR4_MF(TO_BIT(saiUserConfig->MsbFirst)) |
                   SAI_RCR4_FSE(TO_BIT(saiUserConfig->SyncEarly)) |
                   SAI_RCR4_ONDEM(TO_BIT(SYNC_ON_DEMAND)) |
                   SAI_RCR4_FSP(TO_BIT(saiUserConfig->SyncNegPolar)) |
                   SAI_RCR4_FSD(TO_BIT(saiUserConfig->SyncInternal));
    inst->RCR5 = SAI_RCR5_WNW((uint32_t)saiUserConfig->WordNWidth - 1UL) |
                   SAI_RCR5_W0W((uint32_t)saiUserConfig->Word0Width - 1UL) |
                   SAI_RCR5_FBT(saiUserConfig->MsbFirst ? ~(31UL - (uint32_t)saiUserConfig->FirstBitIndex) : (uint32_t)saiUserConfig->FirstBitIndex);
    SAI_DRV_RxResetFifo(inst);
    if ((saiUserConfig->TransferType == SAI_INTERRUPT) || (saiUserConfig->RunErrorReport) ||
        (saiUserConfig->SyncErrorReport) || (saiUserConfig->FrameStartReport))
    {
        INT_SYS_EnableIRQ(SAIRxIrqId[instNum]);
    };
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxDeinit
 * Description   : DeInit tx core
 * Implements    : SAI_DRV_TxDeinit_Activity
 *
 *END**************************************************************************/
void SAI_DRV_TxDeinit(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    SAI_Type* inst = SAIBase[instNum];
    inst->TCSR = 0; /* clear all interrupt enable bits */
    inst->TCSR = SAI_TCSR_SR(1U);  /* call reset bit*/
    inst->TMR = 0; /* reset mask bits */
    if (TxState[instNum]->XferType == SAI_INTERRUPT)
    {
        /* Disable interrupt. */
        INT_SYS_DisableIRQ(SAITxIrqId[instNum]);
    }
    (void)OSIF_SemaDestroy(&TxState[instNum]->Sema);
    SAI_DRV_TxResetVar(instNum);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_RxDeinit
 * Description   : DeInit rx core
 * Implements    : SAI_DRV_RxDeinit_Activity
 *
 *END**************************************************************************/
void SAI_DRV_RxDeinit(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    SAI_Type* inst = SAIBase[instNum];
    inst->RCSR = 0; /* clear all interrupt enable bits */
    inst->RCSR = SAI_RCSR_SR(1U);  /* call reset bit*/
    inst->RMR = 0; /* reset mask bits */
    if (RxState[instNum]->XferType == SAI_INTERRUPT)
    {
        /* Disable interrupt. */
        INT_SYS_DisableIRQ(SAIRxIrqId[instNum]);
    }
    (void)OSIF_SemaDestroy(&RxState[instNum]->Sema);
    SAI_DRV_RxResetVar(instNum);
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_RxGetBitClockFreq
* Description   : Return true freq
* Implements    : SAI_DRV_RxGetBitClockFreq_Activity
*
*END**************************************************************************/
uint32_t SAI_DRV_RxGetBitClockFreq(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    const SAI_Type* inst = SAIBase[instNum];
    uint32_t div;
    uint32_t SourceClock = 0U;
    uint32_t clkSource;
    bool clkInternal = false;

    /* check if sync with other */
    DEV_ASSERT ((inst->RCR2 & SAI_RCR2_SYNC_MASK) == 0U);
    /* check if master clock is internal */
    clkSource = ((inst->RCR2 & SAI_RCR2_MSEL_MASK) >> SAI_RCR2_MSEL_SHIFT);
    switch (clkSource)
    {
#ifdef FEATURE_SAI_MSEL_BUS_CLK
            case (uint32_t)SAI_BUS_CLK:
                clkInternal = true;
    #if (defined(CPU_MPC5748G) || defined(CPU_MPC5746C))
                (void)CLOCK_SYS_GetFreq(SAIClkNames[instNum], &SourceClock);
    #elif (defined(CPU_S32K148))
                (void)CLOCK_SYS_GetFreq(BUS_CLK, &SourceClock);
    #endif
                break;
#endif
#ifdef FEATURE_SAI_MSEL_MCLK_PIN
            case (uint32_t)SAI_EXTERNAL_CLK:
                clkInternal = false;
                break;
#endif
#ifdef FEATURE_SAI_MSEL_FCD
            case (uint32_t)SAI_FCD_CLK:
                SAI_DRV_GetFCDFreq(instNum, &SourceClock);
                clkInternal = ((SourceClock > 0UL) ? true : false);
                break;
#endif
#ifdef FEATURE_SAI_MSEL_SOSC_CLK
            case (uint32_t)SAI_SOSC_CLK:
                clkInternal = true;
    #ifdef CPU_S32K148
                (void)CLOCK_SYS_GetFreq(SOSC_CLK, &SourceClock);
    #endif
                break;
#endif
#ifdef FEATURE_SAI_MSEL_OTHER_MCLK_PIN
            case (uint32_t)SAI_EXTERNAL_CLK_1:
            case (uint32_t)SAI_EXTERNAL_CLK_2:
                clkInternal = false;
                break;
#endif
            default:
                DEV_ASSERT(false);
                break;
    }
    /* check if external master clock or external bit clock */
#ifdef DEV_ERROR_DETECT
    uint32_t rcr2 = inst->RCR2;
    DEV_ASSERT (clkInternal && ((rcr2 & SAI_TCR2_BCD_MASK) == SAI_RCR2_BCD(1U)));
#endif /* DEV_ERROR_DETECT */

    div = SAI_DRV_RxGetBitClockDiv(instNum);
    return SourceClock / div;
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_TxGetBitClockFreq
* Description   : Return true freq
* Implements    : SAI_DRV_TxGetBitClockFreq_Activity
*
*END**************************************************************************/
uint32_t SAI_DRV_TxGetBitClockFreq(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    const SAI_Type* inst = SAIBase[instNum];
    uint32_t div;
    uint32_t SourceClock = 0U;
    uint32_t clkSource;
    bool clkInternal = false;

    /* check if sync with other */
    DEV_ASSERT ((inst->TCR2 & SAI_TCR2_SYNC_MASK) == 0U);
    /* check if master clock is internal */
    clkSource = (inst->TCR2 & SAI_TCR2_MSEL_MASK) >> SAI_TCR2_MSEL_SHIFT;
    switch (clkSource)
    {
#ifdef FEATURE_SAI_MSEL_BUS_CLK
            case (uint32_t)SAI_BUS_CLK:
                clkInternal = true;
    #if (defined(CPU_MPC5748G) || defined(CPU_MPC5746C))
                (void)CLOCK_SYS_GetFreq(SAIClkNames[instNum], &SourceClock);
    #elif (defined(CPU_S32K148))
                (void)CLOCK_SYS_GetFreq(BUS_CLK, &SourceClock);
    #endif
                break;
#endif
#ifdef FEATURE_SAI_MSEL_MCLK_PIN
            case (uint32_t)SAI_EXTERNAL_CLK:
                clkInternal = false;
                break;
#endif
#ifdef FEATURE_SAI_MSEL_FCD
            case (uint32_t)SAI_FCD_CLK:
                SAI_DRV_GetFCDFreq(instNum, &SourceClock);
                clkInternal = ((SourceClock > 0UL) ? true : false);
                break;
#endif
#ifdef FEATURE_SAI_MSEL_SOSC_CLK
            case (uint32_t)SAI_SOSC_CLK:
                clkInternal = true;
    #ifdef CPU_S32K148
                (void)CLOCK_SYS_GetFreq(SOSC_CLK, &SourceClock);
    #endif
                break;
#endif
#ifdef FEATURE_SAI_MSEL_OTHER_MCLK_PIN
            case (uint32_t)SAI_EXTERNAL_CLK_1:
            case (uint32_t)SAI_EXTERNAL_CLK_2:
                clkInternal = false;
                break;
#endif
            default:
                DEV_ASSERT(false);
                break;
    }
    /* check if external master clock or external bit clock */
#ifdef DEV_ERROR_DETECT
    uint32_t tcr2 = inst->TCR2;
    DEV_ASSERT (clkInternal && ((tcr2 & SAI_TCR2_BCD_MASK) == SAI_TCR2_BCD(1U)));
#endif /* DEV_ERROR_DETECT */

    div = SAI_DRV_TxGetBitClockDiv(instNum);
    return SourceClock / div;
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_TxGetBitClockDiv
* Description   : Return true divisor
* Implements    : SAI_DRV_TxGetBitClockDiv_Activity
*
*END**************************************************************************/
uint32_t SAI_DRV_TxGetBitClockDiv(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    const SAI_Type* inst = SAIBase[instNum];
    return (((inst->TCR2 & SAI_TCR2_DIV_MASK) >> SAI_TCR2_DIV_SHIFT) + 1U) * 2U;
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_RxGetBitClockDiv
* Description   : Return true divisor
* Implements    : SAI_DRV_RxGetBitClockDiv_Activity
*
*END**************************************************************************/
uint32_t SAI_DRV_RxGetBitClockDiv(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    const SAI_Type* inst = SAIBase[instNum];
    return (((inst->RCR2 & SAI_RCR2_DIV_MASK) >> SAI_RCR2_DIV_SHIFT) + 1U) * 2U;
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_GetFCDFreq
* Description   : Return true fcd frequency and if clock is from internal
* Implements    : SAI_DRV_GetFCDFreq_Activity
*
*END**************************************************************************/
#ifdef FEATURE_SAI_MSEL_FCD
void SAI_DRV_GetFCDFreq(uint32_t instNum, uint32_t* freq)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    const SAI_Type* inst = SAIBase[instNum];
    uint32_t source;
    clock_names_t clkName = END_OF_PERIPHERAL_CLKS;
    uint32_t SourceClock;
    uint32_t div;

    *freq = 0;
    /* check if fcd is bypassed */
    if ((inst->MCR & SAI_MCR_MOE_MASK) != 0UL)
    {
        source = ((inst->MCR & SAI_MCR_MICS_MASK) >> SAI_MCR_MICS_SHIFT);
        if ((source == (uint32_t)SAI_FCD_PLL) || (source == (uint32_t)SAI_FCD_FXOSC))
        {
            if (source == (uint32_t)SAI_FCD_PLL)
            {
                clkName = PLL_CLK;
            }
            else if (source == (uint32_t)SAI_FCD_FXOSC)
            {
                clkName = FXOSC_CLK;
            }
            else
            {
                /* do nothing */
            }
            (void)CLOCK_SYS_GetFreq(clkName, &SourceClock);
            div = ((inst->MDR & SAI_MDR_DIVIDE_MASK) >> SAI_MDR_DIVIDE_SHIFT) + 1UL;
            *freq = SourceClock / div;
        }
    }
}
#endif /* FEATURE_SAI_MSEL_FCD */

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_TxSetMaskWord
* Description   : Set next frame masked word index
* Implements    : SAI_DRV_TxSetNextMaskWords_Activity
*
*END**************************************************************************/
void SAI_DRV_TxSetNextMaskWords(uint32_t instNum, uint16_t Words)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    SAI_Type* inst = SAIBase[instNum];
    inst->TMR = Words;
}

/*FUNCTION**********************************************************************
*
* Function Name : SAI_DRV_RxSetMaskWord
* Description   : Set next frame masked word index
* Implements : SAI_DRV_RxSetNextMaskWords_Activity
*
*END**************************************************************************/
void SAI_DRV_RxSetNextMaskWords(uint32_t instNum, uint16_t Words)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    SAI_Type* inst = SAIBase[instNum];
    inst->RMR = Words;
}

/*****************************SENDING FUNCTIONS************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_SendBlocking
 * Description   : Send block of data, return when finish sending
 * Implements    : SAI_DRV_SendBlocking_Activity
 *
 *END**************************************************************************/
status_t SAI_DRV_SendBlocking(uint32_t instNum,
                            const uint8_t* data[],
                            uint32_t count,
                            uint32_t timeout)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);
    DEV_ASSERT(TxState[instNum]->status != STATUS_BUSY);

    status_t res;
    status_t osifError = STATUS_SUCCESS;

    TxState[instNum]->Blocking = true;
    SAI_DRV_Send (instNum, data, count);
    if (TxState[instNum]->status == STATUS_BUSY)
    {
        osifError = OSIF_SemaWait(&TxState[instNum]->Sema, timeout);
    }
    TxState[instNum]->Blocking = false;
    if (osifError == STATUS_TIMEOUT)
    {
        /* abort current transfer */
        SAI_DRV_AbortSending (instNum);
        res = STATUS_TIMEOUT;
    }
    else
    {
        DEV_ASSERT(TxState[instNum]->status != STATUS_BUSY);
        /* not time out */
        res = TxState[instNum]->status;
    }

    return res;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxFilFifo
 * Description   : Fill fifo before enable tx to avoid run error flag
 *
 *END**************************************************************************/
static bool SAI_DRV_TxFillFifoInterrupt(uint32_t instNum)
{
    SAI_Type* inst = SAIBase[instNum];
    uint8_t i;
    uint32_t temp = 0UL;
    bool finish;
    bool full;
    bool isTxFull;

    finish = false;
    if (TxState[instNum]->mux == SAI_MUX_LINE)
    {
        for (i = 0U; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsTxChannelEnabled(inst, i))
            {
                isTxFull = SAI_DRV_IsTxFifoFull(inst, i);
                while (!((TxState[instNum]->ChnState[0].count == 0UL) || isTxFull))
                {
                    if (TxState[instNum]->ElementSize == 1U)
                    {
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*TxState[instNum]->ChnState[TxState[instNum]->NextChn].data);
                    }
                    else if (TxState[instNum]->ElementSize == 2U)
                    {
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) TxState[instNum]->ChnState[TxState[instNum]->NextChn].data));
                    }
                    else if (TxState[instNum]->ElementSize == 4U)
                    {
                        SAI_DRV_TxWrite(inst, i, *((uint32_t*) TxState[instNum]->ChnState[TxState[instNum]->NextChn].data));
                    }
                    else
                    {
                        DEV_ASSERT(false);
                    }
                    TxState[instNum]->ChnState[TxState[instNum]->NextChn].data += TxState[instNum]->ElementSize;
                    TxState[instNum]->NextChn++; /* alternate between data block */
                    if (TxState[instNum]->NextChn == TxState[instNum]->ChannelCount)
                    {
                        TxState[instNum]->NextChn = 0U;
                    }
                    TxState[instNum]->ChnState[0].count--;
                    isTxFull = SAI_DRV_IsTxFifoFull(inst, i);
                }
                if (TxState[instNum]->ChnState[0].count == 0UL)
                {
                    finish = true;
                }
                break; /* only one channle enabled in this mode */
            }
        }
    }
    else if (TxState[instNum]->mux == SAI_MUX_MEM)
    {
        full = false;
        while (!((TxState[instNum]->ChnState[0].count == 0UL) || (full)))
        {
            for (i = 0U; i < sai_channel_count[instNum]; i++)
            {
                /* check the first enabled channel fifo */
                if (SAI_DRV_IsTxFifoFull(inst, i))
                {
                    full = true;
                    break;
                }
                if (SAI_DRV_IsTxChannelEnabled(inst, i))
                {
                    if (TxState[instNum]->ElementSize == 1U)
                    {
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*TxState[instNum]->ChnState[0].data);
                    }
                    else if (TxState[instNum]->ElementSize == 2U)
                    {
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) TxState[instNum]->ChnState[0U].data));
                    }
                    else if (TxState[instNum]->ElementSize == 4U)
                    {
                        SAI_DRV_TxWrite(inst, i, *((uint32_t*) TxState[instNum]->ChnState[0U].data));
                    }
                    else
                    {
                        DEV_ASSERT(false);
                    }
                    TxState[instNum]->ChnState[0U].data += TxState[instNum]->ElementSize;
                }
            }
            if (!full)
            {
                TxState[instNum]->ChnState[0U].count--;
            }
        }
        if (TxState[instNum]->ChnState[0].count == 0UL)
        {
            finish = true;
        }
    }
    else /* mux disabled */
    {
        for (i = 0U; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsTxChannelEnabled(inst, i))
            {
                isTxFull = SAI_DRV_IsTxFifoFull(inst, i);
                temp = TxState[instNum]->ChnState[0].count;
                while (!((temp == 0UL) || isTxFull))
                {
                    if (TxState[instNum]->ElementSize == 1U)
                    {
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*TxState[instNum]->ChnState[i].data);
                    }
                    else if (TxState[instNum]->ElementSize == 2U)
                    {
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) TxState[instNum]->ChnState[i].data));
                    }
                    else if (TxState[instNum]->ElementSize == 4U)
                    {
                        SAI_DRV_TxWrite(inst, i, *((uint32_t*) TxState[instNum]->ChnState[i].data));
                    }
                    else
                    {
                        DEV_ASSERT(false);
                    }
                    TxState[instNum]->ChnState[i].data += TxState[instNum]->ElementSize;
                    temp--;
                    isTxFull = SAI_DRV_IsTxFifoFull(inst, i);
                }
            }
        }
        TxState[instNum]->ChnState[0].count = temp;
        if (TxState[instNum]->ChnState[0].count == 0UL)
        {
            finish = true;
        }
    }
    if (finish)
    {
        TxState[instNum]->status = STATUS_SUCCESS;
        if (!TxState[instNum]->Blocking)
        {
            if (TxState[instNum]->Callback != NULL)
            {
                TxState[instNum]->Callback(0U, SAI_TRANSFER_COMPLETE, STATUS_SUCCESS);
            }
        }
    }
    return finish;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_TxFilFifo
 * Description   : Fill fifo before enable tx to avoid run error
 *
 *END**************************************************************************/
static bool SAI_DRV_TxFillFifoDma(uint32_t instNum)
{
    SAI_Type* inst = SAIBase[instNum];
    uint8_t i;
    bool finish;
    bool full;
    bool isTxFull;

    finish = false;
    if ((TxState[instNum]->mux == SAI_MUX_LINE) || (TxState[instNum]->mux == SAI_MUX_DISABLED))
    {
        for (i = 0U; i < sai_channel_count[instNum]; i++)
        {
            if (SAI_DRV_IsTxChannelEnabled(inst, i))
            {
                isTxFull = SAI_DRV_IsTxFifoFull(inst, i);
                /* all channel fifo should be in the same level */
                while (!((TxState[instNum]->ChnState[i].count == 0UL) || isTxFull))
                {
                    if (TxState[instNum]->ElementSize == 1U)
                    {
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*TxState[instNum]->ChnState[i].data);
                    }
                    else if (TxState[instNum]->ElementSize == 2U)
                    {
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) TxState[instNum]->ChnState[i].data));
                    }
                    else if (TxState[instNum]->ElementSize == 4U)
                    {
                        SAI_DRV_TxWrite(inst, i, *((uint32_t*) TxState[instNum]->ChnState[i].data));
                    }
                    else
                    {
                        DEV_ASSERT(false);
                    }
                    TxState[instNum]->ChnState[i].data += TxState[instNum]->ElementSize;
                    TxState[instNum]->ChnState[i].count--;
                    isTxFull = SAI_DRV_IsTxFifoFull(inst, i);
                }
            }
        }
        if (TxState[instNum]->ChnState[0].count == 0UL)
        {
            finish = true;
        }
    }
    else if (TxState[instNum]->mux == SAI_MUX_MEM)
    {
        full = false;
        while (!((TxState[instNum]->ChnState[0].count == 0UL) || (full)))
        {
            for (i = 0U; i < sai_channel_count[instNum]; i++)
            {
                /* check the first enabled channel fifo */
                if (SAI_DRV_IsTxFifoFull(inst, i))
                {
                    full = true;
                    break;
                }
                if (SAI_DRV_IsTxChannelEnabled(inst, i))
                {
                    if (TxState[instNum]->ElementSize == 1U)
                    {
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*TxState[instNum]->ChnState[0].data);
                    }
                    else if (TxState[instNum]->ElementSize == 2U)
                    {
                        SAI_DRV_TxWrite(inst, i, (uint32_t)*((uint16_t*) TxState[instNum]->ChnState[0U].data));
                    }
                    else if (TxState[instNum]->ElementSize == 4U)
                    {
                        SAI_DRV_TxWrite(inst, i, *((uint32_t*) TxState[instNum]->ChnState[0U].data));
                    }
                    else
                    {
                        DEV_ASSERT(false);
                    }
                    TxState[instNum]->ChnState[0U].data += TxState[instNum]->ElementSize;
                }
            }
            if (!full)
            {
                TxState[instNum]->ChnState[0U].count--;
            }
        }
        if (TxState[instNum]->ChnState[0].count == 0UL)
        {
            finish = true;
        }
    }
    else /* should not happen */
    {
        DEV_ASSERT(false);
    }
    if (finish)
    {
        TxState[instNum]->status = STATUS_SUCCESS;
        if (!TxState[instNum]->Blocking)
        {
            if (TxState[instNum]->Callback != NULL)
            {
                TxState[instNum]->Callback(0U, SAI_TRANSFER_COMPLETE, STATUS_SUCCESS);
            }
        }
    }
    return finish;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_Send
 * Description   : Send block of data, return immediately
 * Implements    : SAI_DRV_Send_Activity
 *
 *END**************************************************************************/
void SAI_DRV_Send(uint32_t instNum,
                  const uint8_t* data[],
                  uint32_t count)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);
    DEV_ASSERT(count > 0UL);
    DEV_ASSERT(TxState[instNum]->status != STATUS_BUSY);

    SAI_Type* inst = SAIBase[instNum];

    if (TxState[instNum]->XferType == SAI_DMA)
    {
        SAI_DRV_SendDma(instNum, data, count);
    }
    else if (TxState[instNum]->XferType == SAI_INTERRUPT)
    {
        SAI_DRV_SendInt(instNum, data, count);
    }
    else
    {
        DEV_ASSERT(false);
    }
    /* Unset software reset bit */
    inst->TCSR &= ~SAI_TCSR_SR_MASK;
    /* Enable tx */
    inst->TCSR |= SAI_TCSR_TE(1U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_GetSendingStatus
 * Description   : Get sending status (dma or interrupt)
 * Implements    : SAI_DRV_GetSendingStatus_Activity
 *
 *END**************************************************************************/
status_t SAI_DRV_GetSendingStatus(uint32_t instNum,
                                  uint32_t *countRemain)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    status_t ret = STATUS_ERROR;
    status_t CurStatus = TxState[instNum]->status;

    if ((CurStatus == STATUS_UNSUPPORTED) || (CurStatus == STATUS_SUCCESS))
    {
        if (countRemain != NULL)
        {
            *countRemain = 0;
        }
        ret = STATUS_SUCCESS;
    }
    else if (CurStatus == STATUS_ERROR)
    {
        if (countRemain != NULL)
        {
            *countRemain = 0;
        }
        ret = STATUS_ERROR;
    }
    else if (CurStatus == STATUS_BUSY)
    {
        if (TxState[instNum]->XferType == SAI_DMA)
        {
            if (countRemain != NULL)
            {
                *countRemain = EDMA_DRV_GetRemainingMajorIterationsCount(TxState[instNum]->DmaChannel[0]);
            }
        }
        else
        {
            if (countRemain != NULL)
            {
                *countRemain = TxState[instNum]->ChnState[0].count;
            }
        }
        ret = STATUS_BUSY;
    }
    else if (CurStatus == STATUS_SAI_ABORTED)
    {
        if (countRemain != NULL)
        {
            *countRemain = TxState[instNum]->ChnState[0].count;
        }
        ret = STATUS_SAI_ABORTED;
    }
    else
    {
        /* undefined value */
        DEV_ASSERT(false);
    }
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_AbortSending
 * Description   : Abort ongoing sending (dma or interrupt)
 * Implements    : SAI_DRV_AbortSending_Activity
 *
 *END**************************************************************************/
void SAI_DRV_AbortSending(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    SAI_Type* inst = SAIBase[instNum];
    uint8_t c = 0;
    uint8_t i;

    if (TxState[instNum]->status == STATUS_BUSY)
    {
        TxState[instNum]->status = STATUS_SAI_ABORTED;
        if (TxState[instNum]->XferType == SAI_DMA)
        {
            TxState[instNum]->ChnState[0].count = EDMA_DRV_GetRemainingMajorIterationsCount(TxState[instNum]->DmaChannel[0]);
            /* Disable tx DMA requests for the current instance */
            SAI_DRV_TxDisableFifoReqDma(inst);
            /* stop all dma channel*/
            if ((TxState[instNum]->mux == SAI_MUX_DISABLED) || (TxState[instNum]->mux == SAI_MUX_LINE))
            {
                for (i = 0; i < sai_channel_count[instNum]; i++)
                {
                    if (SAI_DRV_IsTxChannelEnabled(inst, i))
                    {
                        (void)EDMA_DRV_StopChannel(TxState[instNum]->DmaChannel[c]);
                        c++;
                    }
                }
            }
            else
            {
                (void)EDMA_DRV_StopChannel(TxState[instNum]->DmaChannel[0]);
            }
        }
        else
        {
            SAI_DRV_TxDisableFifoReqInt(inst);
        }
        if (TxState[instNum]->Blocking)
        {
            (void)OSIF_SemaPost(&TxState[instNum]->Sema);
        }
    }
}

/*****************************RECEIVING FUNCTIONS*********************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_ReceiveBlocking
 * Description   : Receive block of data, return when finish Receiving
 * Implements    : SAI_DRV_ReceiveBlocking_Activity
 *
 *END**************************************************************************/
status_t SAI_DRV_ReceiveBlocking(uint32_t instNum,
                                 uint8_t* data[],
                                 uint32_t count,
                                 uint32_t timeout)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);
    DEV_ASSERT(RxState[instNum]->status != STATUS_BUSY);

    status_t res;
    status_t osifError;

    RxState[instNum]->Blocking = true;
    SAI_DRV_Receive (instNum, data, count);
    osifError = OSIF_SemaWait(&RxState[instNum]->Sema, timeout);
    RxState[instNum]->Blocking = false;
    if (osifError == STATUS_TIMEOUT)
    {
        /* abort current transfer */
        SAI_DRV_AbortReceiving (instNum);
        res = STATUS_TIMEOUT;
    }
    else
    {
        /* not time out */
        res = RxState[instNum]->status;
    }

    return res;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_Send
 * Description   : Receive block of data, return immediately
 * Implements    : SAI_DRV_Receive_Activity
 *
 *END**************************************************************************/
void SAI_DRV_Receive(uint32_t instNum,
                     uint8_t* data[],
                     uint32_t count)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);
    DEV_ASSERT(count > 0UL);
    DEV_ASSERT(RxState[instNum]->status != STATUS_BUSY);

    SAI_Type* inst = SAIBase[instNum];

    if (RxState[instNum]->XferType == SAI_DMA)
    {
        SAI_DRV_ReceiveDma(instNum, data, count);
    }
    else if (RxState[instNum]->XferType == SAI_INTERRUPT)
    {
        SAI_DRV_ReceiveInt(instNum, data, count);
    }
    else
    {
        DEV_ASSERT(false);
    }
    inst->RCSR &= ~SAI_RCSR_SR_MASK;
    inst->RCSR |= SAI_RCSR_RE(1U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_GetReceivingStatus
 * Description   : Get receiving status (dma or interrupt)
 * Implements    : SAI_DRV_GetReceivingStatus_Activity
 *
 *END**************************************************************************/
status_t SAI_DRV_GetReceivingStatus(uint32_t instNum,
                                    uint32_t *countRemain)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    status_t ret = STATUS_ERROR;
    /* temp variable to avoid status change while checking */
    status_t CurStatus = RxState[instNum]->status;

    if ((CurStatus == STATUS_UNSUPPORTED) || (CurStatus == STATUS_SUCCESS))
    {
        if (countRemain != NULL)
        {
            *countRemain = 0;
        }
        ret = STATUS_SUCCESS;
    }
    else if (CurStatus == STATUS_ERROR)
    {
        if (countRemain != NULL)
        {
            *countRemain = 0;
        }
        ret = STATUS_ERROR;
    }
    else if (CurStatus == STATUS_BUSY)
    {
        if (RxState[instNum]->XferType == SAI_DMA)
        {
            if (countRemain != NULL)
            {
                *countRemain = EDMA_DRV_GetRemainingMajorIterationsCount(RxState[instNum]->DmaChannel[0]);
            }
        }
        else
        {
            if (countRemain != NULL)
            {
                *countRemain = RxState[instNum]->ChnState[0].count;
            }
        }
        ret = STATUS_BUSY;
    }
    else if (CurStatus == STATUS_SAI_ABORTED)
    {
        if (countRemain != NULL)
        {
            *countRemain = RxState[instNum]->ChnState[0].count;
        }
        ret = STATUS_SAI_ABORTED;
    }
    else
    {
        /* undefined value */
        DEV_ASSERT(false);
    }
    return ret;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_AbortReceiving
 * Description   : Abort ongoing receiving (dma or interrupt)
 * Implements    : SAI_DRV_AbortReceiving_Activity
 *
 *END**************************************************************************/
void SAI_DRV_AbortReceiving(uint32_t instNum)
{
    DEV_ASSERT(instNum < SAI_INSTANCE_COUNT);

    SAI_Type* inst = SAIBase[instNum];
    uint8_t c = 0;
    uint8_t i;

    if (RxState[instNum]->status == STATUS_BUSY)
    {
        RxState[instNum]->status = STATUS_SAI_ABORTED;
        if (RxState[instNum]->XferType == SAI_DMA)
        {
            RxState[instNum]->ChnState[0].count = EDMA_DRV_GetRemainingMajorIterationsCount(RxState[instNum]->DmaChannel[0]);
            /* Disable Rx DMA requests for the current instance */
            SAI_DRV_RxDisableFifoReqDma(inst);
            /* stop all dma channel*/
            if ((RxState[instNum]->mux == SAI_MUX_DISABLED) || (RxState[instNum]->mux == SAI_MUX_LINE))
            {
                for (i = 0; i < sai_channel_count[instNum]; i++)
                {
                    if (SAI_DRV_IsRxChannelEnabled(inst, i))
                    {
                        (void)EDMA_DRV_StopChannel(RxState[instNum]->DmaChannel[c]);
                        c++;
                    }
                }
            }
            else
            {
                (void)EDMA_DRV_StopChannel(RxState[instNum]->DmaChannel[0]);
            }
        }
        else
        {
            SAI_DRV_RxDisableFifoReqInt(inst);
        }
        if (RxState[instNum]->Blocking)
        {
            (void)OSIF_SemaPost(&RxState[instNum]->Sema);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SAI_DRV_GetDefaultConfig
 * Description   : Init config structure for I2S interface, interrupt,
 *                 internal generated bit clock 1.4112 MHz, 16 bit word,
 *                 2 channel 1 data line (data line 0), msb first
 * Implements    : SAI_DRV_GetDefaultConfig_Activity
 *
 *END**************************************************************************/
void SAI_DRV_GetDefaultConfig(sai_user_config_t* uc)
{
    uc->BitClkInternal = false;
    uc->BitClkNegPolar = false;
    uc->BitClkFreq = 1411200U;
    uc->ChannelCount = 2U;              /* 2 buffer, one for each channel */
    uc->ChannelEnable = SAI_CHANNEL_0;  /* select data line 0 */
    uc->ElementSize = 2U;               /* 2 byte to transfer */
    uc->FirstBitIndex = 15U;            /* first bit to transfer is 15 because of msb first */
    uc->FrameSize = 2U;                 /* 2 word per frame */
    uc->FrameStartReport = false;
#ifdef FEATURE_SAI_HAS_CHMOD
    uc->MaskMode = SAI_MASK_TRISTATE;
#endif
    uc->MsbFirst = true;
    uc->MuxMode = SAI_MUX_LINE;         /* 2 data buffer is muxed in to one data line */
    uc->RunErrorReport = false;
    uc->SyncEarly = false;
    uc->SyncErrorReport = false;
    uc->SyncMode = SAI_ASYNC;
    uc->SyncNegPolar = false;
    uc->SyncInternal = false;
    uc->SyncWidth = 16U;                /* sync width is first word */
    uc->TransferType = SAI_INTERRUPT;
    uc->Word0Width = 16U;
    uc->WordNWidth = 16U;
    uc->callback = NULL;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
