/*
 * @Author: xjw 
 * @email: fudi.xu@tuya.com
 * @LastEditors: xjw 
 * @file name: led_color_set.h
 * @Description: led color set
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-02-22 15:10:00
 * @LastEditTime: 2021-02-22 15:10:00
 */
#ifndef __LED_COLOR_SET_H__
#define __LED_COLOR_SET_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "tuya_gpio.h"
#include "tuya_pwm.h"
#include "tuya_hal_system.h"

void led_init();
void color1_set();
void color2_set();
void color3_set();
void color4_set();
void led_close();
VOID color_handle(IN int mode);
void stat_unprovision_led();
void stat_ap_sta_uncfg_led();
void stat_ap_sta_disc_led();
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif