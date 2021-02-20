/******************************************************************************
**          Copyright (c) by HSAE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HSAE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : 
**          Author: Garin Gao
**
**          Date  : 2016-12-07
**          
**          
******************************************************************************/

#ifndef __DIONE_PHY_H__
#define __DIONE_PHY_H__

/******************************************************************************
**    INCLUDES
******************************************************************************/
#include "dsp_global.h"
/******************************************************************************
**    MACROS
******************************************************************************/

/******************************************************************************
**    TYPE DEFINITIONS
******************************************************************************/

typedef INT32 (*pfunc_phy_open)(const char *path, INT32 sub_chan, INT32 clk_freq);
typedef INT32 (*pfunc_phy_close)(void);
typedef INT32 (*pfunc_phy_read)(UINT8  *wbuf, INT32 wlen, UINT8  *rbuf, INT32 rlen);
typedef INT32 (*pfunc_phy_write)(UINT8 *wbuf, INT32 wlen);
typedef INT32 (*pfunc_phy_writeRead)(UINT8 *wbuf, INT32 wlen, UINT8 *rbuf, INT32 rlen);

typedef struct
{
    pfunc_phy_open  open;
    pfunc_phy_close close;
    pfunc_phy_read  read;
    pfunc_phy_write write;
    pfunc_phy_writeRead writeRead;
}saf77xx_phy_t;

/******************************************************************************
**    FUNCTION DEFINITIONS
******************************************************************************/
INT32 saf77xx_phy_open(const char *path, INT32 sub_chan, INT32 clk_freq);
INT32 saf77xx_phy_close(void);
INT32 saf77xx_phy_read(UINT8  *wbuf, INT32 wlen, UINT8  *rbuf, INT32 rlen);
INT32 saf77xx_phy_write(UINT8 *wbuf, INT32 wlen);
INT32 saf77xx_phy_writeRead(UINT8  *wbuf, INT32 wlen, UINT8	*rbuf, INT32 rlen);
INT32 saf77xx_phy_init(saf77xx_phy_t *phy);

#endif /*__DIONE_PHY_H__ */

