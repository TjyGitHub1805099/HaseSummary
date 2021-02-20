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
    STD_OFF,//init;
    STD_OFF,//onoff;
    KEYBL_MODE_MANUAL,//mode;
    0,//brightness;
    0//autoBrightness;
};

void KeyBL_Init(void)
{
    //ftm_state_t flexTimer_pwm5_state;

    if (STD_OFF == KeyBacklightManage.init)
    {
        //FTM_DRV_Init(KEYBL_PWM_CHANNEL, &flexTimer_pwm5_cfg_InitConfig, &flexTimer_pwm5_state);
        //FTM_DRV_InitPwm(KEYBL_PWM_CHANNEL, &flexTimer_pwm5_cfg_PwmConfig);
        KeyBacklightManage.init = STD_ON;
    }
}

void KeyBL_Deinit(void)
{
    if (STD_ON == KeyBacklightManage.init)
    {
        //FTM_DRV_DeinitPwm(KEYBL_PWM_CHANNEL);
        //FTM_DRV_Deinit(KEYBL_PWM_CHANNEL);
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
                    //set KeyBacklightManage.brightness
                    //FTM_DRV_UpdatePwmChannel(KEYBL_PWM_CHANNEL);
                }
                else
                {
                    //set KeyBacklightManage.autobrightness
                    //FTM_DRV_UpdatePwmChannel(KEYBL_PWM_CHANNEL);
                }
            }
            else
            {
                //FTM_DRV_UpdatePwmChannel(KEYBL_PWM_CHANNEL);
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
            //FTM_DRV_UpdatePwmChannel(KEYBL_PWM_CHANNEL);
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

/*=========================== END OF FILE: KeyBacklight.c ===========================*/
