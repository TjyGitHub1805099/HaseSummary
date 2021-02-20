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

#include "flash_mx25l6433f_driver.h"
#include "quadspi_driver.h"
#include "osif.h" 


/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @cond DRIVER_INTERNAL_USE_ONLY */

/* Number of attempts for flash commands */
#define FLASH_MX25L6433F_MAX_RETRY  1U
/* Wrap-around value for timeout */
#define FLASH_MX25L6433F_TIMEOUT_WRAP  0xFFFFFFFFU
/* Timeout for synchronous operations  */
#define FLASH_MX25L6433F_TIMEOUT  1U

/* LUT entries used for various command sequences */
#define FLASH_MX25L6433F_LUT_4READ        0U     /* Quad read                    */
#define FLASH_MX25L6433F_LUT_WREN         1U     /* Write enable                 */
#define FLASH_MX25L6433F_LUT_RDSR         2U     /* Read status register         */
#define FLASH_MX25L6433F_LUT_RDCR         3U     /* Read configuration register  */
#define FLASH_MX25L6433F_LUT_WRSR         4U     /* Write status register        */
#define FLASH_MX25L6433F_LUT_4PP          5U     /* Quad program                 */
#define FLASH_MX25L6433F_LUT_SE           6U     /* Sector erase                 */
#define FLASH_MX25L6433F_LUT_BE32K        7U     /* Block erase (32 Kb)          */
#define FLASH_MX25L6433F_LUT_BE           8U     /* Block erase (64 Kb)          */
#define FLASH_MX25L6433F_LUT_CE           9U     /* Chip erase                   */
#define FLASH_MX25L6433F_LUT_PESUS        10U    /* Suspend program/erase        */
#define FLASH_MX25L6433F_LUT_PERES        11U    /* Resume program/erase         */
#define FLASH_MX25L6433F_LUT_WRSCUR       12U    /* Write security register      */
#define FLASH_MX25L6433F_LUT_RDSCUR       13U    /* Read security register       */

#define FLASH_MX25L6433F_LUT_OTHER        15U    /* Shared LUT for less common commands */


/* Macros for serial flash status bits manipulation */
#define FLASH_MX25L6433F_SR_WIP_MASK       0x1u
#define FLASH_MX25L6433F_SR_WIP_SHIFT      0u
#define FLASH_MX25L6433F_SR_WIP(x)         (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_SR_WIP_SHIFT))&FLASH_MX25L6433F_SR_WIP_MASK)

#define FLASH_MX25L6433F_SR_WEL_MASK       0x2u
#define FLASH_MX25L6433F_SR_WEL_SHIFT      1u
#define FLASH_MX25L6433F_SR_WEL(x)         (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_SR_WEL_SHIFT))&FLASH_MX25L6433F_SR_WEL_MASK)

#define FLASH_MX25L6433F_SR_WIP_WEL_MASK   (FLASH_MX25L6433F_SR_WIP_MASK | FLASH_MX25L6433F_SR_WEL_MASK)

#define FLASH_MX25L6433F_SR_BP_MASK        0x3Cu
#define FLASH_MX25L6433F_SR_BP_SHIFT       2u
#define FLASH_MX25L6433F_SR_BP(x)          (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_SR_BP_SHIFT))&FLASH_MX25L6433F_SR_BP_MASK)

#define FLASH_MX25L6433F_SR_QE_MASK        0x40u
#define FLASH_MX25L6433F_SR_QE_SHIFT       6u
#define FLASH_MX25L6433F_SR_QE(x)          (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_SR_QE_SHIFT))&FLASH_MX25L6433F_SR_QE_MASK)

#define FLASH_MX25L6433F_SR_SRWD_MASK      0x80u
#define FLASH_MX25L6433F_SR_SRWD_SHIFT     7u
#define FLASH_MX25L6433F_SR_SRWD(x)        (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_SR_SRWD_SHIFT))&FLASH_MX25L6433F_SR_SRWD_MASK)

/* Macros for serial flash configuration bits manipulation */
#define FLASH_MX25L6433F_CFG_ODS_MASK      0x1u
#define FLASH_MX25L6433F_CFG_ODS_SHIFT     0u
#define FLASH_MX25L6433F_CFG_ODS(x)        (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_CFG_ODS_SHIFT))&FLASH_MX25L6433F_CFG_ODS_MASK)

#define FLASH_MX25L6433F_CFG_TB_MASK       0x8u
#define FLASH_MX25L6433F_CFG_TB_SHIFT      3u
#define FLASH_MX25L6433F_CFG_TB(x)         (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_CFG_TB_SHIFT))&FLASH_MX25L6433F_CFG_TB_MASK)

#define FLASH_MX25L6433F_CFG_DC_MASK       0x40u
#define FLASH_MX25L6433F_CFG_DC_SHIFT      6u
#define FLASH_MX25L6433F_CFG_DC(x)         (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_CFG_DC_SHIFT))&FLASH_MX25L6433F_CFG_DC_MASK)

