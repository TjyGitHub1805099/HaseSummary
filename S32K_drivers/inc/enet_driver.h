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

#ifndef ENET_H
#define ENET_H

#include "device_registers.h"
#include "status.h"
#include "osif.h"
#include <stdlib.h>

/*!
 * @file enet_driver.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro
 * Function-like macros are used instead of inline functions in order to ensure
 * that the performance will not be decreased if the functions will not be
 * inlined by the compiler.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced
 * This macro might be used by the user.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, Global typedef not referenced.
 * The specified typedefs might be used in the user code for composing masks
 * which could be passed to some of the ENET driver functions.
 *
 */

/*!
 * @defgroup enet_driver ENET Driver
 * @ingroup enet
 * @addtogroup enet_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Defines the maximum Ethernet frame size. */
#define ENET_FRAME_MAX_FRAMELEN         1518U
/*! @brief ENET minimum buffer size. */
#define ENET_MIN_BUFFERSIZE             64U

/*! @brief Definitions used for aligning the data buffers */
#define ENET_BUFF_ALIGN(x)       (((uint32_t)(x) + (FEATURE_ENET_BUFF_ALIGNMENT - 1UL)) & ~(FEATURE_ENET_BUFF_ALIGNMENT - 1UL))
#define ENET_BUFF_IS_ALIGNED(x)  (((uint32_t)(x) & ~(FEATURE_ENET_BUFF_ALIGNMENT - 1UL)) != 0UL)

/*! @brief Definitions used for aligning the buffer descriptors */
#define ENET_BUFFDESCR_ALIGN(x)       (((uint32_t)(x) + (FEATURE_ENET_BUFFDESCR_ALIGNMENT - 1UL)) & ~(FEATURE_ENET_BUFFDESCR_ALIGNMENT - 1UL))
#define ENET_BUFFDESCR_IS_ALIGNED(x)  (((uint32_t)(x) & ~(FEATURE_ENET_BUFFDESCR_ALIGNMENT - 1UL)) != 0UL)

/*!
 * @brief Media Independent Interface mode selection
 * Implements : enet_mii_mode_t_Class
 */
typedef enum
{
    ENET_MII_MODE = 0U, /*!< MII mode for data interface. */
    ENET_RMII_MODE      /*!< RMII mode for data interface. */
} enet_mii_mode_t;

/*!
 * @brief Media Independent Interface speed selection
 * Implements : enet_mii_speed_t_Class
 */
typedef enum
{
    ENET_MII_SPEED_100M = 0U, /*!< Speed 100 Mbps. */
    ENET_MII_SPEED_10M        /*!< Speed 10 Mbps. */
} enet_mii_speed_t;

/*!
 * @brief Media Independent Interface full-/half-duplex selection
 * Implements : enet_mii_duplex_t_Class
 */
typedef enum
{
    ENET_MII_HALF_DUPLEX = 0U, /*!< Half-duplex mode. */
    ENET_MII_FULL_DUPLEX       /*!< Full-duplex mode. */
} enet_mii_duplex_t;

/*!
 * @brief Special receive control configurations
 * Implements : enet_rx_special_config_t_Class
 */
typedef enum
{
    ENET_RX_CONFIG_ENABLE_PAYLOAD_LEN_CHECK  = 0x0001U,
    ENET_RX_CONFIG_STRIP_CRC_FIELD           = 0x0002U,
    ENET_RX_CONFIG_FORWARD_PAUSE_FRAMES      = 0x0004U,
    ENET_RX_CONFIG_REMOVE_PADDING            = 0x0008U,
    ENET_RX_CONFIG_ENABLE_FLOW_CONTROL       = 0x0010U,
    ENET_RX_CONFIG_REJECT_BROADCAST_FRAMES   = 0x0020U,
    ENET_RX_CONFIG_ENABLE_PROMISCUOUS_MODE   = 0x0040U,
    ENET_RX_CONFIG_ENABLE_MII_LOOPBACK       = 0x0080U
} enet_rx_special_config_t;

