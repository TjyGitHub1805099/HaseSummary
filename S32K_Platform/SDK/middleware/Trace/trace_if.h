#ifndef _TRACE_IF_H
#define _TRACE_IF_H

/***********************************************************************************
 Constant Macros
***********************************************************************************/
#include"trace.h"
#include "uart1_pal_cfg.h"

void TraceLoopTxByDMA(void *driverState, uart_event_t event, void *userData);
void TraceLoopTxByTask();
void TraceLoopRxByTask();

#endif

