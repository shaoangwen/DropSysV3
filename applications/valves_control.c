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
#include <io_define.h>


void valve_set(uint8_t vlv, uint8_t vlu)
{
    switch(vlv){
        case 1: rt_pin_write(V1_pin, vlu); break;
        case 2: rt_pin_write(V2_pin, vlu); break;
        case 3: rt_pin_write(V3_pin, vlu); break;
        case 4: rt_pin_write(V4_pin, vlu); break;
    }
    rt_kprintf("valves set\n");
}

void stop_run()
{
    rt_pin_write(V1_pin, 0);
    rt_pin_write(V2_pin, 0);
    rt_pin_write(V3_pin, 0);
    //rt_pin_write(V4_pin, 0);
}
