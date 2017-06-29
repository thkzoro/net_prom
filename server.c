/**
 * Copyright (C) 2015-2020  Meari Technology Co., Ltd. All Rights reserved. 
 * 
 * @file    server.c
 * @brief   description here
 * send messge to client 
 * @author  kang
 * @date    2017-6-28
 * 
 * @history v1.2.1.0
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

#define SERVER_PORT 8888
/*S:发送和R:接收缓存区大小*/
#define R_BUF_LEN		64
#define S_BUF_LEN		5*1024*1024
char PTAH[100]="./doc/SD1329/056566767/record/2017";

/* 得到server的设备描述符 */
static int serv_sockfd_get(void) {	
	int ret_val;
	ret_val = socket(AF_INET, SOCK_DGRAM, 0);
	if (ret_val == -1) {
		printf("socket error!\n");
		ret_val = -1;
	}
	return ret_val;
}

static int serv_bind(int *fd, struct sockaddr_in *addr) {
	int ret_val;
	/* 填充server的地址信息*/
	addr->sin_family      = AF_INET;
	addr->sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	addr->sin_addr.s_addr = INADDR_ANY;
	memset(addr->sin_zero, 0, 8);
	/* 绑定服务器地址 */
	ret_val = bind(*fd, (const struct sockaddr *)addr, sizeof(struct sockaddr));
	return ret_val;
}

static int recv_and_reply(int fd, char *buff, struct sockaddr_in *addr) 
{
	// t_val = recv_and_reply(serv_sockfd, rbuf, &client_addr);
	int addrlen;
	int ret_val;
	char p[32] = {0};

	printf("buff:%p %s\n", buff, buff);

	addrlen = sizeof(struct sockaddr);
	
	ret_val = recvfrom(fd, p, R_BUF_LEN-1, 0, (struct sockaddr *)&addr, &addrlen);
	printf("recvfrom ret=%d\n", ret_val);
		if (ret_val > 0) {
			//buff[ret_val] = '\0';
			printf("buff[ret_val]:%s\n", p);
			memcpy(buff, p, ret_val);
			printf("buff[ret_val]:  buf %s\n", buff);
			/*打印客户端的信息*/
			printf("Get Msg From %s:%s", inet_ntoa(addr->sin_addr), buff);
			/* 判断命令是否正确*/
			ret_val = strncmp("download", buff,strlen("download"));
			/*接收到client的命令，进行回复*/
			if (ret_val == 0) {
				sprintf(buff, "cmd:ok.\n");
				sendto(fd, buff, strlen(buff), 0, (const struct sockaddr *)&addr, addrlen);
			} else {
				sprintf(buff, "cmd error\nplease input like this demo\ndownload\n");
				sendto(fd, buff, strlen(buff), 0, (const struct sockaddr *)&addr, addrlen);
			}
		}
		return ret_val;
}
static int send_vedio_to_client(int fd, char *buff, struct sockaddr_in *addr, time_t tm)
{			
	
	int addrlen;
	int ret_val;

	int frame_len;
	int type;

	addrlen = sizeof(struct sockaddr);
	/*根据录像文件路径，初始化视频片段*/
	ret_val = record_parse_init(PTAH);
	if (ret_val != 0) return ret_val;
	
		/* 得到一帧的开始时间 */
	ret_val = record_parse_get_frame_start(tm);
	if (ret_val != 0) return ret_val;

	record_parse_print_all_tm_data_list();

	/* 不断的获取帧，直到结束*/
	do {
		ret_val = record_parse_get_one_frame(S_BUF_LEN, buff, &frame_len, &type);
		if (ret_val == 0) {
			sendto(fd, buff, frame_len, 0, (const struct sockaddr *)&addr, addrlen);
		}
	} while(ret_val == 0);
	if (ret_val == -1) {
			sprintf(buff, "filshed");
			sendto(fd, buff, strlen(buff), 0, (const struct sockaddr *)&addr, addrlen);
			record_parse_get_frame_stop();
			record_parse_deinit();
			ret_val = 0;				
		}
	return ret_val;	
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
	int ret_val;
	/*struct sockaddr_in=sockaddr，地址信息结构体的长度*/
	int addrlen;
	/* len长度, r:接收，s:发送*/
	time_t start_tm = 1483200000;//时间戳，视频开始时间
	int rlen;
	int slen;
	unsigned char rbuf[R_BUF_LEN];
	unsigned char sbuf[S_BUF_LEN];
	/* 得到服务器的设备描述符，并绑定*/
	serv_sockfd = serv_sockfd_get();
	if (serv_sockfd != -1) {
		ret_val = serv_bind(&serv_sockfd, &serv_addr);
		if (ret_val == -1) {
		printf("bind error!\n");
		return -1;
		}
	}

	printf("rbuff:%p\n", rbuf);
	sprintf(rbuf, "1233444");

	/* 接收和发送 */
	while (1) {
		ret_val = recv_and_reply(serv_sockfd, rbuf, &client_addr);
		printf("recv_and_reply() ret=%d\n", ret_val);
		if (ret_val == 0) {
			// send_vedio_to_client(serv_sockfd, sbuf, &client_addr, start_tm);
		}

		usleep(50 * 1000);
	}
	close(serv_sockfd);	
	return 0;
}


