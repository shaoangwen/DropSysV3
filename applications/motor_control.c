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
#include <rtdevice.h>
#include <tmc5130_spi.h>
#include <tmc5130_reg.h>
#include <motor_control.h>
#include <configuration.h>
#include <serial_interface.h>

rt_uint32_t m1_a1 = 1000;   //0~2^16-1; 0~65535
rt_uint32_t m1_d1 = 1400;   //0~2^16-1; 0~65535s
rt_uint32_t m1_v1 = 1;  //0~2^20-1 ;0~1048575
rt_uint32_t m1_vmax = 16383*5*3;    //0~2^23-512; 0~8388096
rt_uint32_t m1_amax = 3000;    //0~2^16-1; 0~65535
rt_uint32_t m1_dmax = 5000;    //0~2^16-1; 0~65535
rt_uint32_t m1_target = 2703360;    //convert to negative integer (~m1_target+1) if needed; range:-2^31~2^31-1; -2147483648~2147483647; 51200 per round; 160mm(screw length)-23.5mm(nut lenght)-2.5mm-2mm=132mm; 132mm/2.5mm=52.8round; 52.8*51200=2703360
rt_uint32_t m1_current = 0x00071C01;    //combination of i_hold i_run and i_delay (not used yet);
rt_uint8_t m1_ihlod = 0x01; //0x01~0x1F;
rt_uint8_t m1_irun = 0x10;  //0x01~0x1F;
rt_uint8_t m1_idelay = 0x07;    //0~0x0F;
struct rt_semaphore m1_sem;
rt_thread_t M1_thread = RT_NULL;
rt_thread_t M1_homing_thread = RT_NULL;

rt_uint32_t m2_a1 = 1000;   //0~2^16-1; 0~65535
rt_uint32_t m2_d1 = 1400;   //0~2^16-1; 0~65535
rt_uint32_t m2_v1 = 1;  //0~2^20-1 ;0~1048575
rt_uint32_t m2_vmax = 163830;    //0~2^23-512; 0~8388096
rt_uint32_t m2_amax = 3000;    //0~2^16-1; 0~65535
rt_uint32_t m2_dmax = 5000;    //0~2^16-1; 0~65535
rt_uint32_t m2_target = 1356800;    //convert to negative integer (~m2_target+1) if needed; range:-2^31~2^31-1; -2147483648~2147483647; 51200 per round; 23.5mm/1mm=23.5round; 23.5*51200=1,203,200
rt_uint32_t m2_current = 0x00071001;    //combination of i_hold i_run and i_delay (not used yet);
rt_uint8_t m2_ihlod = 0x01; //0x01~0x1F;
rt_uint8_t m2_irun = 0x10;  //0x01~0x1F;
rt_uint8_t m2_idelay = 0x07;    //0~0x0F;
struct rt_semaphore m2_sem;
rt_thread_t M2_thread = RT_NULL;
rt_thread_t M2_homing_thread = RT_NULL;

rt_uint32_t m3_a1 = 50000/5;   //0~2^16-1; 0~65535
rt_uint32_t m3_d1 = 54000;   //0~2^16-1; 0~65535
rt_uint32_t m3_v1 = 1;  //0~2^20-1 ;0~1048575
rt_uint32_t m3_vmax = 163830*5;    //0~2^23-512; 0~8388096
rt_uint32_t m3_amax = 65530/5;    //0~2^16-1; 0~65535
rt_uint32_t m3_dmax = 65530;    //0~2^16-1; 0~65535
rt_uint32_t m3_target = 51200*3;    //convert to negative integer (~m3_target+1) if needed; range:-2^31~2^31-1; -2147483648~2147483647; 51200 per round; 107mm/(9.5*pi)=3.6round; 3.6*51200=184320
rt_uint32_t m3_current = 0x00070702;    //combination of i_hold i_run and i_delay (not used yet);
rt_uint8_t m3_ihlod = 0x01; //0x01~0x1F;
rt_uint8_t m3_irun = 0x10;  //0x01~0x1F;
rt_uint8_t m3_idelay = 0x07;    //0~0x0F;
rt_int32_t m3_cycle_delay = 100;    //milliseconds
struct rt_semaphore m3_sem;
rt_thread_t M3_thread = RT_NULL;
rt_thread_t M3_homing_thread = RT_NULL;
rt_thread_t auto_homing_thread = RT_NULL;

uint8_t homing_done_cmd[] = {0xDE, 0xFA, 0x04, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCD, 0xCD, 0xCC};
uint8_t connect_done_cmd[] = {0xDE, 0xFA, 0x05, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCD, 0xCD, 0xCC};
uint8_t drawer_done_cmd[] = {0xDE, 0xFA, 0x06, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCD, 0xCD, 0xCC};
uint8_t stir_homing_done_cmd[] = {0xDE, 0xFA, 0x07, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCD, 0xCD, 0xCC};

