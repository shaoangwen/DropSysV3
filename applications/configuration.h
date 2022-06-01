/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-11     wen       the first version
 */
#ifndef APPLICATIONS_CONFIGURATION_H_
#define APPLICATIONS_CONFIGURATION_H_
#include <rtthread.h>
extern rt_int32_t win_l_kpa;
extern rt_int32_t win_h_kpa;
extern rt_uint8_t inflating_thread_priority;
extern rt_uint8_t p_kpa_read_thread_priority;

extern int door_pos0_dc;
extern int door_pos1_dc;
extern int door_speed;
extern rt_uint8_t door_thread_priority;

extern rt_uint8_t report_pressure_thread_priority;

extern rt_uint8_t M1_thread_priority;
extern rt_uint8_t M2_thread_priority;
extern rt_uint8_t M3_thread_priority;

extern rt_uint8_t M1_homing_thread_priority;
extern rt_uint8_t  M2_homing_thread_priority;
extern rt_uint8_t M3_homing_thread_priority;
extern rt_uint8_t auto_homing_thread_priority;
/*
#ifndef win_l_kpa
rt_int32_t win_l_kpa = 50;
#endif
#ifndef win_h_kpa
rt_int32_t win_h_kpa = 70;
#endif
*/
#endif /* APPLICATIONS_CONFIGURATION_H_ */
