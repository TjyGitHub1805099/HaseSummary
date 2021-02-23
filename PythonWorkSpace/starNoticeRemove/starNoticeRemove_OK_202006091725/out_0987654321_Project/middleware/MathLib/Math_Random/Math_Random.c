#include "Math_Random.h"
#include "System_Tick.h"

uint32  Math_GenRandomNum(void)
{
	uint32 l_seed = 0u;

	l_seed = System_GetTickValue();
	l_seed ^= (l_seed << 13);
	l_seed ^= (l_seed >> 17);
	l_seed ^= (l_seed << 5);
	return (l_seed);
}
