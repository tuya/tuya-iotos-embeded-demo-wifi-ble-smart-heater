/*
 * @file name: tm1650.h
 * @Descripttion: tm1650 drive 
 * @Author: xjw
 * @email: fudi.xu@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-26 16:45:15
 * @LastEditors: xjw
 * @LastEditTime: 2021-05-10 12:05:48
 */

#ifndef __TM1650_H__
#define __TM1650_H__
/* Includes ------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C" {
#endif
#include "uni_log.h"
#include "tuya_cloud_error_code.h"
#include "tuya_pin.h"
#include "tuya_i2c.h"
#include "tuya_hal_system.h"



void display_num(uint8_t num);
void display_status(uint8_t status);
void tm1650_led_init(void);
void tm1650_close(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif