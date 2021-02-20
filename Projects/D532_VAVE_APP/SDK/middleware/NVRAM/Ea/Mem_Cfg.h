
/******************************************************************************
**          Copyright (c) by HASE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HASE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : 
            Modifly:brown
**
**          Date  : 2017-06-20
**          
**          
******************************************************************************/

#ifndef  _MEM_CFG_H_
#define  _MEM_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif


//#include "MemIf_types.h"

#define DeviceIndexAllNumbers 1

#define MEM_EEP_ADDR_ECUSerialNumber_1 PAGEADDSTART_1
#define MEM_EEP_SIZE_ECUSerialNumber_1 12
#define MEM_EEP_ADDR_ECUSerialNumber_2 MEM_ADDR_ECUSerialNumber_1+MEM_SIZE_ECUSerialNumber_1
#define MEM_EEP_SIZE_ECUSerialNumber_2 12

#define MEM_EEP_ADDR_VINDataID_1 MEM_ADDR_ECUSerialNumber_2+MEM_SIZE_ECUSerialNumber_2
#define MEM_EEP_SIZE_VINDataID_1 19
#define MEM_EEP_ADDR_VINDataID_2 MEM_ADDR_VINDataID_1+MEM_SIZE_VINDataID_1
#define MEM_EEP_SIZE_VINDataID_2 19

#define MEM_EEP_ADDR_SupplierHWRefGuide_1 MEM_ADDR_VINDataID_2+MEM_SIZE_VINDataID_2
#define MEM_EEP_SIZE_SupplierHWRefGuide_1 12
#define MEM_EEP_ADDR_SupplierHWRefGuide_2 MEM_ADDR_SupplierHWRefGuide_1+MEM_SIZE_SupplierHWRefGuide_1
#define MEM_EEP_SIZE_SupplierHWRefGuide_2 12

#define MEM_EEP_ADDR_DiagSerNum_1 MEM_ADDR_SupplierHWRefGuide_2+MEM_SIZE_SupplierHWRefGuide_2
#define MEM_EEP_SIZE_DiagSerNum_1 5
#define MEM_EEP_ADDR_DiagSerNum_2 MEM_ADDR_DiagSerNum_1+MEM_SIZE_DiagSerNum_1
#define MEM_EEP_SIZE_DiagSerNum_2 5

#define MEM_EEP_ADDR_BodyCfgInf_1 MEM_ADDR_DiagSerNum_2+MEM_SIZE_DiagSerNum_2
#define MEM_EEP_SIZE_BodyCfgInf_1 42
#define MEM_EEP_ADDR_BodyCfgInf_2 MEM_ADDR_BodyCfgInf_1+MEM_SIZE_BodyCfgInf_1
#define MEM_EEP_SIZE_BodyCfgInf_2 42

#define MEM_EEP_ADDR_PPKValue_1 MEM_ADDR_BodyCfgInf_2+MEM_SIZE_BodyCfgInf_2
#define MEM_EEP_SIZE_PPKValue_1 5
#define MEM_EEP_ADDR_PPKValue_2 MEM_ADDR_PPKValue_1+MEM_SIZE_PPKValue_1
#define MEM_EEP_SIZE_PPKValue_2 5

#define MEM_EEP_ADDR_SoftwareProgramDate_1 MEM_ADDR_PPKValue_2+MEM_SIZE_PPKValue_2
#define MEM_EEP_SIZE_SoftwareProgramDate_1 6
#define MEM_EEP_ADDR_SoftwareProgramDate_2 MEM_ADDR_SoftwareProgramDate_1+MEM_SIZE_SoftwareProgramDate_1
#define MEM_EEP_SIZE_SoftwareProgramDate_2 6


#ifdef __cplusplus
}
#endif

#endif








