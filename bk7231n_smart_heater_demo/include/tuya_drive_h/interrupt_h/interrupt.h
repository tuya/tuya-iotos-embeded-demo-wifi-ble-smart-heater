/*
 * @Author: xjw 
 * @email: fudi.xu@tuya.com
 * @LastEditors: xjw 
 * @file name: interrupt.h
 * @Description: interrupt handle header file 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-02-22 15:10:00
 * @LastEditTime: 2021-02-22 15:10:00
 */
#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "tuya_pin.h"
#include "tuya_hal_system.h"

void tuya_key_irq_cb(void *arg);
void key_interrupt_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif