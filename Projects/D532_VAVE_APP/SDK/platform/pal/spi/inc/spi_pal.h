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

#ifndef SPI_PAL_H
#define SPI_PAL_H

#include "spi_pal_cfg.h"
#include "spi_pal_mapping.h"
#include "status.h"
#include "callbacks.h"

/*! @file SPI_PAL.h */

/*!
 * @defgroup spi_pal SPI PAL
 * @ingroup spi_pal
 * @addtogroup spi_pal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Defines the mechanism to update the rx or tx buffers
 * Implements : spi_transfer_type_t_Class
 */
typedef enum
{
    SPI_USING_DMA         = 0U,    /*!< The driver will use DMA to perform SPI transfer */
    SPI_USING_INTERRUPTS  = 1U,    /*!< The driver will use interrupts to perform SPI transfer */
} spi_transfer_type_t;

/*!
 * @brief Defines the polarity of signals
 * Implements : spi_polarity_t_Class
 */
typedef enum
{
    SPI_ACTIVE_HIGH     = 0U,    /*!< The signal is active high*/
    SPI_ACTIVE_LOW      = 1U,    /*!< The signal is active low */
} spi_polarity_t;

/*!
 * @brief Defines the edges used for sampling and shifting
 * Implements : spi_clock_phase_t_Class
 */
typedef enum
{
    READ_ON_ODD_EDGE     = 0U,    /*!< The SPI signal is read on odd edges of SCK and counting starts after SS activation */
    READ_ON_EVEN_EDGE    = 1U,    /*!< The SPI signal is read on even edges of SCK and counting starts after SS activation */
} spi_clock_phase_t;

/*!
 * @brief Defines the bit order
 * Implements : spi_transfer_bit_order_t_Class
 */
typedef enum
{
    SPI_TRANSFER_MSB_FIRST        = 0U,  /*!< Transmit data starting with most significant bit */
    SPI_TRANSFER_LSB_FIRST        = 1U,  /*!< Transmit data starting with least significant bit */
} spi_transfer_bit_order_t;

/*!
 * @brief Defines the configuration structure for SPI master
 * Implements : spi_master_t_Class
 *
 */
typedef struct
{
    uint32_t baudRate;                  /*!< Clock frequency */
    uint8_t frameSize;                  /*!< Size of frame in bits */
    spi_transfer_bit_order_t bitOrder;  /*!< Select if first bit is MSB or LSB */
    spi_polarity_t clockPolarity;       /*!< Select polarity for Clock */
    spi_polarity_t ssPolarity;          /*!< Select polarity for SS */
    spi_clock_phase_t clockPhase;       /*!< Select clock edges for sampling and shifting*/
    uint8_t ssPin;                      /*!< Select which SS is used */
    spi_transfer_type_t transferType;   /*!< Select if buffers are managed by internal interrupt handler or by DMA */
    uint8_t rxDMAChannel;               /*!< Channel number for DMA rx channel */
    uint8_t txDMAChannel;               /*!< Channel number for DMA tx channel */
    spi_callback_t callback;            /*!< Select the callback to transfer complete */
    void *callbackParam;                /*!< Select additional callback parameters if it's necessary */
    void *extension;                    /*!< This field will be used to add extra settings to the basic configuration like FlexIO pins */
} spi_master_t;

/*!
 * @brief Defines the configuration structure for SPI slave
 * Implements: spi_slave_t_Class
 */
typedef struct
{
    uint8_t frameSize;                  /*!< Size of frame in bits */
    spi_transfer_bit_order_t bitOrder;  /*!< Select if first bit is MSB or LSB */
    spi_polarity_t clockPolarity;       /*!< Select polarity for Clock */
    spi_polarity_t ssPolarity;          /*!< Select polarity for SS */
    spi_clock_phase_t clockPhase;       /*!< Select clock edges for sampling and shifting*/
    spi_transfer_type_t transferType;   /*!< Select if buffers are managed by internal interrupt handler or by DMA */
    uint8_t rxDMAChannel;               /*!< Channel number for DMA rx channel */
    uint8_t txDMAChannel;               /*!< Channel number for DMA tx channel */
    spi_callback_t callback;            /*!< Select the callback to transfer complete */
    void *callbackParam;                /*!< Select additional callback parameters if it's necessary */
    void *extension;                    /*!< This field will be used to add extra settings to the basic configuration like FlexIO*/
} spi_slave_t;

/*!
 * @brief Defines the extension structure for the SPI over FLEXIO
 *Implements : extension_flexio_for_spi_t_Class
 */
typedef struct
{
    uint8_t mosiPin;    /*!< FlexIO pin for MOSI */
    uint8_t misoPin;    /*!< FlexIO pin for MISO */
    uint8_t sckPin;     /*!< FlexIO pin for SCK */
    uint8_t ssPin;      /*!< FlexIO pin for SS */
} extension_flexio_for_spi_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the SPI module in master mode
 *
 * This function initializes and enables the requested SPI module in master mode,
 * configuring the bus parameters.
 *
 * @param[in] instance The name of the instance
 * @param[in] config The configuration structure
 * @return    Error or success status returned by API
 */
