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
#include <pwm.h>
#include <configuration.h>
#include <Door_control.h>
#include <serial_interface.h>

//the control of most servos in the market are not really duty cycle dependent, they relies on the absolute length of pulse. For the 20Kg servo which is the most in stock, 0~180deg corresponded to 0.25-1.25ms pulse width.
//int door_pos0_dc = 10;      //min is 9@400Hz
//int door_pos1_dc = 40;     //max is 50@400Hz
//int door_speed = 10;    //percentage of door speed

static struct rt_semaphore door_sem;
rt_thread_t door_open_thread = RT_NULL;
rt_thread_t door_close_thread = RT_NULL;
uint8_t door_done_cmd[] = {0xDE, 0xFA, 0x03, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCD, 0xCD, 0xCC};

#if 0
void door_run(int pos){
    //rt_kprintf("door_run entered \r\n");
    if (pos == 0){
            pulse = period*door_pos1_dc/100;
            rt_pwm_set(pwm4_dev, PWM4_DEV_CHANNEL4, period, pulse);
            rt_thread_mdelay(50);//to ensure 20 period were output
            rt_thread_mdelay(1000-10*door_speed);
    }else if (pos == 1){
            pulse = period*door_pos0_dc/100;
            rt_pwm_set(pwm4_dev, PWM4_DEV_CHANNEL4, period, pulse);
            rt_thread_mdelay(50);//to ensure 20 period were output
            rt_thread_mdelay(1000-10*door_speed);
    }
    rt_sem_release(&door_sem);
    //rt_kprintf("doorred 1 cycle \r\n");
}
#endif


static void door_open_entry(void *parameter)
{
    rt_kprintf("open door is waiting  for semaphore \r\n");

    rt_sem_take(&door_sem, RT_WAITING_FOREVER);
    //rt_kprintf("door_sem is %d \r\n", door_sem);
    rt_kprintf("opening door \r\n");
    door_run(0);
    rt_thread_mdelay(1500);
    door_open_thread = RT_NULL;
    rt_sem_release(&door_sem);
    rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
    rt_device_write(serial6, 0, &door_done_cmd[0], 11);
    rt_thread_mdelay(50);
    rt_sem_release(&tx6_sem);
}
static void door_close_entry(void *parameter)
{
    rt_kprintf("close door is waiting  for semaphore \r\n");

    rt_sem_take(&door_sem, RT_WAITING_FOREVER);
    //rt_kprintf("door_sem is %d \r\n", door_sem);
    rt_kprintf("closing door \r\n");
    door_run(1);
    rt_thread_mdelay(1500);
    door_close_thread = RT_NULL;
    rt_sem_release(&door_sem);
    rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
    rt_device_write(serial6, 0, &door_done_cmd[0], 11);
    rt_thread_mdelay(50);
    rt_sem_release(&tx6_sem);
}

void door_init(void){
    rt_sem_init(&door_sem, "door_sem", 1, RT_IPC_FLAG_FIFO);
    rt_kprintf("door_sem is %d \r\n", door_sem);
}

void door_open(void){
    if (door_open_thread == RT_NULL){
        door_open_thread = rt_thread_create("door_open", door_open_entry, RT_NULL, 1024, door_thread_priority, 25);
        rt_thread_startup(door_open_thread);
        rt_kprintf("open door thread started \r\n");
    }else{
        //rt_thread_startup(door_open_thread);
        rt_kprintf("open door thread is running \r\n");
    }
    door_open_thread = RT_NULL;
    //rt_sem_release(&door_sem);
}

void door_close(void){
    if (door_close_thread == RT_NULL){
        door_close_thread = rt_thread_create("door_close", door_close_entry, RT_NULL, 1024, door_thread_priority, 25);
        rt_thread_startup(door_close_thread);
        rt_kprintf("close door thread started \r\n");
    }else{
        //rt_thread_startup(door_close_thread);
        rt_kprintf("close door thread is running \r\n");
    }

    //rt_sem_release(&door_sem);
}



void door_run(int pos){
    rt_kprintf("door_run entered \r\n");
    if (pos == 0){
       // rt_kprintf("door_run pos0 \r\n");
        for(int i = door_pos0_dc; i <= door_pos1_dc; i+=2){
           // rt_kprintf("door_run step2 for cycle \r\n");
            pulse = period*i/100;
            rt_pwm_set(pwm4_dev, PWM4_DEV_CHANNEL4, period, pulse);
            //rt_thread_mdelay(50);//to ensure 20 period were output
            rt_thread_mdelay(1000-10*door_speed);
           // rt_kprintf("door_run cycle %d is done \r\n", i);
        }

    }else if (pos == 1){
       // rt_kprintf("door_run pos1 \r\n");
        for(int i = door_pos1_dc; i >= door_pos0_dc; i-=2){
            //rt_kprintf("door_run step2 for cycle \r\n");
            pulse = period*i/100;
            rt_pwm_set(pwm4_dev, PWM4_DEV_CHANNEL4, period, pulse);
            //rt_thread_mdelay(50);//to ensure 20 period were output
            rt_thread_mdelay(1000-10*door_speed);
        }
    }
    //rt_sem_release(&door_sem);
    //rt_kprintf("door run 1 cycle \r\n");
}

MSH_CMD_EXPORT(door_open, open the door);
MSH_CMD_EXPORT(door_close, close the door);
