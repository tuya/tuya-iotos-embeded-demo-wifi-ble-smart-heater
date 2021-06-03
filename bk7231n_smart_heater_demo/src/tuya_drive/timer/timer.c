/*
 * @file name: timer.c
 * @Descripttion: 
 * @Author: xjw
 * @email: fudi.xu@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-26 16:35:33
 * @LastEditors: xjw
 * @LastEditTime: 2021-05-10 15:21:28
 */
/* Includes ------------------------------------------------------------------*/
#include "timer.h"
#include "tuya_device.h"
#include "tuya_cloud_error_code.h"
#include "b3950.h"
#include "tuya_dp_process.h"
#include "gpio_control.h"
#include <string.h>
#include "uni_time.h"
#include "uni_log.h"
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static tuya_timer_t *timer = NULL;
static tuya_timer_t *timer1 = NULL;
static tuya_timer_t *timer2 = NULL;
static tuya_timer_t *timer3 = NULL;
uint32_t timercount = 0;
/* Private functions ---------------------------------------------------------*/
/**
 * @Function: timer_init
 * @Description: Time-base timer
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void timer_init(void)
{
    timer = (tuya_timer_t *)tuya_driver_find(TUYA_DRV_TIMER, TUYA_TIMER0);
    //The timer uses cycle mode
    TUYA_TIMER_CFG(timer, TUYA_TIMER_MODE_PERIOD, tuya_timer0_cb, NULL);
    tuya_timer_init(timer);
    //Start the timer, 1ms is a counting cycle
    tuya_timer_start(timer, 1000);

}
/**
 * @Function: timer_stop
 * @Description: stop timer0
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void timer_stop(void)
{
    tuya_timer_stop(timer);

}
/**
 * @Function: tuya_timer0_cb
 * @Description: tuya_timer0_cb
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void tuya_timer0_cb(void *arg)
{
    static uint32_t s_tick;
    
    //10000 * 1ms = 10s
    if (s_tick++ >= 10000) {
        s_tick = 0;
        timercount++;
        //Transmitting semaphore Temperature acquisition is performed every 10 seconds        
        tuya_hal_semaphore_post(g_temper_binsemap);
        
    }
}
/**
 * @Function: timer_init1
 * @Description: Adjust the buzzer sound
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void timer1_init(void)
{
    timer1 = (tuya_timer_t *)tuya_driver_find(TUYA_DRV_TIMER, TUYA_TIMER1);
    //The timer uses cycle mode
    TUYA_TIMER_CFG(timer1, TUYA_TIMER_MODE_PERIOD, tuya_timer1_cb, NULL);
    tuya_timer_init(timer1);
    //Start the timer, 0.4ms is a counting cycle
    tuya_timer_start(timer1, 400);
    buzzer_set(1);
}
/**
 * @Function: tuya_timer1_cb
 * @Description: tuya_timer1_cb
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void tuya_timer1_cb(void *arg)
{
    static uint32_t s_tick;
    if (s_tick++ >= 200) {
        s_tick = 0;
        tuya_timer_stop(timer1);
        buzzer_set(0);
    }
}
/**
 * @Function: timer_init2
 * @Description: When the timing and temperature adjustment of the device end are not operated for 5 seconds, 
 *               the temperature display state will be returned
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void timer2_init(void)
{
    timer2 = (tuya_timer_t *)tuya_driver_find(TUYA_DRV_TIMER, TUYA_TIMER2);
    //定时器使用周期模式
    TUYA_TIMER_CFG(timer2, TUYA_TIMER_MODE_PERIOD, tuya_timer2_cb, NULL);
    tuya_timer_init(timer2);
    //开启定时器，0.4ms为一个计数周期
    tuya_timer_start(timer2, 1000);
    dev_key_s.display_lock = 5;
}
/**
 * @Function: tuya_timer2_cb
 * @Description: tuya_timer2_cb
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void tuya_timer2_cb(void *arg)
{
    static uint32_t s_tick;
    if (s_tick++ >= 1000) {
        s_tick = 0;
        dev_key_s.display_lock--;
        tuya_hal_semaphore_post(g_temper_binsemap);
        if(dev_key_s.display_lock == 0) {
            tuya_timer_stop(timer2);
        }
    }
}



