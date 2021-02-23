

#if(STD_ON == BINSIG_IGN_MODULE_ACTIVE)
#include "BinSig_Ign_Cfg.h"
#include "System_Signal.h"
#include "trace.h"

#define BINSIG_IGN_INITIALIZE 0XFFFFFFFF
#endif


void BinSig_Ign_Scan(void)
{
#if(STD_ON == BINSIG_IGN_MODULE_ACTIVE)
    boolean l_portState;
    static uint8 l_sampleCounter = BINSIG_IGN_FILTER_TIME;
    uint32 l_value = BINSIG_IGN_INITIALIZE;

    l_portState = BINSIG_IGN_GET_PORT_VALUE;
    if (0 != l_sampleCounter) {
        l_sampleCounter--;
    }
    if (0 == l_sampleCounter) {
        if ((E_OK == System_Signal_Receive(SYSTEM_SIG_ID_IGN, &l_value)) && (l_value != l_portState)) {
#if (STD_ON == BINSIG_IGN_DEBUG_TRACE)
            TracePrintf(INPUT_TRACE, TRACE_INFO, "[BinSig-I]:Signal Ign state changed\r\n");
#endif
            System_Signal_Send(SYSTEM_SIG_ID_IGN, l_portState, STD_OFF);
        }
    }
    if ((E_OK == System_Signal_Receive(SYSTEM_SIG_ID_IGN, &l_value)) && (l_value == l_portState)) {
        l_sampleCounter = BINSIG_IGN_FILTER_TIME;
    }
#endif
}

