#include <protocol/protocol.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /* atoi */

#include "AD820xx.h"
#include "GPIOCtrl.h"
#include "gpio/GPIO.h"

unsigned int msgTblCnt = 0;

MSG_PROTOCOL msgTbl[] = {
{func_VOLSET,   "VOL"},
{func_GPIOSET,  "GPIO"},
{func_MUTE,     "MUTE"},
};
pMSG_PROTOCOL psmsgTbl;

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