void M1_set(){
    //M1_stop();
    //M1_thread = RT_NULL;
     //m1_v1 = m1_vmax/2;
     m1_current = (rt_uint32_t)((m1_idelay << 16) + (m1_irun << 8) + m1_ihlod);
     TMC_SPIWriteInt(spi_20,TMC5130_IHOLD_IRUN, m1_current);
     TMC_SPIWriteInt(spi_20,TMC5130_A1       ,    m1_a1);
     TMC_SPIWriteInt(spi_20,TMC5130_D1       ,    m1_d1);
     //TMC_SPIWriteInt(spi_20,TMC5130_V1       ,    m1_v1);         //0 indicates only AMAX and DMAX are used, A1 and D1 are disabled
     TMC_SPIWriteInt(spi_20,TMC5130_AMAX     ,    m1_amax);
     TMC_SPIWriteInt(spi_20,TMC5130_DMAX     ,    m1_dmax);
}

void M2_set(){
     //m2_v1 = m2_vmax/2;
     m2_current = (rt_uint32_t)((m2_idelay << 16) + (m2_irun << 8) + m2_ihlod);
     TMC_SPIWriteInt(spi_21,TMC5130_IHOLD_IRUN, m2_current);
     TMC_SPIWriteInt(spi_21,TMC5130_A1       ,    m2_a1);
     TMC_SPIWriteInt(spi_21,TMC5130_D1       ,    m2_d1);
     //TMC_SPIWriteInt(spi_21,TMC5130_V1       ,    m2_v1);         //0 indicates only AMAX and DMAX are used, A1 and D1 are disabled
     TMC_SPIWriteInt(spi_21,TMC5130_AMAX     ,    m2_amax);
     TMC_SPIWriteInt(spi_21,TMC5130_DMAX     ,    m2_dmax);
}

void M3_set(){
     //m3_v1 = m3_vmax/2;
     m3_current = (rt_uint32_t)((m3_idelay << 16) + (m3_irun << 8) + m3_ihlod);
     TMC_SPIWriteInt(spi_22,TMC5130_IHOLD_IRUN, m3_current);
     TMC_SPIWriteInt(spi_22,TMC5130_A1       ,    m3_a1);
     TMC_SPIWriteInt(spi_22,TMC5130_D1       ,    m3_d1);
     //TMC_SPIWriteInt(spi_22,TMC5130_V1       ,    m3_v1);         //0 indicates only AMAX and DMAX are used, A1 and D1 are disabled
     TMC_SPIWriteInt(spi_22,TMC5130_AMAX     ,    m3_amax);
     TMC_SPIWriteInt(spi_22,TMC5130_DMAX     ,    m3_dmax);
}

void M1_init(){
    M1_set();
    rt_sem_init(&m1_sem, "m1_sem", 1, RT_IPC_FLAG_FIFO);
    rt_kprintf("m1_sem is %d \r\n", m1_sem);
}

void M2_init(){
    M2_set();
    rt_sem_init(&m2_sem, "m2_sem", 1, RT_IPC_FLAG_FIFO);
    rt_kprintf("m2_sem is %d \r\n", m2_sem);
}

void M3_init(){
    M3_set();
    rt_sem_init(&m3_sem, "m3_sem", 1, RT_IPC_FLAG_FIFO);
    rt_kprintf("m3_sem is %d \r\n", m3_sem);
}


void M1_run_entry(void* param){
    rt_sem_take(&m1_sem, RT_WAITING_FOREVER);
    rt_uint8_t dir = (rt_uint8_t)param;
    rt_kprintf("dir is %d \r\n", dir);
    rt_uint32_t m1_stat;
    m1_v1 = m1_vmax/2;
    TMC_SPIWriteInt(spi_20, TMC5130_V1       ,    m1_v1);
    if (dir == 1){
        TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, m1_target);

    }else if(dir == 0){
        TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, 0);
    }else if(dir == 2){
        TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, (~m1_target + 1));
    }
    TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    0);
    TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    m1_vmax);

    m1_stat = TMC_SPIReadInt(spi_20, TMC5130_RAMPSTAT);
    m1_stat = m1_stat >> 9;
    m1_stat = m1_stat & 0b1;
    while((m1_stat == 0) & ((M1_thread->stat & RT_THREAD_STAT_MASK) == 0x03)){
        //rt_kprintf("M1 thread status  is %d \r\n", (M1_thread->stat & RT_THREAD_STAT_MASK));
        m1_stat = TMC_SPIReadInt(spi_20, TMC5130_RAMPSTAT);
        m1_stat = m1_stat >> 9;
        m1_stat = m1_stat & 0b1;
        rt_thread_mdelay(1);
    }
    if((M1_thread->stat & RT_THREAD_STAT_MASK) == 0x03){
        rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
        rt_device_write(serial6, 0, &drawer_done_cmd[0], 11);
        rt_thread_mdelay(50);
        rt_sem_release(&tx6_sem);
        M1_thread = RT_NULL;
        rt_sem_release(&m1_sem);
    }
    M1_thread = RT_NULL;
}

