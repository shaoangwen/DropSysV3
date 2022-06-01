/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-11     wen       the first version
 */
#ifndef APPLICATIONS_CMD_HANDLER_H_
#define APPLICATIONS_CMD_HANDLER_H_
extern uint8_t scan_code_cmd[];
void cmd_deliver(uint8_t arg[], uint8_t len);
void set_pressure(uint8_t arg[]);
void dropbus_cmd_handle(uint8_t arg[]);
void cosys_cmd_handle(uint8_t arg[]);
void dc_cmd_handle(uint8_t arg[]);
void forward_uart6(uint8_t arg[], uint8_t len);
void code_scanner_handle(uint8_t arg[], uint8_t len);
void scan_code();
#endif /* APPLICATIONS_CMD_HANDLER_H_ */
