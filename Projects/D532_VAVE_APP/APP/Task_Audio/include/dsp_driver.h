/******************************************************************************
**          Copyright (c) by HSAE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HSAE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : dsp_driver.h
**          Author: Zhu Q
**
**          Date  : 2018-04-29
**          
**          
******************************************************************************/

#ifndef  __DSP_DRIVER_H__
#define  __DSP_DRIVER_H__

/******************************************************************************
**    INCLUDES
******************************************************************************/

/******************************************************************************
**    MACROS
******************************************************************************/

/*define error code*/
#define DSP_OK                 ( 0)
#define DSP_ERR                (-1)
#define DSP_ERR_IOCTL          (-2)
#define DSP_ERR_VALUE          (-3)
#define DSP_ERR_MEMORY         (-4)
#define DSP_ERR_DEVICE         (-5)

/*define ioctrl cmd id*/
/*common dsp cmd id*/
#define DSP_SET_MUX            0x00550001  //cmd parameters: tMuxCmd
#define DSP_SET_MIX            0x00550002  //cmd parameters: tMixCmd
#define DSP_SET_VOL            0x00550003  //cmd parameters: tVolCmd
#define DSP_SET_MUTE           0x00550004  //cmd parameters: tMuteCmd
#define DSP_SET_FADE           0x00550005  //cmd parameters: tFadeCmd
#define DSP_SET_BALANCE        0x00550006  //cmd parameters: tBalanceCmd
#define DSP_SET_TONE           0x00550007  //cmd parameters: tToneCmd
#define DSP_SET_GEQ            0x00550008  //cmd parameters: tGEQCmd
#define DSP_SET_LOUDNESS       0x00550009  //cmd parameters: tLoudnessCmd
#define DSP_SET_PEQ            0x0055000A  //cmd parameters: tPEQCmd

/*Tuning purpose cmd id*/
#define DSP_TUNING_CMD_START   0x00550100
#define DSP_TUNING_PEQ         0x00550101
#define DSP_TUNING_DELAY       0x00550102
#define DSP_TUNING_GAIN        0x00550103
#define DSP_TUNING_PHASE       0x00550104
#define DSP_TUNING_LIMITER     0x00550105

/*project private cmd id, any extend cmd should be defined in dsp_driver_proj.h*/
#define DSP_PROJECT_START      0x00551000

/******************************************************************************
**    TYPE DEFINITIONS
******************************************************************************/

/*Input source type*/
typedef enum
{
    INPUT_NONE = 0,
    INPUT_I2S0,
    INPUT_I2S1,
    INPUT_I2S2,
    INPUT_I2S3,
    INPUT_I2S4,
    INPUT_I2S5,
    INPUT_I2S6,
    INPUT_I2S7,
    INPUT_I2S8,
    INPUT_I2S9,
    INPUT_TDM0,
    INPUT_TDM1,
    INPUT_TDM2,
    INPUT_TDM3,
    INPUT_TDM4,
    INPUT_SPDF0,
    INPUT_SPDF1,
    INPUT_AIN01,
    INPUT_AIN23,
    INPUT_AIN0,
    INPUT_AIN1,
    INPUT_AIN2,
    INPUT_AIN3,
    INPUT_AIN4,
    INPUT_AIN5,
    INPUT_AIN6,
    INPUT_AIN7,
    INPUT_HI2S0,
    INPUT_HI2S1,
    INPUT_HI2S2,
    INPUT_HI2S3,
    INPUT_HI2S4,
    INPUT_HI2S5,
    INPUT_HI2S6,
    INPUT_HI2S7,
    INPUT_HI2S8,
    INPUT_TUNER1,
    INPUT_TUNER2,
    INPUT_BEEP0,
    INPUT_BEEP1,
    INPUT_BEEP2,
    INPUT_BEEP3,
    INPUT_MAX
}eINPUT_TYPE;

/*Output channel type*/
typedef enum
{
    OUTPUT_NONE  = 0x0000,
    OUTPUT_FL    = 0x0001,
    OUTPUT_FR    = 0x0002,
    OUTPUT_RL    = 0x0004,
    OUTPUT_RR    = 0x0008,
    OUTPUT_SWL   = 0x0010, //center
    OUTPUT_SWR   = 0x0020, //subwoofer
    OUTPUT_I2S0L = 0x0040,
    OUTPUT_I2S0R = 0x0080,
    OUTPUT_I2S1L = 0x0100,
    OUTPUT_I2S1R = 0x0200,
    OUTPUT_MAX,
} eOUTPUT_TYPE;

/*Input sub channel, such as TDM slots*/
typedef enum
{
    SUB_CHAN_0    = 0x0001,
    SUB_CHAN_1    = 0x0002,
    SUB_CHAN_2    = 0x0004,
    SUB_CHAN_3    = 0x0008,
    SUB_CHAN_4    = 0x0010,
    SUB_CHAN_5    = 0x0020,
    SUB_CHAN_6    = 0x0040,
    SUB_CHAN_7    = 0x0080,

}eSUBCHAN_TYPE;


/*DSP processing channel */
typedef enum
{
    CHANNEL_NONE   = 0x0,
    CHANNEL_MAIN_A = 0x1,  /*Stereo 1*/
    CHANNEL_MAIN_B = 0x2,  /*Stereo 2*/
    CHANNEL_MAIN_C=  0x3,  /*Stereo 3*/
    CHANNEL_MONO_A = 0x4,  /*Mono 1*/
    CHANNEL_MONO_B = 0x5,  /*Mono 2*/
    CHANNEL_MONO_C = 0x6,  /*Mono 3*/
    CHANNEL_MONO_D = 0x7,  /*Mono 4*/
    CHANNEL_MONO_E = 0x8,  /*Mono 5*/
    CHANNEL_MONO_F = 0x9,  /*Mono 6*/
    CHANNEL_MAX,
}eCHANNEL_TYPE;

