/*
 * console.h
 *
 *  Created on: Nov 30, 2017
 *      Author: Administrator
 */

#ifndef SOURCE_CONSOLE_CONSOLE_H_
#define SOURCE_CONSOLE_CONSOLE_H_

void* Console_TaskMain(void* pArg);
void* uart_TaskMain(void* pArg);
void Console_msgParser(char *command);
#endif /* SOURCE_CONSOLE_CONSOLE_H_ */
