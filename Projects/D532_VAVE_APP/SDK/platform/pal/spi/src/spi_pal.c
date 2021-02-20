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

#include "spi_pal.h"
#include "device_registers.h"

/* Include PD files */
#if (defined (SPI_OVER_LPSPI))
    #include "lpspi_master_driver.h"
    #include "lpspi_slave_driver.h"
#endif

#if (defined (SPI_OVER_FLEXIO))
    #include "flexio.h"
    #include "flexio_spi_driver.h"
#endif

#if (defined (SPI_OVER_DSPI))
    #include "dspi_driver.h"
#endif


/* Define state structures for LPSPI */
#if (defined(SPI_OVER_LPSPI))
    /*! @brief LPSPI state structures */
    lpspi_state_t LpspiState[NO_OF_LPSPI_INSTS_FOR_SPI];
    /*! @brief LPSPI state-instance matching */
    static spi_instance_t LpspiStateInstanceMapping[NO_OF_LPSPI_INSTS_FOR_SPI];
    /*! @brief LPSPI  available resources table */
    static bool LpspiStateIsAllocated[NO_OF_LPSPI_INSTS_FOR_SPI];
#endif

/* Define state structures for SPI over FLEXIO */
#if (defined(SPI_OVER_FLEXIO))
    /*! @brief FLEXIO state structures */
    flexio_spi_master_state_t FlexioSpiState[NO_OF_FLEXIO_INSTS_FOR_SPI];
    flexio_device_state_t FlexioState;
    /*! @brief FLEXIO state-instance matching */
    static spi_instance_t FlexioSpiStateInstanceMapping[NO_OF_FLEXIO_INSTS_FOR_SPI];
    /*! @brief FLEXIO  available resources table */
    static bool FlexioSpiStateIsAllocated[NO_OF_FLEXIO_INSTS_FOR_SPI];
#endif

/* Define state structures for DSPI */
#if (defined(SPI_OVER_DSPI))
    /*! @brief DSPI state structures */
    dspi_state_t dspiState[NO_OF_DSPI_INSTS_FOR_SPI];
    /*! @brief DSPI state-instance matching */
    static spi_instance_t DspiStateInstanceMapping[NO_OF_DSPI_INSTS_FOR_SPI];
    /*! @brief DSPI  available resources table */
    static bool DspiStateIsAllocated[NO_OF_DSPI_INSTS_FOR_SPI];
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : SpiAllocateState
 * Description   : Allocates one of the available state structure.
 *
 *END**************************************************************************/
