/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-19     wen       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <serial_interface.h>
#include <cmd_handler.h>
#include <pressure_control.h>


struct serial_configure config1 = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */
struct serial_configure config2 = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */



void serial_config(void)
{
    config1.baud_rate = BAUD_RATE_115200;        //修改波特率为 115200
    config1.data_bits = DATA_BITS_8;           //数据位 8
    config1.stop_bits = STOP_BITS_1;           //停止位 1
    config1.bufsz     = 512;                   //修改缓冲区 buff size 为 128
    config1.parity    = PARITY_NONE;           //无奇偶校验位

    config2.baud_rate = BAUD_RATE_57600;        //修改波特率为 57600
    config2.data_bits = DATA_BITS_8;           //数据位 8
    config2.stop_bits = STOP_BITS_1;           //停止位 1
    config2.bufsz     = 512;                   //修改缓冲区 buff size 为 128
    config2.parity    = PARITY_NONE;           //无奇偶校验位
}



static rt_err_t uart6_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx6_sem);
    return RT_EOK;
}

static rt_err_t uart1_input(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&rx1_sem);
    return RT_EOK;
}

static rt_err_t uart2_input(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(&rx2_sem);
    return RT_EOK;
}


static rt_err_t uart6_tx_done(rt_device_t dev, void *buffer)
{
    rt_sem_release(&tx6_sem);
    rt_kprintf("tx6_sem released d\r\n");
    return RT_EOK;
}

static rt_err_t uart1_tx_done(rt_device_t dev, void *buffer)
{
    rt_sem_release(&tx1_sem);
    rt_kprintf("tx1_sem released d\r\n");
    return RT_EOK;
}

static rt_err_t uart2_tx_done(rt_device_t dev, void *buffer)
{
    rt_sem_release(&tx2_sem);
    rt_kprintf("tx1_sem released d\r\n");
    return RT_EOK;
}


void serial6_thread_entry(void *parameter)
{
    uint8_t sig_head;
    uint8_t cmd_words[11];
    while (1)
    {
        int i = 0;
        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量, 参数-1未使用 ：读取数据偏移量，此参数串口设备未使用*/

        while (rt_device_read(serial6, -1, &sig_head, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&rx6_sem, RT_WAITING_FOREVER);
        }
        //if ((sig_head == 0xEE)|(sig_head == 0xDD)){
        if (sig_head == 0xDD){
           //bug here!! need to delay(2ms 100% recover, 1ms 80% recover) or print something(100% recover) or it will lost the last byte.
            rt_kprintf("sizeof(cmd_words)/sizeof(uint8_t) is %x\r\n", sizeof(cmd_words)/sizeof(uint8_t));
            rt_kprintf("sizeof(cmd_words) is %x\r\n", sizeof(cmd_words));
            rt_kprintf("sizeof(uint8_t) is %x\r\n", sizeof(uint8_t));
            rt_thread_mdelay(2);
            cmd_words [0] = sig_head;
            i = 1;
            while(i < sizeof(cmd_words)/sizeof(uint8_t)){//or sizeof(cmd_words)/sizeof(cmd_words[0])
                rt_device_read(serial6, -1, &sig_head, 1);
                cmd_words [i] = sig_head;
                i += 1 ;
               // rt_kprintf("message is  %x\r\n", sig_head);
                //rt_thread_mdelay(1);
            }
           // rt_kprintf("check point sighead is %x\r\n", sig_head);
            //rt_kprintf("check point i is %d\r\n", i);
            //rt_kprintf("check point cmd_words[i-1] is %x\r\n", cmd_words[i-1]);
            i -= 1;
            if ((cmd_words[i] == 0xCC) && (cmd_words[i-1] == 0xCD) && (cmd_words[i-2] == 0xCD) && (cmd_words[i-3] == 0xFF))
            {

                //if(((cmd_words[i-1] == 0xCD)|(cmd_words[i-1]==0xFF)) && ((cmd_words[i-2] == 0xCD)|(cmd_words[i-2] == 0xFF))){
                rt_kprintf("command received \r\n");
                cmd_deliver(cmd_words, sizeof(cmd_words)/sizeof(cmd_words[0]));
              }
         i = 0;
         // rt_kprintf("cmd is %x\r\n", *cmd_words);
         //cmd_words <<= 8;
         //rt_kprintf("cmd <<  is %x\r\n", cmd_words);
         //cmd_words += sig_head;
         //rt_kprintf("cmd after is %x\r\n", cmd_words);
         // rt_device_write(serial2, 0, &cmd_words, (sizeof(cmd_words) - 1));
         }
    }
}



void serial1_thread_entry(void *parameter)
{
    uint8_t sig_head;
    uint8_t cmd_words[8];
    while (1)
    {
        int i = 0;
        while (rt_device_read(serial1, -1, &sig_head, 1) != 1)
        {
            rt_sem_take(&rx1_sem, RT_WAITING_FOREVER);
        }
        if (sig_head == 0x5E)
        {
            rt_thread_mdelay(2);
            i = 0;
            cmd_words [i] = sig_head;
            i += 1;
            while(i < sizeof(cmd_words)/sizeof(uint8_t)){//also can be presented as sizeof(cmd_words)/sizeof(cmd_words[0])
                rt_device_read(serial1, -1, &sig_head, 1);
                cmd_words [i] = sig_head;
                i +=1 ;
                //rt_kprintf("srial1 message is  %x\r\n", sig_head);

            }
            i -= 1;
            if ((cmd_words[2] == 0x19) && (cmd_words[3] == 0x40))
            {
                cmd_deliver(cmd_words, sizeof(cmd_words)/sizeof(cmd_words[0]));
                //rt_kprintf("cosys ch1 command received\r\n");
             }else if((cmd_words[2] == 0x16) | (cmd_words[2] == 0x17) | (cmd_words[2] == 0x18)){
                 pid_cmd_set(cmd_words);
             }
        }
        i=0;
    }
}