void M2_run_entry(void* param){
    rt_sem_take(&m2_sem, RT_WAITING_FOREVER);
    rt_uint8_t dir = (rt_uint8_t)param;
    rt_kprintf("dir is %d \r\n", dir);
    rt_uint32_t m2_stat;
    m2_v1 = m2_vmax/2;
    TMC_SPIWriteInt(spi_21, TMC5130_V1       ,    m2_v1);
    if (dir == 1){
        TMC_SPIWriteInt(spi_21, TMC5130_XTARGET, m2_target);

    }else if(dir == 0){
        TMC_SPIWriteInt(spi_21, TMC5130_XTARGET, 0);
    }else if(dir == 2){
        TMC_SPIWriteInt(spi_21, TMC5130_XTARGET, (~m2_target + 1));
    }
    TMC_SPIWriteInt(spi_21, TMC5130_RAMPMODE ,    0);
    TMC_SPIWriteInt(spi_21, TMC5130_VMAX     ,    m2_vmax);

    m2_stat = TMC_SPIReadInt(spi_21, TMC5130_RAMPSTAT);
    m2_stat = m2_stat >> 9;
    m2_stat = m2_stat & 0b1;
    while((m2_stat == 0) & ((M2_thread->stat & RT_THREAD_STAT_MASK) == 0x03)){
        m2_stat = TMC_SPIReadInt(spi_21, TMC5130_RAMPSTAT);
        m2_stat = m2_stat >> 9;
        m2_stat = m2_stat & 0b1;
        rt_thread_mdelay(1);
    }
    rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
    rt_device_write(serial6, 0, &connect_done_cmd[0], 11);
    rt_thread_mdelay(50);
    rt_sem_release(&tx6_sem);
    M2_thread = RT_NULL;
    rt_sem_release(&m2_sem);
}

void M3_thread_entry(){
    if(((M3_thread->stat & RT_THREAD_STAT_MASK) == 0x03)){
    //M3_homing();
       //M3_stop();
       //m3_ihlod = 0x10;
       //M3_set();
       rt_sem_take(&m3_sem, RT_WAITING_FOREVER);
       //rt_thread_mdelay(5000);
       rt_uint32_t swstat;
       swstat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
       swstat = swstat & 0b11;
       if ((swstat & 0b10)!=0){
           //TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, 0);
           TMC_SPIWriteInt(spi_22,TMC5130_XACTUAL  ,    0x00000000);
           //rt_kprintf("right aligned \n");
           //return 1;
        } else{
           //move to right rapidly
           TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    1);
           TMC_SPIWriteInt(spi_22, TMC5130_VMAX     ,    m3_vmax/4);//8,388,096 max// 67200 = 1r/s
           rt_kprintf("M3 homing stage1 \n");
           swstat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
           swstat = swstat & 0b11;
           while((swstat & 0b10) == 0){
               swstat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
               swstat = swstat & 0b11;
               rt_hw_us_delay(100);
           }
           TMC_SPIWriteInt(spi_22, TMC5130_VMAX ,   0);
           //TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, 0);
           TMC_SPIWriteInt(spi_22,TMC5130_XACTUAL  ,    0x00000000);
       }
        //turn back 8mm
       rt_kprintf("M3 homing_stage2 move back \n");
       TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, (~51200+1));//mouve back one turn
       TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    0);
       TMC_SPIWriteInt(spi_22, TMC5130_VMAX     ,    655320);
       swstat = TMC_SPIReadInt(spi_22, TMC5130_XACTUAL);
       while(swstat != (~51200+1)){
           swstat = TMC_SPIReadInt(spi_22, TMC5130_XACTUAL);
           rt_hw_us_delay(100);
       }
       rt_kprintf("M3 homing_stage2 homing \n");
       TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    1);
       TMC_SPIWriteInt(spi_22, TMC5130_VMAX     ,    22400);//8,388,096 max// 67200 = 1r/s
       swstat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
       swstat = swstat & 0b11;
       while((swstat & 0b10) == 0){//left switch activated 0b01, right switch activated 0b10
           swstat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
           swstat = swstat & 0b11;
           rt_hw_us_delay(100);
       }

       //move back a little to ensure the swith is not activated
       TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, (~5120+1));//mouve back 1/10turn
           TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    0);
           TMC_SPIWriteInt(spi_22, TMC5130_VMAX     ,    655320);
           swstat = TMC_SPIReadInt(spi_22, TMC5130_XACTUAL);
           while(swstat != (~5120+1)){
               swstat = TMC_SPIReadInt(spi_22, TMC5130_XACTUAL);
               rt_hw_us_delay(100);
           }


       //stop and set position to 0
       TMC_SPIWriteInt(spi_22, TMC5130_VMAX ,   0);
       //TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, 0);
       TMC_SPIWriteInt(spi_22,TMC5130_XACTUAL  ,    0x00000000);
       rt_kprintf("M3 right aligned \n");
       m3_ihlod = 0x01;
       M3_set();
       rt_sem_release(&m3_sem);
    }
    rt_uint32_t cnt;
    cnt = 0;
    while((M3_thread->stat & RT_THREAD_STAT_MASK) == 0x03){
       cnt++;
       rt_kprintf("M3 thread waiting for semphore \r\n");
       rt_sem_take(&m3_sem, RT_WAITING_FOREVER);
       rt_kprintf("M3 thread semphore got \r\n");
       rt_uint32_t m3_stat;
       m3_v1 = m3_vmax/2;
       TMC_SPIWriteInt(spi_22, TMC5130_V1       ,    m3_v1);
       TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, (~m3_target+1));
       TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    0);
       TMC_SPIWriteInt(spi_22, TMC5130_VMAX     ,    m3_vmax);
       m3_stat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
       m3_stat = m3_stat >> 9;
       m3_stat = m3_stat & 0b1;
       while(m3_stat == 0){
           m3_stat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
           m3_stat = m3_stat >> 9;
           m3_stat = m3_stat & 0b1;
           rt_hw_us_delay(100);
       }
       rt_thread_mdelay(m3_cycle_delay);
       TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, 0);
       TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    0);
       TMC_SPIWriteInt(spi_22, TMC5130_VMAX     ,    m3_vmax);
       rt_kprintf("M3 is going back \r\n");
       m3_stat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
       m3_stat = m3_stat >> 9;
       m3_stat = m3_stat & 0b1;
       rt_uint32_t swstat;
       swstat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
       swstat = swstat & 0b11;
       if ((swstat & 0b10)!=0){
           TMC_SPIWriteInt(spi_22,TMC5130_XACTUAL  ,    0x00000000);
           TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, (~5120+1));//mouve back 1/10turn
           TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    0);
           TMC_SPIWriteInt(spi_22, TMC5130_VMAX     ,    655320);
           swstat = TMC_SPIReadInt(spi_22, TMC5130_XACTUAL);
           while(swstat != (~5120+1)){
               swstat = TMC_SPIReadInt(spi_22, TMC5130_XACTUAL);
               rt_hw_us_delay(100);
           }
           //stop and set position to 0
           TMC_SPIWriteInt(spi_22, TMC5130_VMAX ,   0);
            TMC_SPIWriteInt(spi_22,TMC5130_XACTUAL  ,    0x00000000);
       }else{
           while((m3_stat == 0) & ((swstat & 0b10)==0)){
               m3_stat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
               m3_stat = m3_stat >> 9;
               m3_stat = m3_stat & 0b1;
               swstat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
               swstat = swstat & 0b11;
               rt_hw_us_delay(100);
               }
       }
       //if stopped by switch, reset the zero position
       if ((swstat & 0b10)!=0){
                  TMC_SPIWriteInt(spi_22,TMC5130_XACTUAL  ,    0x00000000);
                  TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, (~5120+1));//mouve back 1/10turn
                  TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    0);
                  TMC_SPIWriteInt(spi_22, TMC5130_VMAX     ,    655320);
                  swstat = TMC_SPIReadInt(spi_22, TMC5130_XACTUAL);
                  while(swstat != (~5120+1)){
                      swstat = TMC_SPIReadInt(spi_22, TMC5130_XACTUAL);
                      rt_hw_us_delay(100);
                  }
                  //stop and set position to 0
                  TMC_SPIWriteInt(spi_22, TMC5130_VMAX ,   0);
                   TMC_SPIWriteInt(spi_22,TMC5130_XACTUAL  ,    0x00000000);
              }
       rt_thread_mdelay(m3_cycle_delay);
       rt_kprintf("%d cycles tested \n", cnt);
       rt_sem_release(&m3_sem);
       }
    M3_thread = RT_NULL;
}
/*
void M1_run(void* param){
    if (M1_thread == RT_NULL){
            M1_thread = rt_thread_create("M1_run", M1_run_entry, (void*)param, 1024, M1_thread_priority, 25);
            stat = (M1_thread->stat & RT_THREAD_STAT_MASK);
            rt_kprintf("M1 thread status is %x\n", stat);
            rt_thread_startup(M1_thread);
            rt_kprintf("M1 thread started \r\n");
            stat = (M1_thread->stat & RT_THREAD_STAT_MASK);
            rt_kprintf("M1 thread status is %x\n", stat);
        }else{
            rt_kprintf("Wait! Motor 1 is running \r\n");
            stat = (M1_thread->stat & RT_THREAD_STAT_MASK);
            rt_kprintf("M1 thread status is %x\n", stat);
        }

}
*/


