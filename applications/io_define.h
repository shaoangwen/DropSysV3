/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-10     shao'ang       the first version
 */
#include <rtthread.h>
#include <drv_common.h>
#ifndef APPLICATIONS_IO_DEFINE_H_
#define APPLICATIONS_IO_DEFINE_H_
/*
rt_base_t V1_pin;
rt_base_t V2_pin;
rt_base_t V3_pin;
rt_base_t V4_pin;
rt_base_t PUMP_pin;
rt_base_t M1_en;
rt_base_t M2_en;
rt_base_t M3_en;
rt_base_t R1_sw;
rt_base_t L1_sw;
rt_base_t R2_sw;
rt_base_t L2_sw;
rt_base_t R3_sw;
rt_base_t L3_sw;
*/

#define V1_pin GET_PIN(D, 8) //another way to define a pin
#define V2_pin GET_PIN(D, 9)
#define V3_pin GET_PIN(D, 10)
#define V4_pin GET_PIN(D, 11)
#define PUMP_pin GET_PIN(D, 12)

#define M1_en GET_PIN(D, 5)
#define M2_en GET_PIN(D, 6)
#define M3_en GET_PIN(D, 7)

#define R1_sw GET_PIN(E, 0)
#define L1_sw GET_PIN(E, 1)
#define R2_sw GET_PIN(E, 2)
#define L2_sw GET_PIN(E, 3)
#define R3_sw GET_PIN(E, 4)
#define L3_sw GET_PIN(E, 7)



void GPIO_init();

#endif /* APPLICATIONS_IO_DEFINE_H_ */
