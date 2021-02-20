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

/*!
 * @file lpit_hw_access.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 */
#include <stddef.h>
#include "lpit_hw_access.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : LPIT_GetModuleInformation
 * Description   : Gets the information of LPIT module as: Major version number,
 * minor version number, feature number, number of external trigger inputs.
 *
 *END**************************************************************************/
void LPIT_GetModuleInformation(const LPIT_Type * base,
                               lpit_module_information_t * moduleInfomation)
{
    DEV_ASSERT(moduleInfomation != NULL);

    /* Gets value of information registers */
    moduleInfomation->majorVersionNumber = ((base->VERID) & LPIT_VERID_MAJOR_MASK) \
                                            >> LPIT_VERID_MAJOR_SHIFT;
    moduleInfomation->minorVersionNumber = ((base->VERID) & LPIT_VERID_MINOR_MASK) \
                                            >> LPIT_VERID_MINOR_SHIFT;
    moduleInfomation->featureNumber = ((base->VERID) & LPIT_VERID_FEATURE_MASK) \
                                            >> LPIT_VERID_FEATURE_SHIFT;
    moduleInfomation->numberOfExternalTriggerInputs = ((base->PARAM) & LPIT_PARAM_EXT_TRIG_MASK) \
                                            >> LPIT_PARAM_EXT_TRIG_SHIFT;
    moduleInfomation->numberOfTimerChannels = ((base->PARAM) & LPIT_PARAM_CHANNEL_MASK) \
                                            >> LPIT_PARAM_CHANNEL_SHIFT;
}
/*******************************************************************************
* EOF
******************************************************************************/