void serial2_thread_entry(void *parameter)
{
    uint8_t sig_head;
    uint8_t cmd_words[39];
    uint8_t cmd_words2[7];
    while (1)
    {
        rt_thread_mdelay(2);
        int i = 0;
        //int j = 0;
        while (rt_device_read(serial2, -1, &sig_head, 1) != 1)
        {
            rt_sem_take(&rx2_sem, RT_WAITING_FOREVER);
        }
        if (sig_head == 0x47)
        {
            rt_thread_mdelay(2);
            cmd_words[0] = sig_head;
            cmd_words2[0] = sig_head;
            rt_kprintf("srial2 message is  %x\r\n", sig_head);
            i = 1;
            while(i < sizeof(cmd_words2)/sizeof(uint8_t)){
                rt_device_read(serial2, -1, &sig_head, 1);
                cmd_words[i] = sig_head;
                cmd_words2[i] = sig_head;
                i +=1 ;
                rt_kprintf("srial2 message is  %x\r\n", sig_head);

            }
            i -= 1;
            if ((cmd_words2[1] == 0x58) && (cmd_words[2] == 0x41) && (cmd_words[3] == 0x63) && (cmd_words2[i] == 0x4E) && (cmd_words2[i-1] == 0x44))
            {
                //cmd_words[1] = 0xF2;
                cmd_deliver(cmd_words2, sizeof(cmd_words2)/sizeof(cmd_words2[0]));
                rt_kprintf("bypass command_short \r\n");
             }else{
                  i += 1;
                  while(i < sizeof(cmd_words)/sizeof(uint8_t)){
                    rt_device_read(serial2, -1, &sig_head, 1);
                    cmd_words[i] = sig_head;
                    i += 1;
                    rt_kprintf("srial2 message is  %x\r\n", sig_head);
                }
                i = 0;
                cmd_deliver(cmd_words, sizeof(cmd_words)/sizeof(cmd_words[0]));
                rt_kprintf("bypass command_long \r\n");
            }
        }
        i=0;
    }
}


int uart6_init(void){
    rt_err_t ret = RT_EOK;
    rt_sem_init(&rx6_sem, "rx6_sem", 0, RT_IPC_FLAG_FIFO);
    rt_sem_init(&tx6_sem, "tx6_sem", 1, RT_IPC_FLAG_FIFO);
    serial6 = rt_device_find("uart6");
     if (!serial6){
          rt_kprintf("find uart6 failed!\n");
          return RT_ERROR;
    }
    rt_device_control(serial6, RT_DEVICE_CTRL_CONFIG, &config1);
    rt_device_open(serial6, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX |  RT_DEVICE_FLAG_INT_TX);
    rt_kprintf("uart6 found\r\n");
    rt_device_set_rx_indicate(serial6, uart6_input);
    rt_device_set_tx_complete(serial6, uart6_tx_done);
    rt_thread_t uart6_thread = rt_thread_create("serial6", serial6_thread_entry, RT_NULL, 1024, 17, 10);
    if (uart6_thread != RT_NULL){
        rt_thread_startup(uart6_thread);
        rt_kprintf("uart6 thread started!\n");
    }else{
        ret = RT_ERROR;
    }
    return ret;
}

int uart1_init(void){
    rt_err_t ret = RT_EOK;
    rt_sem_init(&rx1_sem, "rx1_sem", 0, RT_IPC_FLAG_FIFO);
    rt_sem_init(&tx1_sem, "tx1_sem", 1, RT_IPC_FLAG_PRIO);
    serial1 = rt_device_find("uart1");
     if (!serial1){
          rt_kprintf("find uart1 failed!\n");
          return RT_ERROR;
    }
    rt_device_control(serial1, RT_DEVICE_CTRL_CONFIG, &config2);
    rt_device_open(serial1, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX);
    rt_kprintf("uart1 found\r\n");
    rt_device_set_rx_indicate(serial1, uart1_input);
    rt_device_set_tx_complete(serial1, uart1_tx_done);
    rt_thread_t uart1_thread = rt_thread_create("serial1", serial1_thread_entry, RT_NULL, 1024, 18, 10);
    if (uart1_thread != RT_NULL){
        rt_thread_startup(uart1_thread);
        rt_kprintf("uart1 thread started!\n");
    }else{
        ret = RT_ERROR;
    }
    return ret;
}

int uart2_init(void){
    rt_err_t ret = RT_EOK;
    rt_sem_init(&rx2_sem, "rx2_sem", 0, RT_IPC_FLAG_FIFO);
    rt_sem_init(&tx2_sem, "tx2_sem", 1, RT_IPC_FLAG_PRIO);
    serial2 = rt_device_find("uart2");
     if (!serial2){
          rt_kprintf("find uart2 failed!\n");
          return RT_ERROR;
    }
    rt_device_control(serial2, RT_DEVICE_CTRL_CONFIG, &config1);
    rt_device_open(serial2, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_INT_TX);
    rt_kprintf("uart2 found\r\n");
    rt_device_set_rx_indicate(serial2, uart2_input);
    rt_device_set_tx_complete(serial2, uart2_tx_done);
    rt_thread_t uart2_thread = rt_thread_create("serial2", serial2_thread_entry, RT_NULL, 1024, 16, 10);
    if (uart2_thread != RT_NULL){
        rt_thread_startup(uart2_thread);
        rt_kprintf("uart2 thread started!\n");
    }else{
        ret = RT_ERROR;
    }
    return ret;
}

