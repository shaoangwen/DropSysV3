/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-11     wen       the first version
 */
#ifndef APPLICATIONS_PRESSURE_CONTROL_H_
#define APPLICATIONS_PRESSURE_CONTROL_H_
extern uint8_t report_cmd[];
extern uint8_t send_pid[];
extern uint8_t report_pid[];

void read_pressure(void);
void set_pressure(uint8_t arg[]);
void rdp_cmd_init(void);
void report_thread_init_and_start(void);
void read_pid_set(uint8_t pc, uint8_t para);
void pc_trans(uint8_t xcmd[]);
void pid_cmd_set(uint8_t arg[]);
void upload_pid(uint8_t xcmd[], uint8_t len);
void set_pid(uint8_t arg[]);
uint16_t wCRCCheck_Uart_Data(uint8_t *bpData, uint8_t bLen);

#endif /* APPLICATIONS_PRESSURE_CONTROL_H_ */
