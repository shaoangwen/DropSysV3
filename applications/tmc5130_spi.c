/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-16     wen       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <tmc5130_reg.h>
#include <tmc5130_spi.h>

#include "drv_common.h"
#include "drv_spi.h"


/*
struct rt_spi_configuration cfg;
struct rt_spi_device *spi_20;
struct rt_spi_device *spi_21;
struct rt_spi_device *spi_22;
*/

int rt_hw_spi_init2(void)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    rt_err_t ret;
    ret = rt_hw_spi_device_attach("spi2", "spi20", GPIOD, GPIO_PIN_1);
    ret = rt_hw_spi_device_attach("spi2", "spi21", GPIOD, GPIO_PIN_3);
    ret = rt_hw_spi_device_attach("spi2", "spi22", GPIOD, GPIO_PIN_4);
    if (ret != RT_EOK)
    {
        rt_kprintf("spi20 attach failed!\n");
    }else{
        rt_kprintf("spi20 attach success!\n");
    }
    return RT_EOK;
}

static void config_spi2()
{
    cfg.data_width = 8;
    cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB;
    cfg.max_hz = 1 * 1000 *1000;                           /* 1M */
}

static int spi_bus_init()
{
    spi_20 = (struct rt_spi_device *)rt_device_find("spi20");
    spi_21 = (struct rt_spi_device *)rt_device_find("spi21");
    spi_22 = (struct rt_spi_device *)rt_device_find("spi22");
    if(!spi_20){
        rt_kprintf("finding spi20 failed!\n");
        return RT_ERROR;
    }else{
        rt_kprintf("spi20 founded!\n");
    }
    config_spi2();
    rt_err_t ret;
    ret = rt_spi_configure(spi_20, &cfg);
    ret = rt_spi_configure(spi_21, &cfg);
    ret = rt_spi_configure(spi_22, &cfg);
    if (ret != RT_EOK)
    {
        rt_kprintf("spi2x config failed!\n");
        return ret;
    }else{
        rt_kprintf("spi2x config success!\n");
        return RT_EOK;
    }
}




u_int32_t  TMC_SPIReadInt(struct rt_spi_device *spi_dev, u_int8_t reg)
{
    u_int8_t s_buf[5] = {0x00,0x00,0x00,0x00,0x00};
    u_int8_t r_buf[5] = {0x00,0x00,0x00,0x00,0x00};
    u_int32_t status = 0;
    s_buf[0] = reg;

    rt_size_t trans_stat = 0;
    int i = 0;
    while((i<10) & (trans_stat == 0)){
        trans_stat = rt_spi_send(spi_dev, s_buf, 5);
        i++;
    }
    i = 0 ;
    trans_stat = 0;
    while((i<10) & (trans_stat == 0)){
        trans_stat = rt_spi_transfer(spi_dev, s_buf, r_buf, 5);
        //rt_spi_send_then_recv(spi_dev, s_buf, 5, r_buf, 5); //this func will write 0xFF to read data, may cause write to the unkonwn 0x7F register.
        i++;
    }
    /*
    rt_kprintf("GSTAT[0] is 0x%x!\n", r_buf[0]);
    rt_kprintf("GSTAT[1] is 0x%x!\n", r_buf[1]);
    rt_kprintf("GSTAT[2] is 0x%x!\n", r_buf[2]);
    rt_kprintf("GSTAT[5] is 0x%x!\n", r_buf[3]);
    rt_kprintf("GSTAT[4] is 0x%x!\n", r_buf[4]);
    */
    status = (r_buf[4]) + (r_buf[3] << 8) + (r_buf[2] << 16) + (r_buf[1] << 24);
    return status;
}


