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

#ifndef CALLBACKS_H
#define CALLBACKS_H

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, A project should not contain
 * unused type declarations.
 * The header defines callback types for all PAL modules.
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Define the enum of the events which can trigger SPI callback
 * This enum should include the events for all platforms
 */
typedef enum
{
    SPI_EVENT_END_TRANSFER = 0
} spi_event_t;

/* Callback for all peripherals which supports SPI features */
typedef void (*spi_callback_t)(void *driverState, spi_event_t event, void *userData);

/* Define the enum of the events which can trigger UART callback
 * This enum should include the events for all platforms
 */
typedef enum
{
    UART_EVENT_RX_FULL      = 0x00U,    /*!< Rx buffer is full */
    UART_EVENT_TX_EMPTY     = 0x01U,    /*!< Tx buffer is empty */
    UART_EVENT_END_TRANSFER = 0x02U,    /*!< The current transfer is ending */
    UART_EVENT_ERROR        = 0x03U,    /*!< An error occured during transfer */
} uart_event_t;

/* Callback for all peripherals which support UART features */
typedef void (*uart_callback_t)(void *driverState, uart_event_t event, void *userData);


/* Callback for all peripherals which support TIMING features */
typedef void (*timer_callback_t)(void *userData);


/* Define the structure for input parameter to the callback functions for ADC PAL
 */
typedef struct
{
    uint32_t groupIndex;         /*!< Index of the group executing the callback. */
    uint16_t resultBufferTail;   /*!< Offset of the most recent conversion result in the result buffer. */
} adc_callback_info_t;

typedef void (* const adc_callback_t)(const adc_callback_info_t * const callbackInfo, void * userData);

#endif /* CALLBACKS_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
