
#include <stdio.h>
#include "amp/AD820xx.h"
#include "../GPIO/GPIOCtrl.h"
#include "protocol.h"

void System_Init()
{
    printf("System_Init\n");

    // GPIOCtrl_Init
    GPIOCtrl_Init();

    // Volume Init
    AD820xx_Init();
    AD820xx_SetMasterVolume(50);

    protocol_init();
}
