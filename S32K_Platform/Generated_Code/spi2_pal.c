/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : spi2_pal.c
**     Project     : S32K_146
**     Processor   : S32K146_144
**     Component   : spi_pal
**     Version     : Component SDK_S32K14x_08, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K14x_08
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-05-15, 15:28, # CodeGen: 60
**
**     Copyright 1997 - 2015 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     All Rights Reserved.
**     
**     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
**     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
**     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
**     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
**     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
**     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**     THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file spi2_pal.c
** @version 01.00
*/
/*!
**  @addtogroup spi2_pal_module spi2_pal module documentation
**  @{
*/
#include "spi2_pal.h"


/* SPI Master Configurations */

/* SPI Slave Configurations */
spi_slave_t spi2_pal_SlaveConfig0 =
{
    .ssPolarity     = SPI_ACTIVE_HIGH,
    .frameSize      = 8,
    .clockPhase     = READ_ON_ODD_EDGE,
    .clockPolarity  = SPI_ACTIVE_HIGH,
    .transferType   = SPI_USING_DMA,
    .rxDMAChannel   = 4U,
    .txDMAChannel   = 5U,
    .callback       = SpiIpcCall,
    .callbackParam  = NULL
};

/* END spi2_pal. */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/

