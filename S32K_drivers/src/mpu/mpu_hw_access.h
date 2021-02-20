/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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

/*!
 * @file mpu_hw_access.h
 */

#ifndef MPU_HW_ACCESS_H
#define MPU_HW_ACCESS_H

#include "mpu_driver.h"

/*!
 * @brief Memory Protection Unit Hardware Access layer.
 * @{
 */

/*******************************************************************************
 * API
 *******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Enables/Disables the MPU module.
 *
 * @param[in] base The MPU peripheral base address.
 * @param[in] enable State
 *            - true  : Enable the MPU module.
 *            - false : Disable the MPU module.
 */
static inline void MPU_Enable(MPU_Type * const base,
                              bool enable)
{
    base->CESR = (base->CESR & ~MPU_CESR_VLD_MASK) | MPU_CESR_VLD(enable ? 1UL : 0UL);
}

/*!
 * @brief Enables/Disables the region descriptor.
 *
 * @param[in] base The MPU peripheral base address.
 * @param[in] regionNum The region number.
 * @param[in] enable Valid state
 *            - true  : Region descriptor is valid.
 *            - false : Region descriptor is invalid.
 */
static inline void MPU_SetRegionValidCmd(MPU_Type * const base,
                                         uint8_t regionNum,
                                         bool enable)
{
    base->RGD[regionNum].WORD3 = ((base->RGD[regionNum].WORD3 & ~MPU_RGD_WORD3_VLD_MASK)
                               | MPU_RGD_WORD3_VLD(enable ? 1UL : 0UL));
}

#if FEATURE_MPU_HAS_PROCESS_IDENTIFIER
/*!
 * @brief Sets the process identifier.
 *
 * @param[in] base The MPU peripheral base address.
 * @param[in] regionNum The region number.
 * @param[in] pid The process identifier.
 */
static inline void MPU_SetProcessIdentifier(MPU_Type * const base,
                                            uint8_t regionNum,
                                            uint8_t pid)
{
    base->RGD[regionNum].WORD3 = (base->RGD[regionNum].WORD3 & ~MPU_RGD_WORD3_PID_MASK)
                               | MPU_RGD_WORD3_PID(pid);
}

/*!
 * @brief Sets the process identifier mask.
 *
 * @param[in] base The MPU peripheral base address.
 * @param[in] regionNum The region number.
 * @param[in] pidMask The process identifier mask.
 */
static inline void MPU_SetProcessIdentifierMask(MPU_Type * const base,
                                                uint8_t regionNum,
                                                uint8_t pidMask)
{
    base->RGD[regionNum].WORD3 = (base->RGD[regionNum].WORD3 & ~MPU_RGD_WORD3_PIDMASK_MASK)
                               | MPU_RGD_WORD3_PIDMASK(pidMask);
}
#endif

/*!
 * @brief Sets region start and end address.
 *
 * @param[in] base The MPU peripheral base address.
 * @param[in] regionNum MPU region number.
 * @param[in] startAddr Region start address.
 * @param[in] endAddr Region end address.
 */
void MPU_SetRegionAddr(MPU_Type * const base,
                       uint8_t regionNum,
                       uint32_t startAddr,
                       uint32_t endAddr);

/*!
 * @brief Sets access permission for master in region descriptor.
 * Please note that using this function will clear the valid bit of the region,
 * and a further validation might be needed.
 *
 * @param[in] base The MPU peripheral base address.
 * @param[in] regionNum The region number.
 * @param[in] masterAccRight Pointer to master number.
 */
void MPU_SetMasterAccessRight(MPU_Type * const base,
                              uint8_t regionNum,
                              const mpu_master_access_right_t * masterAccRight);

/*!
 * @brief Initializes the MPU module and all regions will be invalid
 * after cleared access permission.
 *
 * @param[in] base The MPU peripheral base address.
 */
void MPU_InitRegion(MPU_Type * const base,
                    uint8_t regionNum);

/*!
 * @brief Gets the error status of a specified slave port.
 *
 * @param[in] base The MPU peripheral base address.
 * @param[in] slaveNum The slave port number.
 * @return The slave ports error status:
 *         - true  : error happens in this slave port.
 *         - false : error didn't happen in this slave port.
 */
static inline bool MPU_GetErrorStatus(const MPU_Type * const base,
                                      uint8_t slaveNum)
{

    return ((base->CESR & FEATURE_MPU_SLAVE(1UL << ((FEATURE_MPU_SLAVE_WIDTH - 1U) - slaveNum))) != 0U);
}

/*!
 * @brief Clears the error flag of a specified slave port.
 *
 * @param[in] base The MPU peripheral base address.
 * @param[in] slaveNum The slave port number.
 */
static inline void MPU_ClearErrorFlag(MPU_Type * const base,
                                      uint8_t slaveNum)
{
    base->CESR = (base->CESR & ~FEATURE_MPU_SLAVE_MASK)
               | FEATURE_MPU_SLAVE(1UL << ((FEATURE_MPU_SLAVE_WIDTH - 1U) - slaveNum));
}

/*!
 * @brief Reports the MPU access error detail information of error slave port.
 *
 * @param[in] base The MPU peripheral base address.
 * @param[in] slaveNum The slave port number.
 * @param[out] errInfoPtr The pointer to the MPU access error information.
 */
void MPU_GetErrorInfo(const MPU_Type * const base,
                      uint8_t slaveNum,
                      mpu_access_err_info_t * errInfoPtr);

/*!
 * @brief Reports the default master access rights.
 *
 * @param[in] masterNum The master number.
 * @param[out] masterAccRight The pointer to the master access rights
 */
void MPU_GetDefaultMasterAccRight(uint8_t masterNum,
                                  mpu_master_access_right_t * masterAccRight);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* MPU_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 *******************************************************************************/
