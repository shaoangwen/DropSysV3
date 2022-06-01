/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-12     wen       the first version
 */
#include <rtthread.h>
rt_int32_t win_l_kpa = 90;
rt_int32_t win_h_kpa = 100;
rt_uint8_t inflating_thread_priority = 17;
rt_uint8_t p_kpa_read_thread_priority = 17;
//the control of most servos in the market are not really duty cycle dependent, they relies on the absolute length of pulse. For the 20Kg servo which is the most in stock, 0~180deg corresponded to 0.25-1.25ms pulse width.
int door_pos0_dc = 10;      //min is 9@400Hz
int door_pos1_dc = 40;     //max is 50@400Hz
int door_speed = 95;    //percentage of door speed
rt_uint8_t door_thread_priority = 17;

rt_uint8_t report_pressure_thread_priority = 19;

rt_uint8_t M1_thread_priority = 20;
rt_uint8_t M2_thread_priority = 17;
rt_uint8_t M3_thread_priority = 20;

rt_uint8_t M1_homing_thread_priority = 19;
rt_uint8_t M2_homing_thread_priority = 18;
rt_uint8_t M3_homing_thread_priority = 18;
rt_uint8_t auto_homing_thread_priority = 19;
