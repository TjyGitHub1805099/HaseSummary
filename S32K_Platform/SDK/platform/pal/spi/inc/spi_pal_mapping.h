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

#ifndef PAL_mapping_H
#define PAL_mapping_H
#include "device_registers.h"
/*
 * Define the available instances for SPI functionality
 */
#if (defined(CPU_S32K144HFT0VLLT) || defined(CPU_S32K144LFT0MLLT) || defined(CPU_S32K146) || defined(CPU_S32K148))
/*
 * @brief Define instances for S32K144 and S32K148 (LPSPI and FlexIO)
 * Implements : spi_instance_t_Class
 */

typedef enum
{
    SPI_OVER_LPSPI00_INSTANCE = 0U,
    SPI_OVER_LPSPI01_INSTANCE = 1U,
    SPI_OVER_LPSPI02_INSTANCE = 2U,
    SPI_OVER_FLEXIO0_INSTANCE = 3U, /* This is a virtual SPI instance over FlexIO */
    SPI_OVER_FLEXIO1_INSTANCE = 4U, /* This is a virtual SPI instance over FlexIO */
} spi_instance_t;

/* On S32K144 and S32K148 are available 5 instances which supports SPI */
#define NUMBER_OF_SPI_PAL_INSTANCES 5U
/* Defines the index limits for all SPIs */
#define LPSPI_HIGH_INDEX 2U
#define FLEXIO_SPI_LOW_INDEX 3U
#define FLEXIO_SPI_HIGH_INDEX 4U
#endif

#if (defined(CPU_S32K142))
/*
 * @brief Define instances for S32K142 (LPSPI and FlexIO)
 * Implements : spi_instance_t_Class
 */

typedef enum
{
    SPI_OVER_LPSPI00_INSTANCE = 0U,
    SPI_OVER_LPSPI01_INSTANCE = 1U,
    SPI_OVER_FLEXIO_0_INSTANCE = 2U, /* This is a virtual SPI instance over FlexIO */
    SPI_OVER_FLEXIO_1_INSTANCE = 3U, /* This is a virtual SPI instance over FlexIO */
} spi_instance_t;

/* On S32K144 and S32K148 are available 5 instances which supports SPI */
#define NUMBER_OF_SPI_PAL_INSTANCES 4U
/* Defines the index limits for all SPIs */
#define LPSPI_HIGH_INDEX 1U
#define FLEXIO_SPI_LOW_INDEX 2U
#define FLEXIO_SPI_HIGH_INDEX 3U
#endif

#if (defined(CPU_MPC5748G))
/*
 * @brief Define instances for MPC5748G (DSPI)
 * Implements : spi_instance_t_Class
 */
typedef enum
{
    SPI_OVER_DSPI0_INSTANCE = 0U,
    SPI_OVER_DSPI1_INSTANCE = 1U,
    SPI_OVER_DSPI2_INSTANCE = 2U,
    SPI_OVER_DSPI3_INSTANCE = 3U,
    SPI_OVER_SPI0_INSTANCE = 4U,
    SPI_OVER_SPI1_INSTANCE = 5U,
    SPI_OVER_SPI2_INSTANCE = 6U,
    SPI_OVER_SPI3_INSTANCE = 7U,
    SPI_OVER_SPI4_INSTANCE = 8U,
    SPI_OVER_SPI5_INSTANCE = 9U,
} spi_instance_t;

/* On MPC5748G are available 10 instances which supports SPI */
#define NUMBER_OF_SPI_PAL_INSTANCES 10U
/* Defines the index limits for all SPIs */
#define DSPI_LOW_INDEX 0U
#define DSPI_HIGH_INDEX 9U
#endif

#if (defined(CPU_MPC5746C))
/*
 * @brief Define instances for MPC5746C (DSPI)
 * Implements : spi_instance_t_Class
 */
typedef enum
{
    SPI_OVER_DSPI0_INSTANCE = 0U,
    SPI_OVER_DSPI1_INSTANCE = 1U,
    SPI_OVER_DSPI2_INSTANCE = 2U,
    SPI_OVER_DSPI3_INSTANCE = 3U,
    SPI_OVER_SPI0_INSTANCE = 4U,
    SPI_OVER_SPI1_INSTANCE = 5U,
    SPI_OVER_SPI2_INSTANCE = 6U,
    SPI_OVER_SPI3_INSTANCE = 7U,
} spi_instance_t;

/* On MPC5746C are available 8 instances which supports SPI */
#define NUMBER_OF_SPI_PAL_INSTANCES 8U
/* Defines the index limits for all SPIs */
#define DSPI_LOW_INDEX 0U
#define DSPI_HIGH_INDEX 7U
#endif

#endif /* PAL_mapping_H */
