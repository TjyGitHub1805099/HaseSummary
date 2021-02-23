#include "LcdBacklight.h"
#include "LcdBacklight_Cfg.h"

static LcdBacklightType LcdBacklightManage =
{
    STD_OFF,
    STD_OFF,
    LCDBL_MODE_MANUAL,
    0,
    0
};

void LcdBL_Init(void)
{

    if (STD_OFF == LcdBacklightManage.init)
    {
        
        
        LcdBacklightManage.init = STD_ON;
    }
}

void LcdBL_Deinit(void)
{
    if (STD_ON == LcdBacklightManage.init)
    {
        
        
        LcdBacklightManage.init = STD_OFF;
    }
}

boolean LcdBL_SetOnOff(boolean onoff)
{
    boolean l_returnValue = E_NOT_OK;

    if (STD_ON == LcdBacklightManage.init)
    {
        if (LcdBacklightManage.onoff != onoff)
        {
            LcdBacklightManage.onoff = onoff;
            if (STD_ON == LcdBacklightManage.onoff)
            {
                if (LCDBL_MODE_MANUAL == LcdBacklightManage.mode)
                {
                    
                    
                }
                else
                {
                    
                    
                }
            }
            else
            {
                
            }
        }
        l_returnValue = E_OK;
    }
    return (l_returnValue);
}

boolean LcdBL_GetOnOff(uint8 * onoff)
{
    boolean l_returnValue = E_NOT_OK;

    if (STD_ON == LcdBacklightManage.init)
    {
        *onoff = LcdBacklightManage.onoff;
        l_returnValue = E_OK;
    }
    return (l_returnValue);
}

boolean LcdBL_SetBrightness(uint8 brightness)
{
    boolean l_returnValue = E_NOT_OK;

    if (STD_ON == LcdBacklightManage.init)
    {
        if (STD_ON == LcdBacklightManage.onoff)
        {
            if (LCDBL_MODE_MANUAL == LcdBacklightManage.mode)
            {
                LcdBacklightManage.brightness = brightness;
            }
            else
            {
                LcdBacklightManage.autoBrightness = brightness;
            }
            
            l_returnValue = E_OK;
        }
    }
    return (l_returnValue);
}

boolean LcdBL_GetBrightness(uint8 * brightness)
{
    boolean l_returnValue = E_NOT_OK;

    if (STD_ON == LcdBacklightManage.init)
    {
        if (LCDBL_MODE_MANUAL == LcdBacklightManage.mode)
        {
            *brightness = LcdBacklightManage.brightness;
        }
        else
        {
            *brightness = LcdBacklightManage.autoBrightness;
        }
        l_returnValue = E_OK;
    }
    return (l_returnValue);
}

boolean LcdBL_SetMode(LcdBL_ModeEType mode)
{
    boolean l_returnValue = E_NOT_OK;

    if (STD_ON == LcdBacklightManage.init)
    {
        LcdBacklightManage.mode = mode;
        l_returnValue = E_OK;
    }
    return (l_returnValue);
}

boolean LcdBL_GetMode(LcdBL_ModeEType * mode)
{
    boolean l_returnValue = E_NOT_OK;

    if (STD_ON == LcdBacklightManage.init)
    {
        *mode = LcdBacklightManage.mode;
        l_returnValue = E_OK;
    }
    return (l_returnValue);
}