static uint8_t SpiAllocateState(bool* isAllocated, spi_instance_t* instanceMapping, spi_instance_t instance, uint8_t numberOfinstances)
{
    uint8_t i;
    /* Allocate one of the LPSPI state structure for this instance */
    for (i = 0;i < numberOfinstances;i++)
    {
        if (isAllocated[i] == false)
        {
            instanceMapping[i] = instance;
            isAllocated[i] = true;
            break;
        }
    }
    return i;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SpiFreeState
 * Description   : Deallocates one of the available state structure.
 *
 *END**************************************************************************/
static void SpiFreeState(bool* isAllocated, spi_instance_t* instanceMapping, spi_instance_t instance, uint8_t numberOfinstances)
{
    uint8_t i;
    /* Allocate one of the LPSPI state structure for this instance */
    for (i = 0;i < numberOfinstances;i++)
    {
        if (instanceMapping[i] == instance)
        {
            isAllocated[i] = false;
            break;
        }
    }
}

#if (defined(SPI_OVER_FLEXIO))
/*FUNCTION**********************************************************************
 *
 * Function Name : FindFlexioState
 * Description   : Search the state structure of the flexio instance
 *
 *END**************************************************************************/
static uint8_t FindFlexioState(spi_instance_t instance)
{
    uint8_t i;
    for (i = 0;i<NO_OF_FLEXIO_INSTS_FOR_SPI;i++)
    {
        if (FlexioSpiStateInstanceMapping[i] == instance)
        {
            break;
        }
    }
    return i;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_MasterInit
 * Description   : Configures the SPI in master mode
 * Implements    : SPI_MasterInit_Activity
 *
 *END**************************************************************************/
status_t SPI_MasterInit(spi_instance_t instance, spi_master_t *config)
{
    status_t status = STATUS_ERROR;
    uint8_t index = 0;

    /* Define SPI PAL over LPSPI */
    #if (defined (SPI_OVER_LPSPI))
    if (instance <= LPSPI_HIGH_INDEX)
    {
        lpspi_master_config_t lpspiConfig;
        lpspiConfig.bitsPerSec = config->baudRate;
        lpspiConfig.whichPcs = (lpspi_which_pcs_t)config->ssPin;
        lpspiConfig.pcsPolarity = (lpspi_signal_polarity_t)config->ssPolarity;
        lpspiConfig.bitcount = config->frameSize;
        CLOCK_SYS_GetFreq((clock_names_t)((uint32_t)PCC_LPSPI0_CLOCK + instance) ,&lpspiConfig.lpspiSrcClk);
        lpspiConfig.clkPhase = (lpspi_clock_phase_t)config->clockPhase;
        lpspiConfig.clkPolarity = (lpspi_sck_polarity_t)config->clockPolarity;
        lpspiConfig.lsbFirst = config->bitOrder;
        lpspiConfig.transferType = (lpspi_transfer_type)config->transferType;
        lpspiConfig.rxDMAChannel = config->rxDMAChannel;
        lpspiConfig.txDMAChannel = config->txDMAChannel;
        lpspiConfig.callback = config->callback;
        lpspiConfig.callbackParam = config->callbackParam;
        lpspiConfig.isPcsContinuous = false;
        /* Allocate one of the LPSPI state structure for this instance */
        index = SpiAllocateState(LpspiStateIsAllocated, LpspiStateInstanceMapping, instance, NO_OF_LPSPI_INSTS_FOR_SPI);
        status = LPSPI_DRV_MasterInit(instance, (lpspi_state_t*)(&LpspiState[index]), &lpspiConfig);
    }
    #endif

    /* Define SPI PAL over FLEXIO */
    #if (defined (SPI_OVER_FLEXIO))
    if ((instance >= FLEXIO_SPI_LOW_INDEX) && (instance <= FLEXIO_SPI_HIGH_INDEX))
    {
        /* FlexIO driver can be used only for the following frame sizes: 8,16 or 32 */
        DEV_ASSERT((config->frameSize==8) || (config->frameSize==16) || (config->frameSize==32));
        flexio_spi_master_user_config_t flexioConfig;
        flexioConfig.baudRate = config->baudRate;
        switch(config->transferType)
        {
            case SPI_USING_DMA:  flexioConfig.driverType = FLEXIO_DRIVER_TYPE_DMA; break;
            case SPI_USING_INTERRUPTS : flexioConfig.driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS; break;
        }
        flexioConfig.bitOrder = (flexio_spi_transfer_bit_order_t)config->bitOrder;
        flexioConfig.transferSize = (flexio_spi_transfer_size_t)(config->frameSize>>3U);
        flexioConfig.clockPolarity = (uint8_t)config->clockPolarity;
        flexioConfig.clockPhase = (uint8_t)config->clockPhase;
        flexioConfig.mosiPin = ((extension_flexio_for_spi_t*)(config->extension))->mosiPin;
        flexioConfig.misoPin = ((extension_flexio_for_spi_t*)(config->extension))->misoPin;
        flexioConfig.sckPin = ((extension_flexio_for_spi_t*)(config->extension))->sckPin;
        flexioConfig.ssPin = ((extension_flexio_for_spi_t*)(config->extension))->ssPin;
        /* Link Flexio Callbacks to the callbacks defined in PAL */
        flexioConfig.callback = config->callback;
        /* Store the callbacks defined by the user in PAL internal callbacks vector */
        flexioConfig.callbackParam = config->callbackParam;
        flexioConfig.rxDMAChannel = config->rxDMAChannel;
        flexioConfig.txDMAChannel = config->txDMAChannel;
        status = FLEXIO_DRV_InitDevice(0U, &FlexioState);
        if (status == STATUS_SUCCESS)
        {
            /* Allocate one of the Flexio state structure for this instance */
            index = SpiAllocateState(FlexioSpiStateIsAllocated, FlexioSpiStateInstanceMapping, instance, NO_OF_FLEXIO_INSTS_FOR_SPI);
            status = FLEXIO_SPI_DRV_MasterInit(0U, &flexioConfig, (flexio_spi_master_state_t*)&FlexioSpiState[index]);
        }
    }
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined (SPI_OVER_DSPI))
        dspi_master_config_t dspiConfig;
        dspiConfig.bitsPerSec = config->baudRate;
        dspiConfig.bitcount = config->frameSize;
        dspiConfig.clkPhase = (dspi_clock_phase_t)config->clockPhase;
        dspiConfig.clkPolarity = (dspi_polarity_t)config->clockPolarity;
        dspiConfig.isClkContinuous = false;
        dspiConfig.lsbFirst = config->bitOrder;
        dspiConfig.transferType = (dspi_transfer_type_t)config->transferType;
        dspiConfig.rxDMAChannel = config->rxDMAChannel;
        dspiConfig.txDMAChannel = config->txDMAChannel;
        dspiConfig.whichPCS = config->ssPin;
        dspiConfig.pcsPolarity = (dspi_polarity_t)config->ssPolarity;
        dspiConfig.callback = config->callback;
        dspiConfig.callbackParam = config->callbackParam;
        /* Allocate one of the DSPI state structure for this instance */
        index = SpiAllocateState(DspiStateIsAllocated, DspiStateInstanceMapping, instance, NO_OF_DSPI_INSTS_FOR_SPI);
        status = DSPI_MasterInit((dspi_instance_t)instance, (dspi_state_t*)&dspiState[index] ,&dspiConfig);
    #endif
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_MasterTransfer
 * Description   : Initializes a non-blocking master transfer
 * Implements    : SPI_MasterTransfer_Activity
 *
 *END**************************************************************************/
status_t SPI_MasterTransfer(spi_instance_t instance, void* txBuffer, void* rxBuffer, uint16_t numberOfFrames)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance <= LPSPI_HIGH_INDEX)
    {
        status = LPSPI_DRV_MasterTransfer((uint32_t)instance, txBuffer, rxBuffer, numberOfFrames);
    }
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if ((instance >= FLEXIO_SPI_LOW_INDEX) && (instance <= FLEXIO_SPI_HIGH_INDEX))
    {
        status = FLEXIO_SPI_DRV_MasterTransfer(&(FlexioSpiState[FindFlexioState(instance)]), txBuffer, rxBuffer, numberOfFrames);
    }
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    status = DSPI_MasterTransfer((dspi_instance_t)instance,  txBuffer, rxBuffer, numberOfFrames);
    #endif
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_MasterTransferBlocking
 * Description   : Initializes a blocking master transfer with time-out
 * Implements    : SPI_MasterTransferBlocking_Activity
 *
 *END**************************************************************************/
status_t SPI_MasterTransferBlocking(spi_instance_t instance, void* txBuffer, void* rxBuffer, uint16_t numberOfFrames, uint16_t timeout)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance <= LPSPI_HIGH_INDEX)
    {
        status = LPSPI_DRV_MasterTransferBlocking((uint32_t)instance, txBuffer, rxBuffer, numberOfFrames, timeout);
    }
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if ((instance >= FLEXIO_SPI_LOW_INDEX) && (instance <= FLEXIO_SPI_HIGH_INDEX))
    {
        status = FLEXIO_SPI_DRV_MasterTransferBlocking(&(FlexioSpiState[FindFlexioState(instance)]), txBuffer, rxBuffer, numberOfFrames, timeout);
    }
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    status = DSPI_MasterTransferBlocking((dspi_instance_t)instance,  txBuffer, rxBuffer, numberOfFrames, timeout);
    #endif
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_SlaveInit
 * Description   : Configures the SPI in slave mode
 * Implements    : SPI_SlaveInit_Activity
 *
 *END**************************************************************************/
