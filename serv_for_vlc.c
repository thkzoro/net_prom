/**
 * Copyright (C) 2015-2020  Meari Technology Co., Ltd. All Rights reserved. 
 * 
 * @file    server.c
 * @brief   description here
 * send messge to client 
 * @author  kang
 * @date    2017-6-30
 * 
 * @history v1.2.3.0
 */
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include "include/record_parse_pub.h"
#include "src/serv_process.h"


#define SERVER_PORT 544
/*S:发送和R:接收缓存区大小*/
#define R_SIZE		64


/* 得到server的设备描述符 */
static int serv_sockfd_get(void) {	
	int ret;
	ret = socket(AF_INET, SOCK_DGRAM, 0);
	if (ret == -1) {
		printf("socket error!\n");
	}
	return ret;
}

static int serv_bind(int *fd, struct sockaddr_in *addr) {
	int ret;
	/* 填充server的地址信息*/
	addr->sin_family      = AF_INET;
	addr->sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	addr->sin_addr.s_addr = INADDR_ANY;
	memset(addr->sin_zero, 0, 8);
	/* 绑定服务器地址 */
	ret = bind(*fd, (const struct sockaddr *)addr, sizeof(struct sockaddr));
	return ret;
}
	
static int client_config(struct sockaddr_in *addr, char *ip) 
{	
	int ret;
	addr->sin_family      = AF_INET;
	addr->sin_port        = htons(544);  /* host to net, short */	
	if (0 == inet_aton(ip, &addr->sin_addr)) {
		printf("invalid server_ip\n");
		ret = -1;
	}
}

/* socket
 * bind
 * sendto/recvfrom
 */
int main(int argc, char **argv)
{
	/* sock_fd, socket的设备描述符*/
	int serv_sockfd;
	struct sockaddr_in serv_addr, client_addr;
	int ret;
	/*struct sockaddr_in=sockaddr，地址信息结构体的长度*/
	int addrlen = sizeof(struct sockaddr);
		
	char rbuf[R_SIZE];
	/* 得到服务器的设备描述符，并绑定*/
	serv_sockfd = serv_sockfd_get();
	if (serv_sockfd != -1) {
		ret = serv_bind(&serv_sockfd, &serv_addr);
		if (ret == -1) {
			printf("bind error!\n");
			return ret;
		}
	}

	ret = client_config(&client_addr,"192.168.0.100");
	printf("client_config ret=%d\n\n",ret);
	/*用于获取client的地址信息*/
	// recvfrom(serv_sockfd, rbuf, R_SIZE, 0, (struct sockaddr *)&client_addr, &addrlen);

	while (1) {
		// int send_vedio_to_client(int fd, struct sockaddr_in *addr, time_t srt_tm)
		printf("开始发送视频\n\n");
		send_vedio_to_client(serv_sockfd, &client_addr);
		printf("结束发送视频\n\n");
		sleep(1);
		}	
	close(serv_sockfd);	
	return 0;
}


