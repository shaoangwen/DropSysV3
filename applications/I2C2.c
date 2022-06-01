/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-08-05     Shao'ang       the first version
 */

#include <rtdevice.h>
#include <rtthread.h>
#include <board.h>
#include <I2C2.h>
//#define i2c_bus "i2c2"
#define pressure_sensor_addr 0x6D  //address of sensor


void i2c_init(const char *name){
    dev = (struct rt_i2c_bus_device *)rt_device_find(name);
    if (!dev){
        rt_kprintf("i2c2 finding failed!\n");
        return ;
    }
}

static rt_err_t write_reg(struct rt_i2c_bus_device *bus, rt_uint8_t reg, rt_uint8_t *data)
{
    rt_uint8_t buf[2];
    struct rt_i2c_msg msgs;
    buf[0] = reg;
    buf[1] = *data;
    msgs.addr = pressure_sensor_addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = buf;
    msgs.len = 2;
    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

static rt_uint8_t read_regs(struct rt_i2c_bus_device *bus, rt_uint8_t reg)
{
    rt_uint8_t buf;
    struct rt_i2c_msg msgs;
    buf = reg;
    msgs.addr = pressure_sensor_addr;
    msgs.flags = RT_I2C_WR;
    msgs.buf = &buf;
    msgs.len = 1;
    /* 调用I2C设备接口传输数据 */
    rt_i2c_transfer(bus, &msgs, 1);
    rt_thread_mdelay(10);
    msgs.flags = RT_I2C_RD;
    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return buf;
    }
    else
    {
        return 0xff;
        rt_kprintf("reading failed!\n");
    }
}




rt_int32_t* air_pressure(){
    // read the calibrate value and write back after &0xFD
    data_buf = 0x00;
    data_buf = read_regs(dev, 0xA5);
    //rt_kprintf("data_buf is %x \r\n", data_buf);
    data_buf =  data_buf & 0xFD;
    write_reg(dev, 0xA5, &data_buf);

    cmd_buf = 0x0A; //measure command, once temperature and once pressure
    write_reg(dev, 0x30, &cmd_buf);      //send command to 0x30 registor
    rt_thread_mdelay(10);
    cmd_buf = read_regs(dev, 0x30); //read the status of measurement, cmd_buf & 0x80 == 0 indicates the finish of measurement
    //rt_kprintf("(cmd_buf & 0x08) is %x \r\n", (cmd_buf & 0x08));
    if((cmd_buf & 0x08) > 0){
        while ((read_regs(dev, 0x30) & 0x08) > 0){
            rt_thread_mdelay(10);
        }
    }
    p1 = read_regs(dev, 0x06);
    //rt_kprintf("p1 is %x \r\n", p1);
    p2 = read_regs(dev, 0x07);
    //rt_kprintf("p2 is %x \r\n", p2);
    p3 = read_regs(dev, 0x08);
    //rt_kprintf("p3 is %x \r\n", p3);
    prs[0] = p1*65536+p2*256+p3;
    prs[1] = p1;
    prs[2] = p2;
    prs[3] = p3;
    //rt_kprintf("raw pressure is %d \r\n", prs);
    rt_int32_t prs_kpa;
    if (prs[0] > 8388608){
        //prs_kpa = ((rt_int32_t)prs[0] - 16777216)/32/1000;
        prs[4] = ((rt_int32_t)prs[0] - 16777216)/32/1000;
        prs_kpa = prs[4];
        //rt_kprintf("vacuum pressure is %d Kpa \r\n", prs_kpa);
        //return prs_kpa;
        return prs;
    }else {
        //prs_kpa = prs/32/1000;
        prs[4] = prs[0]/32/1000;
        prs_kpa = prs[4];
        //rt_kprintf("pressure is %d Kpa \r\n", prs_kpa);
        //return prs_kpa;
        return prs;
    }
}

MSH_CMD_EXPORT(air_pressure, read pump pressure);


