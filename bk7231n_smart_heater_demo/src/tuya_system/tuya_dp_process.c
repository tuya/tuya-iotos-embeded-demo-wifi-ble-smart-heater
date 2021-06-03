/*
 * @file name: tuya_dp_process
 * @Descripttion: The main application function triggered by dp data
 * @Author: xjw
 * @email: fudi.xu@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-26 16:35:33
 * @LastEditors: xjw
 * @LastEditTime: 2021-05-10 15:21:28
 */
/* Includes ------------------------------------------------------------------*/
#include "uni_log.h"
#include "tuya_cloud_wifi_defs.h"
#include "tuya_os_adapter.h"
#include "tuya_cloud_error_code.h"
#include <string.h>
#include <stdlib.h>
/* Private includes ----------------------------------------------------------*/
#include "tuya_dp_process.h"
#include "led_color_set.h"
#include "tuya_device.h"
#include "timer.h"
#include "gpio_control.h"
#include "soc_flash.h"
#include "tm1650.h"
/* Private typedef -----------------------------------------------------------*/
DP_BOOL_T switch_s = {
    .dp_id = DP_SWITCH,
    .power = FALSE,
};
DP_VALUE_T led_s = {
	.dp_id = DP_LED,
	.value = 0,
};
DP_VALUE_T mode_s = {
    .dp_id = DP_MODE,
    .value = 0,
};
DP_VALUE_T temper_s = {
    .dp_id = DP_TEMPER,
    .value = 0,
};
DP_BOOL_T shake_s = {
    .dp_id = DP_SHAKE,
    .power = FALSE,
};
DP_VALUE_T set_temper_s = {
    .dp_id = DP_SET_TEMP,
    .value = 0,
};
DP_BOOL_T time_to_open_s = {
    .dp_id = DP_TIME_ON,
    .power = FALSE,
};
DP_BOOL_T time_to_close_s = {
    .dp_id = DP_TIME_OFF,
    .power = FALSE,
};
DP_VALUE_T shutdown_time_s = {
    .dp_id = DP_SHUTDOWN_TIME,
    .value = 0,
};
DP_KEY_T dev_key_s = {
    .key_notice = 0,
    .key_value  = 0,
    .timer_hour = 0,
    .last_time_count = 0,
    .temp_time_count = 0,
    .display_lock = 0,
};
Data_Memory_T dp_memory_s = {
    .switch_bool = 0,
    .led_mode = 0,
    .relay_mode = 0,
    .temperature_value = 0,
    .shake_bool = 0,
    .set_temper_value = 0,
    .time_on_bool = 0,
    .time_off_bool = 0,
    .status = 0,
};



/* Private define ------------------------------------------------------------*/
#define DIFFERENCE_LEVEL1   4
#define DIFFERENCE_LEVEL2   0

#define STATUS_BIT_LOW_AND_HIGH_1       0x18
#define STATUS_BIT_COOL_0               0xFE
#define STATUS_BIT_ION_AND_C_1          0x22
#define STATUS_BIT_ION_AND_C_AND_COOL_1 0x23
#define STATUS_BIT_SW_1                 0x40
#define STATUS_BIT_SW_0                 0xBF
#define STATUS_BIT_LOW_AND_HIGH_0       0xE7
#define STATUS_BIT_COOL_1               0x01
#define STATUS_BIT_LOW_1                0x10
#define STATUS_BIT_HIGH_1               0x08
#define STATUS_BIT_HIGH_AND_COOL_0      0xF6
#define STATUS_BIT_LOW_AND_COOL_0       0xEE