/*!
 * @brief Special transmit control configurations
 * Implements : enet_tx_special_config_t_Class
 */
typedef enum
{
    ENET_TX_CONFIG_DISABLE_CRC_APPEND           = 0x0001U,
    ENET_TX_CONFIG_ENABLE_MAC_ADDR_INSERTION    = 0x0002U,
} enet_tx_special_config_t;

/*!
 * @brief Interrupt sources
 * Implements : enet_interrupt_enable_t_Class
 */
typedef enum
{
    ENET_BABR_INTERRUPT = ENET_EIR_BABR_MASK,
    ENET_BABT_INTERRUPT = ENET_EIR_BABT_MASK,
    ENET_GRACE_STOP_INTERRUPT = ENET_EIR_GRA_MASK,
    ENET_TX_FRAME_INTERRUPT = ENET_EIR_TXF_MASK,
    ENET_TX_BUFFER_INTERRUPT = ENET_EIR_TXB_MASK,
    ENET_RX_FRAME_INTERRUPT = ENET_EIR_RXF_MASK,
    ENET_RX_BUFFER_INTERRUPT = ENET_EIR_RXB_MASK,
    ENET_MII_INTERRUPT = ENET_EIR_MII_MASK,
    ENET_EBERR_INTERRUPT = ENET_EIR_EBERR_MASK,
    ENET_LATE_COLLISION_INTERRUPT = ENET_EIR_LC_MASK,
    ENET_RETRY_LIMIT_INTERRUPT = ENET_EIR_RL_MASK,
    ENET_UNDERRUN_INTERRUPT = ENET_EIR_UN_MASK,
    ENET_PAYLOAD_RX_INTERRUPT = ENET_EIR_PLR_MASK,
    ENET_WAKEUP_INTERRUPT = ENET_EIR_WAKEUP_MASK,
    ENET_TS_AVAIL_INTERRUPT = ENET_EIR_TS_AVAIL_MASK,
    ENET_TS_TIMER_INTERRUPT = ENET_EIR_TS_TIMER_MASK,
#if FEATURE_ENET_HAS_AVB
    ENET_RX_FLUSH_2_INTERRUPT = ENET_EIR_RXFLUSH_2_MASK,
    ENET_RX_FLUSH_1_INTERRUPT = ENET_EIR_RXFLUSH_1_MASK,
    ENET_RX_FLUSH_0_INTERRUPT = ENET_EIR_RXFLUSH_0_MASK,
    ENET_TX_FRAME_1_INTERRUPT = ENET_EIR_TXF1_MASK,
    ENET_TX_BUFFER_1_INTERRUPT = ENET_EIR_TXB1_MASK,
    ENET_RX_FRAME_1_INTERRUPT = ENET_EIR_RXF1_MASK,
    ENET_RX_BUFFER_1_INTERRUPT = ENET_EIR_RXB1_MASK,
    ENET_TX_FRAME_2_INTERRUPT = ENET_EIR_TXF2_MASK,
    ENET_TX_BUFFER_2_INTERRUPT = ENET_EIR_TXB2_MASK,
    ENET_RX_FRAME_2_INTERRUPT = ENET_EIR_RXF2_MASK,
    ENET_RX_BUFFER_2_INTERRUPT = ENET_EIR_RXB2_MASK,
#endif /* FEATURE_ENET_HAS_AVB */
#if FEATURE_ENET_HAS_RECEIVE_PARSER
    ENET_PARSERERR_INTERRUPT = ENET_EIR_PARSERR_MASK,
    ENET_PARSRF_INTERRUPT = ENET_EIR_PARSRF_MASK,
#endif /* FEATURE_ENET_HAS_RECEIVE_PARSER */
} enet_interrupt_enable_t;

/*!
 * @brief Transmit accelerator configurations
 * Implements : enet_tx_accelerator_t_Class
 */
