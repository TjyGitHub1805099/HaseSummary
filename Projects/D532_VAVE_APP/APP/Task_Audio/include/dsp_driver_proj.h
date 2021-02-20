/******************************************************************************
**          Copyright (c) by HSAE (Chengdu) Technology Center.
**
**          CONFIDENTIAL and PROPRIETARY
**          UNPUBLISHED SOURCE CODE
**          Disclosure, use or reproduction without authorization of
**          HSAE (Chengdu) Technology Center is prohibited.
*******************************************************************************
**
**          File  : project_api.h
**          Author: Zhu Qiang
**
**          Date  : 2018-05-17
**          
**          
******************************************************************************/

#ifndef __DSP_DRIVER_PROJ_H__
#define __DSP_DRIVER_PROJ_H__

/******************************************************************************
**    INCLUDES
******************************************************************************/
#include "dsp_driver_typeconverter.h"
#include "audio_engineer_conf.h"
/******************************************************************************
**    MACROS
******************************************************************************/

#define AUDIO_DRIVER_NODE   "/dev/saf775d-dsp"

#define CAR_MODEL_4SPEAKERS 0
#define CAR_MODEL_6SPEAKERS 1
#define CAR_MODEL_EXTERNAL_AMP 2

//project special command define
#define DSP_SET_BEEP           (DSP_PROJECT_START+0)
#define DSP_WRITE_DATA       (DSP_PROJECT_START+1)
#define DSP_READ_DATA        (DSP_PROJECT_START+2)
#define DSP_SET_WORKMODE     (DSP_PROJECT_START+3)
#define DSP_SET_MICMODE      (DSP_PROJECT_START+4)
#define DSP_SET_CHIME_BY_ID      (DSP_PROJECT_START+5)
#define DSP_SET_PDC_BY_ID      (DSP_PROJECT_START+6)
#define DSP_CLICKCLARK (DSP_PROJECT_START+7)
#define DSP_SET_MIC_GAIN (DSP_PROJECT_START+8)
#define DSP_SET_HFP_MUTE (DSP_PROJECT_START+9)
#define DSP_SET_CHIME_MUTE (DSP_PROJECT_START+10)
#define DSP_SET_SUPERPOSITION_B_GAIN (DSP_PROJECT_START+11)
#define DSP_SET_AMP_TYPE (DSP_PROJECT_START+12)
#define DSP_SET_SDVC_LEVEL (DSP_PROJECT_START+13)
#define DSP_SET_PCHIME (DSP_PROJECT_START+14)
#define DSP_ENTER_ECALL (DSP_PROJECT_START+15)
#define DSP_LEAVE_ECALL (DSP_PROJECT_START+16)
#define DSP_SET_AUDIO_EFFECT_MODE (DSP_PROJECT_START+17)
#define DSP_SET_HFP_DELAY_LINE (DSP_PROJECT_START+18)
#define DSP_SET_HARDWARE_VERSION (DSP_PROJECT_START+19)
#define DSP_SPEAKER_DIAG (DSP_PROJECT_START+20)
#define DSP_SINE_WAVE (DSP_PROJECT_START+21)
#define DSP_SET_AUDIO_BEAT_DISP (DSP_PROJECT_START+22)
#define DSP_SET_CAR_MODEL (DSP_PROJECT_START+23)
#define DSP_WRITE_REGISTER (DSP_PROJECT_START+24)
#define DSP_READ_REGISTER (DSP_PROJECT_START+25)
#define DSP_READ_REGISTER_RESULT (DSP_PROJECT_START+26)


#define DSP_ENGINEER_COMMAND_START (DSP_PROJECT_START + 0x1000)
/* DSP engineer mode code */
#define DSP_ENGINEER_ENTER_CHIME_PDC_MODE (DSP_ENGINEER_COMMAND_START + 0)
#define DSP_ENGINEER_CHIME_ON_SET_SLOT (DSP_ENGINEER_COMMAND_START + 1)
#define DSP_ENGINEER_CHIME_ON (DSP_ENGINEER_COMMAND_START + 2)
#define DSP_ENGINEER_CHIME_OFF (DSP_ENGINEER_COMMAND_START + 3)
#define DSP_ENGINEER_PDC_ON (DSP_ENGINEER_COMMAND_START + 4)
#define DSP_ENGINEER_PDC_OFF (DSP_ENGINEER_COMMAND_START + 5)
#define DSP_ENGINEER_CHIME_ON_SET_CHANNEL (DSP_ENGINEER_COMMAND_START + 6)
#define DSP_ENGINEER_CLICKCLACK_GAIN (DSP_ENGINEER_COMMAND_START + 7)