/* Private variables ---------------------------------------------------------*/
SEM_HANDLE g_temper_binsemap = NULL;
SEM_HANDLE g_key_trigger_binsemap = NULL;
/* Private function prototypes -----------------------------------------------*/
VOID_T user_switch_handle(IN BOOL_T bONOFF);
VOID_T shake_handle(IN BOOL_T bONOFF);
VOID_T rtos_semaphore_init(void);
VOID_T time_open_handle(IN int value);
VOID_T led_handle(IN int mode);
VOID_T relay_handle(IN int mode);
VOID_T time_off_handle(IN int value);
VOID_T report_one_dp_status(int dp_id);
VOID_T set_termper_handle(int set_value);
VOID_T Power_data_recovery(void);
/* Private functions ---------------------------------------------------------*/
/**
 * @Function: deal_dp_proc
 * @Description: Dp data trigger event execution function
 * @Input: IN CONST TY_OBJ_DP_S *root
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID_T deal_dp_proc(IN CONST TY_OBJ_DP_S *root)
{
    UCHAR_T dpid;
    dpid = root->dpid;
    
    switch (dpid){
    
    case DP_SWITCH: {
        user_switch_handle(root->value.dp_bool);
        
        }
        break;

    case DP_TEMPER: {

        }
        break;

    case DP_SHAKE: {
        shake_handle(root->value.dp_bool);
        }
        break;

    case DP_MODE: {
        relay_handle(root->value.dp_enum);

        }
        break;

    case DP_LED: {

        led_handle(root->value.dp_value);
        
        }
        break;

    case DP_SET_TEMP: {
        set_termper_handle(root->value.dp_value);
        }
        break;

    case DP_TIME_ON: {
        time_open_handle(root->value.dp_value);
        }
        break;

    case DP_TIME_OFF: {
        time_off_handle(root->value.dp_value);
        }
        break;
    case DP_SHUTDOWN_TIME: {

        }
        break;
    default:
    
        break;
    }

}
/**
 * @Function: user_switch_handle
 * @Description: user switch handle function
 * @Input: IN BOOL_T bONOFF
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID_T user_switch_handle(IN BOOL_T bONOFF)
{
    BOOL_T bLastStatus ;
    int difference = 0;
    bLastStatus = switch_s.power;

    if( TRUE == bONOFF ) {
        switch_s.power = TRUE;
        //led_status_display
        if(dp_memory_s.status & STATUS_BIT_LOW_AND_HIGH_1) {
            dp_memory_s.status &= STATUS_BIT_COOL_0; 
            dp_memory_s.status |= STATUS_BIT_ION_AND_C_1; 
        }
        else {
            dp_memory_s.status |= STATUS_BIT_ION_AND_C_AND_COOL_1; 
        }
        
        fan_motor_set(switch_s.power);
        led_handle(dp_memory_s.led_mode);
        set_termper_handle(dp_memory_s.set_temper_value);
        difference = set_temper_s.value - temper_s.value;
        if(difference > 4) {
            relay_handle(2);
        }else if(difference > 0) {
            relay_handle(1);
        }else {
            relay_handle(0);
        }
        tuya_hal_semaphore_post(g_temper_binsemap);
    } 
    else {
        relay_handle(0);
        color_handle(0);
        shake_handle(0);
        tm1650_close();
        switch_s.power = FALSE;
        //clear timer close data
        dev_key_s.temp_time_count = 0;
        dev_key_s.timer_hour = 0;
        fan_motor_set(switch_s.power);
    }

    dp_memory_s.switch_bool = switch_s.power;
    PR_DEBUG("switch set:%d",switch_s.power);
    if(bONOFF == bLastStatus) {
        PR_DEBUG("the same switch set");
    }
    report_one_dp_status(DP_SWITCH);
    opUserFlashWriteAppData(&dp_memory_s);

}
/**
 * @Function: shake_handle
 * @Description: shake motor handle function
 * @Input: IN BOOL_T bONOFF
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID_T shake_handle(IN BOOL_T bONOFF)
{
    if(dp_memory_s.switch_bool){
    BOOL_T bLastStatus ;

    bLastStatus = shake_s.power;

    if( TRUE == bONOFF ) {
        shake_s.power = TRUE;
        dp_memory_s.status |= STATUS_BIT_SW_1;
    } else {
        shake_s.power = FALSE;
        dp_memory_s.status &= STATUS_BIT_SW_0;
    }
    PR_DEBUG("switch set:%d",shake_s.power);
    shake_motor_set(shake_s.power);
    if(bONOFF == bLastStatus) {
        PR_DEBUG("the same switch set");
    }
    dp_memory_s.shake_bool = shake_s.power;
        //upload dp data
    report_one_dp_status(DP_SHAKE);
    tuya_hal_semaphore_post(g_temper_binsemap);
    }
}
/**
 * @Function: relay_handle
 * @Description: Heating relay handle function
 * @Input: IN int mode
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID_T relay_handle(IN int mode)
{
    if(dp_memory_s.switch_bool) {
    int bLastStatus ;
    bLastStatus = mode_s.value;
    mode_s.value = mode;
    relay_set(mode_s.value);
    if(mode == bLastStatus) {
        PR_DEBUG("the same mode set");
    }
    dp_memory_s.relay_mode = mode_s.value;
    switch (dp_memory_s.relay_mode) {

    case 0: {
            dp_memory_s.status &= STATUS_BIT_LOW_AND_HIGH_0;
            dp_memory_s.status |= STATUS_BIT_COOL_1;
        }
        break;
    case 1: {
            dp_memory_s.status &= STATUS_BIT_HIGH_AND_COOL_0;
            dp_memory_s.status |= STATUS_BIT_LOW_1;
        }
        break;
    case 2: {
            dp_memory_s.status &= STATUS_BIT_LOW_AND_COOL_0;
            dp_memory_s.status |= STATUS_BIT_HIGH_1;
        }
        break;
        default:
        break;
    }
    //upload dp data
    report_one_dp_status(DP_MODE);
    tuya_hal_semaphore_post(g_temper_binsemap);
    }
}
/**
 * @Function: time_open_handle
 * @Description: Cloud timing on
 * @Input: IN int value
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID_T time_open_handle(IN int value)
{
    if(value == 1) {
    PR_DEBUG("TIMER_ON");
    user_switch_handle(TRUE);
    }
    time_to_open_s.power = 0;
    dp_memory_s.time_on_bool = time_to_open_s.power;
        //upload dp data
    report_one_dp_status(DP_TIME_ON);
}
/**
 * @Function: time_off_handle
 * @Description: Cloud timing off
 * @Input: IN int value
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID_T time_off_handle(IN int value)
{
    if(value == 1) {
    PR_DEBUG("TIMER_OFF");
    user_switch_handle(FALSE);
    led_handle(0);
    }
    time_to_close_s.power = 0;
    dp_memory_s.time_off_bool = time_to_close_s.power;
    report_one_dp_status(DP_TIME_OFF);
}
/**
 * @Function: led_handle
 * @Description: LED control 
 * @Input: IN int value
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID_T led_handle(IN int mode)
{

    if(switch_s.power == 1) {
    led_s.value = mode;
    dp_memory_s.led_mode = led_s.value;
    report_one_dp_status(DP_LED);
    color_handle(mode);
    opUserFlashWriteAppData(&dp_memory_s);
    }
}
/**
 * @Function: calorifier_init
 * @Description: calorifier init function
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID_T calorifier_init()
{
    motor_init();
    relay_init();
    led_init();
    b3950_init();
    thread_init();
    timer_init();
    rtos_semaphore_init();
    Power_data_recovery();
    key_interrupt_init();
    buzzer_init();
    tm1650_led_init();
    thread_init();
}
/**
 * @Function: rtos_semaphore_init
 * @Description: rtos_semaphore_init function
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID_T rtos_semaphore_init(void)
{
    int rt = OPRT_OK;

    rt = tuya_hal_semaphore_create_init(&g_temper_binsemap, 0, 1); 
    if (rt != OPRT_OK) {
        PR_ERR("Create Semaphore error!: %d", rt);
        return;
    }

    rt = tuya_hal_semaphore_create_init(&g_key_trigger_binsemap, 0, 1); 
    if (rt != OPRT_OK) {
        PR_ERR("Create Semaphore error!: %d", rt);
        return;
    }

}

/**
 * @Function: set_termper_handle
 * @Description:set_termper_handle
 * @Input: int set_value
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID_T set_termper_handle(int set_value)
{
    int difference = 0;
    difference = set_value - temper_s.value;
    if(difference > DIFFERENCE_LEVEL1) {
        relay_handle(2);
    }else if(difference > DIFFERENCE_LEVEL2) {
        relay_handle(1);
    }else {
        relay_handle(0);
    }
    set_temper_s.value = set_value;
    report_one_dp_status(DP_SET_TEMP);
    dp_memory_s.set_temper_value = set_temper_s.value;
    opUserFlashWriteAppData(&dp_memory_s);
}
/**
 * @Function: report_one_dp_status
 * @Description:update the dp data
 * @Input: int dp_id
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID_T report_one_dp_status(int dp_id)
{
    
    OPERATE_RET op_ret = OPRT_OK;
    GW_WIFI_NW_STAT_E wifi_state = 0xFF;
    op_ret = get_wf_gw_nw_status(&wifi_state);
    if (OPRT_OK != op_ret) {
        PR_ERR("get wifi state err");
        return;
    }
    if (wifi_state <= STAT_AP_STA_DISC || wifi_state == STAT_STA_DISC) {
        return;
    }
    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(SIZEOF(TY_OBJ_DP_S));
    if(NULL == dp_arr) {
        PR_ERR("malloc failed");
        return;
    }
    memset(dp_arr, 0, SIZEOF(TY_OBJ_DP_S));

    switch (dp_id){
    case DP_SWITCH: {
        dp_arr[0].dpid = switch_s.dp_id;
        dp_arr[0].type = PROP_BOOL;
        dp_arr[0].time_stamp = 0;
        dp_arr[0].value.dp_bool = switch_s.power;

        }
        break;

    case DP_TEMPER: {
        dp_arr[0].dpid = temper_s.dp_id;
        dp_arr[0].type = PROP_VALUE;
        dp_arr[0].time_stamp = 0;
        dp_arr[0].value.dp_value = temper_s.value;
        }
        break;

    case DP_SHAKE: {
        dp_arr[0].dpid = shake_s.dp_id;
        dp_arr[0].type = PROP_BOOL;
        dp_arr[0].time_stamp = 0;
        dp_arr[0].value.dp_bool = shake_s.power;
        }
        break;

    case DP_MODE: {
        dp_arr[0].dpid = mode_s.dp_id;
        dp_arr[0].type = PROP_ENUM;
        dp_arr[0].time_stamp = 0;
        dp_arr[0].value.dp_enum = mode_s.value;
        }
        break;

    case DP_LED: {
        dp_arr[0].dpid = led_s.dp_id;
        dp_arr[0].type = PROP_ENUM;
        dp_arr[0].time_stamp = 0;
        dp_arr[0].value.dp_enum = led_s.value;
        }
        break;

    case DP_SET_TEMP: {
        dp_arr[0].dpid = set_temper_s.dp_id;
        dp_arr[0].type = PROP_VALUE;
        dp_arr[0].time_stamp = 0;
        dp_arr[0].value.dp_value = set_temper_s.value;
        }
        break;

    case DP_TIME_ON: {
        dp_arr[0].dpid = time_to_open_s.dp_id;
        dp_arr[0].type = PROP_BOOL;
        dp_arr[0].time_stamp = 0;
        dp_arr[0].value.dp_bool = time_to_open_s.power;
        }
        break;

    case DP_TIME_OFF: {
        dp_arr[0].dpid = time_to_close_s.dp_id;
        dp_arr[0].type = PROP_BOOL;
        dp_arr[0].time_stamp = 0;
        dp_arr[0].value.dp_bool = time_to_close_s.power;
        }
        break;
    case DP_SHUTDOWN_TIME: {
        dp_arr[0].dpid = shutdown_time_s.dp_id;
        dp_arr[0].type = PROP_VALUE;
        dp_arr[0].time_stamp = 0;
        dp_arr[0].value.dp_value = shutdown_time_s.value;
        }
        break;
    default:
    
        break;
    }

    op_ret = dev_report_dp_json_async(NULL , dp_arr, 1);
    Free(dp_arr);
    dp_arr = NULL;
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }
}

/**
 * @Function: opUserFlashWriteAppData
 * @Description:Write data to Flash
 * @Input: IN Data_Memory_T *pData
 * @Output: none
 * @Return: none
 * @Others: 
 */
