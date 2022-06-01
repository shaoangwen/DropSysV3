/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-10     wen    first version
 */

#include <rtthread.h>
#include <rtdbg.h>
//#include <stm32f4xx_hal_rcc.h>
#include <drv_common.h>
//#include <configuration.h>
#include <I2C2.h>
#include <io_define.h>
#include <tmc5130_spi.h>
#include <valves_control.h>
#include <serial_interface.h>
#include <pump_control.h>
#include <pwm.h>
#include <Door_control.h>
#include <motor_control.h>
#include <pressure_control.h>
#include <app_level.h>




#define DBG_TAG "main"
#define DBG_LVL DBG_LOG


int main(void)
{
    i2c_init(i2c_bus);
    GPIO_init();
    p_kpa_read_init();
    rt_thread_mdelay(50);

    rt_hw_spi_init2();
    rt_thread_mdelay(10);
    TMC5130_init();
    M1_init();
    M2_init();
    M3_init();
    serial_config();
    uart1_init();
    uart2_init();
    uart6_init();
    pwm_init();
    door_init();
    report_thread_init_and_start();
    rt_thread_mdelay(50);
    inflating_start();
    //init_system_entry();

    int i=0;
    while(i<2){
    valve_set(1, 1);
    valve_set(2, 1);
    valve_set(3, 1);
    rt_thread_mdelay(200);
    valve_set(1, 0);
    valve_set(2, 0);
    valve_set(3, 0);
    rt_thread_mdelay(200);
    i++;
    }
    rt_kprintf("System clock info \n");
    rt_kprintf("SYSCLK_Frequency = d% \n", HAL_RCC_GetSysClockFreq());
    rt_kprintf("HCLK_Frequency = d% \n", HAL_RCC_GetHCLKFreq());
    rt_kprintf("PCLK1_Frequency = d% \n", HAL_RCC_GetPCLK1Freq());
    rt_kprintf("PCLK2_Frequency = d% \n", HAL_RCC_GetPCLK2Freq());

    return RT_EOK;
}
MSH_CMD_EXPORT(HAL_RCC_GetSysClockFreq,GET CLOCK FREQ);
