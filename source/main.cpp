/*
 * mian.cpp
 *
 *  Created on: Nov 29, 2017
 *      Author: Administrator
 */

#include "system/system.h"
#include "threads/Threads.h"
#include "network/ssdp/ssdp.h"

int main(void)
{
    // System Init
    System_Init();

    // Init UART
    Init_CreateThreads();

    // Init Network
    SSDP_Init();

    while(1);

    return 0;
}