status_t SPI_SlaveInit(spi_instance_t instance, spi_slave_t *config)
{
    status_t status = STATUS_ERROR;
    uint8_t index = 0;

    /* Define SPI PAL over LPSPI */
    #if (defined (SPI_OVER_LPSPI))
    if (instance <= LPSPI_HIGH_INDEX)
    {
        lpspi_slave_config_t lpspiConfig;
        lpspiConfig.pcsPolarity = (lpspi_signal_polarity_t)config->ssPolarity;
        lpspiConfig.bitcount = config->frameSize;
        lpspiConfig.clkPhase = (lpspi_clock_phase_t)config->clockPhase;
        lpspiConfig.clkPolarity = (lpspi_sck_polarity_t)config->clockPolarity;
        lpspiConfig.lsbFirst = config->bitOrder;
        lpspiConfig.transferType = (lpspi_transfer_type)config->transferType;
        lpspiConfig.rxDMAChannel = config->rxDMAChannel;
        lpspiConfig.txDMAChannel = config->txDMAChannel;
        lpspiConfig.callback = config->callback;
        lpspiConfig.callbackParam = config->callbackParam;
        lpspiConfig.whichPcs = (lpspi_which_pcs_t)0U;
        /* Allocate one of the LPSPI state structure for this instance */
        index = SpiAllocateState(LpspiStateIsAllocated, LpspiStateInstanceMapping, instance, NO_OF_LPSPI_INSTS_FOR_SPI);
        status = LPSPI_DRV_SlaveInit(instance, (lpspi_state_t*)(&LpspiState[index]), &lpspiConfig);
    }
    #endif

    /* Define SPI PAL over FLEXIO */
    #if (defined (SPI_OVER_FLEXIO))
    if ((instance >= FLEXIO_SPI_LOW_INDEX) && (instance <= FLEXIO_SPI_HIGH_INDEX))
    {
        /* FlexIO driver can be used only for the following frame sizes: 8,16 or 32 */
        DEV_ASSERT((config->frameSize==8) || (config->frameSize==16) || (config->frameSize==32));
        flexio_spi_slave_user_config_t flexioConfig;
        switch(config->transferType)
        {
            case SPI_USING_DMA:  flexioConfig.driverType = FLEXIO_DRIVER_TYPE_DMA; break;
            case SPI_USING_INTERRUPTS : flexioConfig.driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS; break;
        }
        flexioConfig.bitOrder = (flexio_spi_transfer_bit_order_t)config->bitOrder;
        flexioConfig.transferSize = (flexio_spi_transfer_size_t)(config->frameSize>>3U);
        flexioConfig.clockPolarity = (uint8_t)config->clockPolarity;
        flexioConfig.clockPhase = (uint8_t)config->clockPhase;
        flexioConfig.mosiPin = ((extension_flexio_for_spi_t*)(config->extension))->mosiPin;
        flexioConfig.misoPin = ((extension_flexio_for_spi_t*)(config->extension))->misoPin;
        flexioConfig.sckPin = ((extension_flexio_for_spi_t*)(config->extension))->sckPin;
        flexioConfig.ssPin = ((extension_flexio_for_spi_t*)(config->extension))->ssPin;
        /* Link Flexio Callbacks to the callbacks defined in PAL */
        flexioConfig.callback = config->callback;
        /* Store the callbacks defined by the user in PAL internal callbacks vector */
        flexioConfig.callbackParam = config->callbackParam;
        flexioConfig.rxDMAChannel = config->rxDMAChannel;
        flexioConfig.txDMAChannel = config->txDMAChannel;
        status = FLEXIO_DRV_InitDevice(0U, &FlexioState);
        if (status == STATUS_SUCCESS)
        {
            /* Allocate one of the Flexio state structure for this instance */
            index = SpiAllocateState(FlexioSpiStateIsAllocated, FlexioSpiStateInstanceMapping, instance, NO_OF_FLEXIO_INSTS_FOR_SPI);
            status = FLEXIO_SPI_DRV_SlaveInit(0U, &flexioConfig, (flexio_spi_master_state_t*)&FlexioSpiState[index]);
        }
    }
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined (SPI_OVER_DSPI))
        /* DSPI in slave mode works only in active low mode */
        DEV_ASSERT(config->ssPolarity == SPI_ACTIVE_LOW);

        dspi_slave_config_t dspiConfig;
        dspiConfig.bitcount = config->frameSize;
        dspiConfig.clkPhase = (dspi_clock_phase_t)config->clockPhase;
        dspiConfig.clkPolarity = (dspi_polarity_t)config->clockPolarity;
        dspiConfig.transferType = (dspi_transfer_type_t)config->transferType;
        dspiConfig.rxDMAChannel = config->rxDMAChannel;
        dspiConfig.txDMAChannel = config->txDMAChannel;
        dspiConfig.callback = config->callback;
        dspiConfig.callbackParam = config->callbackParam;
        /* Allocate one of the DSPI state structure for this instance */
        index = SpiAllocateState(DspiStateIsAllocated, DspiStateInstanceMapping, instance, NO_OF_DSPI_INSTS_FOR_SPI);
        status = DSPI_SlaveInit((dspi_instance_t)instance, (dspi_state_t*)&dspiState[index] ,&dspiConfig);
    #endif
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_SlaveTransfer
 * Description   : Initializes a non-blocking slave transfer
 * Implements    : SPI_SlaveTransfer_Activity
 *
 *END**************************************************************************/
