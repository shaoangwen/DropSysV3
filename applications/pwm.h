/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-12     wen       the first version
 */
#ifndef APPLICATIONS_PWM_H_
#define APPLICATIONS_PWM_H_
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>


extern int PWM4_DEV_CHANNEL3;     /* PWM通道 */

extern int PWM4_DEV_CHANNEL4;      /* PWM通道 */


extern int PWM9_DEV_CHANNEL1;      /* PWM通道 */


extern int PWM9_DEV_CHANNEL2;      /* PWM通道 */


extern int PWM1_DEV_CHANNEL4;      /* PWM通道 */


extern struct rt_device_pwm *pwm4_dev;      /* PWM设备句柄 */
extern struct rt_device_pwm *pwm9_dev;      /* PWM设备句柄 */
extern struct rt_device_pwm *pwm1_dev;      /* PWM设备句柄 */

rt_uint32_t period, pulse;

extern void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
extern void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base);
extern void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base);
int pwm4_init(void);
int pwm9_init(void);
int pwm1_init(void);
void MX_TIM4_Init(void);
void MX_TIM9_Init(void);
void MX_TIM1_Init(void);
void pwm_init();


#endif /* APPLICATIONS_PWM_H_ */
