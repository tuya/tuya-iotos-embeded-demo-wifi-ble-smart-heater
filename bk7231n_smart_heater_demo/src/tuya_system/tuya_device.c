/*
 * @Author: yj 
 * @email: shiliu.yang@tuya.com
 * @LastEditors: XJW 
 * @file name: tuya_device.c
 * @Description:SDK WiFi & BLE for BK7231N
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-02-22 15:10:00
 * @LastEditTime: 2021-05-12 15:10:00
 */

#define _TUYA_DEVICE_GLOBAL

/* Includes ------------------------------------------------------------------*/
#include "uni_log.h"
#include "tuya_iot_wifi_api.h"
#include "tuya_hal_system.h"
#include "tuya_iot_com_api.h"
#include "tuya_cloud_error_code.h"
#include "gw_intf.h"
#include "tuya_gpio.h"
#include "tuya_key.h"
#include "tuya_led.h"
#include <string.h>
/* Private includes ----------------------------------------------------------*/
#include "tuya_device.h"
#include "b3950.h"
#include "interrupt.h"
#include "led_color_set.h"
#include "timer.h"
#include "tuya_thread.h"
#include "tuya_dp_process.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* wifi 相关配置 */
#define WIFI_WORK_MODE_SEL          GWCM_LOW_POWER   //wifi work mode set

#define WIFI_CONNECT_OVERTIME_S     180             //wifi Connection timeout ，单位：s
#define DP_CONUT    9           //DP point number

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @Function: wifi_state_led_reminder
 * @Description:
 * @Input: cur_stat： current WiFi status 
 * @Output: none
 * @Return: none
 * @Others: 
 */
STATIC VOID wifi_state_led_reminder(IN CONST GW_WIFI_NW_STAT_E cur_stat)
{
    
    wf_nw_status_temp = cur_stat;
    PR_NOTICE("wf_nw_status_temp:%d",wf_nw_status_temp);
    
}



/**
 * @Function:mf_user_pre_gpio_test_cb 
 * @Description: 
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID mf_user_pre_gpio_test_cb(VOID_T) 
{
    return;
}

/**
 * @Function:mf_user_enter_callback 
 * @Description: 
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID mf_user_enter_callback(VOID_T) 
{
    return;
}

/**
 * @Function: hw_reset_flash_data
 * @Description: hardware reset, erase user data from flash
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID hw_reset_flash_data(VOID)
{
    return;
}

/**
 * @Function: mf_user_callback
 * @Description: 
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: clean flash data
 */
VOID mf_user_callback(VOID_T) 
{
    hw_reset_flash_data();
    return;
}

/**
 * @Function:mf_user_product_test_cb 
 * @Description: 
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
OPERATE_RET mf_user_product_test_cb(USHORT_T cmd,UCHAR_T *data, UINT_T len, OUT UCHAR_T **ret_data,OUT USHORT_T *ret_len) 
{
    return OPRT_OK;
}

/**
 * @Function:pre_app_init 
 * @Description: 
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID pre_app_init(VOID_T) 
{
    return;
}

/**
 * @Function:app_init 
 * @Description: 
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID app_init(VOID_T) 
{

    /* set the connection network timeout */
    tuya_iot_wf_timeout_set(WIFI_CONNECT_OVERTIME_S);
    return;
}

/**
 * @Function:pre_device_init 
 * @Description: 
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID pre_device_init(VOID_T) 
{
    PR_DEBUG("%s",tuya_iot_get_sdk_info());
    PR_DEBUG("%s:%s", APP_BIN_NAME, DEV_SW_VERSION);
    PR_NOTICE("firmware compiled at %s %s", __DATE__, __TIME__);
    PR_NOTICE("system reset reason:[%d]",tuya_hal_system_get_rst_info());

    SetLogManageAttr(TY_LOG_LEVEL_DEBUG);

    return;
}

/**
 * @Function: hw_report_all_dp_status
 * @Description: update all dp data
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */

VOID_T hw_report_all_dp_status(VOID_T)
{

    OPERATE_RET op_ret = OPRT_OK;
    INT_T dp_cnt = 0;
    dp_cnt = DP_CONUT;
    GW_WIFI_NW_STAT_E wifi_state = 0xFF;
    /* nolink，return */
    op_ret = get_wf_gw_nw_status(&wifi_state);
    if (OPRT_OK != op_ret) {
        PR_ERR("get wifi state err");
        return;
    }
    if (wifi_state <= STAT_AP_STA_DISC || wifi_state == STAT_STA_DISC) {
        return;
    }
    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(dp_cnt*SIZEOF(TY_OBJ_DP_S));
    if(NULL == dp_arr) {
        PR_ERR("malloc failed");
        return;
    }

    memset(dp_arr, 0, dp_cnt*SIZEOF(TY_OBJ_DP_S));

    /*  */
    dp_arr[0].dpid = switch_s.dp_id;
    dp_arr[0].type = PROP_BOOL;
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_bool = switch_s.power;

	dp_arr[1].dpid = temper_s.dp_id;
    dp_arr[1].type = PROP_VALUE;
    dp_arr[1].time_stamp = 0;
    dp_arr[1].value.dp_value = temper_s.value;

	dp_arr[2].dpid = shake_s.dp_id;
    dp_arr[2].type = PROP_BOOL;
    dp_arr[2].time_stamp = 0;
    dp_arr[2].value.dp_bool = shake_s.power;
    
	dp_arr[3].dpid = mode_s.dp_id;
    dp_arr[3].type = PROP_ENUM;
    dp_arr[3].time_stamp = 0;
    dp_arr[3].value.dp_enum = mode_s.value;
    
    dp_arr[4].dpid = led_s.dp_id;
    dp_arr[4].type = PROP_ENUM;
    dp_arr[4].time_stamp = 0;
    dp_arr[4].value.dp_enum = led_s.value;

	dp_arr[5].dpid = set_temper_s.dp_id;
    dp_arr[5].type = PROP_VALUE;
    dp_arr[5].time_stamp = 0;
    dp_arr[5].value.dp_value = set_temper_s.value;

	dp_arr[6].dpid = time_to_open_s.dp_id;
    dp_arr[6].type = PROP_BOOL;
    dp_arr[6].time_stamp = 0;
    dp_arr[6].value.dp_bool = time_to_open_s.power;
    
	dp_arr[7].dpid = time_to_close_s.dp_id;
    dp_arr[7].type = PROP_BOOL;
    dp_arr[7].time_stamp = 0;
    dp_arr[7].value.dp_bool = time_to_close_s.power;

    dp_arr[8].dpid = shutdown_time_s.dp_id;
    dp_arr[8].type = PROP_VALUE;
    dp_arr[8].time_stamp = 0;
    dp_arr[8].value.dp_value = shutdown_time_s.value;
    op_ret = dev_report_dp_json_async(NULL , dp_arr, dp_cnt);
    
    Free(dp_arr);
    dp_arr = NULL;
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }

    return;
}



/**
 * @Function: status_changed_cb
 * @Description: network status changed callback
 * @Input: status: current status
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID status_changed_cb(IN CONST GW_STATUS_E status)
{
    PR_NOTICE("status_changed_cb is status:%d",status);

    if(GW_NORMAL == status) {
        hw_report_all_dp_status();
    }else if(GW_RESET == status) {
        PR_NOTICE("status is GW_RESET");
    }
}

/**
 * @Function: upgrade_notify_cb
 * @Description: firmware download finish result callback
 * @Input: fw: firmware info
 * @Input: download_result: 0 means download succes. other means fail
 * @Input: pri_data: private data
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data)
{
    PR_DEBUG("download  Finish");
    PR_DEBUG("download_result:%d", download_result);
}

/**
 * @Function: get_file_data_cb
 * @Description: firmware download content process callback
 * @Input: fw: firmware info
 * @Input: total_len: firmware total size
 * @Input: offset: offset of this download package
 * @Input: data && len: this download package
 * @Input: pri_data: private data
 * @Output: remain_len: the size left to process in next cb
 * @Return: OPRT_OK: success  Other: fail
 * @Others: none
 */
OPERATE_RET get_file_data_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len, IN CONST UINT_T offset,
                            IN CONST BYTE_T *data, IN CONST UINT_T len, OUT UINT_T *remain_len, IN PVOID_T pri_data)
{
    PR_DEBUG("Rev File Data");
    PR_DEBUG("Total_len:%d ", total_len);
    PR_DEBUG("Offset:%d Len:%d", offset, len);

    return OPRT_OK;
}


/**
 * @Function: gw_ug_inform_cb
 * @Description: gateway ota firmware available nofity callback
 * @Input: fw: firmware info
 * @Output: none
 * @Return: int:
 * @Others: 
 */
