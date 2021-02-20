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

#ifndef ADC_PAL_MAPPING_H
#define ADC_PAL_MAPPING_H
#include "adc_pal_cfg.h"


#if defined(ADC_PAL_S32K1xx)

/* Include peripheral drivers */
#include "adc_driver.h"
#include "adc_hw_access.h"
#include "trgmux_driver.h"
#include "pdb_driver.h"

#endif /* defined(ADC_PAL_S32K1xx) */

#if defined(ADC_PAL_MPC574x)

/* Include peripheral drivers */
#include "adc_c55_driver.h"
#include "bctu_driver.h"

#endif /* defined(ADC_PAL_MPC574x) */



#if defined(ADC_PAL_S32K1xx)

/* Define the available instances */
typedef enum
{
    ADC_PAL_INSTANCE_0 = 0U,
    ADC_PAL_INSTANCE_1 = 1U
} adc_pal_instance_t;

#define NUMBER_OF_ADC_PAL_INSTANCES 2U

#endif /* defined(ADC_PAL_S32K1xx) */


#if defined(ADC_PAL_MPC574x)

/* Define the available instances */
typedef enum
{
    ADC_PAL_INSTANCE_0
} adc_pal_instance_t;

#define NUMBER_OF_ADC_PAL_INSTANCES 1U

#endif /* defined(ADC_PAL_MPC574x) */


#endif /* ADC_PAL_MAPPING_H */
