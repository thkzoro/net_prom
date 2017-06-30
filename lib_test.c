#include <stdio.h>
#include <string.h>
#include <time.h>
#include "include/record_parse_pub.h"

#define BUF_LEN		5*1024*1024

char PTAH[200]="./doc/SD1329/056566767/record/2017";


int main(int argc, char const *argv[])
{
	int ret_val;
	time_t start_tm = 1483200000;//时间戳，视频开始时间
	int frame_len;
	int type;

	unsigned char buff[BUF_LEN];
				
	/*根据录像文件路径，初始化视频片段*/
	ret_val = record_parse_init(PTAH);
	printf("record_parse_init:ret=%d\n", ret_val);
	if (ret_val != 0) return ret_val;

	/* 得到片段列表 */
	record_parse_print_all_tm_data_list();
	/* 得到一帧的开始时间 */
	ret_val = record_parse_get_frame_start(start_tm);
	printf("record_parse_get_frame_start:ret=%d\n", ret_val);
	/* 获取帧*/
	ret_val = record_parse_get_one_frame(BUF_LEN, buff, &frame_len, &type);
	printf("record_parse_get_one_frame:ret=%d\n", ret_val);
	if (ret_val != 0) return ret_val;
	printf("buff len=%d\n", frame_len);
	record_parse_get_frame_stop();
	record_parse_deinit();
		
	return 0;
}