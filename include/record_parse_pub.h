/**
 * Copyright (C) 2017-2020  Meari Technology Co., Ltd. All Rights reserved. 
 * 
 * @file    record_parse_pub.h
 * @brief   public interface declaration and data structure definiation of record_parse module
 * 
 * @author  charles
 * @date    2017-6-27
 * 
 * @history
 */
 
#ifndef _RECORD_PARSE_PUB_H
#define _RECORD_PARSE_PUB_H
 
#ifdef __plusplus
 extern "C" {
#endif

#define MAX_ABS_PATH_SIZE 4096

typedef struct  tm_data_node {
    time_t time_start;
    time_t time_finish;
    char data_abs_path[MAX_ABS_PATH_SIZE];
    struct  tm_data_node* next;
}tm_data_node_t;


/**
*@func     meari_va_get_fragments_init
*@brief    initialize some data before get time fragments and frame
*
*@param   path[IN] the year directory of data, such as /tmp/xxx/SDaa/056565099/record/2017  
*
*@return   On success, return 0. On Error, return -1.
*
*@author   charles
*date      2017-06-27
*/
extern int record_parse_init(char *path);

/**
*@func     record_parse_print_all_tm_data_list
*@brief    print all the node in time data list
*
*@param    void
*
*@return   void
*
*@author   charles
*date      2017-06-27
*/
extern void record_parse_print_all_tm_data_list();



/**
*@func     record_parse_get_frame_start
*@brief    get frame start
*
*@param   start_tm[IN] start time
*
*@return   On success, return 0. On Error, return -1.
*
*@author   charles
*date      2017-06-27
*/
extern int record_parse_get_frame_start(time_t time_start);


/**
*@func     record_parse_get_one_frame
*@brief    get one frame
*
*@param    buff_len[IN] buff length
*@param    buff[IN] buffer
*@param    frame_len[OUT] store the length of frame
*@param    type[OUT] store the type of frame
*
*@return   On success, return 0. On Error, return -1.
*
*@author   charles
*date      2017-06-27
*/
extern int record_parse_get_one_frame(int buff_len, char* buff, int* frame_len, int *type);


/**
*@func     record_parse_get_frame_stop
*@brief    stop getting frame
*
*@param    void[IN] void
*
*@return   void
*
*@author   charles
*date      2017-06-27
*/
extern void record_parse_get_frame_stop();


/**
*@func    record_parse_deinit
*@brief   free some data allocated by record_parse_init
*
*@param   path[IN] the year directory of data, such as /tmp/xxx/SDaa/056565099/record/2017  
*
*@return   On success, return 0. On Error, return -1.
*
*@author   charles
*date      2017-06-27
*/
extern void record_parse_deinit();

#ifdef __cplusplus
}
#endif
#endif