void M1_run(void* param){
    rt_uint8_t stat;
    if (M1_thread == RT_NULL){
                M1_thread = rt_thread_create("M1_run", M1_run_entry, (void*)param, 1024, M1_thread_priority, 25);
                rt_thread_startup(M1_thread);
                rt_kprintf("M1 thread started \r\n");
            }else{
                stat = (M1_thread->stat & RT_THREAD_STAT_MASK);
                switch(stat){
                    case 0x04: M1_thread = rt_thread_create("M1_run", M1_run_entry, (void*)param, 1024, M1_thread_priority, 25); rt_thread_startup(M1_thread); break;
                    default: rt_kprintf("Wait! Motor 1 is running \r\n"); stat = (M1_thread->stat & RT_THREAD_STAT_MASK); rt_kprintf("M1 thread status is %x\n", stat); break;
                }
            }
}




void M2_run(void* param){
    if (M2_thread == RT_NULL){
            M2_thread = rt_thread_create("M2_run", M2_run_entry, (void*)param, 1024, M2_thread_priority, 25);
            rt_thread_startup(M2_thread);
            rt_kprintf("M2 thread started \r\n");
        }else{
            rt_kprintf("Wait! Motor 2 is running \r\n");
        }

}


int M3_run(void){

    rt_err_t ret = RT_EOK;
    if (M3_thread == RT_NULL){
        rt_kprintf("M3 thread is NULL!\n");
        //rt_thread_mdelay(100);
        M3_thread = rt_thread_create("M3_thread", M3_thread_entry, RT_NULL, 1024, M3_thread_priority, 10);
        rt_thread_startup(M3_thread);
        rt_kprintf("M3 thread started!\n");
    }else{
        rt_kprintf("Wait! M3 thread is running!\n");
        ret = RT_ERROR;
        return ret;
    }
    return ret;
}