/* Macros for serial flash security bits manipulation */
#define FLASH_MX25L6433F_SEC_OTP_MASK      0x1u
#define FLASH_MX25L6433F_SEC_OTP_SHIFT     0u
#define FLASH_MX25L6433F_SEC_OTP(x)        (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_SEC_OTP_SHIFT))&FLASH_MX25L6433F_SEC_OTP_MASK)

#define FLASH_MX25L6433F_SEC_LDSO_MASK      0x2u
#define FLASH_MX25L6433F_SEC_LDSO_SHIFT     1u
#define FLASH_MX25L6433F_SEC_LDSO(x)        (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_SEC_LDSO_SHIFT))&FLASH_MX25L6433F_SEC_LDSO_MASK)

#define FLASH_MX25L6433F_SEC_PSB_MASK      0x4u
#define FLASH_MX25L6433F_SEC_PSB_SHIFT     2u
#define FLASH_MX25L6433F_SEC_PSB(x)        (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_SEC_PSB_SHIFT))&FLASH_MX25L6433F_SEC_PSB_MASK)

#define FLASH_MX25L6433F_SEC_ESB_MASK      0x8u
#define FLASH_MX25L6433F_SEC_ESB_SHIFT     3u
#define FLASH_MX25L6433F_SEC_ESB(x)        (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_SEC_ESB_SHIFT))&FLASH_MX25L6433F_SEC_ESB_MASK)

#define FLASH_MX25L6433F_SEC_P_FAIL_MASK      0x20u
#define FLASH_MX25L6433F_SEC_P_FAIL_SHIFT     5u
#define FLASH_MX25L6433F_SEC_P_FAIL(x)        (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_SEC_P_FAIL_SHIFT))&FLASH_MX25L6433F_SEC_P_FAIL_MASK)

#define FLASH_MX25L6433F_SEC_E_FAIL_MASK      0x40u
#define FLASH_MX25L6433F_SEC_E_FAIL_SHIFT     6u
#define FLASH_MX25L6433F_SEC_E_FAIL(x)        (((uint32_t)(((uint32_t)(x))<<FLASH_MX25L6433F_SEC_E_FAIL_SHIFT))&FLASH_MX25L6433F_SEC_E_FAIL_MASK)


 /* MX25L6433F commands */
typedef enum
{
    FLASH_MX25L6433F_READ        = 0x03U,    /*!<  Normal read                                 */
    FLASH_MX25L6433F_FAST_READ   = 0x0BU,    /*!<  Fast read                                   */
    FLASH_MX25L6433F_2READ       = 0xBBU,    /*!<  2 x I/O read                                */
    FLASH_MX25L6433F_DREAD       = 0x3BU,    /*!<  1I / 2O read                                */
    FLASH_MX25L6433F_4READ       = 0xEBU,    /*!<  4 x I/O read                                */
    FLASH_MX25L6433F_QREAD       = 0x6BU,    /*!<  1I / 4O read                                */
    FLASH_MX25L6433F_WREN        = 0x06U,    /*!<  Write enable                                */
    FLASH_MX25L6433F_WRDI        = 0x04U,    /*!<  Write disable                               */
    FLASH_MX25L6433F_RDSR        = 0x05U,    /*!<  Read status register                        */
    FLASH_MX25L6433F_RDCR        = 0x15U,    /*!<  Read configuration register                 */
    FLASH_MX25L6433F_WRSR        = 0x01U,    /*!<  Write status/configuration register         */
    FLASH_MX25L6433F_4PP         = 0x38U,    /*!<  Quad page program                           */
    FLASH_MX25L6433F_SE          = 0x20U,    /*!<  Sector erase                                */
    FLASH_MX25L6433F_BE32K       = 0x52U,    /*!<  Block erase 32KB                            */
    FLASH_MX25L6433F_BE          = 0xD8U,    /*!<  Block erase 64KB                            */
    FLASH_MX25L6433F_CE          = 0x60U,    /*!<  Chip erase                                  */
    FLASH_MX25L6433F_PP          = 0x02U,    /*!<  Page program                                */
    FLASH_MX25L6433F_DP          = 0xB9U,    /*!<  Deep power down                             */
    FLASH_MX25L6433F_RDP         = 0xABU,    /*!<  Release from deep power down                */
    FLASH_MX25L6433F_PESUS       = 0x75U,    /*!<  Program/Erase Suspend                       */
    FLASH_MX25L6433F_PERES       = 0x7AU,    /*!<  Program/Erase Resume                        */
    FLASH_MX25L6433F_RDID        = 0x9FU,    /*!<  Read identification                         */
    FLASH_MX25L6433F_RES         = 0xABU,    /*!<  Read electronic ID                          */
    FLASH_MX25L6433F_REMS        = 0x90U,    /*!<  Read electronic manufacturer & device ID    */
    FLASH_MX25L6433F_ENSO        = 0xB1U,    /*!<  Enter secured OTP                           */
    FLASH_MX25L6433F_EXSO        = 0xC1U,    /*!<  Exit secured OTP                            */
    FLASH_MX25L6433F_WRSCUR      = 0x2FU,    /*!<  Write security register                     */
    FLASH_MX25L6433F_RDSCUR      = 0x2BU,    /*!<  Read security register                      */
    FLASH_MX25L6433F_RSTEN       = 0x66U,    /*!<  Reset Enable                                */
    FLASH_MX25L6433F_RST         = 0x99U,    /*!<  Reset Memory                                */
    FLASH_MX25L6433F_RDSFDP      = 0x5AU,    /*!<  Read SFDP Mode                              */
    FLASH_MX25L6433F_SBL         = 0xC0U,    /*!<  Set Burst Length                            */
    FLASH_MX25L6433F_NOP         = 0x00U,    /*!<  No Operation                                */
} flash_mx25l6433f_commands_t;