/* DSP engineer mode code */
/* AMP engineer mode code */
#define AMP_ENGINEER_SET_PWM (DSP_ENGINEER_COMMAND_START+0xFF+0)
#define AMP_ENGINEER_SET_CHANNEL_CONFIG (DSP_ENGINEER_COMMAND_START+0xFF+1)
/* AMP engineer mode code */

/* AMP command */
#define AMP_COMMAND_START (DSP_PROJECT_START + 0x2000)
#define AMP_EXECUTE_LC_FILTER_DIAGNOSTICS (AMP_COMMAND_START + 0)
#define AMP_LC_FILTER_DIAGNOSTICS_RETURN (AMP_COMMAND_START + 1)



/* beep */
#define PDC_ACTION_START 0
#define PDC_ACTION_STOP 1

#define CHIME_TYPE_LIMIT 0
#define CHIME_TYPE_CONTINOUS 1
#define CHIME_TYPE_CONTINOUS_TONE 2

#define CHIME_ACTION_START 0
#define CHIME_ACTION_STOP 1

#define CHIME_MASK_FL 1
#define CHIME_MASK_FR 2
#define CHIME_MASK_RL 4
#define CHIME_MASK_RR 8

/* ClickClack */
#define CLICK_ACTION 0
#define CLACK_ACTION 1

/* amp type */
#define AMP_INTERNAL 0
#define AMP_EXTERNAL 1

/******************************************************************************
**    VARIABLE DECLARATIONS
******************************************************************************/

/******************************************************************************
**    TYPE DEFINITIONS
******************************************************************************/

//input source format define
typedef enum
{
    INPUT_FORMAT_SINGLE   = 0x0100,  //0-differential, 1-single ended
    INPUT_FORMAT_INVERT_L = 0x0200,  //0-normal, 1-left channel invert
    INPUT_FORMAT_INVERT_R = 0x0400,  //0-normal, 1-right channel invert
}eINPUT_FORMAT;


typedef struct
{
    unsigned char *wdata;
    int wlen;
}tWriteData;

typedef struct
{
    unsigned char *wdata;
    int wlen;
    unsigned char *rdata;
    int rlen;
}tReadData;

/*set beep*/
typedef enum
{
    BEEP_TYPE_MIN = 0,
    BEEP_TYPE_1,
    BEEP_TYPE_2,
    BEEP_TYPE_3,
    BEEP_TYPE_4,
    BEEP_TYPE_5,
    BEEP_TYPE_MAX,
}eBEEP_TYPE;

typedef struct
{
    int type;            /*[@eBeepType}*/
    int on;              /*add or remove Beep*/
    int volume;          /*beep volume*/
    int repeat_time;     /*repeat times */
    int repeat_period;   /*if repeat_time==1, ignore this field, unit ms */
} tBeepCmd;

/*set channel settings, such as delay, upscale, limiter*/
typedef struct
{
    int channel;   /*[@eCHANNEL_TYPE]*/
    int data;
}tTuningCmd;

/* chime pdc */
typedef struct
{
    int action;
	int type;
	int id;
}tChimeCmd_ID;

typedef struct
{
    int action;
	int id;
}tPDCCmd_ID;


typedef struct{
    /* frequency */
    int Tsq_Tb1Ltone_Bk;
    int Tsq_Tb1Htone_Bk;

    int Tsq_TType_Bk;

    /* clip */
    int Tsq_TClip_Bk;

    /* output amplitude */
    int Tsq_TVtone_Bk;

    /* sequencer */
    int T_Bk;
    
} chime_slots_data;

typedef struct{
    /* slots */
    chime_slots_data slots_data[4];

    int slots_enable;

    /* master volume */
    int Tsq_ChimeVol_Bk;

    /* crossfader */
    int Tsq_CrosFStp_Bk;

    /* Linear attack release */
    int Tsq_ArLinStpA_Bk;
    int Tsq_ArRelCoef_Bk;
    int Tsq_ArType_Bk;

    /* not used */
    int att_rel_type;

    /* sequencer */
    int Tsq_TA_Bk;
    int Tsq_TB_Bk;

    /* repetitions */
    int Tsq_Repeate_Bk;

    /* not used */
    int repeat_forever;

    /* chime change threshold */
    int Tsq_ChangeThreshold;

    /* chime decay threshold */
    int Tsq_DecayThreshold;

    /* effect generator */
    int Tsq_addmul_Bk;
    int Tsq_c02_Bk;
    int Tsq_c1_Bk;
    int Tsq_c3_Bk;

    int effect;

    /* */
    int Tsq_SeqStat_Bk;
    int Tsq_Copied;
} chime_coefficients;

