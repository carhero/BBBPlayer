/*
 * protocol.h
 *
 *  Created on: Jan 10, 2018
 *      Author: Administrator
 */

#ifndef SOURCE_NETWORK_PROTOCOL_PROTOCOL_H_
#define SOURCE_NETWORK_PROTOCOL_PROTOCOL_H_

// network define
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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

int protocol_TcpIPConnect(struct sockaddr_in *from_addr);
#endif /* SOURCE_NETWORK_PROTOCOL_PROTOCOL_H_ */
