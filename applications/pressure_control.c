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
#include <pressure_control.h>
#include <configuration.h>

uint8_t stp_cmd[] = {0x5E, 0x00, 0x19, 0x80, 0xE8, 0x03, 0x28, 0x0D};
uint8_t rdp1_cmd[] = {0x5E, 0x0A, 0x19, 0x00, 0x00, 0x00, 0xED, 0x08};
uint8_t rdp2_cmd[] = {0x5E, 0x0B, 0x19, 0x00, 0x00, 0x00, 0xED, 0x08};
uint8_t rdp3_cmd[] = {0x5E, 0x0C, 0x19, 0x00, 0x00, 0x00, 0xED, 0x08};
uint8_t report_cmd[] = {0xDE, 0xC4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCD, 0xCD, 0xCC};
uint8_t send_pid[] = {0xDE, 0xCA, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCD, 0xCD, 0xCC};
uint8_t report_pid[] = {0xDE, 0xB3, 0x0A, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xCD, 0xCD, 0xCC};

uint8_t rdkp_cmd[] = {0x5E, 0x0A, 0x16, 0x01, 0x00, 0x00, 0x17, 0x06};
uint8_t rdki_cmd[] = {0x5E, 0x0B, 0x16, 0x02, 0x00, 0x00, 0x19, 0x15};
uint8_t rdkd_cmd[] = {0x5E, 0x0C, 0x16, 0x03, 0x00, 0x00, 0xE3, 0x1B};
uint8_t rddz_cmd[] = {0x5E, 0x0B, 0x17, 0x00, 0x00, 0x00, 0xED, 0x08};
uint8_t rdzz_cmd[] = {0x5E, 0x0C, 0x18, 0x00, 0x00, 0x00, 0xED, 0x08};

uint8_t st_pid_cmd[] = {0x5E, 0x00, 0x16, 0x81, 0x00, 0x00, 0xED, 0x08};


void rdp_cmd_init(void)
{
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&rdp1_cmd[0], 4);
    rdp1_cmd[6] = (uint8_t) (wReCRC & 0xFF);
    rdp1_cmd[7] = (uint8_t) ((wReCRC >> 8) & 0xFF);

    wReCRC = wCRCCheck_Uart_Data(&rdp2_cmd[0], 4);
    rdp2_cmd[6] = (uint8_t) (wReCRC & 0xFF);
    rdp2_cmd[7] = (uint8_t) ((wReCRC >> 8) & 0xFF);

    wReCRC = wCRCCheck_Uart_Data(&rdp3_cmd[0], 4);
    rdp3_cmd[6] = (uint8_t) (wReCRC & 0xFF);
    rdp3_cmd[7] = (uint8_t) ((wReCRC >> 8) & 0xFF);
}


void set_pressure(uint8_t arg[])
{
    stp_cmd[4] = arg[4];
    stp_cmd[5] = arg[3];
    switch(arg[2])
    {
        case 1: stp_cmd[1] = 0x0A; break;
        case 2: stp_cmd[1] = 0x0B; break;
        case 3: stp_cmd[1] = 0x0C; break;
    }
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&stp_cmd[0], 4);
    stp_cmd[6] = (uint8_t) (wReCRC & 0xFF);
    stp_cmd[7] = (uint8_t) ((wReCRC >> 8) & 0xFF);
    rt_sem_take(&tx1_sem, RT_WAITING_FOREVER);
    rt_thread_mdelay(50);
    rt_device_write(serial1, 0, &stp_cmd[0], 8);
    rt_kprintf("setting pressure %x \r\n", arg[2]);
    rt_thread_mdelay(10);
    rt_sem_release(&tx1_sem);
}


void read_pressure(void)
{
    rt_sem_take(&tx1_sem, RT_WAITING_FOREVER);
    rt_thread_mdelay(55);
    rt_device_write(serial1, 0, &rdp1_cmd[0], 8);
    rt_sem_release(&tx1_sem);

    rt_sem_take(&tx1_sem, RT_WAITING_FOREVER);
    rt_thread_mdelay(55);
    rt_device_write(serial1, 0, &rdp2_cmd[0], 8);
    rt_sem_release(&tx1_sem);

    rt_sem_take(&tx1_sem, RT_WAITING_FOREVER);
    rt_thread_mdelay(55);
    rt_device_write(serial1, 0, &rdp3_cmd[0], 8);
    rt_sem_release(&tx1_sem);
}

static void report_pressure_thread_entry(void *parameter)
{
    while(1)
    {
        read_pressure();
        rt_thread_mdelay(1000);
        rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
        rt_thread_mdelay(10);
        rt_device_write(serial6, 0, &report_cmd[0], 18);
        rt_thread_mdelay(10);
        rt_sem_release(&tx6_sem);
    }
}

void report_thread_init_and_start(void)
{
    rdp_cmd_init();
    rt_thread_t report_pressure_thread = rt_thread_create("report_pressure", report_pressure_thread_entry, RT_NULL, 1024, report_pressure_thread_priority, 10);
    rt_thread_startup(report_pressure_thread);
    //rt_sem_release(&tx1_sem);
}


