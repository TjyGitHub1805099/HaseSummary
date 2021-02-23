

#if(STD_ON == BINSIG_REVERSE_MODULE_ACTIVE)
#include "BinSig_Reverse_Cfg.h"
#include "System_Signal.h"
#include "trace.h"

#define BINSIG_REVERSE_INITIALIZE 0XFFFFFFFF
#endif


void BinSig_Reverse_Scan(void)
{
#if(STD_ON == BINSIG_REVERSE_MODULE_ACTIVE)
    boolean l_portState;
    static uint8 l_sampleCounter = BINSIG_REVERSE_FILTER_TIME;
    uint32 l_value = BINSIG_REVERSE_INITIALIZE;

    l_portState = BINSIG_REVERSE_GET_PORT_VALUE;
    if (0 != l_sampleCounter) {
        l_sampleCounter--;
    }
    if (0 == l_sampleCounter) {
        if ((E_OK == System_Signal_Receive(SYSTEM_SIG_ID_REVERSE, &l_value)) && (l_value != l_portState)) {
#if (STD_ON == BINSIG_REVERSE_DEBUG_TRACE)
            TracePrintf(INPUT_TRACE, TRACE_INFO, "[BinSig-I]:Signal Reverse state changed\r\n");
#endif
            System_Signal_Send(SYSTEM_SIG_ID_REVERSE, l_portState, STD_OFF);
        }
    }
    if ((E_OK == System_Signal_Receive(SYSTEM_SIG_ID_REVERSE, &l_value)) && (l_value == l_portState)) {
        l_sampleCounter = BINSIG_REVERSE_FILTER_TIME;
    }
#endif
}

