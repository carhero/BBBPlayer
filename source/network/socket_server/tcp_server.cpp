/*
 * tcp_server.cpp
 *
 *  Created on: Jan 10, 2018
 *      Author: Administrator
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "socket_server/tcp_server.h"
#include "protocol.h"

#define BUF_SIZE 1024
#define TCP_PORT "14999"

static void error_handling(char *message);

void *TCP_ServerMain(void *pArg)
{
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;

/*    if(argc!=2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }*/

    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock==-1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(TCP_PORT));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");

    clnt_adr_sz=sizeof(clnt_adr);

    for(i=0; i<100; i++)
    {
        memset(message, 0, sizeof(message));

        clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock==-1)
        {
            error_handling("accept() error");
        }
        else
        {
            printf("Connected client %d \n", i+1);
        }

        while((str_len=read(clnt_sock, message, BUF_SIZE))!=0)
        {
            printf("msg from client: %s \n", message);
            write(clnt_sock, message, str_len);

            // PARSER system callback functions
            MessageParser(message);

            memset(message, 0, sizeof(message));
        }

        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}

static void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}