INT_T gw_ug_inform_cb(IN CONST FW_UG_S *fw)
{
    PR_DEBUG("Rev GW Upgrade Info");
    PR_DEBUG("fw->fw_url:%s", fw->fw_url);
    PR_DEBUG("fw->sw_ver:%s", fw->sw_ver);
    PR_DEBUG("fw->file_size:%d", fw->file_size);

    return tuya_iot_upgrade_gw(fw, get_file_data_cb, upgrade_notify_cb, NULL);
}

/**
 * @Function: gw_reset_cb
 * @Description: gateway restart callback, app remove the device 
 * @Input: type:gateway reset type
 * @Output: none
 * @Return: none
 * @Others: reset factory clear flash data
 */
VOID gw_reset_cb(IN CONST GW_RESET_TYPE_E type)
{
    PR_DEBUG("gw_reset_cb type:%d",type);
    if(GW_REMOTE_RESET_FACTORY != type) {
        PR_DEBUG("type is NO GW_REMOTE_RESET_FACTORY");
        return;
    }

    hw_reset_flash_data();
}

/**
 * @Function: dev_obj_dp_cb
 * @Description: obj dp info cmd callback, tuya cloud dp(data point) received
 * @Input: dp:obj dp info
 * @Output: none
 * @Return: none
 * @Others: app send data by dpid  control device stat
 */
VOID dev_obj_dp_cb(IN CONST TY_RECV_OBJ_DP_S *dp)
{
    PR_DEBUG("dp->cid:%s dp->dps_cnt:%d",dp->cid,dp->dps_cnt);
    UCHAR_T i = 0;

    for(i = 0;i < dp->dps_cnt;i++) {
        deal_dp_proc(&(dp->dps[i]));
        //dev_report_dp_json_async(get_gw_cntl()->gw_if.id, dp->dps, dp->dps_cnt);
    }
}

/**
 * @Function: dev_raw_dp_cb
 * @Description: raw dp info cmd callback, tuya cloud dp(data point) received (hex data)
 * @Input: dp: raw dp info
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID dev_raw_dp_cb(IN CONST TY_RECV_RAW_DP_S *dp)
{
    PR_DEBUG("raw data dpid:%d",dp->dpid);
    PR_DEBUG("recv len:%d",dp->len);
#if 1 
    INT_T i = 0;
    for(i = 0;i < dp->len;i++) {
        PR_DEBUG_RAW("%02X ",dp->data[i]);
    }
#endif
    PR_DEBUG_RAW("\n");
    PR_DEBUG("end");
    return;
}

/**
 * @Function: dev_dp_query_cb
 * @Description: dp info query callback, cloud or app actively query device info
 * @Input: dp_qry: query info
 * @Output: none
 * @Return: none
 * @Others: none
 */
STATIC VOID dev_dp_query_cb(IN CONST TY_DP_QUERY_S *dp_qry) 
{
    PR_NOTICE("Recv DP Query Cmd");

    hw_report_all_dp_status();
}

/**
 * @Function: wf_nw_status_cb
 * @Description: tuya-sdk network state check callback
 * @Input: stat: curr network status
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID wf_nw_status_cb(IN CONST GW_WIFI_NW_STAT_E stat)
{
    PR_NOTICE("wf_nw_status_cb,wifi_status:%d", stat);
    wifi_state_led_reminder(stat);

    if(stat == STAT_AP_STA_CONN || stat >= STAT_STA_CONN) {
        hw_report_all_dp_status();
    }
}

/**
 * @Function: device_init
 * @Description: device initialization process 
 * @Input: none
 * @Output: none
 * @Return: OPRT_OK: success  Other: fail
 * @Others: none
 */
OPERATE_RET device_init(VOID_T) 
{
    OPERATE_RET op_ret = OPRT_OK;
    TY_IOT_CBS_S wf_cbs = {
        status_changed_cb,\ 
        gw_ug_inform_cb,\   
        gw_reset_cb,\
        dev_obj_dp_cb,\
        dev_raw_dp_cb,\
        dev_dp_query_cb,\
        NULL,
    };

    op_ret = tuya_iot_wf_soc_dev_init_param(WIFI_WORK_MODE_SEL, WF_START_SMART_FIRST, &wf_cbs, NULL, PRODECT_ID, DEV_SW_VERSION);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_wf_soc_dev_init_param error,err_num:%d",op_ret);
        return op_ret;
    }

    op_ret = tuya_iot_reg_get_wf_nw_stat_cb(wf_nw_status_cb);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_reg_get_wf_nw_stat_cb is error,err_num:%d",op_ret);
        return op_ret;
    }

    calorifier_init();
    
    return op_ret;
}