/*
void M1_stop(){
    if(M1_thread != RT_NULL){
        if(RT_EOK == rt_thread_delete(M1_thread)){
            rt_kprintf("M1 thread stopped!\n");
            M1_thread = RT_NULL;
            rt_sem_trytake(&m1_sem);
            rt_sem_release(&m1_sem);
        }
    }
}
*/
/*
void M1_stop(){
    rt_uint8_t stat;
    if(M1_thread != RT_NULL){
        stat = (M1_thread->stat & RT_THREAD_STAT_MASK);
        rt_kprintf("M1 thread status is %x\n", stat);
        if(stat != RT_THREAD_CLOSE){
            if(RT_EOK == rt_thread_delete(M1_thread)){
                rt_kprintf("M1 thread stopped!\n");
                stat = (M1_thread->stat & RT_THREAD_STAT_MASK);
                rt_kprintf("M1 thread status is %x\n", stat);
                M1_thread = RT_NULL;
        }
        }
    }
    rt_sem_trytake(&m1_sem);
    rt_sem_release(&m1_sem);
}
*/


void M1_stop(){
    rt_uint8_t stat;
    if(M1_thread != RT_NULL){
        if(RT_EOK == rt_thread_delete(M1_thread)){
            rt_kprintf("M1 thread stopped!\n");
            M1_thread = RT_NULL;
        }
    }
    rt_sem_trytake(&m1_sem);
    rt_sem_release(&m1_sem);
}




void M2_stop(){
    if(M2_thread != RT_NULL){
        if(RT_EOK == rt_thread_delete(M2_thread)){
            rt_kprintf("M2 thread stopped!\n");
            M2_thread = RT_NULL;
        }
    }
    rt_sem_trytake(&m2_sem);
    rt_sem_release(&m2_sem);
}

void M3_stop(){
    if(M3_thread != RT_NULL){
        if(RT_EOK == rt_thread_delete(M3_thread)){
            rt_kprintf("M3 thread stopped!\n");
            M3_thread = RT_NULL;
            rt_sem_trytake(&m3_sem);
            rt_sem_release(&m3_sem);
        }else{
            rt_kprintf("Failed to stop M3 thread!\n");
            int i = 0;
            while((i < 5) & (RT_EOK != rt_thread_delete(M3_thread))){
                rt_thread_delete(M3_thread);
                rt_thread_mdelay(50);
                i++;
            }
            rt_uint8_t stat;
            stat = (M3_thread->stat & RT_THREAD_STAT_MASK);
            if (stat == 0x04){
                M3_thread = RT_NULL;
                rt_sem_trytake(&m3_sem);
                rt_sem_release(&m3_sem);
            }else{
                rt_kprintf("can not stop M3 thread!\n");
            }
        }
    }
    M3_thread = RT_NULL;
    //rt_thread_mdelay(200);
    M3_set();
    rt_sem_trytake(&m3_sem);
    rt_sem_release(&m3_sem);
}






void M1_homing_entry(){
    M1_stop();
    rt_sem_take(&m1_sem, RT_WAITING_FOREVER);
    rt_uint32_t swstat;
    swstat = TMC_SPIReadInt(spi_20, TMC5130_RAMPSTAT);
    swstat = swstat & 0b11;
    if ((swstat & 0b10)!=0){
        //TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, 0);
        TMC_SPIWriteInt(spi_20,TMC5130_XACTUAL  ,    0x00000000);
        //rt_kprintf("right aligned \n");
        //return 1;
     } else{
        //move to right rapidly
        TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    1);
        TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    m1_vmax);//8,388,096 max// 67200 = 1r/s
        rt_kprintf("M1 homing stage1 \n");
        swstat = TMC_SPIReadInt(spi_20, TMC5130_RAMPSTAT);
        swstat = swstat & 0b11;
        while((swstat & 0b10) == 0){
            swstat = TMC_SPIReadInt(spi_20, TMC5130_RAMPSTAT);
            swstat = swstat & 0b11;
            rt_thread_mdelay(1);
        }
        TMC_SPIWriteInt(spi_20, TMC5130_VMAX ,   0);
        //TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, 0);
        TMC_SPIWriteInt(spi_20,TMC5130_XACTUAL  ,    0x00000000);
    }
     //turn back 8mm
    rt_kprintf("M1 homing_stage2 move back \n");
    TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, (~51200+1));//move back one turn
    TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    0);
    TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    655320);
    swstat = TMC_SPIReadInt(spi_20, TMC5130_XACTUAL);
    while(swstat != (~51200+1)){
        swstat = TMC_SPIReadInt(spi_20, TMC5130_XACTUAL);
        rt_thread_mdelay(1);
    }
    rt_kprintf("M1 homing_stage2 homing \n");
    TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    1);
    TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    22400);//8,388,096 max// 67200 = 1r/s
    swstat = TMC_SPIReadInt(spi_20, TMC5130_RAMPSTAT);
    swstat = swstat & 0b11;
    while((swstat & 0b10) == 0){//left switch activated 0b01, right switch activated 0b10
        swstat = TMC_SPIReadInt(spi_20, TMC5130_RAMPSTAT);
        swstat = swstat & 0b11;
        rt_thread_mdelay(1);
    }
    //stop and set position to 0
    TMC_SPIWriteInt(spi_20, TMC5130_VMAX ,   0);
    //TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, 0);
    TMC_SPIWriteInt(spi_20,TMC5130_XACTUAL  ,    0x00000000);

    //move back a little to ensure the switch is not activated
            TMC_SPIWriteInt(spi_20, TMC5130_XTARGET, (~20480+1));//mouve back 1mm; 1/2.5turn
                TMC_SPIWriteInt(spi_20, TMC5130_RAMPMODE ,    0);
                TMC_SPIWriteInt(spi_20, TMC5130_VMAX     ,    655320);
                swstat = TMC_SPIReadInt(spi_20, TMC5130_XACTUAL);
                while(swstat != (~20480+1)){
                    swstat = TMC_SPIReadInt(spi_20, TMC5130_XACTUAL);
                    rt_thread_mdelay(1);
                }
            //stop and set position to 0
            TMC_SPIWriteInt(spi_20, TMC5130_VMAX ,   0);
            //TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, 0);
            TMC_SPIWriteInt(spi_20,TMC5130_XACTUAL  ,    0x00000000);


    rt_kprintf("M1 left aligned \n");
    rt_sem_release(&m1_sem);
    rt_kprintf("M1 semaphore released \n");
    M1_homing_thread = RT_NULL;
    rt_kprintf("M1 homing thread set to NULL \n");
    return;
}

