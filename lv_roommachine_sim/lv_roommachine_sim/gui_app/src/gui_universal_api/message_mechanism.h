#ifndef _MESSAGE_MECHANISM_H
#define _MESSAGE_MECHANISM_H


#ifdef _cplusplus
extern "C" {
#endif // _cplusplus

#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"

/*====================================================
*******Message Information Display Mechanism**********
=====================================================*/
/*Prefix: mesg_display_mechanism_
*/
enum {
	message_mech_type_visitor,
	message_mech_type_block,
	};
typedef uint8_t message_mech_type_t;
#if 0
typedef struct {
	uint16_t year;
	int8_t month;
	int8_t day;
}_ui_date_t;

typedef struct {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}_ui_time_t;

typedef struct {
	_ui_date_t date;
	_ui_time_t time;
}_ui_date_time_t;
#endif
/*version 1*/
typedef struct {
	char date[11];  //date:2021-01-01
	char time[9];  //time:09:03:12
}_ui_display_time_t;

enum {
	visitor_file_type_picture = 0x01,
	visitor_file_type_audio = 0x10,
	visitor_file_type_picture_and_audio = 0x11,
};
typedef uint8_t message_file_type;

typedef uint8_t visitor_file_type_t;
typedef uint8_t block_file_type_t;



//#define MESG_PREFIX_LEN		20
typedef struct mesg_mech_node {
	uint16_t index;
	bool b_read;
	//_ui_date_time_t _time;
	_ui_display_time_t _time;
	message_file_type type;  //picture,audio
	//char file_prefix[MESG_PREFIX_LEN];
	char *file_prefix;
	struct mesg_mech_node *next;
	struct mesg_mech_node *pre;
}_mesg_mech_node;

typedef struct {
	uint8_t page_cnt;
	_mesg_mech_node *head;
	_mesg_mech_node *tail;
}_mesg_mech_list_t;

enum {
	mesg_display_mech_page_cmd_next,
	mesg_display_mech_page_cmd_pre,
};
typedef uint8_t mesg_display_mech_page_cmd_t;

_mesg_mech_list_t *message_box_list_get(message_mech_type_t type);
#if 0
/*
******Supply new node memory for storing date_time and put in node to mesg_list tail
***@Para:  date_time: date and time, if NULL all date_time will set to 0
***@mesg_list: list
*/
void mesg_display_mech_list_in(_ui_date_time_t *date_time, _mesg_mech_list_t *mesg_list);
#endif

/*
******Delete node of list
***@Para->del_node: node to be deleted
***@Para-> mesg_list: list
***@Para-> type: visitor or block
*/
void mesg_mech_list_del_node(_mesg_mech_node *del_node, _mesg_mech_list_t *mesg_list, message_mech_type_t type);

/*
******Get the head node of current page,useful for display different page
***@Para-> mesg_list: list
***Return: head node of cureent page
*/
_mesg_mech_node *mesg_mech_list_get_page_head_node(_mesg_mech_list_t *mesg_list);

_mesg_mech_node *mesg_mech_list_get_page_reverse_head_node(_mesg_mech_list_t *mesg_list);

/*
*****Turn display page of list
***@Para-> page_cmd: "mesg_display_mech_page_cmd_pre" for previous page and "mesg_display_mech_page_cmd_next" for next page
***@Para-> mesg_list: list
***@Return: true if page turn correctly
*/
bool mesg_mech_list_page_turn(mesg_display_mech_page_cmd_t page_cmd, _mesg_mech_list_t *mesg_list);
#if 0
/*
********Message Come In
***@Para-> date_time: date and time
***@Para-> mesg_type: visitor message or block, message_mech_type_visitor/message_mech_type_block
***@Para-> file_type: for visirot message, the file can be picture or audio:visitor_file_type_picture, visitor_file_type_audio
***@Para-> data: data stream
***@Para-> data_len: data length
*/
bool mesg_mech_receive_in(_ui_date_time_t *date_time, message_mech_type_t mesg_type, message_file_type file_type, uint8_t *data, uint32_t data_len);

/*
********Message Come In
***@Para-> date_time: date and time
***@Para-> mesg_type: visitor message or block, message_mech_type_visitor/message_mech_type_block
***@Para-> audio_data: audio data stream
***@Para-> audio_data_len: data length
***@Para-> picture_data: picture data stream
***@Para-> picture_data_len: picture data length
***Return: true if success or false otherwise
*/
bool mesg_mech_receive_come_in(_ui_date_time_t *date_time, message_mech_type_t mesg_type, uint8_t *audio_data, uint32_t audio_data_len, uint8_t *picture_data, uint32_t picture_data_len);
#endif

/*
*****Descr: receive message
***@Para->file_path: full_path of file
***@Return: 0 success or -1 otherwise
*/
int mesg_mech_receive_path_come_in(const char *file_path);

/*
*****Descr: retrieve all list data from json file when power on init
*/
void mesg_mech_retrieve_list_data(message_mech_type_t mesg_type);

/*
*****Descr:set mesg node as read
***@Para-> node
***@Para-> mesg_type
*/
void mesg_mech_set_node_read(_mesg_mech_node *node, message_mech_type_t mesg_type);

#ifdef _cplusplus
}
#endif // _cplusplus

#endif // !_MESSAGE_MECHANISM_H