#define DSP_VOL_OFF (-9900)   //-99dB
#define DSP_VOL_0dB (0)       //0dB

/*select the input source into DSP channel*/
typedef struct
{
    int input;    /*[@eINPUT_TYPE]*/
    int subType;  /*[@eSUBCHAN_TYPE]*/
    int channel;  /*[@eCHANNEL_TYPE]*/
    int gain;     /*[mute, -60dB,...,12dB][0.01dB], -99dB means mute*/
}tMuxCmd;

/*mix DSP channel into output channel*/
typedef struct
{
    int channel;  /*[@eCHANNEL_TYPE]*/
    int output;   /*[@eOUTPUT_TYPE], you can use '|' to combin multiple channels.*/
    int gain;     /*[mute, -60dB,...,12dB][0.01dB], -99dB means mute*/
}tMixCmd;

/*set channel volume*/
typedef struct
{
    int channel;  /*[@eCHANNEL_TYPE]*/
    int gain;     /*[mute, -80dB,...,12dB][0.01dB], -99dB means mute.*/
    int ramp;     /*[0, 5000][ms], ramp time*/
}tVolCmd;

/*set channel mute*/
typedef struct
{
    int channel;  /*[@eCHANNEL_TYPE]*/
    int mode;     /*[0, 1], 0 - unmute, 1 - mute.*/
    int ramp;     /*[0, 5000][ms], ramp time*/
}tMuteCmd;

/*set channel fade*/
typedef struct
{
    int channel;  /*[@eCHANNEL_TYPE]*/
    int gainF;    /*[-80dB,...,12dB][0.01dB]*/
    int gainR;    /*[-80dB,...,12dB][0.01dB]*/
}tFadeCmd;

/*set channel balance*/
typedef struct
{
    int channel;  /*[@eCHANNEL_TYPE]*/
    int gainL;    /*[-80dB,...,12dB][0.01dB]*/
    int gainR;    /*[-80dB,...,12dB][0.01dB]*/
}tBalanceCmd;

/*Tone band id*/
typedef enum
{
    TONE_BASS   = 1,
    TONE_MID    = 2,
    TONE_TREBLE = 3,
}eTONE_BAND;

/*define filter type*/
typedef enum
{
    FLT_TYPE_FLAT = 0,
    FLT_1ST_LOWSHELF,   /* 0 */
    FLT_2ND_LOWSHELF,   /* 1 */
    FLT_1ST_HIGHSHELF,  /* 2 */
    FLT_2ND_HIGHSHELF,  /* 3 */
    FLT_1ST_PEAKING,    /* 4 */
    FLT_2ND_PEAKING,    /* 5 */
    FLT_LPF,            /* 6 */
    FLT_HPF,            /* 7 */
    FLT_ALL_PASS,       /* 8 */
    FLT_TYPE_MAX,
} eFILTER_TYPE;

/*gerenrical EQ filter parameters*/
typedef struct
{
    int filter;    /*[@eFILTER_TYPE]*/
    int frequency; /*[20, 20000][Hz]*/
    int factorQ;   /*[0.01, 10000][0.01]*/
    int gain;      /*[-20dB, 20dB][0.01dB]*/
}tFilter;

/*set channel Tone*/
typedef struct
{
    int channel;   /*[@eCHANNEL_TYPE]*/
    int band;      /*[@eTONE_BAND]*/
    int mode;      /*[0, 12], 0-parameter is as tFilter, */
                   /*mode>0 parameter is as coefficients in data[], mode is the data length*/
    union
    {
        tFilter filter;
        int data[12];  /*coefficients: a1,a2,b0,b1,b2*/
    }data;
}tToneCmd;

/*loudness band id*/
typedef enum {
    LOUDNESS_LOW  = 1,
    LOUDNESS_HIGH = 2,
}eLOUDNESS_BAND;

/*set channel Loudness*/
typedef struct
{
    int channel;   /*[@eCHANNEL_TYPE]*/
    int band;      /*[@eLOUDNESS_BAND]*/
    int mode;      /*[0, 12], 0-parameter is as tFilter, */
                   /*mode>0 parameter is as coefficients in data[], mode is the data length*/
    union
    {
        tFilter filter;
        int data[12];  /*coefficients: a1,a2,b0,b1,b2*/
    }data;
}tLoudnessCmd;

/*set channel GEQ*/
typedef struct
{
    int channel;   /*[@eCHANNEL_TYPE]*/
    int band;      /*[1,2,3...]*/
    int mode;      /*[0, 12], 0-parameter is as tFilter, */
                   /*mode>0 parameter is as coefficients in data[], mode is the data length*/
    union
    {
        tFilter filter;
        int data[12];  /*coefficients: a1,a2,b0,b1,b2*/
    }data;
}tGEQCmd;

/*set channel PEQ*/
typedef struct
{
    int channel;   /*[@eCHANNEL_TYPE]*/
    int band;      /*[1,2,3...]*/
    int mode;      /*[0, 12], 0-parameter is as tFilter, */
                   /*mode>0 parameter is as coefficients in data[], mode is the data length*/
    union
    {
        tFilter filter;
        int data[12];  /*coefficients: a1,a2,b0,b1,b2*/
    }data;
}tPEQCmd;

/******************************************************************************
**    FUNCTION DEFINITIONS
******************************************************************************/
int dsp_driver_open(int argc, char **argv);
int dsp_driver_close(void);
int dsp_driver_ioctl(int cmd, void *data);

#endif //__DSP_DRIVER_H__