status_t SPI_SlaveTransfer(spi_instance_t instance, void* txBuffer, void* rxBuffer, uint16_t numberOfFrames)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance <= LPSPI_HIGH_INDEX)
    {
        status = LPSPI_DRV_SlaveTransfer((uint32_t)instance, txBuffer, rxBuffer, numberOfFrames);
    }
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if ((instance >= FLEXIO_SPI_LOW_INDEX) && (instance <= FLEXIO_SPI_HIGH_INDEX))
    {
        status = FLEXIO_SPI_DRV_SlaveTransfer(&(FlexioSpiState[FindFlexioState(instance)]), txBuffer, rxBuffer, numberOfFrames);
    }
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    status = DSPI_SlaveTransfer((dspi_instance_t)instance,  txBuffer, rxBuffer, numberOfFrames);
    #endif
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_SlaveTransferBlocking
 * Description   : Initializes a blocking slave transfer with timeout
 * Implements    : SPI_SlaveTransferBlocking_Activity
 *
 *END**************************************************************************/
status_t SPI_SlaveTransferBlocking(spi_instance_t instance, void* txBuffer, void* rxBuffer, uint16_t numberOfFrames, uint16_t timeout)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance <= LPSPI_HIGH_INDEX)
    {
        status = LPSPI_DRV_SlaveTransferBlocking((uint32_t)instance, txBuffer, rxBuffer, numberOfFrames, timeout);
    }
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if ((instance >= FLEXIO_SPI_LOW_INDEX) && (instance <= FLEXIO_SPI_HIGH_INDEX))
    {
        status = FLEXIO_SPI_DRV_SlaveTransferBlocking(&(FlexioSpiState[FindFlexioState(instance)]), txBuffer, rxBuffer, numberOfFrames, timeout);
    }
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    status = DSPI_SlaveTransferBlocking((dspi_instance_t)instance,  txBuffer, rxBuffer, numberOfFrames, timeout);
    #endif
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_GetDefaultMasterConfig
 * Description   : Gets the default configuration structure for master
 * Implements    : SPI_GetDefaultMasterConfig_Activity
 *
 *END**************************************************************************/
