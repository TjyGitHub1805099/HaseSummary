#include "KeyBacklight.h"
#include "KeyBacklight_Cfg.h"

typedef struct
{
    boolean init;
    boolean onoff;
    KeyBL_ModeEType mode;
    uint8 brightness;
    uint8 autoBrightness;
} KeyBacklightType;

static KeyBacklightType KeyBacklightManage =
{
    STD_OFF,
    STD_OFF,
    KEYBL_MODE_MANUAL,
    0,
    0
};

void KeyBL_Init(void)
{
    

    if (STD_OFF == KeyBacklightManage.init)
    {
        
        
        KeyBacklightManage.init = STD_ON;
    }
}

void KeyBL_Deinit(void)
{
    if (STD_ON == KeyBacklightManage.init)
    {
        
        
        KeyBacklightManage.init = STD_OFF;
    }
}

boolean KeyBL_SetOnOff(boolean onoff)
{
    boolean l_returnValue = E_NOT_OK;

    if (STD_ON == KeyBacklightManage.init)
    {
        if (KeyBacklightManage.onoff != onoff)
        {
            KeyBacklightManage.onoff = onoff;
            if (STD_ON == KeyBacklightManage.onoff)
            {
                if (KEYBL_MODE_MANUAL == KeyBacklightManage.mode)
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

boolean KeyBL_GetOnOff(uint8 * onoff)
{
    boolean l_returnValue = E_NOT_OK;

    if (STD_ON == KeyBacklightManage.init)
    {
        *onoff = KeyBacklightManage.onoff;
        l_returnValue = E_OK;
    }
    return (l_returnValue);
}

boolean KeyBL_SetBrightness(uint8 brightness)
{
    boolean l_returnValue = E_NOT_OK;

    if (STD_ON == KeyBacklightManage.init)
    {
        if (STD_ON == KeyBacklightManage.onoff)
        {
            if (KEYBL_MODE_MANUAL == KeyBacklightManage.mode)
            {
                KeyBacklightManage.brightness = brightness;
            }
            else
            {
                KeyBacklightManage.autoBrightness = brightness;
            }
            
            l_returnValue = E_OK;
        }
    }
    return (l_returnValue);
}

boolean KeyBL_GetBrightness(uint8 * brightness)
{
    boolean l_returnValue = E_NOT_OK;

    if (STD_ON == KeyBacklightManage.init)
    {
        if (KEYBL_MODE_MANUAL == KeyBacklightManage.mode)
        {
            *brightness = KeyBacklightManage.brightness;
        }
        else
        {
            *brightness = KeyBacklightManage.autoBrightness;
        }
        l_returnValue = E_OK;
    }
    return (l_returnValue);
}

boolean KeyBL_SetMode(KeyBL_ModeEType mode)
{
    boolean l_returnValue = E_NOT_OK;

    if (STD_ON == KeyBacklightManage.init)
    {
        KeyBacklightManage.mode = mode;
        l_returnValue = E_OK;
    }
    return (l_returnValue);
}

boolean KeyBL_GetMode(KeyBL_ModeEType * mode)
{
    boolean l_returnValue = E_NOT_OK;

    if (STD_ON == KeyBacklightManage.init)
    {
        *mode = KeyBacklightManage.mode;
        l_returnValue = E_OK;
    }
    return (l_returnValue);
}