typedef enum
{
    ENET_TX_ACCEL_ENABLE_SHIFT16        = ENET_TACC_SHIFT16_MASK,
    ENET_TX_ACCEL_INSERT_IP_CHECKSUM    = ENET_TACC_IPCHK_MASK,
    ENET_TX_ACCEL_INSERT_PROTO_CHECKSUM = ENET_TACC_PROCHK_MASK
} enet_tx_accelerator_t;

/*!
 * @brief Receive accelerator configurations
 * Implements : enet_rx_accelerator_t_Class
 */
typedef enum
{
    ENET_RX_ACCEL_REMOVE_PAD         = ENET_RACC_PADREM_MASK,
    ENET_RX_ACCEL_ENABLE_IP_CHECK    = ENET_RACC_IPDIS_MASK,
    ENET_RX_ACCEL_ENABLE_PROTO_CHECK = ENET_RACC_PRODIS_MASK,
    ENET_RX_ACCEL_ENABLE_MAC_CHECK   = ENET_RACC_LINEDIS_MASK,
    ENET_RX_ACCEL_ENABLE_SHIFT16     = ENET_RACC_SHIFT16_MASK
} enet_rx_accelerator_t;

/*!
 * @brief Send/Receive buffer information for the user
 * Implements : enet_buffer_t_Class
 */
typedef struct
{
    uint8_t *data;
    uint16_t length;
} enet_buffer_t;

/*!
 * @brief Send/Receive internal buffer descriptor
 * Implements : enet_buffer_descriptor_t_Class
 */
#if (defined(CORE_BIG_ENDIAN))
typedef struct
{
    uint16_t control; /*!< Buffer descriptor control and status. */
    uint16_t length;  /*!< Buffer descriptor data length. */
    uint8_t *buffer;  /*!< Data buffer pointer. */
#if FEATURE_ENET_HAS_ENHANCED_BD
    uint32_t enh1;    /*!< First word of the extended part of the buffer descriptor. */
    uint32_t enh2;    /*!< Second word of the extended part of the buffer descriptor. */
    uint32_t enh3;    /*!< Third word of the extended part of the buffer descriptor. */
    uint32_t timestamp;  /*!< Timestamp of the frame. */
    uint32_t reserved1;  /*!< Reserved. */
    uint32_t reserved2;  /*!< Reserved. */
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */
} enet_buffer_descriptor_t;
#elif (defined(CORE_LITTLE_ENDIAN))
typedef struct
{
    uint16_t length;  /*!< Buffer descriptor data length. */
    uint16_t control; /*!< Buffer descriptor control and status. */
    uint8_t *buffer;  /*!< Data buffer pointer. */
#if FEATURE_ENET_HAS_ENHANCED_BD
    uint32_t enh1;    /*!< First word of the extended part of the buffer descriptor. */
    uint32_t enh2;    /*!< Second word of the extended part of the buffer descriptor. */
    uint32_t enh3;    /*!< Third word of the extended part of the buffer descriptor. */
    uint32_t timestamp;  /*!< Timestamp of the frame. */
    uint32_t reserved1;  /*!< Reserved. */
    uint32_t reserved2;  /*!< Reserved. */
#endif /* FEATURE_ENET_HAS_ENHANCED_BD */
} enet_buffer_descriptor_t;
#else
    #error "Endianness not defined!"
#endif

/*!
 * @brief Event specifier for the callback function
 * Implements : enet_event_t_Class
 */
typedef enum
{
    ENET_RX_EVENT,
    ENET_TX_EVENT,
    ENET_ERR_EVENT,
    ENET_WAKE_UP_EVENT
} enet_event_t;

/*!
 * @brief Callback function invoked when one of the events in "enet_event_t" is encountered
 * Implements : enet_callback_t_Class
 */
typedef void (*enet_callback_t)(uint8_t instance, enet_event_t event);

/*!
 * @brief Defines the ENET buffer descriptors ring configuration structure
 * Implements : enet_buffer_config_t_Class
 */
