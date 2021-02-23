#ifndef _SID_0X22_CFG_H
#define _SID_0X22_CFG_H
#include"Std_Types.h"
#include"diagCommon_cfg.h"
#include"sid0x22_if.h"
#include"MemMap.h"
#include"SWVersion.h"
#include"diagAppWllr_cfg.h"

//*************************************************************************************************
#define PROGRAMMING_FINGERPRINT_BACKUP_MAX       5
#define HSAE_CA004	0x08
#define HSAE_CA005	0x0A
#define HSAE_CA002	0x0C
#define HSAE_CA003	0x0E
#define HSAE_CA008	0x0F


//********************************************** IVI **********************************************
#define IVI_DID_UDS_INNER_TEST                         0x1234
#define IVI_DID_UDS_INNER_TEST_LEN                     4
//----------------------------------------------------------------------------------------------
#define IVI_DID_SPARE_PARTS_NUM_NISSAN                 0xF1A1
#define IVI_DID_SPARE_PARTS_NUM_NISSAN_LEN             10
//----------------------------------------------------------------------------------------------
//#define IVI_DID_SPARE_PARTS_NUM_RENAULT                0xF187
//#define IVI_DID_SPARE_PARTS_NUM_RENAULT_LEN            10
//----------------------------------------------------------------------------------------------
#define IVI_DID_SYS_SUPPLIER_NUM                       0xF18A
#define IVI_DID_SYS_SUPPLIER_NUM_LEN                   4
//----------------------------------------------------------------------------------------------
#define IVI_DID_ECU_SERIAL_NUM                         0xF18C
#define IVI_DID_ECU_SERIAL_NUM_LEN                     20
//----------------------------------------------------------------------------------------------
#define IVI_DID_VIN                                    0xF190
#define IVI_DID_VIN_LEN                                17
//----------------------------------------------------------------------------------------------
#define IVI_DID_VEHICLE_MANUF_ECU_HW_NUM               0xF191
#define IVI_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN           10
//----------------------------------------------------------------------------------------------
#define IVI_DID_SYS_SUPPLIER_ECU_SW_NUM                0xF194
#define IVI_DID_SYS_SUPPLIER_ECU_SW_NUM_LEN            ((IOC_SOFTWARE_VERSION_LEN-1)+2)    //maximum is 32
//----------------------------------------------------------------------------------------------
#define IVI_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM        0xF195
#define IVI_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN    ((IOC_SOFTWARE_VERSION_LEN-1)+2)    //maximum is 32
//----------------------------------------------------------------------------------------------
#define IVI_DID_DIAG_ID_CODE                           0xF1A0
#define IVI_DID_DIAG_ID_CODE_LEN                       1
//----------------------------------------------------------------------------------------------
#define IVI_DID_OPERATIONAL_REFERENCE                  0xF012
#define IVI_DID_OPERATIONAL_REFERENCE_LEN              10
//----------------------------------------------------------------------------------------------
#define IVI_DID_CAN_DIAG_MUX_STATE                     0xEF82
#define IVI_DID_CAN_DIAG_MUX_STATE_LEN                 136
//----------------------------------------------------------------------------------------------
#define IVI_DID_INPUT_LINES                            0x0101
#define IVI_DID_INPUT_LINES_LEN                        2
//----------------------------------------------------------------------------------------------
#define IVI_DID_TCU_ACC                                0x0102
#define IVI_DID_TCU_ACC_LEN                            1
//----------------------------------------------------------------------------------------------
#define IVI_DID_BATTERY_LEVEL                          0x0200
#define IVI_DID_BATTERY_LEVEL_LEN                      1
//----------------------------------------------------------------------------------------------
#define IVI_DID_SPEED_INFORMATION                      0x0201
#define IVI_DID_SPEED_INFORMATION_LEN                  2



