/*
 * @file name: led_color_set.c
 * @Descripttion: led color set
 * @Author: xjw
 * @email: fudi.xu@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-26 16:35:33
 * @LastEditors: xjw
 * @LastEditTime: 2021-05-10 15:21:28
 */
#include "led_color_set.h"
/*============================ PROTOTYPES ====================================*/
#define LED_CLOSE 0
#define COLOR1 1
#define COLOR2 2
#define COLOR3 3
#define COLOR4 4
#define STAT_UNPROVISION_LED 5
#define STAT_AP_STA_UNCFG_LED 6
#define STAT_AP_STA_DISC_LED 7
/*============================ IMPLEMENTATION ================================*/
static tuya_pwm_t *rgb_r_pwm = NULL;
static tuya_pwm_t *rgb_g_pwm = NULL;
static tuya_pwm_t *rgb_b_pwm = NULL;
uint8_t rgb_r_pwm_pin = TY_GPIOA_7;
uint8_t rgb_g_pwm_pin = TY_GPIOA_8;
uint8_t rgb_b_pwm_pin = TY_GPIOA_6;
/**
 * @Function: led_init()
 * @Description: led PWM init
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void led_init()
{
    /*color Red init*/
    rgb_r_pwm = (tuya_pwm_t *)tuya_driver_find(TUYA_DRV_PWM, TUYA_PWM0);
    TUYA_PWM_CFG(rgb_r_pwm, rgb_r_pwm_pin, 10 * 1000, 0);
    tuya_pwm_init(rgb_r_pwm);
    tuya_pwm_start(rgb_r_pwm);

    /*color Green init*/
    rgb_g_pwm = (tuya_pwm_t *)tuya_driver_find(TUYA_DRV_PWM, TUYA_PWM1);
    TUYA_PWM_CFG(rgb_g_pwm, rgb_g_pwm_pin, 10 * 1000, 0);
    tuya_pwm_init(rgb_g_pwm);
    tuya_pwm_start(rgb_g_pwm);

    /*color Bule init*/
    rgb_b_pwm = (tuya_pwm_t *)tuya_driver_find(TUYA_DRV_PWM, TUYA_PWM5);
    TUYA_PWM_CFG(rgb_b_pwm, rgb_b_pwm_pin, 10 * 1000, 0);
    tuya_pwm_init(rgb_b_pwm);
    tuya_pwm_start(rgb_b_pwm);
    
}
/**
 * @Function: color_handle
 * @Description: LED MODE SET
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
VOID color_handle(IN int mode)
{
    switch (mode){

    case LED_CLOSE: {
        led_close();
    }
    break;
    case COLOR1: {
        color1_set();
    }
    break;
    case COLOR2: {
        color2_set();
    }
    break;
    case COLOR3: {
        color3_set();
    }
    break;
    case COLOR4: {
        color4_set();
    }
    break;
    case STAT_UNPROVISION_LED: {
        stat_unprovision_led();
    }
    break;
    case STAT_AP_STA_UNCFG_LED: {
        stat_ap_sta_uncfg_led();
    }
    break;
    case STAT_AP_STA_DISC_LED: {
        stat_ap_sta_disc_led();
    }
    break;
    default:
    break;
    }

}
/**
 * @Function: color1_set
 * @Description: LED  COLOR SET COLOR1
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void color1_set()
{
    tuya_pwm_duty_set(rgb_r_pwm, 1.0);
    tuya_pwm_duty_set(rgb_g_pwm, 0);
    tuya_pwm_duty_set(rgb_b_pwm, 0);

}
/**
 * @Function: color2_set
 * @Description: LED  COLOR SET COLOR2
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void color2_set()
{
    tuya_pwm_duty_set(rgb_r_pwm, 0);
    tuya_pwm_duty_set(rgb_g_pwm, 1.0);
    tuya_pwm_duty_set(rgb_b_pwm, 0);

}
/**
 * @Function: color3_set
 * @Description: LED  COLOR SET COLOR3
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void color3_set()
{
    tuya_pwm_duty_set(rgb_r_pwm, 0);
    tuya_pwm_duty_set(rgb_g_pwm, 0);
    tuya_pwm_duty_set(rgb_b_pwm, 1.0);

}
/**
 * @Function: color4_set
 * @Description: LED  COLOR SET COLOR4
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void color4_set()
{
    tuya_pwm_duty_set(rgb_r_pwm, 0.15);
    tuya_pwm_duty_set(rgb_g_pwm, 0.8);
    tuya_pwm_duty_set(rgb_b_pwm, 0.8);

}
/**
 * @Function: led_close
 * @Description: LED CLOSE
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void led_close()
{
    tuya_pwm_duty_set(rgb_r_pwm, 0);
    tuya_pwm_duty_set(rgb_g_pwm, 0);
    tuya_pwm_duty_set(rgb_b_pwm, 0);

}
/**
 * @Function: stat_unprovision_led
 * @Description: Network indicator light
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void stat_unprovision_led()
{
    static float pwm_duty = 0;
    tuya_pwm_duty_set(rgb_r_pwm, pwm_duty);
    tuya_pwm_duty_set(rgb_g_pwm, 0);
    tuya_pwm_duty_set(rgb_b_pwm, 0);
    pwm_duty = pwm_duty + 0.1;
    if(pwm_duty > 1) {
        pwm_duty = 0;
    }
}
/**
 * @Function: stat_ap_sta_uncfg_led
 * @Description: Network indicator light
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void stat_ap_sta_uncfg_led()
{
    static float pwm_duty = 0;
    tuya_pwm_duty_set(rgb_r_pwm, 0);
    tuya_pwm_duty_set(rgb_g_pwm, 0);
    tuya_pwm_duty_set(rgb_b_pwm, pwm_duty);
    pwm_duty = pwm_duty + 0.1;
    if(pwm_duty > 1) {
        pwm_duty = 0;
    }
}
/**
 * @Function: stat_ap_sta_disc_led
 * @Description: Network indicator light
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void stat_ap_sta_disc_led()
{
    static float pwm_duty = 0;
    tuya_pwm_duty_set(rgb_r_pwm, 0);
    tuya_pwm_duty_set(rgb_g_pwm, pwm_duty);
    tuya_pwm_duty_set(rgb_b_pwm, 0);
    pwm_duty = pwm_duty + 0.1;
    if(pwm_duty > 1) {
        pwm_duty = 0;
    }
}