void M1_homing(){
    //M1_stop();
    if (M1_homing_thread == RT_NULL){
        M1_homing_thread = rt_thread_create("M1_homing", M1_homing_entry, RT_NULL, 1024, M1_homing_thread_priority, 25);
        rt_thread_startup(M1_homing_thread);
        rt_kprintf("M1 homing... \r\n");
    }else{
        rt_kprintf("Wait! Motor 1 is homing \r\n");
    }
}

void M2_homing_entry(){
    M2_stop();
    rt_sem_take(&m2_sem, RT_WAITING_FOREVER);
    rt_uint32_t swstat;
    swstat = TMC_SPIReadInt(spi_21, TMC5130_RAMPSTAT);
    swstat = swstat & 0b11;
    if ((swstat & 0b10)!=0){
        //TMC_SPIWriteInt(spi_21, TMC5130_XTARGET, 0);
        TMC_SPIWriteInt(spi_21,TMC5130_XACTUAL  ,    0x00000000);
        //rt_kprintf("right aligned \n");
        //return 1;
     } else{
        //move to right rapidly
        TMC_SPIWriteInt(spi_21, TMC5130_RAMPMODE ,    1);
        TMC_SPIWriteInt(spi_21, TMC5130_VMAX     ,    m2_vmax);//8,388,096 max// 67200 = 1r/s
        rt_kprintf("M2 homing stage1 \n");
        swstat = TMC_SPIReadInt(spi_21, TMC5130_RAMPSTAT);
        swstat = swstat & 0b11;
        while((swstat & 0b10) == 0){
            swstat = TMC_SPIReadInt(spi_21, TMC5130_RAMPSTAT);
            swstat = swstat & 0b11;
            rt_thread_mdelay(1);
        }
        TMC_SPIWriteInt(spi_21, TMC5130_VMAX ,   0);
        //TMC_SPIWriteInt(spi_21, TMC5130_XTARGET, 0);
        TMC_SPIWriteInt(spi_21,TMC5130_XACTUAL  ,    0x00000000);
    }
     //turn back 8mm
    rt_kprintf("M2 homing_stage2 move back \n");
    TMC_SPIWriteInt(spi_21, TMC5130_XTARGET, (~51200+1));//mouve back one turn
    TMC_SPIWriteInt(spi_21, TMC5130_RAMPMODE ,    0);
    TMC_SPIWriteInt(spi_21, TMC5130_VMAX     ,    655320);
    swstat = TMC_SPIReadInt(spi_21, TMC5130_XACTUAL);
    while(swstat != (~51200+1)){
        swstat = TMC_SPIReadInt(spi_21, TMC5130_XACTUAL);
        rt_thread_mdelay(1);
    }
    rt_kprintf("M2 homing_stage2 homing \n");
    TMC_SPIWriteInt(spi_21, TMC5130_RAMPMODE ,    1);
    TMC_SPIWriteInt(spi_21, TMC5130_VMAX     ,    22400);//8,388,096 max// 67200 = 1r/s
    swstat = TMC_SPIReadInt(spi_21, TMC5130_RAMPSTAT);
    swstat = swstat & 0b11;
    while((swstat & 0b10) == 0){//left switch activated 0b01, right switch activated 0b10
        swstat = TMC_SPIReadInt(spi_21, TMC5130_RAMPSTAT);
        swstat = swstat & 0b11;
        rt_thread_mdelay(1);
    }
    //stop and set position to 0
    TMC_SPIWriteInt(spi_21, TMC5130_VMAX ,   0);
    //TMC_SPIWriteInt(spi_21, TMC5130_XTARGET, 0);
    TMC_SPIWriteInt(spi_21,TMC5130_XACTUAL  ,    0x00000000);

    //move back a little to ensure the swith is not activated
        TMC_SPIWriteInt(spi_21, TMC5130_XTARGET, (~25600+1));//mouve back 0.5mm; 1/2turn
            TMC_SPIWriteInt(spi_21, TMC5130_RAMPMODE ,    0);
            TMC_SPIWriteInt(spi_21, TMC5130_VMAX     ,    655320);
            swstat = TMC_SPIReadInt(spi_21, TMC5130_XACTUAL);
            while(swstat != (~25600+1)){
                swstat = TMC_SPIReadInt(spi_21, TMC5130_XACTUAL);
                rt_thread_mdelay(1);
            }


        //stop and set position to 0
        TMC_SPIWriteInt(spi_21, TMC5130_VMAX ,   0);
        //TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, 0);
        TMC_SPIWriteInt(spi_21,TMC5130_XACTUAL  ,    0x00000000);

    rt_kprintf("M2 left aligned \n");
    rt_sem_release(&m2_sem);
    M2_homing_thread = RT_NULL;
}

