
#include <stdio.h>
#include "amp/AD820xx.h"

void System_Init()
{
    printf("System_Init\n");

    // Volume Init
    AD820xx_Init();
    AD820xx_SetMasterVolume(50);
}