//********************************************** IC ***********************************************
#define IC_DID_UDS_INNER_TEST                          0x1234
#define IC_DID_UDS_INNER_TEST_LEN                      4
//----------------------------------------------------------------------------------------------
#define IC_DID_SID0X22_SUPPORTED_DID                   0xDFDF
#define IC_DID_SID0X22_SUPPORTED_DID_LEN               124         //2*62
//----------------------------------------------------------------------------------------------
#define IC_DID_SPARE_PARTS_NUM_NISSAN                  0xF1A1
#define IC_DID_SPARE_PARTS_NUM_NISSAN_LEN              10
//----------------------------------------------------------------------------------------------
//#define IC_DID_SPARE_PARTS_NUM_RENAULT                 0xF187
//#define IC_DID_SPARE_PARTS_NUM_RENAULT_LEN             10
//----------------------------------------------------------------------------------------------
#define IC_DID_DIAG_ID_CODE                            0xF1A0
#define IC_DID_DIAG_ID_CODE_LEN                        1
//----------------------------------------------------------------------------------------------
#define IC_DID_SYS_SUPPLIER_NUM                        0xF18A
#define IC_DID_SYS_SUPPLIER_NUM_LEN                    4
//----------------------------------------------------------------------------------------------
#define IC_DID_SYS_SUPPLIER_ECU_SW_NUM                 0xF194
#define IC_DID_SYS_SUPPLIER_ECU_SW_NUM_LEN             IOC_SOFTWARE_VERSION_ROM_HEX_LEN        //maximum is 32
//----------------------------------------------------------------------------------------------
#define IC_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM         0xF195
#define IC_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN     IOC_SOFTWARE_VERSION_LEN                //maximum is 32
//----------------------------------------------------------------------------------------------
#define IC_DID_LATEST_SPARE_PARTS_NUM                  0xF012
#define IC_DID_LATEST_SPARE_PARTS_NUM_LEN              10
//----------------------------------------------------------------------------------------------
#define IC_DID_SPEED_OMETER                            0xF40D
#define IC_DID_SPEED_OMETER_LEN                        2
//----------------------------------------------------------------------------------------------
#define IC_DID_SPEED_OUTPUT                            0x0E00
#define IC_DID_SPEED_OUTPUT_LEN                        2
//----------------------------------------------------------------------------------------------
#define IC_DID_ODOMETER_OUTPUT                         0x0E01
#define IC_DID_ODOMETER_OUTPUT_LEN                     3
//----------------------------------------------------------------------------------------------
#define IC_DID_ODOMETER                                0x0E02
#define IC_DID_ODOMETER_LEN	                           2
//----------------------------------------------------------------------------------------------
#define IC_DID_FUEL_METER                              0x0E03
#define IC_DID_FUEL_METER_LEN	                       1
//----------------------------------------------------------------------------------------------
#define IC_DID_WATER_TEMPERATURE                       0x0E04
#define IC_DID_WATER_TEMPERATURE_LEN		           1
//----------------------------------------------------------------------------------------------
#define IC_DID_0E07_INFO                               0x0E07
#define IC_DID_0E07_INFO_LEN		                   19
//----------------------------------------------------------------------------------------------
#define IC_DID_DISTANCE_TO_EMPTY                       0x0E08
#define IC_DID_DISTANCE_TO_EMPTY_LEN                   2
//----------------------------------------------------------------------------------------------
#define IC_DID_AMBIENT_TEMPERATURE                     0x0E09
#define IC_DID_AMBIENT_TEMPERATURE_LEN		           1
//----------------------------------------------------------------------------------------------
#define IC_DID_0E0A_INFO                               0x0E0A
#define IC_DID_0E0A_INFO_LEN                           3
//----------------------------------------------------------------------------------------------
#define IC_DID_GENERAL_POWER_METER                     0x0E0B
#define IC_DID_GENERAL_POWER_METER_LEN		           1
//----------------------------------------------------------------------------------------------
#define IC_DID_SOC_METER                               0x0E0C
#define IC_DID_SOC_METER_LEN	                       1
//----------------------------------------------------------------------------------------------
#define IC_DID_VOLT_METER                              0x0E0D
#define IC_DID_VOLT_METER_LEN	                       1
//----------------------------------------------------------------------------------------------
#define IC_DID_BRAKE_PAD                               0x0E0E
#define IC_DID_BRAKE_PAD_LEN				           2
//----------------------------------------------------------------------------------------------
#define IC_DID_0E0F_INFO                               0x0E0F
#define IC_DID_0E0F_INFO_LEN		                   7
//----------------------------------------------------------------------------------------------
#define IC_DID_ENG_SHUT_TIMER                          0x0E10
#define IC_DID_ENG_SHUT_TIMER_LEN		               2
//----------------------------------------------------------------------------------------------
#define IC_DID_TORQUE_DISTRIBUTION                     0x0E11
#define IC_DID_TORQUE_DISTRIBUTION_LEN	               1
//----------------------------------------------------------------------------------------------
#define IC_DID_0E12_INFO                               0x0E12
#define IC_DID_0E12_INFO_LEN                           7
//----------------------------------------------------------------------------------------------
#define IC_DID_CC_SL_REQ                               0x0E13
#define IC_DID_CC_SL_REQ_LEN			               1
//----------------------------------------------------------------------------------------------
#define IC_DID_SCR_REMAINING                           0x0E14
#define IC_DID_SCR_REMAINING_LEN                       1
//----------------------------------------------------------------------------------------------
#define IC_DID_SCR_DISTANCE                            0x0E15
#define IC_DID_SCR_DISTANCE_LEN                        2
//----------------------------------------------------------------------------------------------
#define IC_DID_OIL_DISTANCE                            0x0E16
#define IC_DID_OIL_DISTANCE_LEN                        2
//----------------------------------------------------------------------------------------------
#define IC_DID_OIL_RESET                               0x0E17
#define IC_DID_OIL_RESET_LEN                           1
//----------------------------------------------------------------------------------------------
#define IC_DID_ABSOLUTE_TIME                           0x0E18
#define IC_DID_ABSOLUTE_TIME_LEN                       2
//----------------------------------------------------------------------------------------------
#define IC_DID_0E19_INFO                               0x0E19
#define IC_DID_0E19_INFO_LEN                           1
//----------------------------------------------------------------------------------------------
#define IC_DID_ASST_CHG                                0x0E1A
#define IC_DID_ASST_CHG_LEN                            1
//----------------------------------------------------------------------------------------------
#define IC_DID_EV_INDICATOR                            0x0E1B
#define IC_DID_EV_INDICATOR_LEN                        1
//----------------------------------------------------------------------------------------------
#define IC_DID_APO_DISPLAY                             0x0E1C
#define IC_DID_APO_DISPLAY_LEN                         1
//----------------------------------------------------------------------------------------------
#define IC_DID_SOC_DISPLAY                             0x0E1D
#define IC_DID_SOC_DISPLAY_LEN                         1
//----------------------------------------------------------------------------------------------
#define IC_DID_ECO_APO                                 0x0E1E
#define IC_DID_ECO_APO_LEN                             1
//----------------------------------------------------------------------------------------------
#define IC_DID_REGEN_BRAKE_TRQ                         0x0E1F
#define IC_DID_REGEN_BRAKE_TRQ_LEN                     1
//----------------------------------------------------------------------------------------------
#define IC_DID_MECHA_BRAKE_TRQ                         0x0E20
#define IC_DID_MECHA_BRAKE_TRQ_LEN                     1
//----------------------------------------------------------------------------------------------
#define IC_DID_ENERGY_FLOW                             0x0E21
#define IC_DID_ENERGY_FLOW_LEN                         2
//----------------------------------------------------------------------------------------------
#define IC_DID_0E22_INFO                               0x0E22
#define IC_DID_0E22_INFO_LEN                           1
//----------------------------------------------------------------------------------------------
#define IC_DID_OIL_DIST_K9K                            0x0E23
#define IC_DID_OIL_DIST_K9K_LEN                        2
//----------------------------------------------------------------------------------------------
#define IC_DID_0E24_INFO                               0x0E24
#define IC_DID_0E24_INFO_LEN                           10
//----------------------------------------------------------------------------------------------
#define IC_DID_TIRE_PRESS_FR                           0x0E25
#define IC_DID_TIRE_PRESS_FR_LEN                       1
//----------------------------------------------------------------------------------------------
#define IC_DID_TIRE_PRESS_FL                           0x0E26
#define IC_DID_TIRE_PRESS_FL_LEN                       1
//----------------------------------------------------------------------------------------------
#define IC_DID_TIRE_PRESS_RR                           0x0E27
#define IC_DID_TIRE_PRESS_RR_LEN                       1
//----------------------------------------------------------------------------------------------
#define IC_DID_TIRE_PRESS_RL                           0x0E28
#define IC_DID_TIRE_PRESS_RL_LEN                       1
//----------------------------------------------------------------------------------------------
#define IC_DID_0E29_INFO                               0x0E29
#define IC_DID_0E29_INFO_LEN                           3
//----------------------------------------------------------------------------------------------
#define IC_DID_RAM_ERROR_DATA                          0x0E2A
#define IC_DID_RAM_ERROR_DATA_LEN                      1
//----------------------------------------------------------------------------------------------
#define IC_DID_0E2B_INFO                               0x0E2B
#define IC_DID_0E2B_INFO_LEN                           5
//----------------------------------------------------------------------------------------------
#define IC_DID_0E2C_INFO                               0x0E2C
#define IC_DID_0E2C_INFO_LEN                           13
//----------------------------------------------------------------------------------------------
#define IC_DID_DISPLAY_NONDISPLAY_INFO                 0x0E80
#define IC_DID_DISPLAY_NONDISPLAY_INFO_LEN             44
//----------------------------------------------------------------------------------------------
#define IC_DID_WARNING_LAMP_LIGHTING_RECORDS           0x0E90
#define IC_DID_WARNING_LAMP_LIGHTING_RECORDS_LEN       INNER_WL_ID_MAX
//----------------------------------------------------------------------------------------------
#define IC_DID_WARNING_LAMP_LIGHTING_MILEAGE           0xDA70
#define IC_DID_WARNING_LAMP_LIGHTING_MILEAGE_LEN       (INNER_WL_ID_MAX*3)
//----------------------------------------------------------------------------------------------
#define IC_DID_CAN_COMMUNICATION_SUPPORT_INFO          0xEF82
#define IC_DID_CAN_COMMUNICATION_SUPPORT_INFO_LEN      136
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_TOTAL_MILEAGE                       0xF0D0      //SnapShotData
#define IC_DID_SSD_TOTAL_MILEAGE_LEN                   3
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_BAT_VOLTAGE                         0xF0D1      //SnapShotData
#define IC_DID_SSD_BAT_VOLTAGE_LEN                     1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_IGN_VOLTAGE                         0xF0D2      //SnapShotData
#define IC_DID_SSD_IGN_VOLTAGE_LEN                     1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_AGING_COUNTER                       0xF0D3      //SnapShotData
#define IC_DID_SSD_AGING_COUNTER_LEN                   1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_VEHICLE_SPEED                       0xF0D4      //SnapShotData
#define IC_DID_SSD_VEHICLE_SPEED_LEN                   2
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_AGING_RESULT                        0x0104      //SnapShotData
#define IC_DID_SSD_AGING_RESULT_LEN                    1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_MEDIA_SOURCE                        0x0A01      //SnapShotData
#define IC_DID_SSD_MEDIA_SOURCE_LEN                    1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_CURRENT_SONG_NAME                   0x0A05      //SnapShotData
#define IC_DID_SSD_CURRENT_SONG_NAME_LEN               1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_BT_ADDRESS                  		   0x0404      //SnapShotData
#define IC_DID_SSD_BT_ADDRESS_LEN               	   1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_BT_CONNECT_STATE                    0x0405      //SnapShotData
#define IC_DID_SSD_BT_CONNECT_STATE_LEN                1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_BT_CALL_STATE                       0x0406      //SnapShotData
#define IC_DID_SSD_BT_CALL_STATE_LEN                   1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_WIFI_SIGNAL_NUM                     0x0304      //SnapShotData
#define IC_DID_SSD_WIFI_SIGNAL_NUM_LEN                 1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_WIFI_SIGNAL_INTENSITY               0x0307      //SnapShotData
#define IC_DID_SSD_WIFI_SIGNAL_INTENSITY_LEN           1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_WIFI_CONNECT_STATE                  0x0308      //SnapShotData
#define IC_DID_SSD_WIFI_CONNECT_STATE_LEN              1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_WIFI_CONNECT_NUM                    0x030C      //SnapShotData
#define IC_DID_SSD_WIFI_CONNECT_NUM_LEN                1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_WIFI_GPS_INFO                       0x0603      //SnapShotData
#define IC_DID_SSD_WIFI_GPS_INFO_LEN                   1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_WIFI_GPS_DETAIL                     0x060D      //SnapShotData
#define IC_DID_SSD_WIFI_GPS_DETAIL_LEN                 1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_SYS_VERSION                         0x0701      //SnapShotData
#define IC_DID_SSD_SYS_VERSION_LEN                     1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_SYS_AUDIO_VOLUME                    0x0704      //SnapShotData
#define IC_DID_SSD_SYS_AUDIO_VOLUME_LEN                1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_SYS_UUID		                       0x0705      //SnapShotData
#define IC_DID_SSD_SYS_UUID_LEN                		   20
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_SYS_WIRELESS_CHARGE_STATE		   0x0708      //SnapShotData
#define IC_DID_SSD_SYS_WIRELESS_CHARGE_STATE_LEN       1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_SYS_STOP_SIGNAL		   			   0x0709      //SnapShotData
#define IC_DID_SSD_SYS_STOP_SIGNAL_LEN                 1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_SYS_STEER_SIGNAL		   			   0x070A      //SnapShotData
#define IC_DID_SSD_SYS_STEER_SIGNAL_LEN                1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_SYS_REVERSE_SIGNAL		   		   0x070B      //SnapShotData
#define IC_DID_SSD_SYS_REVERSE_SIGNAL_LEN              1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_SYS_EQPIN		   		   		   0x0711      //SnapShotData
#define IC_DID_SSD_SYS_EQPINL_LEN                      1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_VOICE_ASSIS_PAGE		   		       0x0F01      //SnapShotData
#define IC_DID_SSD_VOICE_ASSIS_PAGE_LEN                1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_TCU_CONNECT_STATE		   		   0x0501      //SnapShotData
#define IC_DID_SSD_TCU_CONNECT_STATE_LEN               1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_TCU_NET_CONNECT_STATE		   	   0x0502      //SnapShotData
#define IC_DID_SSD_TCU_NET_CONNECT_STATE_LEN           1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_TCU_4G_SIGNAL_INTENSITY		   	   0x0503      //SnapShotData
#define IC_DID_SSD_TCU_4G_SIGNAL_INTENSITY_LEN         1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_MAIN_VERSION					   	   0x0001      //SnapShotData
#define IC_DID_SSD_MAIN_VERSION_LEN         		   1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_HARDWARE_VERSION					   0x0002      //SnapShotData
#define IC_DID_SSD_HARDWARE_VERSION_LEN         	   1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_MCU_VERSION					   	   0x0003      //SnapShotData
#define IC_DID_SSD_MCU_VERSION_LEN         		       1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_SOC_VERSION					       0x0004      //SnapShotData
#define IC_DID_SSD_SOC_VERSION_LEN         	           1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_MCAN_VERSION					   	   0x0005      //SnapShotData
#define IC_DID_SSD_MCAN_VERSION_LEN         	       1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_BT_SW_VERSION					   0x0006      //SnapShotData
#define IC_DID_SSD_BT_SW_VERSION_LEN         		   1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_GPS_VERSION					       0x0007      //SnapShotData
#define IC_DID_SSD_GPS_VERSION_LEN         	           1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_VR_VERSION					       0x0008      //SnapShotData
#define IC_DID_SSD_VR_VERSION_LEN         	           1
//----------------------------------------------------------------------------------------------
#define IC_DID_SSD_AVM_VERSION					       0x0009      //SnapShotData
#define IC_DID_SSD_AVM_VERSION_LEN         	           1






