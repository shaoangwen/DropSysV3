/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-11     wen       the first version
 */
#ifndef APPLICATIONS_SERIAL_INTERFACE_H_
#define APPLICATIONS_SERIAL_INTERFACE_H_

void serial_config(void);
int uart1_init(void);
int uart2_init(void);
int uart6_init(void);
void serial1_thread_entry(void *parameter);
void serial2_thread_entry(void *parameter);
void serial6_thread_entry(void *parameter);

rt_device_t serial1;
rt_device_t serial2;
rt_device_t serial6;
struct rt_semaphore rx1_sem;
struct rt_semaphore rx2_sem;
struct rt_semaphore rx6_sem;
struct rt_semaphore tx6_sem;
struct rt_semaphore tx1_sem;
struct rt_semaphore tx2_sem;




#endif /* APPLICATIONS_SERIAL_INTERFACE_H_ */
