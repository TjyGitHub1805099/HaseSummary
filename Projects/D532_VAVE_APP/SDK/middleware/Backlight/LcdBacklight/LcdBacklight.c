#include "LcdBacklight.h"
#include "LcdBacklight_Cfg.h"

static LcdBacklightType LcdBacklightManage =
{
    STD_OFF,//init;
    STD_OFF,//onoff;
    LCDBL_MODE_MANUAL,//mode;
    0,//brightness;
    0//autoBrightness;
};

void LcdBL_Init(void)
{
    ftm_state_t flexTimer_pwm5_state;

    if (STD_OFF == LcdBacklightManage.init)
    {
        //FTM_DRV_Init(LCDBL_PWM_CHANNEL, &flexTimer_pwm5_cfg_InitConfig, &flexTimer_pwm5_state);
        //FTM_DRV_InitPwm(LCDBL_PWM_CHANNEL, &flexTimer_pwm5_cfg_PwmConfig);
        LcdBacklightManage.init = STD_ON;
    }
}

void LcdBL_Deinit(void)
{
    if (STD_ON == LcdBacklightManage.init)
    {
        //FTM_DRV_DeinitPwm(LCDBL_PWM_CHANNEL);
        //FTM_DRV_Deinit(LCDBL_PWM_CHANNEL);
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
                    //set LcdBacklightManage.brightness
                    //FTM_DRV_UpdatePwmChannel(LCDBL_PWM_CHANNEL);
                }
                else
                {
                    //set LcdBacklightManage.autobrightness
                    //FTM_DRV_UpdatePwmChannel(LCDBL_PWM_CHANNEL);
                }
            }
            else
            {
                //FTM_DRV_UpdatePwmChannel(LCDBL_PWM_CHANNEL);
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
            //FTM_DRV_UpdatePwmChannel(LCDBL_PWM_CHANNEL);
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

/*=========================== END OF FILE: LcdBacklight.c ===========================*/