//********************************************** AVM **********************************************
#define AVM_DID_UDS_INNER_TEST                         0x1234
#define AVM_DID_UDS_INNER_TEST_LEN                     4
//----------------------------------------------------------------------------------------------
#define AVM_DID_PRODUCTION_LINE				           0x0101
#define AVM_DID_PRODUCTION_LINE_LEN       			   1
//----------------------------------------------------------------------------------------------
#define AVM_DID_DATA_MONITOR_SERVICE_REQUEST           0x0102
#define AVM_DID_DATA_MONITOR_SERVICE_REQUEST_LEN       4
//----------------------------------------------------------------------------------------------
#define AVM_DID_CALIBRATION_STATE                      0x0110
#define AVM_DID_CALIBRATION_STATE_LEN                  2
//----------------------------------------------------------------------------------------------
#define AVM_DID_PROGRAMING_DATE                        0xF199
#define AVM_DID_PROGRAMING_DATE_LEN                    4
//----------------------------------------------------------------------------------------------
#define AVM_DID_CAN_DIAG_MUX_STATE                     0xEF82
#define AVM_DID_CAN_DIAG_MUX_STATE_LEN                 136
//----------------------------------------------------------------------------------------------
#define AVM_DID_SPARE_PARTS_NUM_NISSAN                 0xF1A1
#define AVM_DID_SPARE_PARTS_NUM_NISSAN_LEN             10
//----------------------------------------------------------------------------------------------
#define AVM_DID_VEHICLE_MANUF_ECU_HW_NUM               0xF191
#define AVM_DID_VEHICLE_MANUF_ECU_HW_NUM_LEN           10
//----------------------------------------------------------------------------------------------
#define AVM_DID_OPERATIONAL_REFERENCE                  0xF012
#define AVM_DID_OPERATIONAL_REFERENCE_LEN              10
//----------------------------------------------------------------------------------------------
#define AVM_DID_SYS_SUPPLIER_NUM                       0xF18A
#define AVM_DID_SYS_SUPPLIER_NUM_LEN                   4
//----------------------------------------------------------------------------------------------
#define AVM_DID_ECU_SERIAL_NUM                         0xF18C
#define AVM_DID_ECU_SERIAL_NUM_LEN                     20
//----------------------------------------------------------------------------------------------
#define AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM        0xF195
#define AVM_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN    4
//----------------------------------------------------------------------------------------------
#define AVM_DID_DIAG_ID_CODE                           0xF1A0
#define AVM_DID_DIAG_ID_CODE_LEN                       1
//----------------------------------------------------------------------------------------------
#define AVM_DID_BOOT_VERSION                           0xF180
#define AVM_DID_BOOT_VERSION_LEN                       9
//----------------------------------------------------------------------------------------------
#define AVM_DID_ENCRYPT_IC_STATUS                      0xFD04
#define AVM_DID_ENCRYPT_IC_STATUS_LEN                  2
//----------------------------------------------------------------------------------------------
#define AVM_DID_ORIGINAL_STATUS                        0xFD05
#define AVM_DID_ORIGINAL_STATUS_LEN                    3



