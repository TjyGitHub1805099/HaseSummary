

#if(STD_ON == BINSIG_TCU_ACC_MODULE_ACTIVE)
#include "BinSig_Tcu_Acc_Cfg.h"
#include "System_Signal.h"
#include "trace.h"

#define BINSIG_TCU_ACC_INITIALIZE 0XFFFFFFFF
#endif


void BinSig_Tcu_Acc_Scan(void)
{
#if(STD_ON == BINSIG_TCU_ACC_MODULE_ACTIVE)
    boolean l_portState;
    static uint8 l_sampleCounter = BINSIG_TCU_ACC_FILTER_TIME;
    uint32 l_value = BINSIG_TCU_ACC_INITIALIZE;

    l_portState = BINSIG_TCU_ACC_GET_PORT_VALUE;
    if (0 != l_sampleCounter) {
        l_sampleCounter--;
    }
    if (0 == l_sampleCounter) {
        if ((E_OK == System_Signal_Receive(SYSTEM_SIG_ID_TCU_ACC, &l_value)) && (l_value != l_portState)) {
#if (STD_ON == BINSIG_TCU_ACC_DEBUG_TRACE)
            TracePrintf(INPUT_TRACE, TRACE_INFO, "[BinSig-I]:Signal Tcu_Acc state changed\r\n");
#endif
            System_Signal_Send(SYSTEM_SIG_ID_TCU_ACC, l_portState, STD_OFF);
        }
    }
    if ((E_OK == System_Signal_Receive(SYSTEM_SIG_ID_TCU_ACC, &l_value)) && (l_value == l_portState)) {
        l_sampleCounter = BINSIG_TCU_ACC_FILTER_TIME;
    }
#endif
}