typedef struct
{
    uint16_t rxRingSize;                              /*!< Receive buffer descriptors number. */
    uint16_t txRingSize;                              /*!< Transmit buffer descriptors number. */
    enet_buffer_descriptor_t *rxRingAligned;          /*!< Aligned receive buffer descriptor ring start address. */
    enet_buffer_descriptor_t *txRingAligned;          /*!< Aligned transmit buffer descriptor ring start address. */
    uint8_t *rxBufferAligned;                         /*!< Receive data buffers start address. */
} enet_buffer_config_t;

/*!
 * @brief Defines the ENET module configuration structure
 * Implements : enet_config_t_Class
 */
typedef struct
{
    /* Accelerator configuration */
    uint8_t rxAccelerConfig;      /*!< Receive accelerator, A logical OR of "enet_rx_accelerator_t". */
    uint8_t txAccelerConfig;      /*!< Transmit accelerator, A logical OR of "enet_tx_accelerator_t". */
    /* Basic configuration */
    uint16_t maxFrameLen;         /*!< Maximum frame length. */
    uint32_t interrupts;          /*!< MAC interrupt source. A logical OR of "enet_interrupt_enable_t". */
    /* MII configuration */
    enet_mii_mode_t miiMode;      /*!< MII mode. */
    enet_mii_speed_t miiSpeed;    /*!< MII Speed. */
    enet_mii_duplex_t miiDuplex;  /*!< MII duplex. */
    /* Receive and transmit special configurations */
    uint32_t rxConfig;    /*!< MAC receive special configuration. A logical OR of "enet_rx_special_config_t". */
    uint32_t txConfig;    /*!< MAC transmit special configuration. A logical OR of "enet_tx_special_config_t". */
    /* Callback function */
    enet_callback_t callback;
} enet_config_t;

/*!
 * @brief Internal driver state structure
 * Implements : enet_state_t_Class
 */
typedef struct
{
    enet_buffer_descriptor_t *rxBdBase;    /*!< Receive buffer descriptor base address pointer. */
    enet_buffer_descriptor_t *rxBdCurrent; /*!< The current available receive buffer descriptor pointer. */
    enet_buffer_descriptor_t *rxBdAlloc;   /*!< Pointer used for enqueuing Rx buffers provided using ENET_DRV_ProvideRxBuff. */
    enet_buffer_descriptor_t *txBdBase;    /*!< Transmit buffer descriptor base address pointer. */
    enet_buffer_descriptor_t *txBdCurrent; /*!< The current available transmit buffer descriptor pointer. */
    enet_callback_t callback;              /*!< Callback function. */
} enet_state_t;

/*!
 * @brief Statistics counters enumeration
 * Implements : enet_counter_t_Class
 */