//********************************************** HUD **********************************************
#define HUD_DID_SPARE_PARTS_NUM_NISSAN                 0xF1A1
#define HUD_DID_SPARE_PARTS_NUM_NISSAN_LEN             10
//----------------------------------------------------------------------------------------------
#define HUD_DID_DIAG_ID_CODE                  		   0xF1A0
#define HUD_DID_DIAG_ID_CODE_LEN              		   10
//----------------------------------------------------------------------------------------------
#define HUD_DID_SYS_SUPPLIER_NUM                  	   0xF18A
#define HUD_DID_SYS_SUPPLIER_NUM_LEN              	   4
//----------------------------------------------------------------------------------------------
#define HUD_DID_SYS_SUPPLIER_ECU_SW_NUM                0xF194
#define HUD_DID_SYS_SUPPLIER_ECU_SW_NUM_LEN            IOC_SOFTWARE_VERSION_ROM_HEX_LEN        //maximum is 32
//----------------------------------------------------------------------------------------------
#define HUD_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM        0xF195
#define HUD_DID_SYS_SUPPLIER_ECU_SW_VERSION_NUM_LEN    IOC_SOFTWARE_VERSION_LEN                //maximum is 32

//----------------------------------------------------------------------------------------------
#define HUD_DID_OPERATIONAL_REFERENCE                  0xF012
#define HUD_DID_OPERATIONAL_REFERENCE_LEN              10
//----------------------------------------------------------------------------------------------
#define HUD_DID_INI_CALIBRATION_DATA                   0x01A1
#define HUD_DID_INI_CALIBRATION_DATA_LEN               180
//----------------------------------------------------------------------------------------------
#define HUD_DID_NEW_CALIBRATION_DATA                   0x01A2
#define HUD_DID_NEW_CALIBRATION_DATA_LEN               180
//----------------------------------------------------------------------------------------------
#define HUD_DID_MAIN_SW_ONOFF_USER1                    0x0111
#define HUD_DID_MAIN_SW_ONOFF_USER1_LEN                1
//----------------------------------------------------------------------------------------------
#define HUD_DID_MAIN_SW_ONOFF_USER2                    0x0121
#define HUD_DID_MAIN_SW_ONOFF_USER2_LEN                1
//----------------------------------------------------------------------------------------------
#define HUD_DID_MAIN_SW_ONOFF_USER3                    0x0131
#define HUD_DID_MAIN_SW_ONOFF_USER3_LEN                1
//----------------------------------------------------------------------------------------------
#define HUD_DID_MAIN_SW_ONOFF_USER4                    0x0141
#define HUD_DID_MAIN_SW_ONOFF_USER4_LEN                1
//----------------------------------------------------------------------------------------------
#define HUD_DID_MAIN_SW_ONOFF_USER5                    0x0151
#define HUD_DID_MAIN_SW_ONOFF_USER5_LEN                1
//----------------------------------------------------------------------------------------------
#define HUD_DID_MAIN_SW_ONOFF_USERDEFAULT              0x0161
#define HUD_DID_MAIN_SW_ONOFF_USERDEFAULT_LEN          1





//********************************************************************************************************
//                                                 IVI
//********************************************************************************************************
uint16 IVI_sid0x22ReadUdsInnerTestDid(uint32 ch, uint8* respData);
//uint16 IVI_sid0x22ReadSparePartsNumNissan(uint32 ch, uint8* respData);
//tUdsNRCType IVI_sid0x2eWriteSparePartsNumNissan(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
uint16 IVI_sid0x22ReadSparePartsNumRenault(uint32 ch, uint8* respData);
tUdsNRCType IVI_sid0x2eWriteSparePartsNumRenault(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
uint16 IVI_sid0x22ReadSysSupplierNum(uint32 ch, uint8* respData);
tUdsNRCType IVI_sid0x2eWriteSysSupplierNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
tUdsNRCType IVI_sid0x2eWriteVin(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
uint16 IVI_sid0x22ReadEcuSerialNum(uint32 ch, uint8* respData);
tUdsNRCType IVI_sid0x2eWriteEcuSerialNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
uint16 IVI_sid0x22ReadVehicleManufEcuHwNum(uint32 ch, uint8* respData);
tUdsNRCType IVI_sid0x2eWriteVehicleManufEcuHwNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
uint16 IVI_sid0x22ReadSysSupplierEcuSwNum(uint32 ch, uint8* respData);
//tUdsNRCType IVI_sid0x2eWriteSysSupplierEcuSwNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
uint16 IVI_sid0x22ReadSysSupplierEcuSwVersionNum(uint32 ch, uint8* respData);
//tUdsNRCType IVI_sid0x2eWriteSysSupplierEcuSwVersionNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
uint16 IVI_sid0x22ReadDiagIdCode(uint32 ch, uint8* respData);
uint16 IVI_sid0x22ReadOperationalReference(uint32 ch, uint8* respData);
uint16 IVI_sid0x22ReadCanDiagMuxState(uint32 ch, uint8* respData);
uint16 IVI_sid0x22ReadInputLines(uint32 ch, uint8* respData);
uint16 IVI_sid0x22ReadTcuAcc(uint32 ch, uint8* respData);
uint16 IVI_sid0x22ReadBatteryLevel(uint32 ch, uint8* respData);
uint16 IVI_sid0x22ReadSpeedInformation(uint32 ch, uint8* respData);


//********************************************************************************************************
//                                                 IC
//********************************************************************************************************
uint16 IC_sid0x22ReadUdsInnerTestDid(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadSupportedDid(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadSparePartsNumNissan(uint32 ch, uint8* respData);
tUdsNRCType IC_sid0x2eWriteSparePartsNumNissan(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
//uint16 IC_sid0x22ReadSparePartsNumRenault(uint32 ch, uint8* respData);
//tUdsNRCType IC_sid0x2eWriteSparePartsNumRenault(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
uint16 IC_sid0x22ReadDiagIdCode(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadSysSupplierNum(uint32 ch, uint8* respData);
tUdsNRCType IC_sid0x2eWriteSysSupplierNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
uint16 IC_sid0x22ReadSysSupplierSwNum(uint32 ch, uint8* respData);
//tUdsNRCType IC_sid0x2eWriteSysSupplierSwNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
uint16 IC_sid0x22ReadSysSupplierSwVersionNum(uint32 ch, uint8* respData);
//tUdsNRCType IC_sid0x2eWriteSysSupplierSwVersionNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
uint16 IC_sid0x22ReadLatestSparePartsNum(uint32 ch, uint8* respData);
tUdsNRCType IC_sid0x2eWriteLatestSparePartsNum(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
uint16 IC_sid0x22ReadSpeedOmeter(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadSpeedOutput(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadOdometerOutput(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadOdometer(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadFuelMeter(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadWaterTemperature(uint32 ch, uint8* respData);
//uint16 IC_sid0x22ReadDid0E07Info(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadDistanceToEmpty(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadAmbientTemperature(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadDid0E0AInfo(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadGeneralPowerMeter(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadSocMeter(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadVoltMeter(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadBrakePad(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadDid0E0FInfo(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadEngShutTimer(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadTorqueDistribution(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadDid0E12Info(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadCcSlReq(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadScrRemaining(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadScrDistance(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadOilDistance(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadOilReset(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadAbsoluteTime(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadDid0E19Info(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadAsstChg(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadEvIndicator(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadApoDisplay(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadSocDisplay(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadEcoApo(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadRegenBrakeTrq(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadMechaBrakeTrq(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadEnergyFlow(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadDid0E22Info(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadOilDistK9k(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadDid0E24Info(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadTirePressFr(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadTirePressFl(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadTirePressRr(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadTirePressRl(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadDid0E29Info(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadRamErrorData(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadDid0E2BInfo(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadDid0E2CInfo(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadDisplayNondisplayInfo(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadWarningLampLightingRecords(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadWarningLampLightingMileage(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadCanCommunicationSupportInfo(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadSsdTotalMileage(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadSsdBatVoltage(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadSsdIgnVoltage(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadSsdAgingCounter(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadSsdVehicleSpeed(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadUUID(uint32 ch, uint8* respData);
uint16 IC_sid0x22ReadEQPIN(uint32 ch, uint8* respData);

//********************************************************************************************************
//                                                 AVM
//********************************************************************************************************
uint16 AVM_sid0x22ReadUdsInnerTestDid(uint32 ch, uint8* respData);
uint16 AVM_sid0x22ReadProductionLine(uint32 ch, uint8* respData);
tUdsNRCType AVM_sid0x2eWriteProductionLine(uint32 ch, uint16 didNum, const uint8* value, uint16 len);
uint16 AVM_sid0x22ReadDataMonitorServiceRequest(uint32 ch, uint8* respData);
uint16 AVM_sid0x22ReadCalibrationState(uint32 ch, uint8* respData);
uint16 AVM_sid0x22ReadProgramingDate(uint32 ch, uint8* respData);
uint16 AVM_sid0x22ReadCanDiagMuxState(uint32 ch, uint8* respData);
uint16 AVM_sid0x22ReadSparePartsNumNissan  (uint32 ch, uint8* respData);
//uint16 AVM_sid0x22ReadVehicleManufEcuHwNum(uint32 ch, uint8* respData);
uint16 AVM_sid0x22ReadOperationalReference(uint32 ch, uint8* respData);
uint16 AVM_sid0x22ReadSysSupplierNum(uint32 ch, uint8* respData);
uint16 AVM_sid0x22ReadEcuSerialNum(uint32 ch, uint8* respData);
uint16 AVM_sid0x22ReadSysSupplierEcuSwVersionNum(uint32 ch, uint8* respData);
uint16 AVM_sid0x22ReadDiagIdCode(uint32 ch, uint8* respData);
uint16 AVM_sid0x22ReadEncryptICStatus(uint32 ch, uint8* respData);
uint16 AVM_sid0x22ReadOriginalStatus(uint32 ch, uint8* respData);
//uint16 AVM_sid0x22ReadBootVersion(uint32 ch, uint8* respData);

//********************************************************************************************************
//                                                 AVM
//********************************************************************************************************
tUdsNRCType HUD_sid0x2eWriteMainSWONOFF(uint32 ch, uint16 didNum, const uint8* value, uint16 len);

tUdsNRCType HUD_sid0x2eWriteCalibrationData(uint32 ch, uint16 didNum, const uint8* value, uint16 len);


extern NVM_SEC_VAR_UNSPECIFIED uint8 g_AVM_ProductionLine[AVM_DID_PRODUCTION_LINE_LEN];   //define in sid0x22_cfg.c line 37
extern NVM_SEC_VAR_UNSPECIFIED uint8 g_IVI_VIN[IVI_DID_VIN_LEN];                          //define in sid0x22_cfg.c line 20
#endif
