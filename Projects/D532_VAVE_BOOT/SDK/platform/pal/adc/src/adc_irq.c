/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#include "adc_pal_cfg.h"
#include "adc_irq.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Define default interrupt handlers for ADC instances.
 * Default interrupt handlers already declared in startup files. */

#if defined(ADC_PAL_S32K1xx)

void ADC0_IRQHandler(void)
{
    ADC_S32K1xx_IrqHandler(ADC_PAL_INSTANCE_0);
}

void ADC1_IRQHandler(void)
{
    ADC_S32K1xx_IrqHandler(ADC_PAL_INSTANCE_1);
}

#endif /* defined(ADC_PAL_S32K1xx) */


#if defined(ADC_PAL_MPC574x)

#endif /* defined(ADC_PAL_MPC574x) */


/*******************************************************************************
 * EOF
 ******************************************************************************/
