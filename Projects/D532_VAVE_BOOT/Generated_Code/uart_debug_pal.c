/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : uart_debug_pal.c
**     Project     : D532_MC_USB_SWDL
**     Processor   : S32K148_176
**     Component   : uart_pal
**     Version     : Component SDK_S32K14x_08, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K14x_08
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-02-20, 20:27, # CodeGen: 0
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
** @file uart_debug_pal.c
** @version 01.00
*/
/*!
**  @addtogroup uart_debug_pal_module uart_debug_pal module documentation
**  @{
*/
#include "uart_debug_pal.h"

/* Serial User Configurations */
uart_user_config_t uart_debug_pal_config = {
    .baudRate         = 256000U,
    .bitCount         = UART_8_BITS_PER_CHAR,
    .parityMode       = UART_PARITY_DISABLED,
    .stopBitCount     = UART_ONE_STOP_BIT,
    .transferType     = UART_USING_INTERRUPTS,
    .rxDMAChannel     = 0U,
    .txDMAChannel     = 0U,
    .rxCallback       = NULL,
    .rxCallbackParam  = NULL,
    .txCallback       = NULL,
    .txCallbackParam  = NULL,
    .extension        = NULL
};

/* END uart_debug_pal. */
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