status_t SPI_MasterInit(spi_instance_t instance, spi_master_t *config);

/*!
 * @brief Initializes the SPI module in slave mode
 *
 * This function initializes and enables the requested SPI module in slave mode,
 * configuring the bus parameters.
 *
 * @param[in] instance The name of the instance
 * @param[in] config The configuration structure
 * @return    Error or success status returned by API
 */
status_t SPI_SlaveInit(spi_instance_t instance, spi_slave_t* config);

/*!
 * @brief Update the SS
 *
 * This function changes the SS, if this feature is available.
 *
 * @param[in] instance The name of the instance
 * @param[in] ss The number of SS
 * @return    Error or success status returned by API
 */
status_t SPI_SetSS(spi_instance_t, uint8_t ss);

/*!
 * @brief Initializes a non-blocking master transfer
 *
 * This function initializes a non-blocking master transfer.
 *
 * @param[in] instance The name of the instance
 * @param[in] txBuffer Pointer to tx buffer.
 * @param[in] rxBuffer Pointer to rx buffer.
 * @param[in] numberOfFrames Number of frames sent/received
 * @return    Error or success status returned by API
 */
status_t SPI_MasterTransfer(spi_instance_t instance, void* txBuffer, void* rxBuffer, uint16_t numberOfFrames);

/*!
 * @brief Initializes a blocking master transfer
 *
 * This function initializes a blocking master transfer.
 *
 * @param[in] instance The name of the instance
 * @param[in] txBuffer Pointer to tx buffer.
 * @param[in] rxBuffer Pointer to rx buffer.
 * @param[in] numberOfFrames Number of frames sent/received
 * @param[in] timeout Transfer time-out in ms
 * @return    Error or success status returned by API
 */
status_t SPI_MasterTransferBlocking(spi_instance_t instance, void* txBuffer, void* rxBuffer, uint16_t numberOfFrames, uint16_t timeout);

/*!
 * @brief Initializes a non-blocking slave transfer
 *
 * This function initializes a non-blocking slave transfer.
 *
 * @param[in] instance The name of the instance
 * @param[in] txBuffer Pointer to tx buffer.
 * @param[in] rxBuffer Pointer to rx buffer.
 * @param[in] numberOfFrames Number of frames sent/received
 * @return    Error or success status returned by API
 */
status_t SPI_SlaveTransfer(spi_instance_t instance, void* txBuffer, void* rxBuffer, uint16_t numberOfFrames);

/*!
 * @brief Initializes a blocking slave transfer
 *
 * This function initializes a blocking slave transfer.
 *
 * @param[in] instance The name of the instance
 * @param[in] txBuffer Pointer to tx buffer.
 * @param[in] rxBuffer Pointer to rx buffer.
 * @param[in] numberOfFrames Number of frames sent/received
 * @param[in] timeout Transfer time-out in ms
 * @return    Error or success status returned by API
 */
status_t SPI_SlaveTransferBlocking(spi_instance_t instance, void* txBuffer, void* rxBuffer, uint16_t numberOfFrames, uint16_t timeout);

/*!
 * @brief Gets the status of the last transfer
 *
 * This function return the status of the last transfer. Using this function the user can check
 * if the transfer is still in progress or if time-out event occurred.
 *
 * @param[in] instance The name of the instance
 * @param[in] txBuffer Pointer to tx buffer.
 * @param[in] rxBuffer Pointer to rx buffer.
 * @param[in] numberOfFrames Number of frames sent/received
 * @param[in] timeout Transfer time-out in ms
 * @return    Error or success status returned by API
 */
status_t SPI_GetStatus(spi_instance_t instance);

/*!
 * @brief Gets the default configuration structure for master
 *
 * The default configuration structure is:
 *
 * @param[out] config Pointer to configuration structure
 * @return    Error or success status returned by API
 */
status_t SPI_GetDefaultMasterConfig(spi_master_t *config);

/*!
 * @brief Gets the default configuration structure for slave
 *
 * The default configuration structure is:
 *
 * @param[out] config Pointer to configuration structure
 * @return    Error or success status returned by API
 */
status_t SPI_GetDefaultSlaveConfig(spi_slave_t *config);

/*!
 * @brief De-initializes the spi master module
 *
 * This function de-initialized the spi master module.
 *
 * @param[in] instance The name of the instance
 * @return    Error or success status returned by API
 */
status_t SPI_MasterDeinit(spi_instance_t instance);

/*!
 * @brief De-initializes the spi slave module
 *
 * This function de-initialized the spi slave module.
 *
 * @param[in] instance The name of the instance
 * @return    Error or success status returned by API
 */
status_t SPI_SlaveDeinit(spi_instance_t instance);

#if defined(__cplusplus)
}
#endif

/*! @}*/
#endif /* SPI_PAL_H */


/*******************************************************************************
 * EOF
 ******************************************************************************/
