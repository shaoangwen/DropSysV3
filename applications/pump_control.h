/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-11     wen       the first version
 */
#ifndef APPLICATIONS_PUMP_CONTROL_H_
#define APPLICATIONS_PUMP_CONTROL_H_
#include <rtthread.h>
extern rt_int32_t p_kpa;
extern rt_thread_t p_kpa_read_thread;
extern rt_thread_t inflating_thread;
void p_kpa_read_entry();
int p_kpa_read_init();
int inflating_start(void);
void inflating_stop(void);

#endif /* APPLICATIONS_PUMP_CONTROL_H_ */