/* Values for lastCommand property  */
#define FLASH_MX25L6433F_LASTCMD_NONE   0  /* Last command does not require busy check in external memory */
#define FLASH_MX25L6433F_LASTCMD_WRITE  1  /* Last command was a write */
#define FLASH_MX25L6433F_LASTCMD_ERASE  2  /* Last command was an erase */
#define FLASH_MX25L6433F_LASTCMD_OTHER  3  /* Other command which requires busy check */


/* Pointer to runtime state structures */
static flash_mx25l6433f_state_t * g_flashMx25l6433fStatePtr[QuadSPI_INSTANCE_COUNT] = {NULL};


/*******************************************************************************
 * Private Functions
 ******************************************************************************/


 /*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_Timeout
 * Description   : Checks for timeout condition
 *
 *END**************************************************************************/
static bool FLASH_MX25L6433F_DRV_Timeout(uint32_t startTime, uint32_t timeout)
{
    uint32_t currentTime;
    bool retVal;

    currentTime = OSIF_GetMilliseconds();
    if (currentTime >= startTime)
    {
        retVal = ((currentTime - startTime) > timeout)?true:false;
    }
    else
    {
        /* wrap around */
        retVal = ((FLASH_MX25L6433F_TIMEOUT_WRAP - startTime + currentTime) > timeout)?true:false;
    }
    return retVal;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_WriteEnable
 * Description   : Enables the serial flash memory for a program or erase operation
 *
 *END**************************************************************************/
static status_t FLASH_MX25L6433F_DRV_WriteEnable(uint32_t instance)
{
    uint32_t retries = FLASH_MX25L6433F_MAX_RETRY + 1U;
    status_t status = STATUS_TIMEOUT;
    status_t cmdStatus;
    uint8_t srValue = 0U;

    while (retries > 0U)
    {
        /* send WREN command */
        cmdStatus = QSPI_DRV_IpCommand(instance, FLASH_MX25L6433F_LUT_WREN, 1U);
        /* send RDSR command */
        if (cmdStatus == STATUS_SUCCESS)
        {
            cmdStatus = QSPI_DRV_IpRead(instance, FLASH_MX25L6433F_LUT_RDSR, 0U, &srValue, NULL, 1U, QSPI_TRANSFER_TYPE_SYNC, FLASH_MX25L6433F_TIMEOUT);
        }
        if (cmdStatus == STATUS_SUCCESS)
        {
            /* check WEL == 1 */
            if (srValue & FLASH_MX25L6433F_SR_WEL_MASK)
            {
                status = STATUS_SUCCESS;
                break;
            }
        }
        else
        {
            /* record error */
            status = cmdStatus;
        }
        retries--;
    }

    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_CheckLastCommand
 * Description   : Checks the outcome of the last program/erase operation
 *
 *END**************************************************************************/
static status_t FLASH_MX25L6433F_DRV_CheckLastCommand(uint32_t instance)
{
    uint8_t secValue;
    status_t status = STATUS_ERROR;
    flash_mx25l6433f_state_t * state;

    state = g_flashMx25l6433fStatePtr[instance];

    switch ((flash_mx25l6433f_commands_t)(state->lastCommand))
    {
        case FLASH_MX25L6433F_LASTCMD_WRITE:
            /* check success of program command */
            /* read security register */
            QSPI_DRV_IpRead(instance, FLASH_MX25L6433F_LUT_RDSCUR, 0U, &secValue, NULL, 1U, QSPI_TRANSFER_TYPE_SYNC, FLASH_MX25L6433F_TIMEOUT);
            if (secValue & FLASH_MX25L6433F_SEC_P_FAIL_MASK)
            {
                status = STATUS_ERROR;
            }
            else
            {
                status = STATUS_SUCCESS;
            }
            break;
        case FLASH_MX25L6433F_LASTCMD_ERASE:
            /* check success of erase command */
            /* read security register */
            QSPI_DRV_IpRead(instance, FLASH_MX25L6433F_LUT_RDSCUR, 0U, &secValue, NULL, 1U, QSPI_TRANSFER_TYPE_SYNC, FLASH_MX25L6433F_TIMEOUT);
            if (secValue & FLASH_MX25L6433F_SEC_E_FAIL_MASK)
            {
                status = STATUS_ERROR;
            }
            else
            {
                status = STATUS_SUCCESS;
            }
            break;
        case FLASH_MX25L6433F_LASTCMD_OTHER:
        default:
            status = STATUS_SUCCESS;
            break;
    }
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_InitLut
 * Description   : Initializes LUT sequences for MX25L6433F commands
 *
 *END**************************************************************************/
static inline void FLASH_MX25L6433F_DRV_InitLut(uint32_t instance)
{
/* 4 x Read Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_4READ),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_4READ,  /* 4READ command on 1 data line */
                    QSPI_LUT_CMD_ADDR, QSPI_LUT_PADS_4, 24U);                   /* 24-bit address on 4 data lines */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_4READ) + 1U,
                    QSPI_LUT_CMD_MODE, QSPI_LUT_PADS_4, 0xFFU,                  /* Performance enhance indicator: 0xFF */
                    QSPI_LUT_CMD_DUMMY, QSPI_LUT_PADS_4, 4U);                   /* 4 Dummy cycles (for a total of 6) */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_4READ) + 2U,
                    QSPI_LUT_CMD_READ, QSPI_LUT_PADS_4, 0x10U,                  /* Read data on 4 data lines */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);                    /* End of sequence */

