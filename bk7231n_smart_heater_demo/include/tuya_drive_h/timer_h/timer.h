/*
 * @file name: timer.h
 * @Descripttion: timer header file
 * @Author: xjw
 * @email: fudi.xu@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-27 10:50:15
 * @LastEditors: xjw
 * @LastEditTime: 2021-05-10 11:05:48
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef __cplusplus
    extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "tuya_timer.h"
#include "tuya_hal_system.h"
#include "tuya_pin.h"

/* Exported types ------------------------------------------------------------*/
extern uint32_t timercount;
void timer_init(void);
void timer_stop(void);
void timer1_init(void);
void timer2_init(void);
void timer3_init(void);

void tuya_timer0_cb(void *arg);
void tuya_timer1_cb(void *arg);
void tuya_timer2_cb(void *arg);
void tuya_timer3_cb(void *arg);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