INT_T opUserFlashWriteAppData(IN Data_Memory_T *pData)
{
    
    int opRet;
    
    opRet = opSocFlashWrite(SAVE_TYP1,APP_DP_DATA_OFFSET, (UCHAR_T *)(pData), SIZEOF(Data_Memory_T));
    if(opRet != OPRT_OK) {
        PR_ERR("dp data write flash error %d!", opRet);
        return OPRT_COM_ERROR;
    }

    return OPRT_OK;
}
/**
 * @Function: Power_data_recovery
 * @Description:Power_data_recovery
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID_T Power_data_recovery(void)
{
    Data_Memory_T *power_down_memory = (Data_Memory_T *)Malloc(SIZEOF(Data_Memory_T));
    memset(power_down_memory,0,sizeof(Data_Memory_T));  
    uiSocFlashRead(SAVE_TYP1, APP_DP_DATA_OFFSET, sizeof(Data_Memory_T), (UCHAR_T *)(power_down_memory));

    dp_memory_s.led_mode = power_down_memory->led_mode;
    dp_memory_s.switch_bool = 0;
    dp_memory_s.set_temper_value = power_down_memory->set_temper_value;
    //device timer close clear
    dev_key_s.temp_time_count = 0;
    dev_key_s.timer_hour = 0;
    Free(power_down_memory);
}