status_t SPI_GetDefaultMasterConfig(spi_master_t *config)
{
    config->baudRate      = 100000U;
    config->ssPolarity    = SPI_ACTIVE_HIGH;
    config->frameSize     = 8U;
    config->clockPhase    = READ_ON_ODD_EDGE;
    config->clockPolarity = SPI_ACTIVE_HIGH;
    config->bitOrder      = SPI_TRANSFER_MSB_FIRST;
    config->transferType  = SPI_USING_INTERRUPTS;
    config->rxDMAChannel  = 255U;
    config->txDMAChannel  = 255U;
    config->callback      = NULL;
    config->callbackParam = NULL;
    config->ssPin         = 0;
    config->extension     = NULL;
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_GetDefaultSlaveConfig
 * Description   : Gets the default configuration structure for slave
 * Implements    : SPI_GetDefaultSlaveConfig_Activity
 *
 *END**************************************************************************/
status_t SPI_GetDefaultSlaveConfig(spi_slave_t *config)
{
    config->ssPolarity    = SPI_ACTIVE_HIGH;
    config->frameSize     = 8U;
    config->clockPhase    = READ_ON_ODD_EDGE;
    config->clockPolarity = SPI_ACTIVE_HIGH;
    config->bitOrder      = SPI_TRANSFER_MSB_FIRST;
    config->transferType  = SPI_USING_INTERRUPTS;
    config->rxDMAChannel  = 255U;
    config->txDMAChannel  = 255U;
    config->callback      = NULL;
    config->callbackParam = NULL;
    config->extension     = NULL;
    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_MasterDeinit
 * Description   : De-initializes the spi master module
 * Implements    : SPI_MasterDeinit_Activity
 *
 *END**************************************************************************/
status_t SPI_MasterDeinit(spi_instance_t instance)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance <= LPSPI_HIGH_INDEX)
    {
        status = LPSPI_DRV_MasterDeinit((uint32_t)instance);
        if (status == STATUS_SUCCESS)
        {
            SpiFreeState(LpspiStateIsAllocated, LpspiStateInstanceMapping, instance, NO_OF_LPSPI_INSTS_FOR_SPI);
        }
    }
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if ((instance >= FLEXIO_SPI_LOW_INDEX) && (instance <= FLEXIO_SPI_HIGH_INDEX))
    {
        status = FLEXIO_SPI_DRV_MasterDeinit(&(FlexioSpiState[FindFlexioState(instance)]));
        if (status == STATUS_SUCCESS)
        {
            SpiFreeState(FlexioSpiStateIsAllocated, FlexioSpiStateInstanceMapping, instance, NO_OF_FLEXIO_INSTS_FOR_SPI);
        }
    }
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    status = DSPI_Deinit((dspi_instance_t)instance);
    if (status == STATUS_SUCCESS)
    {
        SpiFreeState(DspiStateIsAllocated, DspiStateInstanceMapping, instance, NO_OF_DSPI_INSTS_FOR_SPI);
    }
    #endif
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_SlaveDeinit
 * Description   : De-initializes the spi slave module
 * Implements    : SPI_SlaveDeinit_Activity
 *
 *END**************************************************************************/
status_t SPI_SlaveDeinit(spi_instance_t instance)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance <= LPSPI_HIGH_INDEX)
    {
		LpspiStateIsAllocated[instance] = false;
        status = LPSPI_DRV_SlaveDeinit((uint32_t)instance);
    }
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if ((instance >= FLEXIO_SPI_LOW_INDEX) && (instance <= FLEXIO_SPI_HIGH_INDEX))
    {
        status = FLEXIO_SPI_DRV_SlaveDeinit(&(FlexioSpiState[FindFlexioState(instance)]));
    }
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    status = DSPI_Deinit((dspi_instance_t)instance);
    #endif
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_SetSS
 * Description   : Changes the SS, if this feature is available.
 * Implements    : SPI_SetSS_Activity
 *
 *END**************************************************************************/
