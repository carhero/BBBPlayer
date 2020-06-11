#include <protocol/protocol.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /* atoi */

#include "AD820xx.h"
#include "GPIOCtrl.h"
#include "gpio/GPIO.h"

// Network API
#include <iostream>
#include "network/socketlib/SocketClient.h"
using namespace std;
using namespace exploringBB;


unsigned int msgTblCnt = 0;

#define BUF_SIZE  1024

MSG_PROTOCOL msgTbl[] = {
{func_VOLSET,   "VOL"},
{func_GPIOSET,  "GPIO"},
{func_MUTE,     "MUTE"},
};
pMSG_PROTOCOL psmsgTbl;

//SocketClient sc;
int sock;

static void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int protocol_TcpIPConnect(struct sockaddr_in *from_addr)
{
    // int sock;
    char message[BUF_SIZE] = {0,};
    int str_len;
    struct sockaddr_in serv_adr;

  char *TDP_PORT = "14999";

    sock=socket(PF_INET, SOCK_STREAM, 0);
    if(sock==-1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=from_addr->sin_addr.s_addr; //inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(TDP_PORT));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
    {
        error_handling("connect() error!");
    }
    else
    {
        puts("Connected...........");
        printf("TCP Server IP : %08X\n\r",serv_adr.sin_addr.s_addr);
        //sendto(sock, "Z1VOLUP;", sizeof("Z1VOLUP;"), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
        strcpy(message, "Z1VOL-35;");
        //send (int __fd, const void *__buf, size_t __n, int __flags);
        //send(sock, (char *)message, sizeof(message), 0);
        write(sock, message, strlen(message));
        printf("message : %s\n\r",message);
    }

    //close(sock);
    return 0;
}

void protocol_init()
{
    msgTblCnt = (sizeof(msgTbl)/sizeof(MSG_PROTOCOL));
    psmsgTbl = msgTbl;
}

void MessageParser(char *msg)
{
    unsigned int msgCnt = msgTblCnt;
    while(msgCnt--)
    {
        if(strstr(msg, msgTbl[msgCnt].msgName))
        {
            msgTbl[msgCnt].callback(msg);
            break;
        }
    }
}

void func_VOLSET(char *msg)
{
    msg += strlen("VOL");
    printf("func_VOLSET:%s\n", msg);

    AD820xx_SetMasterVolume(atoi(msg));
}

void func_GPIOSET(char *msg)
{
    unsigned char RetVal = 0, uStep, value = 0;

    msg += strlen("GPIO");
    printf("func_GPIOSET:%s\n",msg);

    if(!strncmp((char *)msg, "on", uStep = strlen("on")))
    {
        GPIOCtrl_SetOut(HIGH);
    }
    else if(!strncmp((char *)msg, "off", uStep = strlen("off")))
    {
        GPIOCtrl_SetOut(LOW);
    }
    else if(!strncmp((char *)msg, "swing", uStep = strlen("swing")))
    {
        GPIOCtrl_swingPort();
    }
    else
    {
        printf("*** Unknown command! ***\n");
    }
}

void func_MUTE(char *msg)
{
    unsigned char RetVal = 0, uStep, value = 0;

    msg += strlen("MUTE");
    printf("func_MUTE:%s\n",msg);

    if(!strncmp((char *)msg, "on", uStep = strlen("on")))
    {
      AD820xx_SetMasterMute(1);
    }
    else
    {
      AD820xx_SetMasterMute(0);
    }
}

