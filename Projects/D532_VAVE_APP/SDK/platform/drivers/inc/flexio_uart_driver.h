/*
 * Copyright (c) 2014 - 2016, Freescale Semiconductor, Inc.
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

#ifndef FLEXIO_UART_DRIVER_H
#define FLEXIO_UART_DRIVER_H

#include <stddef.h>
#include <stdbool.h>
#include "device_registers.h"
#include "flexio.h"
#include "edma_driver.h"
#include "callbacks.h"

/*!
 * @addtogroup flexio_uart_drv
 * @{
 */

/*******************************************************************************
 * Enumerations.
 ******************************************************************************/


 /*!
 * @brief flexio_uart driver direction (tx or rx)
 *
 * This structure describes the direction configuration options for the flexio_uart driver.
 * Implements : flexio_uart_driver_direction_t_Class
 */
typedef enum
{
    FLEXIO_UART_DIRECTION_TX       = 0x01U,    /*!< Tx UART driver */
    FLEXIO_UART_DIRECTION_RX       = 0x00U,    /*!< Rx UART driver */
} flexio_uart_driver_direction_t;


/*******************************************************************************
* Definitions
******************************************************************************/


 /*!
 * @brief Driver configuration structure
 *
 * This structure is used to provide configuration parameters for the flexio_uart driver at initialization time.
 * Implements : flexio_uart_user_config_t_Class
 */
typedef struct
{
    flexio_driver_type_t driverType;            /*!< Driver type: interrupts/polling/DMA */
    uint32_t baudRate;                          /*!< Baud rate in hertz */
    uint8_t bitCount;                           /*!< Number of bits per word */
    flexio_uart_driver_direction_t direction;   /*!< Driver direction: Tx or Rx */
    uint8_t dataPin;                            /*!< Flexio pin to use as Tx or Rx pin */
    uart_callback_t callback;                   /*!< User callback function. Note that this function will be
                                                     called from the interrupt service routine, so its
                                                     execution time should be as small as possible. It can be
                                                     NULL if it is not needed */
    void *callbackParam;                        /*!< Parameter for the callback function */
    uint8_t dmaChannel;                         /*!< DMA channel number. Only used in DMA mode */
} flexio_uart_user_config_t;


/*!
 * @brief Driver internal context structure
 *
 * This structure is used by the flexio_uart driver for its internal logic. It must
 * be provided by the application through the FLEXIO_UART_DRV_Init() function, then
 * it cannot be freed until the driver is de-initialized using FLEXIO_UART_DRV_DeInit().
 * The application should make no assumptions about the content of this structure.
 */
typedef struct
{
/*! @cond DRIVER_INTERNAL_USE_ONLY */
    flexio_common_state_t flexioCommon;        /* Common flexio drivers structure */
    uint8_t *data;                             /* Transmit/Receive buffer. */
    uint32_t remainingBytes;                   /* Number of remaining bytes to be transferred. */
    uint8_t dmaChannel;                        /* DMA channel number */
    uart_callback_t callback;                  /* User callback function */
    void *callbackParam;                       /* Parameter for the callback function */
    flexio_uart_driver_direction_t direction;  /* Driver direction: Tx or Rx */
    flexio_driver_type_t driverType;           /* Driver type: interrupts/polling/DMA */
    status_t status;                           /* Current status of the driver */
    bool driverIdle;                           /* Idle/busy state of the receiver */
    semaphore_t idleSemaphore;                 /* Semaphore used by blocking functions */
    bool blocking;                             /* Specifies if the current transfer is blocking */
    uint8_t bitCount;                          /* Number of bits per word */
    uint8_t txFlush;                           /* Used for flushing Tx buffer before ending a transmission */
/*! @endcond */
} flexio_uart_state_t;


