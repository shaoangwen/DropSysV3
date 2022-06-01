/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-10     shao'ang       the first version
 */
#ifndef APPLICATIONS_I2C2_H_
#define APPLICATIONS_I2C_H_

#ifndef i2c_bus
#define i2c_bus "i2c2"
#endif
rt_uint8_t data_buf;
rt_uint8_t cmd_buf; //measure command buffer, write to 0x30 register
rt_uint8_t p1;  //8 bit MSB of 24 bit pressure
rt_uint8_t p2;  //8 bit CSB of 24 bit pressure
rt_uint8_t p3;  //8 bit LSB of 24 bit pressure
rt_int32_t prs[5];    //24 bit pressure, raw value
struct rt_i2c_bus_device *dev;    //define the i2c device

void i2c_init();
rt_int32_t* air_pressure();

#endif /* APPLICATIONS_I2C2_H_ */
