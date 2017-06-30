/**
 * Copyright (C) 2015-2020  Meari Technology Co., Ltd. All Rights reserved. 
 * 
 * @file    server.c
 * @brief   description here
 * receiver messge
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
#include <stdlib.h>

/* socket
 * connect
 * send/recv
 */

#define SERVER_PORT 544
/*S:发送和R:接收缓存区大小*/
#define BUF_SIZE	1024*1024	

/*存放文件的路径*/
char dir_path[BUF_SIZE]="/home/book/work/meari/net_progm/doc/download.h264";


/* 得到设备描述符 */
static int client_config(struct sockaddr_in *addr, char *ip) 
{	
	int ret;
	ret = socket(AF_INET, SOCK_DGRAM, 0);
	if (ret == -1) {
		printf("client socket error!\n");
	} else {
		addr->sin_family      = AF_INET;
		addr->sin_port        = htons(SERVER_PORT);  /* host to net, short */	
		if (0 == inet_aton(ip, &addr->sin_addr)) {
			printf("invalid server_ip\n");
			ret = -1;
		}
		memset(addr->sin_zero, 0, 8);	
	}	
	return ret;
}


static int recv_and_handle(char* buffer)
{
	int ret;
	
}

int main(int argc, char **argv)
{
	int client_sockfd;
	/*定义一个服务器地址*/
	struct sockaddr_in serv_addr, client_addr;
	
	int ret;
	unsigned int recv_size;
	unsigned char buffer[BUF_SIZE];

	int NetTimeout = 2000;
	

	/*struct sockaddr_in=sockaddr，地址信息结构体的长度*/
	int addrlen =  sizeof(struct sockaddr); 

	char frame_buff[BUF_SIZE];
	FILE *fp;	/*定义一个文件指针，用于存放接收到的文件*/

	int frm_n = 4096;//帧大小
	printf("argc=%d\n", argc);
	if (argc < 2) {
		printf("use message:\n");
		printf("%s <server_ip>\n", argv[0]);
		return -1;
	} else if (argc == 3) {
		frm_n = atoi(argv[2]);
		printf("atoi(argv[2])=%d\n", atoi(argv[2]));
	}

	client_sockfd = client_config(&serv_addr, argv[1]); 	
 	if (client_sockfd == -1) {
 		return -1;
 	}

 	
	printf("client_config ok\n");
 	
	/* 打开二进制文件，只写*/
	if((fp=fopen(dir_path, "wb"))==NULL) {
		printf("文件打开错误\n");
		return -1;
	} 
	printf("fopen file ok\n");

	sendto(client_sockfd, "download", strlen("download"), 0, (const struct sockaddr *)&serv_addr, addrlen);
	printf("开始接收, frm_n=%d\n\n",frm_n);
	setsockopt( client_sockfd, SOL_SOCKET, SO_RCVTIMEO, &NetTimeout, sizeof( int ) );

	for (int i = 0; i < frm_n; ++i) {
		recv_size = recvfrom(client_sockfd, frame_buff, BUF_SIZE-1, 0, (struct sockaddr *)&serv_addr, &addrlen);
		fwrite(frame_buff, recv_size, 1, fp);
	}
	
	fclose(fp);
	printf("fclose ok\n");
	close(client_sockfd);
	return 0;
}