typedef enum
{
    ENET_CTR_RMON_T_DROP = 0U,
    ENET_CTR_RMON_T_PACKETS,
    ENET_CTR_RMON_T_BC_PKT,
    ENET_CTR_RMON_T_MC_PKT,
    ENET_CTR_RMON_T_CRC_ALIGN,
    ENET_CTR_RMON_T_UNDERSIZE,
    ENET_CTR_RMON_T_OVERSIZE,
    ENET_CTR_RMON_T_FRAG,
    ENET_CTR_RMON_T_JAB,
    ENET_CTR_RMON_T_COL,
    ENET_CTR_RMON_T_P64,
    ENET_CTR_RMON_T_P65TO127,
    ENET_CTR_RMON_T_P128TO255,
    ENET_CTR_RMON_T_P256TO511,
    ENET_CTR_RMON_T_P512TO1023,
    ENET_CTR_RMON_T_P1024TO2047,
    ENET_CTR_RMON_T_P_GTE2048,
    ENET_CTR_RMON_T_OCTETS,
    ENET_CTR_IEEE_T_DROP,
    ENET_CTR_IEEE_T_FRAME_OK,
    ENET_CTR_IEEE_T_1COL,
    ENET_CTR_IEEE_T_MCOL,
    ENET_CTR_IEEE_T_DEF,
    ENET_CTR_IEEE_T_LCOL,
    ENET_CTR_IEEE_T_EXCOL,
    ENET_CTR_IEEE_T_MACERR,
    ENET_CTR_IEEE_T_CSERR,
    ENET_CTR_IEEE_T_SQE,
    ENET_CTR_IEEE_T_FDXFC,
    ENET_CTR_IEEE_T_OCTETS_OK = 29U,
    ENET_CTR_RMON_R_PACKETS = 33U,
    ENET_CTR_RMON_R_BC_PKT,
    ENET_CTR_RMON_R_MC_PKT,
    ENET_CTR_RMON_R_CRC_ALIGN,
    ENET_CTR_RMON_R_UNDERSIZE,
    ENET_CTR_RMON_R_OVERSIZE,
    ENET_CTR_RMON_R_FRAG,
    ENET_CTR_RMON_R_JAB,
    ENET_CTR_RMON_R_RESVD_0,
    ENET_CTR_RMON_R_P64,
    ENET_CTR_RMON_R_P65TO127,
    ENET_CTR_RMON_R_P128TO255,
    ENET_CTR_RMON_R_P256TO511,
    ENET_CTR_RMON_R_P512TO1023,
    ENET_CTR_RMON_R_P1024TO2047,
    ENET_CTR_RMON_R_P_GTE2048,
    ENET_CTR_RMON_R_OCTETS,
    ENET_CTR_IEEE_R_DROP,
    ENET_CTR_IEEE_R_FRAME_OK,
    ENET_CTR_IEEE_R_CRC,
    ENET_CTR_IEEE_R_ALIGN,
    ENET_CTR_IEEE_R_MACERR,
    ENET_CTR_IEEE_R_FDXFC,
    ENET_CTR_IEEE_R_OCTETS_OK
} enet_counter_t;

typedef struct {
    uint32_t errMask;    /*!< Mask of ENET_RX_ENH_ERR_*, representing the errors that occurred. */
    uint32_t timestamp;  /*!< Timestamp of the received frame. */
    uint16_t checksum;   /*!< One's complement sum of the payload section of the IP frame. */
    uint8_t vlanPrio;    /*!< Frame priority level. Valid values are from 0 (best effort) to 7 (highest). */
    uint8_t headerLen;   /*!< Number of 32-bit words found within the IP and its following protocol headers. */
    uint8_t proto;       /*!< The 8-bit protocol field found within the IP header of the frame. */
    bool unicast;        /*!< Indicates that the frame is an unicast frame. */
    bool ipv6;           /*!< Indicates that the frame is an IPv6 frame. */
    bool ipv4Frag;       /*!< Indicates that the frame is an IPv4 fragment frame. */
} enet_rx_enh_info_t;

typedef struct {
    uint32_t errMask;    /*!< Mask of ENET_TX_ENH_ERR_*, representing the errors that occurred. */
    uint32_t timestamp;  /*!< Timestamp of the transmitted frame. */
} enet_tx_enh_info_t;


#define ENET_RX_ENH_ERR_PROTO_CHECKSUM   (0x00000010U) /*!< Set when the checksum of the protocol is invalid or an unknown
                                                            protocol is found and checksumming could not be performed. */
#define ENET_RX_ENH_ERR_IPHDR_CHECKSUM   (0x00000020U) /*!< Set when either a non-IP frame is received or the IP header checksum
                                                            was invalid. */
#define ENET_RX_ENH_ERR_COLLISION        (0x02000000U) /*!< Set when the frame was received with a collision detected during
                                                            reception. */
#define ENET_RX_ENH_ERR_PHY              (0x04000000U) /*!< Set when the frame was received with an Error character on the PHY
                                                            interface.*/
