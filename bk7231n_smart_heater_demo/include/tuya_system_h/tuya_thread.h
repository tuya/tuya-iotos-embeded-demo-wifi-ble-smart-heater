/*
 * @Author: xjw 
 * @email: fudi.xu@tuya.com
 * @LastEditors: xjw 
 * @file name: tuya_thread.h
 * @Description: The main thread handles files header file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-02-22 15:10:00
 * @LastEditTime: 2021-05-17 15:10:00
 */
#ifndef __TUYA_THREAD_H__
#define __TUYA_THREAD_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "tuya_hal_thread.h"
#include "tuya_uart.h"
#include "tuya_hal_system.h"

void thread_init(void);

extern int wf_nw_status_temp;
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif