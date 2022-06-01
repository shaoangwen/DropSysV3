/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-11     wen       the first version
 */
#include <pump_control.h>
#include <I2C2.h>
#include <io_define.h>
#include <configuration.h>
#include <pressure_control.h>
#include <serial_interface.h>

rt_thread_t p_kpa_read_thread = RT_NULL;
rt_thread_t inflating_thread = RT_NULL;
uint8_t pump_off_cmd[] = {0xDE, 0xA1, 0x05, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCD, 0xCD, 0xCC};
uint8_t pump_on_cmd[] = {0xDE, 0xA1, 0x05, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xCD, 0xCD, 0xCC};


rt_int32_t p_kpa = 0;
void p_kpa_read_entry(){
    rt_int32_t *p_list;
    while(1){
        p_list = air_pressure();
        p_kpa = *(p_list + 4);
        report_cmd[10] = (uint8_t) *(p_list + 1);
        report_cmd[11] = (uint8_t) *(p_list + 2);
        report_cmd[12] = (uint8_t) *(p_list + 3);
        report_cmd[13] = (uint8_t) *(p_list + 4);
        //rt_kprintf("report_cmd [8] is %x\n", report_cmd[8]);
        //rt_kprintf("report_cmd [9] is %x\n", report_cmd[9]);
        //rt_kprintf("report_cmd [10] is %x\n", report_cmd[10]);
        rt_thread_mdelay(50);
    }
}

static void current_pressure(){
    rt_kprintf("pressure is %d Kpa \r\n", p_kpa);
}
int p_kpa_read_init(){
    rt_err_t ret = RT_EOK;
    p_kpa_read_thread = rt_thread_create("p_kpa_read", p_kpa_read_entry, RT_NULL, 1024, p_kpa_read_thread_priority, 10);
    if (p_kpa_read_thread != RT_NULL){
            rt_thread_startup(p_kpa_read_thread);
            rt_kprintf("p_kpa_reading thread started!\n");
        }else{
            ret = RT_ERROR;
        }
        return ret;
}


void inflating_thread_entry()
{
    //p_kpa = air_pressure();
    while(1){
        if( p_kpa <= win_l_kpa ){
            rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
            rt_device_write(serial6, 0, &pump_on_cmd[0], 11);
            rt_pin_write(PUMP_pin, 1);
            rt_thread_mdelay(50);
            rt_sem_release(&tx6_sem);
            while( p_kpa <= win_h_kpa){
                rt_pin_write(PUMP_pin, 1);
                rt_thread_mdelay(50);
                //p_kpa = air_pressure();
            }
            rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
            rt_device_write(serial6, 0, &pump_off_cmd[0], 11);
            rt_pin_write(PUMP_pin, 0);
            rt_thread_mdelay(50);
            rt_sem_release(&tx6_sem);
        }
        //p_kpa = air_pressure();
        rt_thread_mdelay(50);
    }
}

int inflating_start(void){
    rt_err_t ret = RT_EOK;
    if (inflating_thread == RT_NULL){
        inflating_thread = rt_thread_create("inflating", inflating_thread_entry, RT_NULL, 1024, inflating_thread_priority, 10);
    }else{
        ret = RT_ERROR;
        return ret;
    }
    if (inflating_thread != RT_NULL){
        rt_thread_startup(inflating_thread);
        rt_kprintf("inflating thread started!\n");
    }else{
        ret = RT_ERROR;
    }
    return ret;
}

void inflating_stop(void){
    if(inflating_thread != RT_NULL){
        if(RT_EOK == rt_thread_delete(inflating_thread)){
            rt_kprintf("inflating thread stopped!\n");
            inflating_thread = RT_NULL;
            rt_pin_write(PUMP_pin, 0);
        }
    }
}


MSH_CMD_EXPORT(inflating_start, start inflating);
MSH_CMD_EXPORT(inflating_stop, stop inflating);
MSH_CMD_EXPORT(current_pressure, read pressure in cylender);



