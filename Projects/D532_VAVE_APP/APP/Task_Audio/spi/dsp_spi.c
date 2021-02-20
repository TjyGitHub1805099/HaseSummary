/*
 * dsp_spi.c
 *
 *  Created on: 2018Äê9ÔÂ15ÈÕ
 *      Author: gaojing
 */
#include "Std_Types.h"
#include "spi_saf775d.h"
#include "dsp_driver.h"
#include "dsp_spi.h"
#include "dsp_global.h"
#include "dione_phy.h"

#include "BoardDefines.h"

#define SPI_SYNCHRONOUS_TIMEOUT 100

UINT8 spi_tempo_array[100];

dsp_instance dsp_inst;
int dsp_spi_status = 0;

static INT32 saf775d_phy_read(UINT8 *wbuf, INT32 wlen,UINT8 *rbuf, INT32 rlen)
{
    INT32 ret = DSP_OK;
    status_t l_ret;

#if 0
    UINT8 buffer[100];
#endif

    if(rbuf == NULL)
    {
        return (DSP_ERR);
    }

    if(dsp_spi_status != 1)
    {
        return (DSP_ERR);
    }

#if 0
    /* TEMP:  */
    INT32 length = wlen + rlen;
    for(int i = 0; i < wlen; i++){
    	buffer[i] = wbuf[i];
    }
    for(int i = 0; i < rlen; i++){
    	buffer[wlen + i] = 0x00;
    }

    l_ret = LPSPI_DRV_MasterTransferBlocking(dsp_inst.instance_id, buffer, spi_tempo_array, length, SPI_SYNCHRONOUS_TIMEOUT);
	if(l_ret != STATUS_SUCCESS)
	{
		//LOG_OUT(LOG_LEVEL_ERR, "saf775d_phy_read error.\n");
		return (DSP_ERR);
	}

	for(int i = 0; i < rlen; i++){
		rbuf[i] = spi_tempo_array[wlen + i];
	}
#endif

    l_ret = LPSPI_DRV_MasterTransferBlocking(dsp_inst.instance_id, wbuf, spi_tempo_array, wlen, SPI_SYNCHRONOUS_TIMEOUT);
    if(l_ret != STATUS_SUCCESS)
    {
        //LOG_OUT(LOG_LEVEL_ERR, "saf775d_phy_read error.\n");
        return (DSP_ERR);
    }

    System_Task_Delay(1);

    memset(spi_tempo_array, 0, rlen);
    l_ret = LPSPI_DRV_MasterTransferBlocking(dsp_inst.instance_id, spi_tempo_array, rbuf, rlen, SPI_SYNCHRONOUS_TIMEOUT);
    if(l_ret != STATUS_SUCCESS)
    {
    	//LOG_OUT(LOG_LEVEL_ERR, "saf775d_phy_read error.\n");
        return (DSP_ERR);
    }

    return ret;
}

UINT8 rbuf[100];
static INT32 saf775d_phy_write(UINT8 *wbuf, INT32 wlen)
{
    INT32 ret = DSP_OK;
    status_t l_ret;

    if( wbuf == NULL )
    {
        return(DSP_ERR);
    }

    if(dsp_spi_status != 1)
	{
		return (DSP_ERR);
	}

	l_ret = LPSPI_DRV_MasterTransferBlocking(dsp_inst.instance_id, wbuf, spi_tempo_array, wlen, SPI_SYNCHRONOUS_TIMEOUT);
	if(l_ret != STATUS_SUCCESS)
	{
		LOG_OUT(LOG_LEVEL_ERRO, "saf775d_phy_write error%s.\n", "");
		return (DSP_ERR);
	}

    return ret;
}

static INT32 saf775d_phy_writeRead(UINT8 *wbuf, INT32 wlen,UINT8 *rbuf, INT32 rlen)
{
    INT32 ret = DSP_OK;
    status_t l_ret;

#if 0
    UINT8 buffer[100];
#endif

    if((wbuf == NULL) || (rbuf == NULL))
    {
        return (DSP_ERR);
    }

    if(dsp_spi_status != 1)
    {
        return (DSP_ERR);
    }

#if 0
    /* TEMP:  */
    INT32 length = wlen + rlen;
    for(int i = 0; i < wlen; i++){
    	buffer[i] = wbuf[i];
    }
    for(int i = 0; i < rlen; i++){
    	buffer[wlen + i] = 0x00;
    }

    l_ret = LPSPI_DRV_MasterTransferBlocking(dsp_inst.instance_id, buffer, spi_tempo_array, length, SPI_SYNCHRONOUS_TIMEOUT);
	if(l_ret != STATUS_SUCCESS)
	{
		//LOG_OUT(LOG_LEVEL_ERR, "saf775d_phy_read error.\n");
		return (DSP_ERR);
	}

	for(int i = 0; i < rlen; i++){
		rbuf[i] = spi_tempo_array[wlen + i];
	}
#endif

    l_ret = LPSPI_DRV_MasterTransferBlocking(dsp_inst.instance_id, wbuf, spi_tempo_array, wlen, SPI_SYNCHRONOUS_TIMEOUT);
    if(l_ret != STATUS_SUCCESS)
    {
        //LOG_OUT(LOG_LEVEL_ERR, "saf775d_phy_read error.\n");
        return (DSP_ERR);
    }

    System_Task_Delay(1);

    memset(spi_tempo_array, 0, rlen);
    l_ret = LPSPI_DRV_MasterTransferBlocking(dsp_inst.instance_id, spi_tempo_array, rbuf, rlen, SPI_SYNCHRONOUS_TIMEOUT);
    if(l_ret != STATUS_SUCCESS)
    {
    	//LOG_OUT(LOG_LEVEL_ERR, "saf775d_phy_read error.\n");
        return (DSP_ERR);
    }

    return ret;
}

static const saf77xx_phy_t saf775d_phy =
{
    .open  = NULL,
    .close = NULL,
    .read  = saf775d_phy_read,
    .write = saf775d_phy_write,
    .writeRead = saf775d_phy_writeRead,
};


int dsp_spi_init()
{
	status_t l_ret;
	INT32 ret;

	dsp_inst.instance_id = SPI_SAF775D;

	l_ret = LPSPI_DRV_MasterInit(SPI_SAF775D, &(dsp_inst.lpspiState), &spi_saf775d_MasterConfig0);
	if(l_ret != STATUS_SUCCESS ){
		return -1;
	}

	ret = saf77xx_phy_init((saf77xx_phy_t *)&saf775d_phy);
	if(ret < 0)
	{
		//LOG_OUT(LOG_LEVEL_INFO, "[DIONE]phy init error\n");
	}

	dsp_spi_status = 1;

	/* reset pin */

	Port_SetOutLevel(DSP_RST_N, PORT_LOW);
	System_Task_Delay(5);
	Port_SetOutLevel(DSP_RST_N, PORT_HIGH);
	System_Task_Delay(10);

	ret = dione_radio_preInit();
	if(ret<0)
	{
		//LOG_OUT(LOG_LEVEL_INFO"[DIONE]RADIO open error\n");
	}

	ret = dsp_driver_open(0, NULL);
	if(ret<0)
	{

	}

	return ret;
}

