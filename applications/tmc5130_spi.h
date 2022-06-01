/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-11     wen       the first version
 */
#ifndef APPLICATIONS_TMC5130_SPI_H_
#define APPLICATIONS_TMC5130_SPI_H_
#include <rtthread.h>
#include <rtdevice.h>
#include <tmc5130_reg.h>

struct rt_spi_configuration cfg;
struct rt_spi_device *spi_20;
struct rt_spi_device *spi_21;
struct rt_spi_device *spi_22;

int rt_hw_spi_init2();
void TMC5130_init();

u_int32_t  TMC_SPIReadInt(struct rt_spi_device *spi_dev, u_int8_t reg);
void TMC_SPIWriteInt(struct rt_spi_device *spi_dev, u_int8_t reg, u_int32_t dat);


#endif /* APPLICATIONS_TMC5130_SPI_H_ */