void TMC_SPIWriteInt(struct rt_spi_device *spi_dev, u_int8_t reg, u_int32_t dat)
{
    u_int8_t s_buf[5] = {0x00,0x00,0x00,0x00,0x00};
    s_buf[0] = (reg|0x80);
    s_buf[1] = (dat >> 24) & 0xFF;
    s_buf[2] = (dat >> 16) & 0xFF;
    s_buf[3] = (dat >> 8) & 0xFF;
    s_buf[4] = (dat & 0xFF);
    rt_size_t trans_stat = 0;
    int i = 0;
    while((i<10) & (trans_stat == 0)){
        trans_stat = rt_spi_send(spi_dev, s_buf, 5);
        i++;
    }
}
void TMC5130_init()
{
    spi_bus_init();
    u_int8_t gstat = 0;
    gstat = TMC_SPIReadInt(spi_20, TMC5130_GSTAT);
    //TMC_SPIReadInt(spi_20, 0x6F);
    rt_kprintf("GSTAT is %x!\n", gstat);
    gstat = TMC_SPIReadInt(spi_21, TMC5130_GSTAT);
    //TMC_SPIReadInt(spi_20, 0x6F);
    rt_kprintf("GSTAT is %x!\n", gstat);
    gstat = TMC_SPIReadInt(spi_22, TMC5130_GSTAT);
    //TMC_SPIReadInt(spi_20, 0x6F);
    rt_kprintf("GSTAT is %x!\n", gstat);
    TMC_SPIWriteInt(spi_20,TMC5130_GCONF,        0x00000004);
    TMC_SPIWriteInt(spi_20,TMC5130_CHOPCONF,     0x000100c3);
    TMC_SPIWriteInt(spi_20,TMC5130_IHOLD_IRUN, 0x00071E01);
    TMC_SPIWriteInt(spi_20,TMC5130_PWMCONF,  0x000401c8);

    TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, 0);
    TMC_SPIWriteInt(spi_20,TMC5130_XACTUAL  ,    0x00000000);
    TMC_SPIWriteInt(spi_20,TMC5130_VACTUAL  ,    0x00000000);
    TMC_SPIWriteInt(spi_20,TMC5130_VSTART   ,    5);
    TMC_SPIWriteInt(spi_20,TMC5130_A1       ,    1000);
    TMC_SPIWriteInt(spi_20,TMC5130_V1       ,    0);         //只用AMAX，DMAX

    TMC_SPIWriteInt(spi_20,TMC5130_DMAX     ,    5000);
    TMC_SPIWriteInt(spi_20,TMC5130_D1       ,    1400);
    TMC_SPIWriteInt(spi_20,TMC5130_VSTOP    ,    10);
    TMC_SPIWriteInt(spi_20,TMC5130_TZEROWAIT,    1000);
    TMC_SPIWriteInt(spi_20,TMC5130_AMAX     ,    5000);
    TMC_SPIWriteInt(spi_20,TMC5130_SWMODE     ,    0x0000000E); //0xE, only right switch enabled, 0xD, only left switch enabled, 0xF all enabled




    TMC_SPIWriteInt(spi_21,TMC5130_GCONF,        0x00000004);
    TMC_SPIWriteInt(spi_21,TMC5130_CHOPCONF,     0x000100c3);
    TMC_SPIWriteInt(spi_21,TMC5130_IHOLD_IRUN, 0x00071E01);
    TMC_SPIWriteInt(spi_21,TMC5130_PWMCONF,  0x000401c8);

    TMC_SPIWriteInt(spi_21, TMC5130_XTARGET, 0);
    TMC_SPIWriteInt(spi_21,TMC5130_XACTUAL  ,    0x00000000);
    TMC_SPIWriteInt(spi_21,TMC5130_VACTUAL  ,    0x00000000);
    TMC_SPIWriteInt(spi_21,TMC5130_VSTART   ,    5);
    TMC_SPIWriteInt(spi_21,TMC5130_A1       ,    1000);
    TMC_SPIWriteInt(spi_21,TMC5130_V1       ,    0);         //只用AMAX，DMAX

    TMC_SPIWriteInt(spi_21,TMC5130_DMAX     ,    5000);
    TMC_SPIWriteInt(spi_21,TMC5130_D1       ,    1400);
    TMC_SPIWriteInt(spi_21,TMC5130_VSTOP    ,    10);
    TMC_SPIWriteInt(spi_21,TMC5130_TZEROWAIT,    1000);
    TMC_SPIWriteInt(spi_21,TMC5130_AMAX     ,    5000);
    TMC_SPIWriteInt(spi_21,TMC5130_SWMODE     ,    0x0000000E);



    TMC_SPIWriteInt(spi_22,TMC5130_GCONF,        0x00000004);
    TMC_SPIWriteInt(spi_22,TMC5130_CHOPCONF,     0x000100c3);
    TMC_SPIWriteInt(spi_22,TMC5130_IHOLD_IRUN, 0x00070701);
    TMC_SPIWriteInt(spi_22,TMC5130_PWMCONF,  0x000401c8);

    TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, 0);
    TMC_SPIWriteInt(spi_22,TMC5130_XACTUAL  ,    0x00000000);
    TMC_SPIWriteInt(spi_22,TMC5130_VACTUAL  ,    0x00000000);
    TMC_SPIWriteInt(spi_22,TMC5130_VSTART   ,    5);
    TMC_SPIWriteInt(spi_22,TMC5130_A1       ,    1000);
    TMC_SPIWriteInt(spi_22,TMC5130_V1       ,    0);         //只用AMAX，DMAX

    TMC_SPIWriteInt(spi_22,TMC5130_DMAX     ,    5000);
    TMC_SPIWriteInt(spi_22,TMC5130_D1       ,    1400);
    TMC_SPIWriteInt(spi_22,TMC5130_VSTOP    ,    10);
    TMC_SPIWriteInt(spi_22,TMC5130_TZEROWAIT,    1000);
    TMC_SPIWriteInt(spi_22,TMC5130_AMAX     ,    5000);
    TMC_SPIWriteInt(spi_22,TMC5130_SWMODE     ,    0x0000000E);
}

MSH_CMD_EXPORT(TMC5130_init, initialize tmc);