/* WREN Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_WREN),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_WREN,   /* WREN command on 1 data line */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);                    /* End of sequence */

/* WRSR Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_WRSR),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_WRSR,   /* WRSR command on 1 data line */
                    QSPI_LUT_CMD_WRITE, QSPI_LUT_PADS_1, 2U);                   /* Send data on 1 data line */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_WRSR) + 1U,
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U,                     /* End of sequence */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);

/* RDSR Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_RDSR),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_RDSR,   /* RDSR command on 1 data line */
                    QSPI_LUT_CMD_READ, QSPI_LUT_PADS_1, 1U);                    /* Read data on 1 data line */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_RDSR) + 1U,
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U,                     /* End of sequence */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);

/* RDCR Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_RDCR),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_RDCR,   /* RDCR command on 1 data line */
                    QSPI_LUT_CMD_READ, QSPI_LUT_PADS_1, 1U);                    /* Read data on 1 data line */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_RDCR) + 1U,
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U,                     /* End of sequence */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);

/* 4PP Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_4PP),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_4PP,    /* 4PP command on 1 data line */
                    QSPI_LUT_CMD_ADDR, QSPI_LUT_PADS_4, 24U);                   /* 24-bit address on 4 data lines */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_4PP) + 1U,
                    QSPI_LUT_CMD_WRITE, QSPI_LUT_PADS_4, 0x10U,                 /* Send data on 4 data lines */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);

/* SE Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_SE),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_SE,     /* SE command on 1 data line */
                    QSPI_LUT_CMD_ADDR, QSPI_LUT_PADS_1, 24U);                   /* 24-bit address on 1 data line */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_SE) + 1U,
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U,                     /* End of sequence */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);

/* BE Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_BE),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_BE,     /* BE command on 1 data line */
                    QSPI_LUT_CMD_ADDR, QSPI_LUT_PADS_1, 24U);                   /* 24-bit address on 1 data line */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_BE) + 1U,
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U,                     /* End of sequence */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);

/* BE32K Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_BE32K),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_BE32K,  /* BE32K command on 1 data line */
                    QSPI_LUT_CMD_ADDR, QSPI_LUT_PADS_1, 24U);                   /* 24-bit address on 1 data line */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_BE32K) + 1U,
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U,                     /* End of sequence */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);

/* CE Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_CE),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_CE,     /* CE command on 1 data line */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);                    /* End of sequence */

/* PESUS Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_PESUS),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_PESUS,  /* PESUS command on 1 data line */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);                    /* End of sequence */

/* PERES Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_PERES),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_PERES,  /* PERES command on 1 data line */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);                    /* End of sequence */

/* WRSCUR Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_WRSCUR),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_WRSCUR, /* WRSCUR command on 1 data line */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);                    /* End of sequence */

/* RDSCUR Sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_RDSCUR),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, FLASH_MX25L6433F_RDSCUR, /* RDSCUR command on 1 data line */
                    QSPI_LUT_CMD_READ, QSPI_LUT_PADS_1, 1U);                    /* Read data on 1 data line */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_RDSCUR) + 1U,
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U,                     /* End of sequence */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_InitSharedLut
 * Description   : Programs the sequence for ENSO command in the last LUT sequence
 *
 *END**************************************************************************/
