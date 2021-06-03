/*
 * @file name: tc309.h
 * @Descripttion: tc309 drive header file
 * @Author: xjw
 * @email: fudi.xu@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-04-27 10:50:15
 * @LastEditors: xjw
 * @LastEditTime: 2021-05-10 11:05:48
 */

#ifndef __TC309_H__
#define __TC309_H__
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
#include "tuya_cloud_error_code.h"
#include "uni_log.h"

#define KEY0    0xff
#define KEY1    0x17f
#define KEY2    0x1bf
#define KEY3    0x1df
#define KEY4    0x1ef
#define KEY5    0x1f7
#define KEY6    0x1fb
#define KEY7    0x1fd
#define KEY8    0x1fe
/**
 * @brief TC309 init gpio pin structure
 */
typedef struct 
{
    UCHAR_T ucSdaPin;            ///< SDA pin
    UCHAR_T ucSclPin;            ///< SCL pin
}TC309_PIN_T;



int UserTc309Init();
int UserTc309Set_Sensitivity(UCHAR_T level);
void i2c_read_direct(UCHAR_T dev_addr,UCHAR_T * dest_buf,UCHAR_T len);



#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __SM2135_H__ */
