/*
 * protocol.h
 *
 *  Created on: Jan 10, 2018
 *      Author: Administrator
 */

#ifndef SOURCE_NETWORK_PROTOCOL_PROTOCOL_H_
#define SOURCE_NETWORK_PROTOCOL_PROTOCOL_H_

typedef struct _MSG_PROTOCOL
{
    void(*callback)(char *);
    char *msgName;

}MSG_PROTOCOL, *pMSG_PROTOCOL;

void MessageParser(char *msg);
void protocol_init();

void func_VOLSET(char *msg);
void func_GPIOSET(char *msg);
void func_MUTE(char *msg);

#endif /* SOURCE_NETWORK_PROTOCOL_PROTOCOL_H_ */
