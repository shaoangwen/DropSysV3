/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-11     wen       the first version
 */
#include <rtthread.h>
#include <serial_interface.h>
#include <valves_control.h>
#include <cmd_handler.h>
#include <pressure_control.h>
#include <pump_control.h>
#include <Door_control.h>
#include <io_define.h>
#include <motor_control.h>
#include <app_level.h>
//extern uint8_t report_cmd[];

uint8_t scan_code_cmd[] = {0x7E, 0x00, 0x08, 0x01, 0x00, 0x02, 0x01, 0xAB, 0xCD};


void cmd_deliver(uint8_t arg[], uint8_t len){
    if (arg[0] == 0xEE)
    {
        rt_kprintf("dc_command \r\n");
        dc_cmd_handle(arg);
    }else if (arg[0] == 0xDD)
    {
        dropbus_cmd_handle(arg);
        rt_kprintf("dropbus_command \r\n");
    }else if(arg[0] == 0x5E)
    {
        cosys_cmd_handle(arg);
    }else if(arg[0] == 0x47)
    {
        code_scanner_handle(arg, len);
    }
}


void code_scanner_handle(uint8_t arg[], uint8_t len)
{
    forward_uart6(arg, len);
}

void forward_uart6(uint8_t arg[], uint8_t len)
{
    rt_kprintf("command forwarding %x \r\n", len);
    rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
    rt_kprintf("sizeof(tmp)/sizeof(arg[0]) is %x\r\n", len);
    rt_device_write(serial6, 0, &arg[0], len);
    rt_sem_release(&tx6_sem);
    rt_kprintf("command forwarded \r\n");
}

void dropbus_cmd_handle(uint8_t arg[])
{
    if (arg[1] == 0xA1)
    {
        switch(arg[2])
        {
            case 0xF1: stop_run(); rt_kprintf("run stopped\r\n"); break;
            case 1: valve_set(1, arg[3]); rt_kprintf("v1 set\r\n"); break;
            case 2: valve_set(2, arg[3]); rt_kprintf("v2 set\r\n"); break;
            case 3: valve_set(3, arg[3]); rt_kprintf("v3 set\r\n"); break;
            case 4: valve_set(4, arg[3]); rt_kprintf("v4 set\r\n"); break;
            case 5: inflating_stop(); rt_pin_write(PUMP_pin, arg[3]); rt_kprintf("pump set\r\n"); break;
        }
    }
    else if(arg[1] == 0xA2)
    {
         set_pressure(arg);
        return;
     }
    else if(arg[1] == 0xFA){
        switch(arg[2])
        {
            case 0x02: scan_code(); break;
            case 0x03: if(arg[3] == 0x00){door_close();}else if(arg[3] == 0x01){door_open();} break;
            case 0x04: auto_homing(); break;
            case 0x05: connect(arg[3]); break;
            case 0x06: drawer(arg[3]); break;
            case 0x07: M3_homing(); break;
            case 0x08: stir(arg[3]); break;
            case 0x09: inflating_control(arg[3]); break;
            case 0xA1: shut_down(); break;
        }
        return;
     }
    else if(arg[1] == 0xB3){
        read_pid_set(arg[2], arg[3]);
        rt_thread_mdelay(165);
        if(arg[3] == 0x05){
            upload_pid(send_pid, 18);
        }else{
            upload_pid(report_pid, 11);
        }
    }
    else if(arg[1] == 0xA6){
        set_pid(arg);
    }
    else if((arg[1] == 0xA3) | (arg[1] == 0xA4) | (arg[1] == 0xA5)){
        set_motor(arg);
    }
}


void cosys_cmd_handle(uint8_t arg[])
{
    if ((arg[2] == 0x19) && (arg[3] == 0x40))
    {
        switch(arg[1])
        {
            case 0x0A: report_cmd[2] = arg[5]; report_cmd[3] = arg[4]; break;
            case 0x0B: report_cmd[4] = arg[5]; report_cmd[5] = arg[4]; break;
            case 0x0C: report_cmd[6] = arg[5]; report_cmd[7] = arg[4]; break;
        }
    }

}

void scan_code(){
    rt_sem_take(&tx2_sem, RT_WAITING_FOREVER);
    rt_device_write(serial2, 0, &scan_code_cmd[0], 9);
    rt_sem_release(&tx2_sem);
    rt_kprintf("scan code command forwarded \r\n");
}

void dc_cmd_handle(uint8_t arg[])
{

}

MSH_CMD_EXPORT(scan_code, scan_code);
