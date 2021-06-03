/*
 * @file name: gpio_control.c
 * @Descripttion: gpio control file
 * @Author: xjw
 * @email: fudi.xu@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-26 16:45:15
 * @LastEditors: xjw
 * @LastEditTime: 2021-05-10 12:05:48
 */
#include "gpio_control.h"
/*============================ define ========================================*/
#define relay_leve1 22
#define relay_leve2 20
#define fan_motor   16
#define shake_motor 14
#define key_notice_pin 9
#define buzzer_pin  28

#define mode0   0
#define mode1   1
#define mode2   2 
/* Private functions ---------------------------------------------------------*/
/**
 * @Function: relay_init
 * @Description: Relay control pin initialization
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void relay_init()
{
    tuya_pin_init(relay_leve1, TUYA_PIN_MODE_OUT_PP_LOW);
    tuya_pin_init(relay_leve2, TUYA_PIN_MODE_OUT_PP_LOW);
}
/**
 * @Function: motor_init
 * @Description: motor control pin initialization
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void motor_init()
{
    tuya_pin_init(fan_motor, TUYA_PIN_MODE_OUT_PP_LOW);
    tuya_pin_init(shake_motor, TUYA_PIN_MODE_OUT_PP_LOW);
    tuya_pin_init(key_notice_pin, TUYA_PIN_MODE_IN_FL);
}
/**
 * @Function: buzzer_init
 * @Description: buzzer control pin initialization
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void buzzer_init()
{
    tuya_pin_init(buzzer_pin, TUYA_PIN_MODE_OUT_PP_LOW);
}
/**
 * @Function: relay_set
 * @Description: Heating mode setting
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void relay_set(int level)
{
    switch (level){

    case mode0: {
        tuya_pin_write(relay_leve1, TUYA_PIN_LOW);
        tuya_pin_write(relay_leve2, TUYA_PIN_LOW);
        }
        break;
    case mode1: {
        tuya_pin_write(relay_leve1, TUYA_PIN_HIGH);
        tuya_pin_write(relay_leve2, TUYA_PIN_LOW);
        }
        break;
    case mode2: {
        tuya_pin_write(relay_leve1, TUYA_PIN_LOW);
        tuya_pin_write(relay_leve2, TUYA_PIN_HIGH);
        }
        break;
        default:
        break;
    }

}
/**
 * @Function: fan_motor_set
 * @Description: fan_motor_set
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void fan_motor_set(IN    BOOL_T bONOFF)
{
    if(bONOFF == TRUE) {
    tuya_pin_write(fan_motor, TUYA_PIN_HIGH);
    }else {
    tuya_pin_write(fan_motor, TUYA_PIN_LOW);
    }

}
/**
 * @Function: shake_motor_set
 * @Description: shake_motor_set
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void shake_motor_set(IN     BOOL_T bONOFF)
{
    if(bONOFF == TRUE) {
    tuya_pin_write(shake_motor, TUYA_PIN_HIGH);
    }else {
    tuya_pin_write(shake_motor, TUYA_PIN_LOW);
    }

}
/**
 * @Function: buzzer_set
 * @Description: buzzer_set
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 
 */
void buzzer_set(IN     BOOL_T bONOFF)
{
    if(bONOFF == TRUE) {
    tuya_pin_write(buzzer_pin, TUYA_PIN_HIGH);
    }else {
    tuya_pin_write(buzzer_pin, TUYA_PIN_LOW);
    }
}