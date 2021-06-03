/*
 * @Author: xjw 
 * @email: fudi.xu@tuya.com
 * @LastEditors: xjw 
 * @file name: tuya_dp_process.h
 * @Description: The main application function triggered by dp data header file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-02-22 15:10:00
 * @LastEditTime: 2021-05-17 15:10:00
 */

#ifndef __TUYA_DP_PROCESS_H__
#define __TUYA_DP_PROCESS_H__
/* Includes ------------------------------------------------------------------*/
#include "tuya_cloud_types.h"
#include "tuya_iot_com_api.h"
#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/

typedef UINT8_T DP_ID_T;


typedef struct {
    DP_ID_T dp_id;
    
    bool power;
}DP_BOOL_T;

typedef struct {
    DP_ID_T dp_id;
    
    INT_T value;
}DP_VALUE_T;

typedef struct {
    bool key_notice;
    
    UINT16_T key_value;
    UINT8_T  timer_hour;
    uint32_t last_time_count;
    uint32_t temp_time_count;
    UINT8_T  display_lock;
}DP_KEY_T;


typedef struct {
    bool switch_bool;
    int  led_mode;
    int  relay_mode;
    int  temperature_value;
    bool shake_bool;
    int  set_temper_value;
    bool time_on_bool;
    bool time_off_bool;
    uint8_t  status;
}Data_Memory_T;


/* Exported constants --------------------------------------------------------*/              

extern DP_BOOL_T switch_s;
extern DP_VALUE_T led_s;
extern DP_VALUE_T mode_s;
extern DP_VALUE_T temper_s;
extern DP_BOOL_T shake_s;
extern DP_VALUE_T set_temper_s;
extern DP_BOOL_T time_to_open_s;
extern DP_BOOL_T time_to_close_s;
extern DP_VALUE_T shutdown_time_s;
extern Data_Memory_T dp_memory_s;
extern DP_KEY_T dev_key_s;
extern SEM_HANDLE g_temper_binsemap;
extern SEM_HANDLE g_key_trigger_binsemap;
/* Exported macro ------------------------------------------------------------*/

#define DP_SWITCH           1
#define DP_TEMPER           3
#define DP_SHAKE            8
#define DP_MODE             5
#define DP_LED              101
#define DP_SET_TEMP         2
#define DP_TIME_ON          102
#define DP_TIME_OFF         103
#define DP_SHUTDOWN_TIME    104
/* Exported functions ------------------------------------------------------- */
VOID_T deal_dp_proc(IN CONST TY_OBJ_DP_S *root);
VOID_T calorifier_init();
VOID_T report_one_dp_status(int dp_id);
INT_T opUserFlashWriteAppData(IN Data_Memory_T *pData);
VOID_T time_off_handle(IN int value);
VOID_T time_open_handle(IN int value);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TUYA_DP_PROCESS */