static inline void FLASH_MX25L6433F_DRV_InitSharedLut(uint32_t instance,
                                                      flash_mx25l6433f_commands_t command)
{
/* Program simple command sequence */
    QSPI_DRV_SetLut(instance, (4U * FLASH_MX25L6433F_LUT_OTHER),
                    QSPI_LUT_CMD_CMD, QSPI_LUT_PADS_1, command,   /* command on 1 data line */
                    QSPI_LUT_CMD_STOP, QSPI_LUT_PADS_1, 0U);      /* End of sequence */
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_InitDevice
 * Description   : Initializes the MX25L6433F device
 *
 *END**************************************************************************/
static inline status_t FLASH_MX25L6433F_DRV_InitDevice(uint32_t instance,
                                                       const flash_mx25l6433f_user_config_t * userConfigPtr)
{
    uint8_t srValue;
    uint8_t crValue;
    uint8_t srWrite[2U];
    status_t status;
    uint32_t startTime;

    /* Enable quad operations, configure drive strength, configure dummy cycles to 0 (<80MHz) */
    /* send RDSR command */
    status = QSPI_DRV_IpRead(instance, FLASH_MX25L6433F_LUT_RDSR, 0U, &srValue, NULL, 1U, QSPI_TRANSFER_TYPE_SYNC, FLASH_MX25L6433F_TIMEOUT);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* send RDCR command */
    status = QSPI_DRV_IpRead(instance, FLASH_MX25L6433F_LUT_RDCR, 0U, &crValue, NULL, 1U, QSPI_TRANSFER_TYPE_SYNC, FLASH_MX25L6433F_TIMEOUT);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* Check if current settings are ok */
    if (((srValue & FLASH_MX25L6433F_SR_QE_MASK) == 0U) ||
        (((crValue & FLASH_MX25L6433F_CFG_ODS_MASK) >> FLASH_MX25L6433F_CFG_ODS_SHIFT) != userConfigPtr->outputDriverStrength) ||
        ((crValue & FLASH_MX25L6433F_CFG_DC_MASK) != 0U))
    {
        /* enable write before WRSR command */
        status = FLASH_MX25L6433F_DRV_WriteEnable(instance);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
        /* QE not set, write SR to set it */
        srWrite[0U] = (uint8_t)(srValue | FLASH_MX25L6433F_SR_QE_MASK);
        crValue &= (uint8_t)(~(FLASH_MX25L6433F_CFG_ODS_MASK | FLASH_MX25L6433F_CFG_DC_MASK));
        srWrite[1U] = (uint8_t)(crValue | FLASH_MX25L6433F_CFG_ODS(userConfigPtr->outputDriverStrength));
        status = QSPI_DRV_IpWrite(instance, FLASH_MX25L6433F_LUT_WRSR, 0U, srWrite, 2U, QSPI_TRANSFER_TYPE_SYNC, FLASH_MX25L6433F_TIMEOUT);
        if (status != STATUS_SUCCESS)
        {
            return status;
        }
        /* Wait for the command to complete. */
        startTime = OSIF_GetMilliseconds();
        do
        {
            status = FLASH_MX25L6433F_DRV_GetStatus(instance);
        }
        while ((status == STATUS_BUSY) && !FLASH_MX25L6433F_DRV_Timeout(startTime, FLASH_MX25L6433F_TIMEOUT));
        if (status == STATUS_BUSY)
        {
            status = STATUS_TIMEOUT;
        }
    }
    return status;
}

/*! @endcond */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_Init
 * Description   : Initialize the serial flash memory driver
 * Implements : FLASH_MX25L6433F_DRV_Init_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_Init(uint32_t instance,
                                   const flash_mx25l6433f_user_config_t * userConfigPtr,
                                   flash_mx25l6433f_state_t * state)
{
    status_t status;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);
    DEV_ASSERT(g_flashMx25l6433fStatePtr[instance] == NULL);

    /* Copy configuration parameters to state structure */
    state->outputDriverStrength = userConfigPtr->outputDriverStrength;
    state->dmaSupport = userConfigPtr->dmaSupport;
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_NONE;
    state->suspendedCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_NONE;
    /* QuadSPI is assumed to be already initialized by the application */
    /* Initialize LUT */
    FLASH_MX25L6433F_DRV_InitLut(instance);
    /* AHB Setup */
    QSPI_DRV_SetAhbSeqId(instance, FLASH_MX25L6433F_LUT_4READ);

    /* Initialize device */
    status = FLASH_MX25L6433F_DRV_InitDevice(instance, userConfigPtr);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }

    g_flashMx25l6433fStatePtr[instance] = state;

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_Deinit
 * Description   : De-initialize the MX25L6433F flash driver
 * Implements : FLASH_MX25L6433F_DRV_Deinit_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_Deinit(uint32_t instance)
{
    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    g_flashMx25l6433fStatePtr[instance] = NULL;
    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_SetProtection
 * Description   : Configure protected area of the device
 * Implements : FLASH_MX25L6433F_DRV_SetProtection_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_SetProtection(uint32_t instance,
                                            flash_mx25l6433f_prot_dir_t direction,
                                            flash_mx25l6433f_prot_size_t size)
{
    uint8_t srWrite[2U];    /* status register value / configuration register value */
    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);
    status_t status;
    flash_mx25l6433f_state_t * state;

    state = g_flashMx25l6433fStatePtr[instance];

    /* enable write before WRSR command */
    status = FLASH_MX25L6433F_DRV_WriteEnable(instance);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* prepare new SR value; keep QE bit set */
    srWrite[0U] = (uint8_t)(FLASH_MX25L6433F_SR_BP(size) | FLASH_MX25L6433F_SR_QE_MASK);
    /* prepare new CR value; preserve ODS setting */
    srWrite[1U]= (uint8_t)(FLASH_MX25L6433F_CFG_TB(direction) | FLASH_MX25L6433F_CFG_ODS(state->outputDriverStrength));
    /* write protection bits to SR */
    status = QSPI_DRV_IpWrite(instance, FLASH_MX25L6433F_LUT_WRSR, 0U, srWrite, 2U, QSPI_TRANSFER_TYPE_SYNC, FLASH_MX25L6433F_TIMEOUT);
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_OTHER;

    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_GetProtection
 * Description   : Get protected area of the device
 * Implements : FLASH_MX25L6433F_DRV_GetProtection_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_GetProtection(uint32_t instance,
                                            flash_mx25l6433f_prot_dir_t * direction,
                                            flash_mx25l6433f_prot_size_t * size)
{
    uint8_t srValue;
    uint8_t crValue;
    status_t status;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_NONE;
    /* send RDSR command */
    status = QSPI_DRV_IpRead(instance, FLASH_MX25L6433F_LUT_RDSR, 0U, &srValue, NULL, 1U, QSPI_TRANSFER_TYPE_SYNC, FLASH_MX25L6433F_TIMEOUT);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* send RDCR command */
    status = QSPI_DRV_IpRead(instance, FLASH_MX25L6433F_LUT_RDCR, 0U, &crValue, NULL, 1U, QSPI_TRANSFER_TYPE_SYNC, FLASH_MX25L6433F_TIMEOUT);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* compute protection settings */
    srValue = (uint8_t)((srValue & FLASH_MX25L6433F_SR_BP_MASK) >> FLASH_MX25L6433F_SR_BP_SHIFT);
    *size = (flash_mx25l6433f_prot_size_t)srValue;
    crValue = (uint8_t)((crValue & FLASH_MX25L6433F_CFG_TB_MASK) >> FLASH_MX25L6433F_CFG_TB_SHIFT);
    *direction = (flash_mx25l6433f_prot_dir_t)crValue;

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_SetSecureLock
 * Description   : Locks the customer sector of the secured OTP area
 * Implements : FLASH_MX25L6433F_DRV_SetSecureLock_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_SetSecureLock(uint32_t instance)
{
    status_t status;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* enable write before programming */
    status = FLASH_MX25L6433F_DRV_WriteEnable(instance);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* send WRSCUR command */
    status = QSPI_DRV_IpCommand(instance, FLASH_MX25L6433F_LUT_WRSCUR, 1U);
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_OTHER;

    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_GetSecureLock
 * Description   : Get lock status of the secured OTP area
 * Implements : FLASH_MX25L6433F_DRV_GetSecureLock_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_GetSecureLock(uint32_t instance,
                                            flash_mx25l6433f_secure_lock_t *lock)
{
    status_t status;
    uint8_t secValue;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);
    state = g_flashMx25l6433fStatePtr[instance];
    /* Send RDCR command */
    status = QSPI_DRV_IpRead(instance, FLASH_MX25L6433F_LUT_RDSCUR, 0U, &secValue, NULL, 1U, QSPI_TRANSFER_TYPE_SYNC, FLASH_MX25L6433F_TIMEOUT);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* Extract lock indicators */
    lock->userAreaLock = (bool)((secValue & FLASH_MX25L6433F_SEC_LDSO_MASK) >> FLASH_MX25L6433F_SEC_LDSO_SHIFT);
    lock->factoryAreaLock = (bool)((secValue & FLASH_MX25L6433F_SEC_OTP_MASK) >> FLASH_MX25L6433F_SEC_OTP_SHIFT);
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_NONE;
    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_Read
 * Description   : Read data from serial flash
 * Implements : FLASH_MX25L6433F_DRV_Read_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_Read(uint32_t instance,
                                   uint32_t address,
                                   uint8_t * data,
                                   uint32_t size)
{
    flash_mx25l6433f_state_t * state;
    qspi_transfer_type_t transferType;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    if (state->dmaSupport)
    {
        transferType = QSPI_TRANSFER_TYPE_ASYNC_DMA;
    }
    else
    {
        transferType = QSPI_TRANSFER_TYPE_ASYNC_INT;
    }
    /* launch read command and return */
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_NONE;
    return QSPI_DRV_IpRead(instance, FLASH_MX25L6433F_LUT_4READ, address, data, NULL, size, transferType, 1U);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_Erase4K
 * Description   : Erase a 4k sector in the serial flash
 * Implements : FLASH_MX25L6433F_DRV_Erase4K_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_Erase4K(uint32_t instance, uint32_t address)
{
    status_t status;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);
    state = g_flashMx25l6433fStatePtr[instance];

    /* enable write before erasing */
    status = FLASH_MX25L6433F_DRV_WriteEnable(instance);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* launch erase command and return */
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_ERASE;
    return QSPI_DRV_IpErase(instance, FLASH_MX25L6433F_LUT_SE, address);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_Erase32K
 * Description   : Erase a 32k block in the serial flash
 * Implements : FLASH_MX25L6433F_DRV_Erase32K_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_Erase32K(uint32_t instance, uint32_t address)
{
    status_t status;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* enable write before erasing */
    status = FLASH_MX25L6433F_DRV_WriteEnable(instance);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* launch erase command and return */
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_ERASE;
    return QSPI_DRV_IpErase(instance, FLASH_MX25L6433F_LUT_BE32K, address);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_Erase64K
 * Description   : Erase a 64k block in the serial flash
 * Implements : FLASH_MX25L6433F_DRV_Erase64K_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_Erase64K(uint32_t instance, uint32_t address)
{
    status_t status;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* enable write before erasing */
    status = FLASH_MX25L6433F_DRV_WriteEnable(instance);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* launch erase command and return */
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_ERASE;
    return QSPI_DRV_IpErase(instance, FLASH_MX25L6433F_LUT_BE, address);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_EraseAll
 * Description   : Erases the entire serial flash
 * Implements : FLASH_MX25L6433F_DRV_EraseAll_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_EraseAll(uint32_t instance)
{
    status_t status;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* enable write before erasing */
    status = FLASH_MX25L6433F_DRV_WriteEnable(instance);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* launch erase command and return */
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_ERASE;
    return QSPI_DRV_IpCommand(instance, FLASH_MX25L6433F_LUT_CE, 1U);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_EraseVerify
 * Description   : Checks whether or not an area in the serial flash is erased
 * Implements : FLASH_MX25L6433F_DRV_EraseVerify_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_EraseVerify(uint32_t instance, uint32_t address, uint32_t size)
{
    qspi_transfer_type_t transferType;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* not possible to use DMA for blank check */
    transferType = QSPI_TRANSFER_TYPE_ASYNC_INT;
    /* launch read command and return */
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_NONE;
    return QSPI_DRV_IpRead(instance, FLASH_MX25L6433F_LUT_4READ, address, NULL, NULL, size, transferType, 1U);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_Program
 * Description   : Writes data in serial flash
 * Implements : FLASH_MX25L6433F_DRV_Program_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_Program(uint32_t instance,
                                      uint32_t address,
                                      uint8_t * data,
                                      uint32_t size)
{
    status_t status;
    qspi_transfer_type_t transferType;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* enable write before programming */
    status = FLASH_MX25L6433F_DRV_WriteEnable(instance);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    if (state->dmaSupport)
    {
        transferType = QSPI_TRANSFER_TYPE_ASYNC_DMA;
    }
    else
    {
        transferType = QSPI_TRANSFER_TYPE_ASYNC_INT;
    }
    /* launch program command and return */
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_WRITE;
    return QSPI_DRV_IpWrite(instance, FLASH_MX25L6433F_LUT_4PP, address, data, size, transferType, 1U);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_ProgramVerify
 * Description   : Verifies the correctness of the programmed data
 * Implements : FLASH_MX25L6433F_DRV_ProgramVerify_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_ProgramVerify(uint32_t instance,
                                            uint32_t address,
                                            const uint8_t * data,
                                            uint32_t size)
{
    qspi_transfer_type_t transferType;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* not possible to use DMA for blank check */
    transferType = QSPI_TRANSFER_TYPE_ASYNC_INT;
    /* launch read command and return */
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_NONE;
    return QSPI_DRV_IpRead(instance, FLASH_MX25L6433F_LUT_4READ, address, NULL, data, size, transferType, 1U);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_GetStatus
 * Description   : Get the status of the last operation
 * Implements : FLASH_MX25L6433F_DRV_GetStatus_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_GetStatus(uint32_t instance)
{
    status_t status;
    uint8_t srValueRead;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* Check if the QuadSPI command is complete */
    status = QSPI_DRV_IpGetStatus(instance);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* Check if the operation has finished in the serial flash */
    if (state->lastCommand != FLASH_MX25L6433F_LASTCMD_NONE)
    {
        /* Send RDSR command */
        QSPI_DRV_IpRead(instance, FLASH_MX25L6433F_LUT_RDSR, 0U, &srValueRead, NULL, 1U, QSPI_TRANSFER_TYPE_SYNC, FLASH_MX25L6433F_TIMEOUT);
        /* Check WIP == 0, WEL == 0 */
        if ((srValueRead & FLASH_MX25L6433F_SR_WIP_WEL_MASK) != 0U)
        {
            /* Write/erase in progress */
            return STATUS_BUSY;
        }
        /* Check operation result */
        status = FLASH_MX25L6433F_DRV_CheckLastCommand(instance);
    }
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_Reset
 * Description   : Reset the serial flash device
 * Implements : FLASH_MX25L6433F_DRV_Reset_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_Reset(uint32_t instance)
{
    status_t status;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* Prepare RSTEN sequence in the shared LUT entry */
    FLASH_MX25L6433F_DRV_InitSharedLut(instance, FLASH_MX25L6433F_RSTEN);
    /* send RSTEN command */
    status = QSPI_DRV_IpCommand(instance, FLASH_MX25L6433F_LUT_OTHER, 1U);
    if (status != STATUS_SUCCESS)
    {
        return status;
    }
    /* Prepare RSTEN sequence in the shared LUT entry */
    FLASH_MX25L6433F_DRV_InitSharedLut(instance, FLASH_MX25L6433F_RST);
    /* send RST command */
    status = QSPI_DRV_IpCommand(instance, FLASH_MX25L6433F_LUT_OTHER, 1U);
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_NONE;
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_EnterOTP
 * Description   : Enters OTP mode
 * Implements : FLASH_MX25L6433F_DRV_EnterOTP_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_EnterOTP(uint32_t instance)
{
    status_t status;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* Prepare ENSO sequence in the shared LUT entry */
    FLASH_MX25L6433F_DRV_InitSharedLut(instance, FLASH_MX25L6433F_ENSO);
    /* send ENSO command */
    status = QSPI_DRV_IpCommand(instance, FLASH_MX25L6433F_LUT_OTHER, 1U);
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_NONE;
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_ExitOTP
 * Description   : Exits OTP mode
 * Implements : FLASH_MX25L6433F_DRV_ExitOTP_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_ExitOTP(uint32_t instance)
{
    status_t status;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* Prepare EXSO sequence in the shared LUT entry */
    FLASH_MX25L6433F_DRV_InitSharedLut(instance, FLASH_MX25L6433F_EXSO);
    /* send EXSO command */
    status = QSPI_DRV_IpCommand(instance, FLASH_MX25L6433F_LUT_OTHER, 1U);
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_NONE;
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_EnterDPD
 * Description   : Enters Deep Power Down mode
 * Implements : FLASH_MX25L6433F_DRV_EnterDPD_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_EnterDPD(uint32_t instance)
{
    status_t status;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* Prepare DP sequence in the shared LUT entry */
    FLASH_MX25L6433F_DRV_InitSharedLut(instance, FLASH_MX25L6433F_DP);
    /* send DP command */
    status = QSPI_DRV_IpCommand(instance, FLASH_MX25L6433F_LUT_OTHER, 1U);
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_NONE;
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_ExitDPD
 * Description   : Exits Deep Power Down mode
 * Implements : FLASH_MX25L6433F_DRV_ExitDPD_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_ExitDPD(uint32_t instance)
{
    status_t status;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* Prepare RDP sequence in the shared LUT entry */
    FLASH_MX25L6433F_DRV_InitSharedLut(instance, FLASH_MX25L6433F_RDP);
    /* send RDP command */
    status = QSPI_DRV_IpCommand(instance, FLASH_MX25L6433F_LUT_OTHER, 1U);
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_NONE;
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_Suspend
 * Description   : Suspends current program or erase operation
 * Implements : FLASH_MX25L6433F_DRV_Suspend_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_Suspend(uint32_t instance)
{
    status_t status;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);
    state = g_flashMx25l6433fStatePtr[instance];
    DEV_ASSERT(state->suspendedCommand == FLASH_MX25L6433F_LASTCMD_NONE);
    DEV_ASSERT((state->lastCommand == FLASH_MX25L6433F_LASTCMD_WRITE) ||
               (state->lastCommand == FLASH_MX25L6433F_LASTCMD_ERASE));

    state->suspendedCommand = state->lastCommand;
    /* Send Suspend command */
    status = QSPI_DRV_IpCommand(instance, FLASH_MX25L6433F_LUT_PESUS, 1U);
    state->lastCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_OTHER;
    return status;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : FLASH_MX25L6433F_DRV_Resume
 * Description   : Resumes a suspended program or erase operation
 * Implements : FLASH_MX25L6433F_DRV_Resume_Activity
 *
 *END**************************************************************************/
status_t FLASH_MX25L6433F_DRV_Resume(uint32_t instance)
{
    status_t status;
    flash_mx25l6433f_state_t * state;

    DEV_ASSERT(instance < QuadSPI_INSTANCE_COUNT);

    state = g_flashMx25l6433fStatePtr[instance];
    /* Send RDP command */
    status = QSPI_DRV_IpCommand(instance, FLASH_MX25L6433F_LUT_PERES, 1U);
    /* Restore last command */
    state->lastCommand = state->suspendedCommand;
    state->suspendedCommand = (uint8_t)FLASH_MX25L6433F_LASTCMD_NONE;
    return status;
}


/*******************************************************************************
 * EOF
 ******************************************************************************/
