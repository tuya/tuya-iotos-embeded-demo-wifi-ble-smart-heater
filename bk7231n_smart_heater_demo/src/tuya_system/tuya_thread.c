/*
 * @file name: 
 * @Descripttion: 
 * @Author: xjw
 * @email: fudi.xu@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-26 16:35:33
 * @LastEditors: xjw
 * @LastEditTime: 2021-05-11 15:21:28
 */
#include "tuya_thread.h"
#include "uni_thread.h"
#include "uni_log.h"
#include "tuya_iot_wifi_api.h"
#include "tuya_cloud_error_code.h"
/* Private includes ----------------------------------------------------------*/
#include "tuya_device.h"
#include "b3950.h"
#include "tuya_dp_process.h"
#include "timer.h"
#include "tm1650.h"
#include "tc309.h"

/*============================ PROTOTYPES ====================================*/
void update_temperature_thread(void);
void flash_write_thread(void);
void tc309_keyscan_task(void);
void idle_task(void);

/*============================ IMPLEMENTATION ================================*/
int wf_nw_status_temp = -1;
/**
 * @Function: thread_init
 * @Description: thread_init
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void thread_init(void)
{
    int rt = OPRT_OK;

    rt = tuya_hal_thread_create(NULL, "update_dp_thread", 512*4, TRD_PRIO_4, update_temperature_thread, NULL);
    if (rt != OPRT_OK) {
        PR_ERR("Create update_dp_thread error!: %d", rt); 
        return;
    }

    rt = tuya_hal_thread_create(NULL, "tc309_keyscan_task", 512*4, TRD_PRIO_5, tc309_keyscan_task, NULL);
    if (rt != OPRT_OK) {
        PR_ERR("Create tc309_keyscan_task error!: %d", rt); 
        return;
    }

    rt = tuya_hal_thread_create(NULL, "idle_task1", 512*4, TRD_PRIO_6, idle_task, NULL);
    if (rt != OPRT_OK) {
        PR_ERR("Create idle_task error!: %d", rt); 
        return;
    }

    
}
/**
 * @Function: update_temperature_thread
 * @Description: update_temperature_thread function
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void update_temperature_thread(void)
{
    static int last_temper = 0;
    while(1)
    {
        tuya_hal_semaphore_wait(g_temper_binsemap);
        last_temper = cur_temper_get();
        if(last_temper > 50) {
            last_temper = 50;
        }
        else if(last_temper < -20) {
            last_temper = -20;
        }
        temper_s.value = last_temper;
        dp_memory_s.temperature_value = last_temper;

        PR_DEBUG("CURRENT TIME : %d",timercount);
        if((switch_s.power == 1) && (dev_key_s.display_lock == 0)) {
            display_num(dp_memory_s.temperature_value);
            display_status(dp_memory_s.status);
            report_one_dp_status(DP_TEMPER);
            report_one_dp_status(DP_SHUTDOWN_TIME);
        }

    }

}
/**
/**
 * @Function: tc309_keyscan_task
 * @Description: Keys trigger action execution functions
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void tc309_keyscan_task(void)
{
    static uint16_t key_value = 0x1ff;
    UserTc309Init();
    UserTc309Set_Sensitivity(7);
    while(1)
    {

    //GET KEY_VALUE
        tuya_hal_semaphore_wait(g_key_trigger_binsemap);
        uint8_t *buffer = (uint8_t *)Malloc(2*sizeof(uint8_t));
        //tc309 read 0x08 0x09
        i2c_read_direct(0x40,buffer,2);
        key_value = (buffer[0] << 4) & 0x1ff;
        key_value |= (buffer[1] >> 4) & 0x0f;
        PR_DEBUG("BUF0:%x",buffer[0]);
        PR_DEBUG("BUF1:%x",buffer[1]);
        PR_DEBUG("key_value:%x",key_value);
        //tc309
        dev_key_s.key_value = key_value;
        Free(buffer);
        //Actions are performed according to key values
        switch (key_value){

        case KEY0: {
            //shake key
            if(dp_memory_s.switch_bool) {
                timer1_init();
                PR_DEBUG("key_value0");
                if(dp_memory_s.shake_bool == 1) {
                    shake_handle(0);
                }else {
                    shake_handle(1);
                }
            }

        }
        break;
        case KEY1: {
            //relay key
            if(dp_memory_s.switch_bool) {
                timer1_init();
                PR_DEBUG("key_value1");
                static uint8_t cur_mode = 0;
                cur_mode = dp_memory_s.relay_mode;
                cur_mode = (cur_mode + 1 < 3) ? (cur_mode + 1) : 0;
                relay_handle(cur_mode);
                PR_DEBUG("relay mode:%d",dp_memory_s.relay_mode);
            }

        }
        break;
        case KEY2: {
            //timer close key
            if(dp_memory_s.switch_bool) {
                timer1_init();
                PR_DEBUG("key_value2");
                if(dev_key_s.last_time_count && ((timercount - dev_key_s.last_time_count) < 2)) {
                    dev_key_s.timer_hour++;
                }
                dev_key_s.last_time_count = timercount;
                if(dev_key_s.timer_hour > 12) {
                dev_key_s.timer_hour = 0;
                }
                //Avoid triggering the shutdown directly by pressing the timing button for the first time
                if(dev_key_s.timer_hour > 0) {
                    dev_key_s.key_notice = 1;
                }else {
                    dev_key_s.key_notice = 0;
                }

                dev_key_s.temp_time_count = timercount + dev_key_s.timer_hour*360;
                timer2_init();
                display_num(dev_key_s.timer_hour);
                display_status(0x04);
            }
        }
        break;
        case KEY3: {
            //Set target temperature
            if(dp_memory_s.switch_bool) {
                timer1_init();
                PR_DEBUG("key_value3");
                static uint8_t cur_set_temp = 0;
                cur_set_temp = dp_memory_s.set_temper_value;
                cur_set_temp = (cur_set_temp + 1 < 41) ? (cur_set_temp + 1) : 15;
                set_termper_handle(cur_set_temp);
                timer2_init();
                display_num(dp_memory_s.set_temper_value);
                display_status(0x02);
            }
        }
        break;
        case KEY4: {
            //Short press:led mode key  long press: Reconfigure the network mode
            if(dp_memory_s.switch_bool) {
                timer1_init();
                static uint32_t count = 0;
                while(tuya_gpio_read(9) == 0){
                    count++;
                    if(count > 700000){
                        break;
                    }
                }
                if(count > 700000) {
                    count = 0;
                    PR_DEBUG("wf_unc");
                    tuya_iot_wf_gw_unactive();
                }else {
                    PR_DEBUG("key_value4");
                    static uint8_t cur_ledmode = 0;
                    cur_ledmode = dp_memory_s.led_mode;
                    cur_ledmode = (cur_ledmode + 1 < 5) ? (cur_ledmode + 1) : 0;
                    led_handle(cur_ledmode);
                }
                PR_DEBUG("led mode:%d",dp_memory_s.led_mode);
            }            

        }
        break;
        case KEY5:{
            //switch key
            timer1_init();
            PR_DEBUG("key_value5");
            if(dp_memory_s.switch_bool == 1) {
                user_switch_handle(0);
                tm1650_close();
            }else {
                user_switch_handle(1);
            }
        }
        break;
        case KEY6: {
            PR_DEBUG("key_value6");
        }
        break;
        case KEY7: {
            PR_DEBUG("key_value7");
        }
        break;
        case KEY8: {
            PR_DEBUG("key_value8");
        }
        break;
        default:
        break;
        }
        //set timing time
        PR_DEBUG("KEY_HOUR:%d",dev_key_s.timer_hour);
        
    }
}

void idle_task(void)
{

    static uint8_t wf_nw_led = 1;
    while(1)
    {

        shutdown_time_s.value = ((dev_key_s.temp_time_count > timercount) && (0 < (dev_key_s.temp_time_count - timercount)/6 < 720)) ? (dev_key_s.temp_time_count - timercount)/6 : 0;
        dev_key_s.timer_hour = shutdown_time_s.value/60;
        if((timercount > dev_key_s.temp_time_count) && dev_key_s.temp_time_count && dev_key_s.key_notice) {
            PR_DEBUG("SHUTDOWN TIME");
            time_off_handle(1);
            dev_key_s.temp_time_count = 0;
            dev_key_s.timer_hour = 0;
            dev_key_s.key_notice = 0;
            tuya_hal_system_sleep(1000);
        }
        if((dp_memory_s.temperature_value >= dp_memory_s.set_temper_value) && dp_memory_s.relay_mode) {
            relay_handle(0);
        }
        if(wf_nw_led && dp_memory_s.switch_bool) {
            switch (wf_nw_status_temp)
            {
                //WiFi connection network timeout, enter low power mode
                case STAT_LOW_POWER: {
                    color_handle(0);
                    wf_nw_led = 0;
                }
                break;
                //SamrtConfig connected network mode, waiting for connection 
                case STAT_UNPROVISION: {
                    color_handle(5);
                }

                break;
                //ap connected network mode, waiting for connection 
                case STAT_AP_STA_UNCFG: {
                    color_handle(6);
                }
                break;
                //SamrtConfig/ap connecting...
                case STAT_AP_STA_DISC:
                case STAT_STA_DISC: {
                    color_handle(7);
                }
                break;
                //Already connected to Tuya Cloud 
                case STAT_CLOUD_CONN:
                case STAT_AP_CLOUD_CONN: {
                    color_handle(0);
                    wf_nw_led = 0;
                }
                break;

                default:
                break;
            }
            tuya_hal_system_sleep(200);
        }
    }
}
