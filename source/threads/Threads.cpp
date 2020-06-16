/*
 * Init.cpp
 *
 *  Created on: Nov 30, 2017
 *      Author: Administrator
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <socket_server/ssdp_server.h>
#include <tcpserver/tcp_server.h>
#include <ssdp/ssdp.h>
#include "../console/console.h"
#include "include/user_conf.h"
//#include "/drivers/uart/BBBEcho.h"


void* foo(void* ptr)
{
  for (int index=0; index < 30; index++)
  {
	  printf("Current Count: %d\n", index);
  }
  usleep(1000);
  return 0;
}

void* bar(void* ptr)
{
	int x = *(int*)ptr;
	for (int index=x; index < 60; index++)
	  {
		  printf("Current Count: %d\n", index);
	  }
	usleep(1000);
	return 0;
}

static void* CreateThread_SSDP(void* ptr)
{
	SSDP_Init();
	return 0;
}

#if 0 /* yhcha, Jun 15 2020 : Block */
static void* CreateThread_SSDP(void* ptr)
{
	SSDP_Init();
	return 0;
}
#endif

//

//extern void* uart_TaskMain(void* pArg);

int Init_CreateThreads(void)
{
  int  iret1 = 0;
  int  iret2 = 0;
  int  iret3 = 0;
  int  iret4 = 0;
  int  idata = 1;

  pthread_t first;     // spawn new thread that calls foo()
  pthread_t second;    // spawn new thread that calls bar(0)
  pthread_t third;     // spawn new thread that calls bar(0)
  pthread_t fourth;     // spawn new thread that calls bar(0)

  printf("main, foo and bar now execute concurrently...\n");
//  iret1 = pthread_create( &first, NULL, foo, NULL);
#if (__BBB_FUNC_TCPSERVER__)
  iret2 = pthread_create( &second, NULL, TCP_ServerMain, NULL);
#endif

#if (__BBB_FUNC_CONSOLE__)
  iret3 = pthread_create( &third, NULL, Console_TaskMain, NULL);
#endif

#if (__BBB_FUNC_SSDP__)
  iret4 = pthread_create( &fourth, NULL, CreateThread_SSDP, NULL);
#endif

//  iret4 = pthread_create( &fourth, NULL, uart_TaskMain, NULL);

  // synchronize threads:
  //pthread_join(first, NULL);                // pauses until first finishes
  //pthread_join(second, NULL);               // pauses until second finishes

  printf("Thread 1:NULL\n");
  printf("Thread 2:TCP_ServerMain\n");
  printf("Thread 3:Console_TaskMain\n");
  printf("Thread 4:CreateThread_SSDP\n");

  return 0;
}