/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name FLEXIO_UART Driver
 * @{
 */

#if defined(__cplusplus)
extern "C" {
#endif


/*!
 * @brief Initialize the FLEXIO_UART driver
 *
 * This function initializes the FLEXIO_UART driver.
 *
 * @param instance  FLEXIO peripheral instance number
 * @param userConfigPtr    Pointer to the FLEXIO_UART user configuration structure. The function
 *                         reads configuration data from this structure and initializes the
 *                         driver accordingly. The application may free this structure after
 *                         the function returns.
 * @param state     Pointer to the FLEXIO_UART driver context structure. The driver uses
 *                  this memory area for its internal logic. The application must make no
 *                  assumptions about the content of this structure, and must not free this
 *                  memory until the driver is de-initialized using FLEXIO_UART_DRV_Deinit().
 * @return    Error or success status returned by API
 */
status_t FLEXIO_UART_DRV_Init(uint32_t instance,
                              const flexio_uart_user_config_t * userConfigPtr,
                              flexio_uart_state_t * state);

/*!
 * @brief De-initialize the FLEXIO_UART driver
 *
 * This function de-initializes the FLEXIO_UART driver. The driver can't be used
 * again until reinitialized. The context structure is no longer needed by the driver and
 * can be freed after calling this function.
 *
 * @param state    Pointer to the FLEXIO_UART driver context structure.
 * @return    Error or success status returned by API
 */
status_t FLEXIO_UART_DRV_Deinit(flexio_uart_state_t * state);


/*!
 * @brief Set the baud rate and bit width for any subsequent UART communication
 *
 * This function sets the baud rate and bit width for the UART driver.
 * Note that due to module limitation not any baud rate can be achieved. The driver
 * will set a baud rate as close as possible to the requested baud rate, but there may
 * still be substantial differences, for example if requesting a high baud rate while
 * using a low-frequency FlexIO clock. The application should call
 * FLEXIO_UART_DRV_GetBaudRate() after FLEXIO_UART_DRV_SetConfig() to check
 * what baud rate was actually set.
 *
 * @param state    Pointer to the FLEXIO_UART driver context structure.
 * @param baudRate  the desired baud rate in hertz
 * @param bitCount  number of bits per word
 * @return    Error or success status returned by API
 */
status_t FLEXIO_UART_DRV_SetConfig(flexio_uart_state_t *state,
                                   uint32_t baudRate,
                                   uint8_t bitCount);


/*!
 * @brief Get the currently configured baud rate
 *
 * This function returns the currently configured UART baud rate.
 *
 * @param state    Pointer to the FLEXIO_UART driver context structure.
 * @param baudRate  the current baud rate in hertz
 * @return    Error or success status returned by API
 */
status_t FLEXIO_UART_DRV_GetBaudRate(flexio_uart_state_t *state, uint32_t *baudRate);


/*!
 * @brief Perform a blocking UART transmission
 *
 * This function sends a block of data and
 * only returns when the transmission is complete.
 *
 * @param state     Pointer to the FLEXIO_UART driver context structure.
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t FLEXIO_UART_DRV_SendDataBlocking(flexio_uart_state_t *state,
                                          const uint8_t * txBuff,
                                          uint32_t txSize,
                                          uint32_t timeout);

/*!
 * @brief Perform a non-blocking UART transmission
 *
 * This function sends a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the FLEXIO_UART_DRV_GetTransmitStatus() function (if
 * the driver is initialized in polling mode).
 *
 * @param state     Pointer to the FLEXIO_UART driver context structure.
 * @param txBuff    pointer to the data to be transferred
 * @param txSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_UART_DRV_SendData(flexio_uart_state_t *state,
                                  const uint8_t * txBuff,
                                  uint32_t txSize);


/*!
 * @brief Perform a blocking UART reception
 *
 * This function receives a block of data and
 * only returns when the transmission is complete.
 *
 * @param state     Pointer to the FLEXIO_UART driver context structure.
 * @param rxBuff    pointer to the receive buffer
 * @param rxSize    length in bytes of the data to be received
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t FLEXIO_UART_DRV_ReceiveDataBlocking(flexio_uart_state_t *state,
                                             uint8_t * rxBuff,
                                             uint32_t rxSize,
                                             uint32_t timeout);

/*!
 * @brief Perform a non-blocking UART reception
 *
 * This function receives a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode) or by the FLEXIO_UART_DRV_GetReceiveStatus() function (if
 * the driver is initialized in polling mode).
 *
 * @param state     Pointer to the FLEXIO_UART driver context structure.
 * @param rxBuff    pointer to the receive buffer
 * @param rxSize    length in bytes of the data to be received
 * @return    Error or success status returned by API
 */
status_t FLEXIO_UART_DRV_ReceiveData(flexio_uart_state_t *state,
                                     uint8_t * rxBuff,
                                     uint32_t rxSize);



/*!
 * @brief Get the status of the current non-blocking UART transfer
 *
 * This function returns the current status of a non-blocking UART transfer.
 * A return code of STATUS_BUSY means the transfer is still in progress.
 * Otherwise the function returns a status reflecting the outcome of the last transfer.
 * When the driver is initialized in polling mode this function also advances the transfer
 * by checking and handling the transmit and receive events, so it must be called
 * frequently to avoid overflows or underflows.
 *
 * @param state     Pointer to the FLEXIO_UART driver context structure.
 * @param bytesRemaining  the remaining number of bytes to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_UART_DRV_GetStatus(flexio_uart_state_t * state,
                                   uint32_t *bytesRemaining);



/*!
 * @brief Aborts a non-blocking UART transfer
 *
 * This function aborts a non-blocking UART transfer.
 *
 * @param state     Pointer to the FLEXIO_UART driver context structure.
 * @return    Error or success status returned by API
 */
status_t FLEXIO_UART_DRV_TransferAbort(flexio_uart_state_t * state);


/*!
 * @brief Provide a buffer for receiving data
 *
 * This function can be used to provide a new buffer for receiving data to the driver.
 * It can be called from the user callback when event STATUS_UART_RX_OVERRUN is reported.
 * This way the reception will continue without interruption.
 *
 * @param state    Pointer to the FLEXIO_UART driver context structure.
 * @param rxBuff    pointer to the buffer where to store received data
 * @param rxSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_UART_DRV_SetRxBuffer(flexio_uart_state_t *state,
                                     uint8_t * rxBuff,
                                     uint32_t rxSize);


/*!
 * @brief Provide a buffer for transmitting data
 *
 * This function can be used to provide a new buffer for transmitting data to the driver.
 * It can be called from the user callback when event STATUS_UART_TX_UNDERRUN is reported.
 * This way the transmission will continue without interruption.
 *
 * @param state    Pointer to the FLEXIO_UART driver context structure.
 * @param txBuff    pointer to the buffer containing transmit data
 * @param txSize    length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t FLEXIO_UART_DRV_SetTxBuffer(flexio_uart_state_t *state,
                                     const uint8_t * txBuff,
                                     uint32_t txSize);


/*! @}*/
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* FLEXIO_UART_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
