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
#include <rtdevice.h>
#include <io_define.h>

void GPIO_init(){
    rt_pin_mode(V1_pin, PIN_MODE_OUTPUT);
    rt_pin_mode(V2_pin, PIN_MODE_OUTPUT);
    rt_pin_mode(V3_pin, PIN_MODE_OUTPUT);
    rt_pin_mode(V4_pin, PIN_MODE_OUTPUT);
    rt_pin_mode(PUMP_pin, PIN_MODE_OUTPUT);
    rt_pin_mode(M1_en, PIN_MODE_OUTPUT);
    rt_pin_mode(M2_en, PIN_MODE_OUTPUT);
    rt_pin_mode(M3_en, PIN_MODE_OUTPUT);

    rt_pin_mode(R1_sw, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(L1_sw, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(R2_sw, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(L2_sw, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(R3_sw, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(L3_sw, PIN_MODE_INPUT_PULLUP);


    rt_pin_write(V1_pin, PIN_LOW);
    rt_pin_write(V2_pin, PIN_LOW);
    rt_pin_write(V3_pin, PIN_LOW);
    rt_pin_write(V4_pin, PIN_HIGH);
    rt_pin_write(PUMP_pin, PIN_LOW);
    rt_pin_write(M1_en, PIN_LOW);
    rt_pin_write(M2_en, PIN_LOW);
    rt_pin_write(M3_en, PIN_LOW);
}


