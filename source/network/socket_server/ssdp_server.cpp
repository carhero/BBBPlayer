/*
 * ssdp_server.cpp
 *
 *  Created on: Jan 3, 2018
 *      Author: Administrator
 */

/*
 * ssdp_server.c
 *
 *  Created on: Dec 19, 2017
 *      Author: Administrator
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>

//#define BUF_SIZE 500
#define HEADER "PARC"
#define DEV_NAME "BeagleBoneBlack"
#define MODEL_NAME "BBBPlayer"
//#define ETH_NAME "eth0"
#define ETH_NAME "usb0"
#define PORT "14999"

#pragma pack(push,1)
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
#pragma pack(pop)

device_descriptor_t st_packet;

void error_handling(char *message);


struct ifreq mac;

void* SSDP_Init(void *pArg)
{
    int serv_sock = 0;
    int str_len;
    socklen_t clnt_adr_sz;
    device_descriptor_t msg;
    unsigned int send_cnt = 0;
    struct sockaddr_in serv_adr, clnt_adr;

    serv_sock=socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock==-1)
    {
        error_handling("UDP socket creation error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(PORT));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
    {
        error_handling("bind() error");
    }

    strcpy(mac.ifr_name, ETH_NAME);
    if (0 == ioctl(serv_sock, SIOCGIFHWADDR, &mac))
    {
        int i;
        printf("MAC:");
        for (i = 0; i < 6; ++i)
        {
            printf("%02x", (unsigned char) mac.ifr_addr.sa_data[i]);
        }
        puts("\n");
    }

    while(1)
    {
        clnt_adr_sz=sizeof(clnt_adr);

        memset(&st_packet, 0, sizeof(st_packet));

        str_len=recvfrom(serv_sock, (unsigned char*)&st_packet, sizeof(st_packet), 0,
                                (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        //count = 0;

        if (str_len != sizeof(device_descriptor_t))
        {
          continue;
        }

        memcpy(&msg, (unsigned char*)&st_packet, sizeof(device_descriptor_t));

        if (msg.announce != 0x1 && memcmp(msg.unique_header, HEADER, sizeof(HEADER)) == 0)
        {
            continue;
        }

        if(str_len >= 64)
        {
            #if 1 // debug
            printf("str_len size : %d\n", str_len);
            printf("unique_header:%s\n", st_packet.unique_header);
            printf("tcp_ip_port:%d\n", 14999);
//            printf("device_name:%s\n", st_packet.device_name);
//            printf("model_name:%s\n", st_packet.model_name);
//            printf("serial_number:%s\n", st_packet.serial_number);
            printf("clnt_adr.sin_addr:%X\n", clnt_adr.sin_addr.s_addr);
            #endif
        }
        memcpy(msg.unique_header, HEADER, sizeof(HEADER));
        msg.reserved1_rw0 = htons(0x00);
        msg.reserved2_rw0 = htons(0x00);
        msg.announce = htons(0x00);
        msg.powering_off = htons(0x00);
        msg.version = htonl(0x00000001U);
        msg.tcp_ip_port = 14999;
        memcpy(msg.device_name, DEV_NAME, sizeof(DEV_NAME));
        memcpy(msg.model_name, MODEL_NAME, sizeof(MODEL_NAME));

        sprintf((char *)msg.serial_number, "%02x%02x%02x%02x%02x%02x",\
        mac.ifr_addr.sa_data[0], mac.ifr_addr.sa_data[1], mac.ifr_addr.sa_data[2], \
        mac.ifr_addr.sa_data[3], mac.ifr_addr.sa_data[4], mac.ifr_addr.sa_data[5]);

        sendto(serv_sock, &msg, sizeof(msg), 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
        sendto(serv_sock, &msg, sizeof(msg), 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);

        printf("\n----------------------------------------------------%d\n", send_cnt++);

        //usleep(100);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}