status_t SPI_SetSS(spi_instance_t instance, uint8_t ssPin)
{
    status_t status = STATUS_ERROR;
    (void)ssPin;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance <= LPSPI_HIGH_INDEX)
    {
        status = STATUS_UNSUPPORTED;
    }
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if ((instance >= FLEXIO_SPI_LOW_INDEX) && (instance <= FLEXIO_SPI_HIGH_INDEX))
    {
        status = STATUS_UNSUPPORTED;
    }
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    status = DSPI_UpdateCS((dspi_instance_t)instance, ssPin);
    #endif
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_GetStatus
 * Description   : Gets the status of the last transfer
 *
 *END**************************************************************************/
status_t SPI_GetStatus(spi_instance_t instance)
{
    status_t status = STATUS_ERROR;

    /* Define SPI PAL over LPSPI */
    #if defined(SPI_OVER_LPSPI)
    if (instance <= LPSPI_HIGH_INDEX)
    {
        status = STATUS_UNSUPPORTED;
    }
    #endif

    /* Define SPI PAL over FLEXIO */
    #if defined(SPI_OVER_FLEXIO)
    if ((instance >= FLEXIO_SPI_LOW_INDEX) && (instance <= FLEXIO_SPI_HIGH_INDEX))
    {
        status = STATUS_UNSUPPORTED;
    }
    #endif

    /* Define SPI PAL over DSPI */
    #if (defined(SPI_OVER_DSPI))
    status = STATUS_UNSUPPORTED;
    #endif
    return status;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
