/*
 * crate_thread.c
 *
 *  Created on: Jun 07, 2017
 *      Author: yhcha
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// network define
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <dlfcn.h>

#include <ssdp/ssdp.h>

#define BUF_SIZE  1024

// SSDP packet data structure
//#pragma pack(push,1)
typedef struct _device_descriptor_t
{
    int8_t  unique_header[0x4];
    int8_t  reserved1_rw0;
    int8_t  reserved2_rw0;
    int8_t  announce;
    int8_t  powering_off;
    int32_t version;
    int32_t tcp_ip_port;
    int8_t  device_name[0x10];
    int8_t  model_name[0x10];
    int8_t  serial_number[0x10];
} device_descriptor_t /*__attribute__ ((__packed__))*/;

device_descriptor_t st_packet;
/*device_descriptor_t st_packet = {
    .unique_header = "PARC",
    .reserved1_rw0 = 0,
    .reserved2_rw0 = 0,
    .announce = 1,
    .version = 0x01000000,
    .tcp_ip_port = 0,
};*/

// Mqtt json data variable
unsigned int JsonStrLen;
unsigned char JsonBuf[1024];
unsigned int JsonFlagUpdate;

// linux thread variable
pthread_t th_PollUpdate;    // polling update thread
pthread_t th_SSDPSender;    // network SSDP serch msg sending thread
pthread_t th_SSDPReceiver;  // network SSDP serch msg receiver thread

// Socket variable
char message[BUF_SIZE];
int ssdp_skip_flag = 0;
int Usock = 0, sock = 0;  // UDP, TCP socket
struct sockaddr_in serv_addr,from_adr;
socklen_t adr_sz;

// ------------------------------------------------
// API Functions
// ------------------------------------------------
void SSDP_InitVariable(void)
{
    st_packet.reserved1_rw0 = 0;
    st_packet.reserved2_rw0 = 0;
    st_packet.announce = 1;
    st_packet.version = 0x01000000;
    st_packet.tcp_ip_port = 0;
    memcpy(st_packet.unique_header, "PARC", sizeof("PARC"));


}

void Thread_SetUpdateFlag(unsigned int update)
{
  JsonFlagUpdate = update;
}

unsigned int Thread_GetUpdateFlag(void)
{
  return JsonFlagUpdate;
}

void CopyJsonBuffer(const char *pBuf, unsigned int Length)
{
  memset(JsonBuf, 0, sizeof(JsonBuf));

  memcpy(JsonBuf, pBuf, Length);
}

// ------------------------------------------------
// thread task
// ------------------------------------------------
void* thread_PollingUpdate(void* ptr)
{
  unsigned int BufCnt = 0;
  unsigned int cnt = 0;
  printf("thread_PollingUpdate init...\n");

  while(1)
  {
    while( (sock < 0) || ssdp_skip_flag == 0) // If TCP socket is not ready, polling update should be not called
    {
      ;
    }

    //printf("PollingUpdate loop %d\n", cnt++ );
    if(Thread_GetUpdateFlag())
    {
      JsonFlagUpdate = 0;
      printf("JsonFlagUpdate requsted \n");
      printf("Copyed buffer:%s \n", JsonBuf);

      // if data sending is avilable, send tcp/ip command to MRX
      //msg_parser_mqtt_data(JsonBuf, strlen(JsonBuf));
    }

    /* escape routine is needed to close TCP socket
    if(escape condition)
    {
      if(ssdp_skip_flag)
      {
        close(sock);
      }
      break;
    }
    */

    sleep(0.1);
  }

  return 0;
}


// ------------------------------------------------
// TEST Code for TCP Connection with MRX
// ------------------------------------------------
#define BUF_SIZE 1024
void error_handling(char *message);

//int main(int argc, char *argv[])
int TCP_Connection(struct sockaddr_in *from_addr)
{
    // int sock;
    char message[BUF_SIZE];
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
    printf("TCP Server IP : %X\n",serv_adr.sin_addr.s_addr);
  }

    //close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

// ------------------------------------------------
// SSDP Sender function
// ------------------------------------------------
static int MSearchMessageDataSend(void)
{
    Usock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(Usock==-1)
    {
      printf("socket() error\n");
      return -1;
    }

    // It is very important to create broadcast message on UDP
    int broadcast = 1;
    if( setsockopt(Usock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) != 0 )
    {
        perror("setsockopt : ");
        close(Usock);
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&st_packet, 0, sizeof(st_packet));

    //st_packet.unique_header = "PARC";
    memcpy(st_packet.unique_header, "PARC", sizeof("PARC"));
    st_packet.announce = 0x01;
    st_packet.version = 0x01000000;
    st_packet.tcp_ip_port = 0;

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("255.255.255.255");
    serv_addr.sin_port=htons(atoi("14999"));

  sendto(Usock, (unsigned char*)&st_packet, sizeof(st_packet), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  return 0;
  //ssdp_skip_flag = 0;
}

void* thread_SSDPSender(void* ptr)
{
  unsigned int cnt = 0;
  printf("thread_SSDPSender init...\n");

  while(1)
  {
    while(ssdp_skip_flag)  // If have tcp connection with MRX, then this flag is set to 1
    {
      ;
    }

    // Send broadcast ping message
    printf("send SSDP msg... \n");

    MSearchMessageDataSend();

    sleep(2);
  }
  return 0;
}

void* thread_SSDPReceiver(void* ptr)
{
  unsigned int cnt = 0;
  int count1;

  printf("thread_SSDPReceiver init...\n");

  while(1)
  {
    int ret = 0;
    // If TCP connection is estbalished, then it should be skipped.
    while(ssdp_skip_flag)  // If have tcp connection with MRX, then this flag is set to 1
    {
      ;
    }

    adr_sz = sizeof(from_adr);

    memset(&from_adr, 0, sizeof(from_adr));
    //ret = recvfrom(Usock, message, BUF_SIZE, 0,(struct sockaddr*)&from_adr, &adr_sz);
    ret = recvfrom(Usock, &st_packet, sizeof(st_packet), 0,(struct sockaddr*)&from_adr, &adr_sz);

    if(ret > 0)
    {
      #if 1 // debug
      printf("rev size : %d\n", ret);
      printf("unique_header:%s\n", st_packet.unique_header);
      printf("tcp_ip_port:%d\n", st_packet.tcp_ip_port);
      printf("device_name:%s\n", st_packet.device_name);
      printf("model_name:%s\n", st_packet.model_name);
      printf("serial_number:%s\n", st_packet.serial_number);
      printf("from_adr.sin_addr:%X\n", from_adr.sin_addr.s_addr);
      #endif

      if(from_adr.sin_addr.s_addr != 0)  // add more conditions
      {
        if(TCP_Connection(&from_adr) == 0)
        {
          ssdp_skip_flag = 1;
          // break;
        }
      }

      // Clear temp buffer data
      memset(message, 0, BUF_SIZE);
    }

    adr_sz = 0;

    sleep(2);
  }
  return 0;
}


int SSDP_Init(void)
{
    SSDP_InitVariable();
    pthread_create( &th_PollUpdate, NULL, thread_PollingUpdate, NULL);
    pthread_create( &th_SSDPSender, NULL, thread_SSDPSender, NULL);
    pthread_create( &th_SSDPReceiver, NULL, thread_SSDPReceiver, NULL);

  // synchronize threads:
  //pthread_join(th_PollUpdate, NULL);                // pauses until first finishes
  //pthread_join(th_NetServer, NULL);               // pauses until second finishes

    printf("Create thread completed.\n");

    return 0;
}
