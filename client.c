/**
 * Copyright (C) 2015-2020  Meari Technology Co., Ltd. All Rights reserved. 
 * 
 * @file    server.c
 * @brief   description here
 * receiver messge
 * @author  kang
 * @date    2017-6-27
 * 
 * @history v1.2.0.0
 */
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

/* socket
 * connect
 * send/recv
 */

#define SERVER_PORT 8888
/*S:发送和R:接收缓存区大小*/
#define S_BUF_LEN		100	
#define R_BUF_LEN		5*1024*1024

/* 得到server的设备描述符 */
static int clent_config(struct sockaddr_in *addr, char *ip) {	
	int ret_val;
	ret_val = socket(AF_INET, SOCK_DGRAM, 0);
	if (ret_val == -1) {
		printf("socket error!\n");
		ret_val = -1;
	} else {
		addr->sin_family      = AF_INET;
		addr->sin_port        = htons(SERVER_PORT);  /* host to net, short */	
		if (0 == inet_aton(ip, &addr->sin_addr)) {
			printf("invalid server_ip\n");
			ret_val = -1;
		}
		memset(addr->sin_zero, 0, 8);	
	}	

	return ret_val;
}

int main(int argc, char **argv)
{
	int client_sockfd;
	struct sockaddr_in serv_addr;
	
	int ret_val;
	unsigned char sbuf[S_BUF_LEN];
	unsigned char rbuf[R_BUF_LEN];
	/* 数据长度，s:发送；r:接收*/
	int slen;
	int rlen;
	/*struct sockaddr_in=sockaddr，地址信息结构体的长度*/
	int addrlen;

	FILE *fp;

	if (argc != 2) {
		printf("Usage:\n");
		printf("%s <server_ip>\n", argv[0]);
		return -1;
	}

	client_sockfd = clent_config(&serv_addr, argv[1]); 	
 	if (client_sockfd == -1) {
 		return -1;
 	}

 	addrlen = sizeof(struct sockaddr);
	while (1) {
		if (fgets(sbuf, S_BUF_LEN-1, stdin)) { /*从输入流中接收字符串，包含最后的回车*/
			sendto(client_sockfd, sbuf, strlen(sbuf), 0, (const struct sockaddr *)&serv_addr, addrlen);
		}
		rlen = recvfrom(client_sockfd, rbuf, R_BUF_LEN-1, 0, (struct sockaddr *)&serv_addr, &addrlen);
		if (rlen > 0) {
			rbuf[rlen] = '\0';
			printf("%s\n", rbuf); /*打印回复信息*/
			ret_val = strncmp("cmd:ok.", rbuf,strlen("cmd:ok."));
			if (ret_val == 0) {
				/* 打开二进制文件，只写*/
				if((fp=fopen("/home/book/work/meari/net_progm/doc/download.h264","wb"))==NULL) {
					printf("文件打开错误\n");
				} else {
					while (ret_val != -1) {
						/*从服务器上接收信息*/
						rlen = recvfrom(client_sockfd, rbuf, R_BUF_LEN-1, 0, (struct sockaddr *)&serv_addr, &addrlen);	
						if (rlen > 0) {
							ret_val = strncmp("filshed", rbuf,strlen("filshed"));
							if (ret_val == 0) {
							rbuf[rlen] = '\0';
							printf("%s\n",rbuf);
							fclose(fp);	
							ret_val = -1;
							} else {
							/*download file*/
							fwrite(rbuf, 1, sizeof(rbuf), fp);
							}
						}
					}
				}
			}
		}				
	}	
	close(client_sockfd);
	return 0;
}

