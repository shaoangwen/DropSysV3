/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-12     wen       the first version
 */
#ifndef APPLICATIONS_DOOR_CONTROL_H_
#define APPLICATIONS_DOOR_CONTROL_H_
#include <rtthread.h>

rt_thread_t door_open_thread;
rt_thread_t door_close_thread;

void door_init(void);
void door_open(void);
void door_close(void);
void door_run(int pos);

#endif /* APPLICATIONS_DOOR_CONTROL_H_ */
