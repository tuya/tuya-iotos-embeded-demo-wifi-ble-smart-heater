/*
 * @Author: xjw
 * @email: fudi.xu@tuya.com
 * @LastEditors: xjw
 * @file name: interrupt.c
 * @Description: interrupt handle file 
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-4-29 15:22:03
 * @LastEditTime: 2021-04-29 15:35:04
 */
#include "interrupt.h"
#include "tuya_dp_process.h"
#define KEY_INT 9

void key_interrupt_init(void)
{
    //SW1 Falling edge trigger
    tuya_pin_irq_init(KEY_INT, TUYA_PIN_MODE_IN_IRQ_FALL, tuya_key_irq_cb, NULL);

}

//Press SW1 to interrupt the callback function
void tuya_key_irq_cb(void *arg)
{
    tuya_hal_semaphore_post(g_key_trigger_binsemap);
}