typedef struct{
    /* $001 for pdc1, $002 for pdc2 */
    int Tsq_SeqStat_Bk; // load the backup coefficients

    /* frequency */
    int T1b1Ltone_Bk;
    int T1b1Htone_Bk;
    /* volume */
    int T1Vtone_Bk;
    /* clip */
    int T1Clip_Bk;
    /* superposition */
    int T2b1Ltone_Bk;  // FL
    int T2b1Htone_Bk;  // FR
    int T2Vtone_Bk;    // RL
    int T2Clip_Bk;     // RR

    /* TA */
    int TA_Bk;
    /* TB */
    int TB_Bk;

    /* attack time */
    int ArLinStpA_Bk;
    /* release time */
    int ArRelCoef_Bk;

    /* chime fadeout */
    int repeate_Bk;

    /* trigger, set to $7FF */
    int Tsq_Copied;
} pdc_coefficients;

/* polyphonic chime */
typedef struct{
    /* frequency should be converted to dsp standard */
    uint32_t frequency[8];
    uint32_t volume[8];

    uint32_t delay[4];
    uint32_t delay_volume[4];
    uint32_t attack[4];
    uint32_t sustain[4];
    uint32_t decay[4];

    uint32_t lfo_frequency;
    uint32_t lfo_amplitude;
    uint32_t lfo_offset;

    uint32_t repetition;
    uint32_t repeat_sample;  

    uint32_t master_volume;
    uint32_t soft_mute;
} pchime_register_parameter;

/* ClickClark */
typedef struct{
    int32_t action;
}tClickClackCmd;

typedef struct
{
    int32_t action;
	int32_t id;
}tPChimeCmd_ID;

typedef struct
{
    int32_t frequency;
	int32_t gain;
}tSineCmd_ID;

typedef struct
{
    int32_t type;
	int32_t address;
    int32_t value;
}tWriteRegisterCmd_ID;


/* DSP engineer mode code */
/* chime */
#define CHIME_WAVETYPE_SINE 0
#define CHIME_WAVETYPE_TRIANGULAR 1
#define CHIME_RELEASE_LINEAR 0
#define CHIME_RELEASE_EXPONENTIAL 1
#define CHIME_REPEAT_CONTINOUS -1
#define CHIME_EFFECT_ADD 0
#define CHIME_EFFECT_MULTIPLY 1
#define PDC_NON_CONTINOUS 0
#define PDC_CONTINOUS 1
#define PDC_NUMBER_1 0x001
#define PDC_NUMBER_2 0x002

#if (DSP_ENGINEER_CODE_SWITCH == 1)
typedef struct{
    /* frequency */
    int32_t frequency;

    int32_t wave_type;

    /* clip */
    int32_t clip;

    /* output amplitude */
    int32_t amplitude;

    /* sequencer */
    int32_t slot_time;

} chime_slots_parameter;

typedef struct{
    /* slots */
    //chime_slots_parameter slots_data[4];

    int32_t slots_enable;

    /* master volume */
    int32_t master_volume;

    /* crossfader */
    int32_t crossfader_time;

    /* Linear attack release */
    int32_t attack;
    int32_t release;

    int32_t release_type;

    /* sequencer */
    int32_t TA_time;
    int32_t TB_time;

    /* repetitions: -1 means forever */
    int32_t repetitions;

    /* not used */
    int32_t no_TB_continous;

    /* chime change threshold */
    int32_t change_threshold;

    /* chime decay threshold */
    int32_t decay_threshold;

    /* effect generator */
    int32_t Tsq_c02_Bk;
    int32_t Tsq_c1_Bk;
    int32_t Tsq_c3_Bk;

    int32_t effect;

    int32_t additional_boost;

    int32_t fs;
} chime_parameter;

typedef struct{
    /* pdc number */
    int32_t pdc_number;

    /* amplitude */
    int32_t amplitude;

    /* frequency */
    int32_t f;

    /* Linear attack release */
    int32_t attack;
    int32_t release;

    /* sequencer */
    int32_t TA_time;
    int32_t TB_time;

    /* clip */
    int32_t clip;

    /* superposition */
    int32_t fl;
    int32_t fr;
    int32_t rl;
    int32_t rr;

    /* chime fade out */
    int32_t tcfo;

    int32_t fs;

    int32_t continous;
} pdc_parameter;
#endif

/* DSP engineer mode code */

/******************************************************************************
**    FUNCTION DEFINITIONS
******************************************************************************/
INT32 dsp_project_init(int argc, char **argv);
INT32 dsp_project_deinit(void);
INT32 dsp_project_ioctl(int cmd, void *params);


#endif /*__DSP_DRIVER_PROJ_H__*/
