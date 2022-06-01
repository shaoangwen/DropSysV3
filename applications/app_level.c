#include <rtthread.h>
#include <configuration.h>
#include <serial_interface.h>
#include <valves_control.h>
#include <cmd_handler.h>
#include <pressure_control.h>
#include <pump_control.h>
#include <Door_control.h>
#include <io_define.h>
#include <motor_control.h>


uint8_t system_ready_cmd[] = {0xDE, 0xFA, 0xA2, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCD, 0xCD, 0xCC};
uint8_t shut_down_ready_cmd[] = {0xDE, 0xFA, 0xA1, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xCD, 0xCD, 0xCC};


void connect(dir){
    if(dir == 0x00){
        rt_uint8_t dire = 0;
        M2_run((void*) dire);
    }else if(dir == 0x01){
        rt_uint8_t dire = 2;
        M2_run((void*) dire);
    }
}

void drawer(dir){
    if(dir == 0x00){
        rt_uint8_t dire = 0;
        M1_run((void*) dire);
    }else if(dir == 0x01){
        rt_uint8_t dire = 2;
        M1_run((void*) dire);
    }
}

void stir(dir){
    if (dir == 0){
        M3_stop();
    }else if(dir == 1){
        M3_run();
    }
}

void inflating_control(dir){
    if (dir == 0){
        inflating_stop();
    }else if(dir == 1){
        inflating_start();
    }
}


void init_system_entry(){
    auto_homing();
    rt_thread_mdelay(1000);
    rt_sem_take(&m2_sem, RT_WAITING_FOREVER);
    rt_sem_release(&m2_sem);
    rt_sem_take(&m3_sem, RT_WAITING_FOREVER);
    rt_sem_release(&m3_sem);
    rt_thread_mdelay(1000);
    rt_sem_take(&m1_sem, RT_WAITING_FOREVER);
    rt_sem_release(&m1_sem);
    while(p_kpa < win_l_kpa){
        rt_thread_mdelay(100);
    }
    rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
    rt_device_write(serial6, 0, &system_ready_cmd[0], 11);
    rt_thread_mdelay(50);
    rt_sem_release(&tx6_sem);
}

void shut_down(){
    auto_homing();
    rt_thread_mdelay(1000);
    rt_sem_take(&m2_sem, RT_WAITING_FOREVER);
    rt_sem_release(&m2_sem);
    rt_sem_take(&m3_sem, RT_WAITING_FOREVER);
    rt_sem_release(&m3_sem);
    rt_thread_mdelay(1000);
    rt_sem_take(&m1_sem, RT_WAITING_FOREVER);
    rt_sem_release(&m1_sem);
    inflating_stop();
    rt_pin_write(PUMP_pin, 0);

    rt_sem_take(&tx6_sem, RT_WAITING_FOREVER);
    rt_device_write(serial6, 0, &shut_down_ready_cmd[0], 11);
    rt_thread_mdelay(50);
    rt_sem_release(&tx6_sem);
}