void M2_homing(){
    M2_stop();
    if (M2_homing_thread == RT_NULL){
        M2_homing_thread = rt_thread_create("M2_homing", M2_homing_entry, RT_NULL, 1024, M2_homing_thread_priority, 25);
        rt_thread_startup(M2_homing_thread);
        rt_kprintf("M2 homing... \r\n");
    }else{
        rt_kprintf("Wait! Motor 2 is homing \r\n");
    }
}





void M3_homing_entry(){
    M3_stop();
    rt_thread_mdelay(50);
    //m3_ihlod = 0x10;
    //M3_set();
    rt_sem_take(&m3_sem, RT_WAITING_FOREVER);
    rt_uint32_t swstat;
    swstat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
    swstat = swstat & 0b11;
    if ((swstat & 0b10)!=0){
        //TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, 0);
        TMC_SPIWriteInt(spi_22,TMC5130_XACTUAL  ,    0x00000000);
        //rt_kprintf("right aligned \n");
        //return 1;
     } else{
        //move to right rapidly
        TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    1);
        TMC_SPIWriteInt(spi_22, TMC5130_VMAX     ,    m3_vmax/4);//8,388,096 max// 67200 = 1r/s
        rt_kprintf("M3 homing stage1 \n");
        swstat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
        swstat = swstat & 0b11;
        while((swstat & 0b10) == 0){
            swstat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
            swstat = swstat & 0b11;
            rt_hw_us_delay(100);
        }
        TMC_SPIWriteInt(spi_22, TMC5130_VMAX ,   0);
        //TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, 0);
        TMC_SPIWriteInt(spi_22,TMC5130_XACTUAL  ,    0x00000000);
    }
     //turn back 8mm
    rt_kprintf("M3 homing_stage2 move back \n");
    TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, (~51200+1));//mouve back one turn
    TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    0);
    TMC_SPIWriteInt(spi_22, TMC5130_VMAX     ,    655320);
    swstat = TMC_SPIReadInt(spi_22, TMC5130_XACTUAL);
    while(swstat != (~51200+1)){
        swstat = TMC_SPIReadInt(spi_22, TMC5130_XACTUAL);
        rt_hw_us_delay(100);
    }
    rt_kprintf("M3 homing_stage2 homing \n");
    TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    1);
    TMC_SPIWriteInt(spi_22, TMC5130_VMAX     ,    22400);//8,388,096 max// 67200 = 1r/s
    swstat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
    swstat = swstat & 0b11;
    while((swstat & 0b10) == 0){//left switch activated 0b01, right switch activated 0b10
        swstat = TMC_SPIReadInt(spi_22, TMC5130_RAMPSTAT);
        swstat = swstat & 0b11;
        rt_hw_us_delay(100);
    }

    //move back a little to ensure the switch is not activated
    TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, (~5120+1));//mouve back 1/10turn
        TMC_SPIWriteInt(spi_22, TMC5130_RAMPMODE ,    0);
        TMC_SPIWriteInt(spi_22, TMC5130_VMAX     ,    655320);
        swstat = TMC_SPIReadInt(spi_22, TMC5130_XACTUAL);
        while(swstat != (~5120+1)){
            swstat = TMC_SPIReadInt(spi_22, TMC5130_XACTUAL);
            rt_hw_us_delay(100);
        }


    //stop and set position to 0
    TMC_SPIWriteInt(spi_22, TMC5130_VMAX ,   0);
    //TMC_SPIWriteInt(spi_22, TMC5130_XTARGET, 0);
    TMC_SPIWriteInt(spi_22,TMC5130_XACTUAL  ,    0x00000000);
    rt_kprintf("M3 right aligned \n");
    m3_ihlod = 0x01;
    M3_set();
    rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
    rt_device_write(serial6, 0, &stir_homing_done_cmd[0], 11);
    rt_thread_mdelay(50);
    rt_sem_release(&tx6_sem);
    rt_sem_release(&m3_sem);
    M3_homing_thread = RT_NULL;
}