void read_pid_set(uint8_t pc, uint8_t para){
     rdkp_cmd[1] = pc;
     rdki_cmd[1] = pc;
     rdkd_cmd[1] = pc;
     rddz_cmd[1] = pc;
     rdzz_cmd[1] = pc;
     uint16_t wReCRC = 0;
     wReCRC = wCRCCheck_Uart_Data(&rdkp_cmd[0], 4);
     rdkp_cmd[6] = (uint8_t) (wReCRC & 0xFF);
     rdkp_cmd[7] = (uint8_t) ((wReCRC >> 8) & 0xFF);

     wReCRC = wCRCCheck_Uart_Data(&rdki_cmd[0], 4);
     rdki_cmd[6] = (uint8_t) (wReCRC & 0xFF);
     rdki_cmd[7] = (uint8_t) ((wReCRC >> 8) & 0xFF);

     wReCRC = wCRCCheck_Uart_Data(&rdkd_cmd[0], 4);
     rdkd_cmd[6] = (uint8_t) (wReCRC & 0xFF);
     rdkd_cmd[7] = (uint8_t) ((wReCRC >> 8) & 0xFF);

     wReCRC = wCRCCheck_Uart_Data(&rddz_cmd[0], 4);
     rddz_cmd[6] = (uint8_t) (wReCRC & 0xFF);
     rddz_cmd[7] = (uint8_t) ((wReCRC >> 8) & 0xFF);

     wReCRC = wCRCCheck_Uart_Data(&rdzz_cmd[0], 4);
     rdzz_cmd[6] = (uint8_t) (wReCRC & 0xFF);
     rdzz_cmd[7] = (uint8_t) ((wReCRC >> 8) & 0xFF);

     switch(para){
         case 0x00: pc_trans(rdkp_cmd); break;
         case 0x01: pc_trans(rdki_cmd); break;
         case 0x02: pc_trans(rdkd_cmd); break;
         case 0x03: pc_trans(rddz_cmd); break;
         case 0x04: pc_trans(rdzz_cmd); break;
         case 0x05: pc_trans(rdkp_cmd); pc_trans(rdki_cmd); pc_trans(rdkd_cmd); pc_trans(rddz_cmd); pc_trans(rdzz_cmd); break;
     }
}

void pc_trans(uint8_t xcmd[]){
    rt_sem_take(&tx1_sem, RT_WAITING_FOREVER);
    rt_thread_mdelay(55);
    rt_device_write(serial1, 0, &xcmd[0], 8);
    rt_thread_mdelay(10);
    rt_sem_release(&tx1_sem);
    rt_kprintf("pressure controller command transfered!");
}


void pid_cmd_set(uint8_t arg[]){
    send_pid[3] = arg[1];
    report_pid[2] = arg[1];
    if(arg[2] == 0x16){
        switch(arg[3]){
            case 0x41: send_pid[5] = arg[4]; send_pid[4] = arg[5]; report_pid[3] = 0x00; report_pid[5] = arg[4]; report_pid[4] = arg[5]; break;
            case 0x42: send_pid[7] = arg[4]; send_pid[6] = arg[5]; report_pid[3] = 0x01; report_pid[5] = arg[4]; report_pid[4] = arg[5]; break;
            case 0x43: send_pid[9] = arg[4]; send_pid[8] = arg[5]; report_pid[3] = 0x02; report_pid[5] = arg[4]; report_pid[4] = arg[5]; break;
        }
    }else if (arg[2] == 0x17){
        send_pid[11] = arg[4]; send_pid[10] = arg[5]; report_pid[3] = 0x03; report_pid[5] = arg[4]; report_pid[4] = arg[5];
    }else if(arg[2] == 0x18){
        send_pid[13] = arg[4]; send_pid[12] = arg[5]; report_pid[3] = 0x04; report_pid[5] = arg[4]; report_pid[4] = arg[5];
    }
}


void upload_pid(uint8_t xcmd[], uint8_t len){
    rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
    rt_thread_mdelay(5);
    rt_device_write(serial6, 0, &xcmd[0], len);
    rt_thread_mdelay(10);
    rt_sem_release(&tx6_sem);
}


void set_pid(uint8_t arg[]){
    st_pid_cmd[1] = arg[2];
    switch(arg[3]){
        case 0x00: st_pid_cmd[2] = 0x16; st_pid_cmd[3] = 0x81; st_pid_cmd[5] = arg[4]; st_pid_cmd[4] = arg[5]; break;
        case 0x01: st_pid_cmd[2] = 0x16; st_pid_cmd[3] = 0x82; st_pid_cmd[5] = arg[4]; st_pid_cmd[4] = arg[5]; break;
        case 0x02: st_pid_cmd[2] = 0x16; st_pid_cmd[3] = 0x83; st_pid_cmd[5] = arg[4]; st_pid_cmd[4] = arg[5]; break;
        case 0x03: st_pid_cmd[2] = 0x17; st_pid_cmd[3] = 0x80; st_pid_cmd[5] = arg[4]; st_pid_cmd[4] = arg[5]; break;
        case 0x04: st_pid_cmd[2] = 0x18; st_pid_cmd[3] = 0x80; st_pid_cmd[5] = arg[4]; st_pid_cmd[4] = arg[5]; break;
    }
    uint16_t wReCRC = 0;
    wReCRC = wCRCCheck_Uart_Data(&st_pid_cmd[0], 4);
    st_pid_cmd[6] = (uint8_t) (wReCRC & 0xFF);
    st_pid_cmd[7] = (uint8_t) ((wReCRC >> 8) & 0xFF);
    pc_trans(st_pid_cmd);
}




uint16_t wCRCCheck_Uart_Data(uint8_t *bpData, uint8_t bLen)
{
    uint8_t i, j;
    uint8_t bTemp = 0;
    uint16_t wResulrCrc = 0xffff;
    if (bLen == 0) {
        goto Exit;
    }

    for (i = 1; i < bLen; i++) {
        bTemp = *(bpData + 2 + i);  //bpData[3:6) (3 bytes) were used to perform CRC
        for (j = 0; j < 8; j++) {
            if (((wResulrCrc ^ bTemp) & 0x0001) != 0)
                    {
                wResulrCrc >>= 1;
                wResulrCrc ^= 0x1021;
            } else {
                wResulrCrc >>= 1;
            }
            bTemp >>= 1;
        }
    }
    Exit: return wResulrCrc + 1;
}
