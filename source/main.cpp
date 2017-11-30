/*
 * mian.cpp
 *
 *  Created on: Nov 29, 2017
 *      Author: Administrator
 */

#include "system/system.h"
#include "threads/Threads.h"

int main(void)
{
    // System Init
    System_Init();

    // Init Thread
    Init_CreateThreads();
    // 
    while(1);

    return 0;
}


