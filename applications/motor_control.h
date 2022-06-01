/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-11     wen       the first version
 */
#ifndef APPLICATIONS_MOTOR_CONTROL_H_
#define APPLICATIONS_MOTOR_CONTROL_H_

rt_uint32_t m1_a1;
rt_uint32_t m1_d1;
rt_uint32_t m1_v1;
rt_uint32_t m1_vmax;
rt_uint32_t m1_amax;
rt_uint32_t m1_dmax;
rt_uint32_t m1_target;
rt_uint32_t m1_current;
rt_uint8_t m1_ihlod;
rt_uint8_t m1_irun;
rt_uint8_t m1_idelay;
struct rt_semaphore m1_sem;
rt_thread_t M1_thread;
rt_thread_t M1_homing_thread;

rt_uint32_t m2_a1;
rt_uint32_t m2_d1;
rt_uint32_t m2_v1;
rt_uint32_t m2_vmax;
rt_uint32_t m2_amax;
rt_uint32_t m2_dmax;
rt_uint32_t m2_target;
rt_uint32_t m2_current;
rt_uint8_t m2_ihlod;
rt_uint8_t m2_irun;
rt_uint8_t m2_idelay;
struct rt_semaphore m2_sem;
rt_thread_t M2_thread;
rt_thread_t M2_homing_thread;

rt_uint32_t m3_a1;
rt_uint32_t m3_d1;
rt_uint32_t m3_v1;
rt_uint32_t m3_vmax;
rt_uint32_t m3_amax;
rt_uint32_t m3_dmax;
rt_uint32_t m3_target;
rt_uint32_t m3_current;
rt_uint8_t m3_ihlod;
rt_uint8_t m3_irun;
rt_uint8_t m3_idelay;
rt_int32_t m3_cycle_delay;
struct rt_semaphore m3_sem;
rt_thread_t M3_thread;
rt_thread_t M3_homing_thread;
rt_thread_t auto_homing_thread;



void M1_set();
void M1_init();
void M1_run_entry(void* param);
void M1_run(void* param);
void M1_stop();

void M2_set();
void M2_init();
void M2_run_entry(void* param);
void M2_run(void* param);
void M2_stop();

void M3_set();
void M3_init();
void M3_thread_entry();
int M3_run(void);
void M3_stop();


void M1_homing_entry();
void M2_homing_entry();
void M3_homing_entry();

void M1_homing();
void M2_homing();
void M3_homing();

void auto_homing();
void set_motor(uint8_t arg[]);

#endif /* APPLICATIONS_MOTOR_CONTROL_H_ */
