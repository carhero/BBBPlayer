/*
 * mian.cpp
 *
 *  Created on: Nov 29, 2017
 *      Author: Administrator
 */

#include "system/system.h"
#include "threads/Threads.h"
#include "network/ssdp/ssdp.h"
#include "../example/eeprom_i2c.h"
#include "include/user_conf.h"
//#include "network/socket_server/ssdp_server.h"
//#include <linux/gpio.h>       // Required for the GPIO functions

int main(void)
{
    // System Init
    System_Init();

    // Init UART
    Init_CreateThreads();

    // Init Network
//    SSDP_Init();

#if (__BBB_FUNC_EEPROM__)
    EEPROM_I2CInit();
#endif

    while (1);

    return 0;
}

