#ifndef STD_TYPES_H
#define STD_TYPES_H

#include "User_Types.h"
#include "Platform_Types.h"

#define STD_HIGH (1u)
#define STD_LOW (0u)

#define STD_ACTIVE (1u)
#define STD_IDLE (0u)

#define STD_ON (1u)
#define STD_OFF (0u)

#ifndef E_OK
#define E_OK (0u)
#endif

#ifndef E_NOT_OK
#define E_NOT_OK (1u)
#endif

#ifndef    NULL
#ifdef __cplusplus
#define NULL (0u)
#else
#define NULL ((void*)0)
#endif
#endif

#ifndef NULL_PTR
#define NULL_PTR ((void*)0)
#endif

typedef uint8 Std_ReturnType;

#define VARIANT_PRE_COMPILE    0x00 /* module config use variant pre compile */
#define VARIANT_LINK_TIME      0x01 /* module config use variant link time */
#define VARIANT_POST_BUILD     0x02 /* module config use variant post build */

#endif
