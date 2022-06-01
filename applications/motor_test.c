/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-13     wen       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <tmc5130_spi.h>
#include <tmc5130_reg.h>
#include <motor_control.h>

// 正负数转换参考 https://blog.csdn.net/doudouwa1234/article/details/46696919// ~num+1 正转负，~(num-1)负转正
void test_1(){
    //定位模式,转一圈
    TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, (~51200 + 1)); // 10 round reverse
    //TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, 51200*10); //number of microsteps, 200 steps (1.8deg full step) per round; 1 full step = 256 microsteps, max:2^31 = 51200*41943 51200 per round, total 41943 rounds
    //TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, 51200*30  | 0x80000000); // 1 round reverse
    TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    0);
    TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    655320);//8,388,096 max

    //定位模式停止
//  TMC_SPIWriteInt(motor, TMC5130_XTARGET, 0);
//  TMC_SPIWriteInt(motor, TMC5130_XACTUAL, 0); //务必TMC5130_XACTUAL == TMC5130_XTARGET
//  TMC_SPIWriteInt(motor, TMC5130_VMAX ,   0);
}

void test_all_rwd(){
    TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    2);
    TMC_SPIWriteInt(spi_20, TMC5130_VMAX,    163830);
    TMC_SPIWriteInt(spi_21, TMC5130_RAMPMODE ,    2);
    TMC_SPIWriteInt(spi_21, TMC5130_VMAX,    163830);
    TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    2);
    TMC_SPIWriteInt(spi_22, TMC5130_VMAX,    163830);
}

void test_all_fwd(){
    TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    1);
    TMC_SPIWriteInt(spi_20, TMC5130_VMAX,    163830);
    TMC_SPIWriteInt(spi_21, TMC5130_RAMPMODE ,    1);
    TMC_SPIWriteInt(spi_21, TMC5130_VMAX,    163830);
    TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    1);
    TMC_SPIWriteInt(spi_22, TMC5130_VMAX,    163830);
}





void test_fwd(){
    //速度模式,正转, switch R
    TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    1);
    TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    655320);//8,388,096 max// 67200 = 1r/s
}

void test_rwd(){
    //速度模式,反转 switch L
    TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    2);
    TMC_SPIWriteInt(spi_20, TMC5130_VMAX,    163830);
}
void test_stop(){
    //速度模式停止
    TMC_SPIWriteInt(spi_20, TMC5130_VMAX ,   0);
    TMC_SPIWriteInt(spi_21, TMC5130_VMAX ,   0);
    TMC_SPIWriteInt(spi_22, TMC5130_VMAX ,   0);
}

int homing(){
    rt_uint32_t swstat;
      swstat = TMC_SPIReadInt(spi_20, TMC5130_RAMPSTAT);
      swstat = swstat & 0b11;
      if ((swstat & 0b10)!=0){
          TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, 0);
          TMC_SPIWriteInt(spi_20,TMC5130_XACTUAL  ,    0x00000000);
          //rt_kprintf("right aligned \n");
          //return 1;
      } else{
          //test_fwd();
          //速度模式,正转, switch R
          TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    1);
          TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    655320);//8,388,096 max// 67200 = 1r/s
          rt_kprintf("homing \n");
          while((swstat & 0b10) == 0){
              swstat = TMC_SPIReadInt(spi_20, TMC5130_RAMPSTAT);
              swstat = swstat & 0b11;
          }
          TMC_SPIWriteInt(spi_20, TMC5130_VMAX ,   0);
          TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, 0);
          TMC_SPIWriteInt(spi_20,TMC5130_XACTUAL  ,    0x00000000);
      }
      //turn back 8mm
      rt_kprintf("homing_stage2 \n");

      TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, (~102400 + 1));
      TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    0);
      TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    655320);
      swstat = TMC_SPIReadInt(spi_20, TMC5130_XACTUAL);
      while(swstat != (~102400 + 1)){
                    swstat = TMC_SPIReadInt(spi_20, TMC5130_XACTUAL);
                }
      //速度模式,正转, switch R,slowly
        TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    1);
        TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    22400);//8,388,096 max// 67200 = 1r/s
        swstat = TMC_SPIReadInt(spi_20, TMC5130_RAMPSTAT);
        while((swstat & 0b10) == 0){
                      swstat = TMC_SPIReadInt(spi_20, TMC5130_RAMPSTAT);
                      swstat = swstat & 0b11;
                  }
        //stop and set position to 0
        TMC_SPIWriteInt(spi_20, TMC5130_VMAX ,   0);
          TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, 0);
          TMC_SPIWriteInt(spi_20,TMC5130_XACTUAL  ,    0x00000000);
          rt_kprintf("right aligned \n");
          return 2;

 }

void read_pos(){
    TMC_SPIReadInt(spi_20, TMC5130_XACTUAL);
}

void turn_around(){
        rt_uint32_t cnt;
        cnt = 0;
        homing();
        while(1){
            cnt ++;
            //定位模式,转一圈
    #if 0
            rt_kprintf("Forwarding\n");
                TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, (512000*3)  | 0x80000000); // 1 round reverse
                TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    0);
                TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    655320);//8,388,096 max
                rt_thread_mdelay(3000);
                rt_kprintf("Zeroing\n");
                TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, 0);
                TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    0);
                TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    655320);//8,388,096 max
                rt_thread_mdelay(3000);
                rt_kprintf("Reversing\n");
                TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, (512000*3));
                TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    0);
                TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    655320);//8,388,096 max
                rt_thread_mdelay(3000);
                rt_kprintf("Zeroing\n");
                TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, 0);
                TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    0);
                TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    655320);//8,388,096 max
                rt_thread_mdelay(3000);
    #endif
                //rt_kprintf("Reversing\n");
                TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, (~1536000 + 1));
                TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    0);
                TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    982980);//8,388,096 max
                rt_thread_mdelay(5000);
                //rt_kprintf("Zeroing\n");
                TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, 0);
                TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    0);
                TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    982980);//8,388,096 max
                rt_thread_mdelay(5000);
                rt_kprintf("%d cycles tested \n", cnt);
        }
}





MSH_CMD_EXPORT(test_1, sigle run);
MSH_CMD_EXPORT(test_fwd, forwardding);
MSH_CMD_EXPORT(test_rwd, reversing);
MSH_CMD_EXPORT(test_stop, motor stop);
MSH_CMD_EXPORT(turn_around, motor turn around);
MSH_CMD_EXPORT(homing, motor homing fun);
MSH_CMD_EXPORT(read_pos, read current possition);

MSH_CMD_EXPORT(test_all_fwd, all_forwardding);
MSH_CMD_EXPORT(test_all_rwd, all_forwardding);