void M3_homing(){
    //M3_stop();
    if (M3_homing_thread == RT_NULL){
        M3_homing_thread = rt_thread_create("M3_homing", M3_homing_entry, RT_NULL, 1024, M3_homing_thread_priority, 25);
        rt_thread_startup(M3_homing_thread);
        rt_kprintf("M3 homing... \r\n");
    }else{
        rt_kprintf("Wait! Motor 3 is homing \r\n");
    }
}

void auto_homing_entry(){
    M3_homing();
    M2_homing();
    rt_sem_take(&m2_sem, RT_WAITING_FOREVER);
    rt_sem_release(&m2_sem);
    M1_homing();
    rt_thread_mdelay(100);
    rt_kprintf("waiting for M3 semaphore... \r\n");
    rt_sem_take(&m3_sem, RT_WAITING_FOREVER);
    rt_kprintf("M3 semaphore taken \r\n");
    rt_sem_release(&m3_sem);
    rt_sem_take(&m1_sem, RT_WAITING_FOREVER);
    rt_kprintf("M1 semaphore taken \r\n");
    //M1_homing_thread = RT_NULL;
    //rt_kprintf("M1 homing thread set to NULL \n");
    rt_sem_release(&m1_sem);

    rt_kprintf("waiting for uart6 bus semaphore... \r\n");
    rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
    rt_device_write(serial6, 0, &homing_done_cmd[0], 11);
    rt_kprintf("homing done command sent. \r\n");
    rt_thread_mdelay(50);
    rt_sem_release(&tx6_sem);
    //send signal again to ensure the transfer is successed
    rt_thread_mdelay(50);
    rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
    rt_device_write(serial6, 0, &homing_done_cmd[0], 11);
    rt_kprintf("homing done command sent. \r\n");
    rt_thread_mdelay(50);
    rt_sem_release(&tx6_sem);
    //send signal again to ensure the transfer is successed
    rt_thread_mdelay(50);
    rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
    rt_device_write(serial6, 0, &homing_done_cmd[0], 11);
    rt_kprintf("homing done command sent. \r\n");
    rt_thread_mdelay(50);
    rt_sem_release(&tx6_sem);
    auto_homing_thread = RT_NULL;
}
void auto_homing(){
    if(auto_homing_thread == RT_NULL){
        //M1_stop();
        //M2_stop();
        //M3_stop();
        auto_homing_thread = rt_thread_create("auto_homing", auto_homing_entry, RT_NULL, 1024, auto_homing_thread_priority, 25);
        rt_thread_startup(auto_homing_thread);
        rt_kprintf("auto homing... \r\n");
    }else{
        rt_kprintf("auto homing is running \r\n");
    }

 }


void test_M1_f(){
    rt_uint8_t dir = 0;
    M1_run((void*) dir);
}

void test_M1_r(){
    rt_uint8_t dir = 2;
    M1_run((void*) dir);
}


void test_M2_f(){
    rt_uint8_t dir = 1;
    M2_run((void*) dir);
}

void test_M2_r(){
    rt_uint8_t dir = 2;
    M2_run((void*) dir);
}

void set_motor(uint8_t arg[]){
    rt_uint32_t vlu;
    vlu = (arg[3] << 24) + (arg[4] << 16) + (arg[5] << 8) + arg[6];
    if(arg[1] == 0xA3){
        switch(arg[2]){
        case 0x01: m1_vmax = vlu; break;
        case 0x02: m1_ihlod = vlu; break;
        case 0x03: m1_irun = vlu; break;
        }
        M1_set();
    }else if(arg[1] == 0xA4){
        switch(arg[2]){
        case 0x01: m2_vmax = vlu; break;
        case 0x02: m2_ihlod = vlu; break;
        case 0x03: m2_irun = vlu; break;
        }
        M2_set();
    }else if(arg[1] == 0xA5){
        switch(arg[2]){
        case 0x01: m3_vmax = vlu; break;
        case 0x02: m3_ihlod = vlu; break;
        case 0x03: m3_irun = vlu; break;
        case 0x04: m3_cycle_delay = vlu; break;
        case 0x05: m3_amax = vlu; break;
        case 0x06: m3_dmax = vlu; break;
        case 0x07: m3_target = vlu; break;
        }
        M3_set();
    }
}
















MSH_CMD_EXPORT(test_M1_f, M1 forward);
MSH_CMD_EXPORT(test_M1_r, M1 reverse);
MSH_CMD_EXPORT(test_M2_f, M2 forward);
MSH_CMD_EXPORT(test_M2_r, M2 reverse);
MSH_CMD_EXPORT(M1_run, M1 thread start);
MSH_CMD_EXPORT(M1_stop, M1 thread start);
MSH_CMD_EXPORT(M3_run, M3 thread start);
MSH_CMD_EXPORT(M3_stop, M3 thread start);
MSH_CMD_EXPORT(M1_homing, M1 homing thread);
MSH_CMD_EXPORT(M2_homing, M2 homing thread);
MSH_CMD_EXPORT(M3_homing, M3 homing thread);
MSH_CMD_EXPORT(auto_homing, auto homing thread);