#define ENET_RX_ENH_ERR_MAC              (0x80000000U) /*!< Set when the frame stored in the system memory was received with an
                                                            error (typically, a receive FIFO overflow). */

#define ENET_TX_ENH_ERR_TIMESTAMP        (0x00000100U) /*!< Indicates that the MAC reported a different frame type than a timestamp
                                                            frame. */
#define ENET_TX_ENH_ERR_OVERFLOW         (0x00000200U) /*!< Indicates that the MAC reported that there was a FIFO overflow condition
                                                            on transmit. */
#define ENET_TX_ENH_ERR_LATE_COLLISION   (0x00000400U) /*!< Indicates that the MAC reported that there was a Late Collision on
                                                            transmit. */
#define ENET_TX_ENH_ERR_FRAME            (0x00000800U) /*!< Indicates that the MAC reported that the uDMA reported an error when
                                                            providing the packet. */
#define ENET_TX_ENH_ERR_EXCESS_COLLISION (0x00001000U) /*!< Indicates that the MAC reported an excess collision error on
                                                            transmit.  */
#define ENET_TX_ENH_ERR_UNDERFLOW        (0x00002000U) /*!< Indicates that the MAC reported an underflow error on transmit. */


/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Initialization and De-initialization
 * @{
 */

/*!
 * @brief Gets the default configuration structure
 *
 * This function gets the default configuration structure, with the following settings:
 * - no interrupt enabled
 * - maximum receive frame length equal to the maximum Ethernet frame length
 * - no special receive/transmit control configuration
 * - no acceleration function enabled
 * - RMII mode, full-duplex, 100Mbps for MAC and PHY data interface
 * - no callback installed
 *
 * @param[out] config The configuration structure
 */
void ENET_DRV_GetDefaultConfig(enet_config_t *config);

/*!
 * @brief Initializes the ENET module
 *
 * This function initializes and enables the ENET module, configuring receive and transmit control
 * settings, the receive and transmit descriptors rings, and the MAC physical address.
 *
 * @param[in] instance Instance number
 * @param[in] state Pointer to the state structure which will be used for holding
 * the internal state of the driver.
 * @param[in] config The module configuration structure
 * @param[in] bufferConfig The buffer descriptors configuration structure
 * @param[in] macAddr The physical address of the MAC
 */
void ENET_DRV_Init(uint8_t instance,
                   enet_state_t *state,
                   const enet_config_t *config,
                   const enet_buffer_config_t *bufferConfig,
                   const uint8_t *macAddr);

/*!
 * @brief Deinitializes the ENET module
 *
 * This function disables the interrupts and then disables the ENET module.
 *
 * @param[in] instance Instance number
 */
void ENET_DRV_Deinit(uint8_t instance);

/*! @} */

/*!
 * @name Transmission and reception operations
 * @{
 */

/*!
 * @brief Reads a received Ethernet frame
 *
 * This function reads the first received Ethernet frame in the Rx queue. The buffer received
 * as parameter will be updated by the driver and the .data field will point to a memory area
 * containing the frame data.
 *
 * Note: Once the application finished processing the buffer, it could be reused by the
 * driver for further receptions by invoking ENET_DRV_ProvideRxBuff.
 *
 * Important: The driver does not ensure synchronization between different threads trying
 * to read a frame at the same time. This synchronization shall be implemented by the
 * application.
 *
 * @param[in]  instance Instance number
 * @param[out] buff The buffer containing the frame
 * @param[out] info Enhanced information related to the buffer. If this information is not needed,
 * this parameter should be NULL.
 * @return STATUS_SUCCESS if a frame was successfully read, STATUS_ENET_RX_QUEUE_EMPTY if
 * there is no available frame in the queue.
 */
status_t ENET_DRV_ReadFrame(uint8_t instance,
                            enet_buffer_t * buff,
                            enet_rx_enh_info_t * info);

/*!
 * @brief Provides a receive buffer to be used by the driver for reception.
 *
 * This function provides a buffer which can further be used by the reception mechanism
 * in order to store the received data.
 *
 * Note: The application can either provide a buffer previously obtained in a ENET_DRV_ReadFrame
 * call (when it is no longer needed after being fully processed), or allocate a new buffer,
 * pointing to a memory area having the required alignment (see FEATURE_ENET_BUFF_ALIGNMENT).
 * The former approach is recommended as it has a simpler usage model and re-uses the same initial
 * memory range for the entire driver lifetime operation. The later approach could provide more
 * flexibility, but since it involves constant memory free/alloc operations it is only recommended
 * with an efficient pool-based memory allocator.
 *
 * Important: The driver does not ensure synchronization between different threads trying
 * to provide a buffer at the same time. This synchronization shall be implemented by the
 * application.
 *
 * Important: The application is responsible for providing one Rx buffer for every frame it
 * receives, otherwise the reception ring can fill-up, affecting further reception.
 *
 * Usage example:
 *
 * stat = ENET_DRV_ReadFrame(INST_ETHERNET1, &rxBuff);
 *
 * if (stat == STATUS_SUCCESS)
 * {
 *     process_buffer(&rxBuff);
 *     ENET_DRV_ProvideRxBuff(INST_ETHERNET1, &rxBuff);
 * }
 *
 * @param[in] instance Instance number
 * @param[in] buff The buffer to be added to the reception ring
 */
void ENET_DRV_ProvideRxBuff(uint8_t instance,
                            enet_buffer_t * buff);

/*!
 * @brief Sends an Ethernet frame
 *
 * This function sends an Ethernet frame, contained in the buffer received as parameter.
 *
 * Note: Since the transmission of the frame is not complete when this function returns,
 * the application must not change/alter/re-use the provided buffer until after a call
 * to ENET_DRV_GetTransmitStatus for the same buffer returns STATUS_SUCCESS.
 *
 * Important: The driver does not ensure synchronization between different threads trying
 * to send a frame at the same time. This synchronization shall be implemented by the
 * application.
 *
 * @param[in] instance Instance number
 * @param[in] buff The buffer containing the frame
 * @return STATUS_SUCCESS if the frame was successfully enqueued for transmission,
 * STATUS_ENET_TX_QUEUE_FULL if there is no available space for the frame in the queue.
 */
status_t ENET_DRV_SendFrame(uint8_t instance,
                            const enet_buffer_t * buff);

/*!
 * @brief Checks if the transmission of a buffer is complete and extracts information
 * related to the transmission.
 *
 * This function checks if the transmission of the given buffer is complete and extracts
 * information related to the transmission.
 *
 * Note: If the buffer is not found in the Tx ring, the function will return STATUS_ENET_BUFF_NOT_FOUND;
 * in this case, the enhanced information will not be extracted.
 *
 * @param[in] instance Instance number
 * @param[in] buff The transmit buffer for which the status shall be checked
 * @param[out] info Enhanced information related to the buffer. If this information is not needed,
 * this parameter should be NULL.
 * @return STATUS_BUSY if the frame is still enqueued for transmission, STATUS_ENET_BUFF_NOT_FOUND
 * if the buffer was not found in the Tx queue, STATUS_SUCCESS otherwise,.
 */
status_t ENET_DRV_GetTransmitStatus(uint8_t instance,
                                    const enet_buffer_t * buff,
                                    enet_tx_enh_info_t * info);

/*! @} */

/*!
 * @name MDIO configuration and operation
 * @{
 */

/*!
 * @brief Enables the MDIO interface
 *
 * @param[in] instance Instance number
 * @param[in] miiPreambleDisabled Enables/disables prepending a preamble to the MII management frame.
 */
void ENET_DRV_EnableMDIO(uint8_t instance,
                         bool miiPreambleDisabled);

/*!
 * @brief Reads the selected register of the PHY
 *
 * @param[in]  instance Instance number
 * @param[in]  phyAddr PHY device address
 * @param[in]  phyReg PHY register address
 * @param[out] data Data read from the PHY
 * @param[in] timeoutMs Timeout for the read operation (in milliseconds)
 */
status_t ENET_DRV_MDIORead(uint8_t instance,
                           uint8_t phyAddr,
                           uint8_t phyReg,
                           uint16_t *data,
                           uint32_t timeoutMs);

/*!
 * @brief Writes the selected register of the PHY
 *
 * @param[in] instance Instance number
 * @param[in] phyAddr PHY device address
 * @param[in] phyReg PHY register address
 * @param[in] data Data to be written in the specified register of the PHY
 * @param[in] timeoutMs Timeout for the write operation (in milliseconds)
 */
status_t ENET_DRV_MDIOWrite(uint8_t instance,
                            uint8_t phyAddr,
                            uint8_t phyReg,
                            uint16_t data,
                            uint32_t timeoutMs);

/*! @} */

/*!
 * @name MAC Address configuration
 * @{
 */

/*!
 * @brief Configures the physical address of the MAC
 *
 * @param[in] instance Instance number
 * @param[in] macAddr The MAC address to be configured
 */
void ENET_DRV_SetMacAddr(uint8_t instance,
                         const uint8_t *macAddr);

/*!
 * @brief Gets the physical address of the MAC
 *
 * @param[in]  instance Instance number
 * @param[out] macAddr The physical address of the MAC
 */
void ENET_DRV_GetMacAddr(uint8_t instance,
                         uint8_t *macAddr);

/*!
 * @brief Enables/Disables forwarding of unicast traffic having a specific MAC address
 * as destination.
 *
 * @param[in]  instance Instance number
 * @param[in]  macAddr The physical address
 * @param[in]  enable If true, the application will receive all the unicast traffic having as
 * destination address the provided MAC address; if false, stop forwarding this kind of
 * traffic
 */
void ENET_DRV_SetUnicastForward(uint8_t instance,
                                const uint8_t *macAddr,
                                bool enable);

/*!
 * @brief Enables/Disables forwarding of multicast traffic having a specific MAC address
 * as destination.
 *
 * @param[in]  instance Instance number
 * @param[in]  macAddr The physical address
 * @param[in]  enable If true, the application will receive all the multicast traffic having as
 * destination address the provided MAC address; if false, stop forwarding this kind of
 * traffic
 */
void ENET_DRV_SetMulticastForward(uint8_t instance,
                                  const uint8_t *macAddr,
                                  bool enable);

/*!
 * @brief Enables/Disables forwarding of the multicast traffic, irrespective of the destination
 * MAC address.
 *
 * @param[in]  instance Instance number
 * @param[in]  enable If true, the application will receive all the multicast traffic; if false,
 * stop forwarding this kind of traffic
 */
void ENET_DRV_SetMulticastForwardAll(uint8_t instance,
                                     bool enable);

/*! @} */

/*!
 * @name Other basic operations
 * @{
 */

/*!
 * @brief Sets the MAC in sleep mode or normal mode
 *
 * @param[in] instance Instance number
 * @param[in] enable If true, set MAC in sleep mode; if false, set MAC in normal mode
 */
void ENET_DRV_SetSleepMode(uint8_t instance,
                           bool enable);

/*!
 * @brief Enables/Disables the MIB counters.
 *
 * Note: When enabling the counters, their values are reset.
 *
 * @param[in] instance Instance number
 * @param[in] enable Enable/Disable MIB counters
 */
void ENET_DRV_ConfigCounters(uint8_t instance,
                             bool enable);

/*!
 * @brief Gets statistics from the specified counter
 *
 * @param[in] instance Instance number
 * @param[in] counter The counter to be read
 * @return The value of the requested counter
 */
uint32_t ENET_DRV_GetCounter(uint8_t instance,
                             enet_counter_t counter);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif  /* ENET_H */


/*******************************************************************************
 * EOF
 ******************************************************************************/

