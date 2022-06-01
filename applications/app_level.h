/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-26     wen       the first version
 */
#ifndef APPLICATIONS_APP_LEVEL_H_
#define APPLICATIONS_APP_LEVEL_H_
#include <rtthread.h>
void connect(dir);
void drawer(dir);
void stir(dir);
void inflating_control(dir);
void init_system_entry();
void shut_down();
#endif /* APPLICATIONS_APP_LEVEL_H_ */
