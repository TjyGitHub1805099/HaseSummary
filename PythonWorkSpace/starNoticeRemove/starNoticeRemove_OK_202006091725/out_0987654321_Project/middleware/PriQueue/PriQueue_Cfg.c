
#include "Std_Types.h"
#include "PriQueue.h"









const uint8 PriQueue_MaxChannel = PRIQUEUE_CH_MAX;
PriQueueManageType PriQueueManage[PRIQUEUE_CH_MAX];



PriQueue_BufDef(Dtc, PRIQUEUE_LENGTH_DTC);

const PriQueueCfgType PriQueueCfg[] = 
{
    {PRIQUEUE_LENGTH_DTC, PriQueue_GetFlagBufName(Dtc), &PriQueue_GetHeadBufName(Dtc), PriQueue_GetNodeBufName(Dtc)},
};





