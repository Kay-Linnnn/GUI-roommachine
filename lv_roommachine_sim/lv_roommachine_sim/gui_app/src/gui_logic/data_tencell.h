#ifndef _DATA_TENCELL_H
#define _DATA_TENCELL_H


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "scr_system_setting.h"
#include "message_mechanism.h"


/*=========================
*******Global Declare******
==========================*/
	enum {
		item_sound_voice,
		item_sound_bell,
		item_sound_touch,
		item_sound_microphone,
		item_sound_type,
		item_sound_max,
	};
	typedef uint8_t datajs_item_sound_t;
/*=======================
********Command**********
========================*/

/*
*****Descr: universal command, for linux, use "system" function to run command
***@Para->command: command
***@Return : 0 if success or -1 otherwise
*/
int datajs_command_universal(const char *command);


/*-----------------------------------
**********Add/Delete Module**********
------------------------------------*/
/*
*****Descr: add system informatio module in system book
*/
int datajs_add_system_information_in_system_book(void);

/*
*****Descr: delete system informatio module in system book
*/
int datajs_del_system_information_in_system_book(void);

/*
*****Descr: add address module in system book
*/
int datajs_add_system_object_address(void);

/*
*****Descr: add sound module in system book
*/
int datajs_add_sound_in_system_book(void);

/*
*****Descr: delete sound module in system book
*/
int datajs_del_sound_in_system_book(void);

/*
*****Descr: add screen protect module in system book
*/
int  datajs_add_screen_protect_in_system_book();

/*
*****Descr: delete screen protect module in system book
*/
int datajs_del_screen_protect_in_system_book();

/*
*****Descr: add general password module in system book
*/
int datajs_add_general_pwd_in_system_book();

/*
*****Descr:s delete general password module in system book
*/
int datajs_del_general_pwd_in_system_book();

/*==================================
******System Information Bolck******
===================================*/
#if 0
/*
******Descr: Init, get all device information datas at first
***@Para->info_addr: address of memory data of system infomation
***@Return : 0 if success or -1 otherwise
*/
int datajs_get_system_init_all(_system_info_type *info_addr);


/*
*****Descr: set value of system infomation item, like sin, mac, ip, etc...,only one each time
***@Para->item_index: index of system information item, system_info_index_sin,ect..., see "system_info_index_t"
***@Para->set_string: string value to set
***@Return : 0 if success or -1 otherwise
*/
int datajs_set_system_item_value(system_info_index_t item_index, const char *set_string);
#endif

/*
******Descr: Init, get all device information datas at first
***@Para->info_addr: address of memory data of system infomation
***@Return : 0 if success or -1 otherwise
*/
int datajs_get_system_info_all_item_value(_system_info_type *info_addr);

/*
*****Descr: set value of system infomation item, like sin, mac, ip, etc... separately
***@Para->item_index: index of system information item, system_info_index_sin,ect..., see "system_info_index_t"
***@Para->set_string: string value to set
***@Return : 0 if success or -1 otherwise
*/
int datajs_set_system_info_item_value_separately(system_info_index_t item_index, const char *set_string);

/*
*****Descr: get ip from address
***@Para->address_data: address memory data
***@Para->ip_addr: address to get ip value
***@Return: 0 if success or -1 otherwise
*/
int datajs_get_address_ip(const _address_set_type *address_data, char *ip_addr);

/*
*****Descr: get ip from address_num, you should turn address to number self
***@Para->addr_num: address number
***@Para->ip_addr: address to get ip value
***@Return: 0 if success or -1 otherwise
*/
int datajs_get_address_ip_by_addrnum(const char *addr_num, char *ip_addr);

/*
*****Descr: get all item value of address object in systemcfg book, such as region,building,unit, ect...
***@Para-> p_addr: pointer to address memory
***@Return: 0 if success or -1 otherwise
*/
int datajs_get_address_all_item_value(_address_set_type *p_addr);

/*
*****Descr: set address all item value in system setting book
*/
int datajs_set_address_all_item_value(const _address_set_type *p_addr);

/*===========Block==============
***********Screen Protect*******
===============================*/
/*
*****Descr: get screen protect all value in system book
*/
int datajs_get_screen_protect_all_item_value(_screen_protect_data_type *screen_protect_address);

/*
*****Descr: set screen protect item value separately
***@Para-> index: time or mode,"screen_protect_index_time"/"screen_protect_index_mode"
***@Prar-> set_value: set_value of intergar type
***@Return: 0 if success or -1 othersise
*/
int datajs_set_screen_protect_item_value_separately(screen_protect_index index, uint8_t set_value);


/*=========Block=============
********Passwod**************
============================*/
/*
*****Descr: Get system pasword from system book
***@Para-> addr_system_pwd: address of memory data of system passwod
***@Return: 0 if success or -1 othersise
*/
int datajs_get_system_password_item_value(_system_password_data_type *addr_system_pwd);

/*
*****Descr: set system password in system book
***@Para-> string value of password want
***@Return: 0 if success or -1 othersise
*/
int datajs_set_system_password_item_value_separately(const char *set_value);

/*
*****Descr: get general pwd in system book
***@Para-> general_pwd: two dimension memory data of general password
***@Return: 0 if success or -1 othersise
*/
int datajs_get_general_password_all_item_value(_general_password_data_type *general_pwd);

/*
*****Descr: set general password item value
***@Para->index: password mode
***@Para->set pwd: string value of pwd to set
***@Return: 0 if success or -1 otherwise
*/
int datajs_set_general_password_item_value_separately(pwd_mode_type index, const char *set_pwd);


/*=========Block=====================
*********Visitor Left****************
===================================*/

/*
*****Descr: for message mechanism,once message come in, tail insert message data information to seleted array of roorjs, if array not exist, then array will create automatically
***@Para-> node: message mechanism node, bring in all useful information like: index,date&time, b_read, type
***@Para-> onwer: visitor or block meesage,see "message_mech_type_visitor"/"message_mech_type_block"
***@Return: 0 if success or -1 otherwise
*/
int datajs_message_mech_array_come_in(const _mesg_mech_node *node, message_mech_type_t owner);

/*
*****@Descr: for message mechanism, delete seleted node information of array in rootjs by index of array, the "index" item of array will reorder automatically
***@Para-> node_index: node index of list, given index of "message_mech_node" is begin with 1, so there do node_index decrese 1 "node_index--" auto
***@Para-> onwer: visitor or block meesage,see "message_mech_type_visitor"/"message_mech_type_block"
***@Return: 0 if success or -1 otherwise
***Note: set "auto_del_array" true if you want to delete array auto when array is empty
*/
int datajs_del_message_mech_array(int node_index, message_mech_type_t owner);

/*
*****Descr: get message mechanism array item by index
***@Para-> node: address to taks array item information
***@Para-> node_index: node index in list
***@Para-> owner: visitor or block meesage,see "message_mech_type_visitor"/"message_mech_type_block"
***@Para-> des_prefix: address to get file_prefix
***Return: 0 if success or -1 otherwise
*/
int datajs_get_message_mech_array_item_by_index(_mesg_mech_node *node, int node_index, message_mech_type_t owner, char *des_prefix);

/*
*****Descr: set message mech node as read
***@Para: node_index: index of node
***Return: 0 if success or -1 otherwise
*/
int datajs_set_message_mech_read_item(message_mech_type_t owner, int node_index);

/*
*****Descr: for message mechanism, get size of seleted array in rootjs
***@Para-> owner:visitor or block meesage,see "message_mech_type_visitor"/"message_mech_type_block"
***Return: array size, -1 if wrong
*/
int datajs_get_message_mech_array_size(message_mech_type_t owner);

/*============Block============
*************Sound*************
==============================*/
/*
*****Descr: Get sound all item value
***@Para->ptr_sound: address of sound data to get value
***@Return: 0 if success or -1 otherwise
*/
int datajs_get_sound_all_item_value(_sound_data_type *ptr_sound);

/*
******Descr: set sound item value separately
***@Para->which: tell which item's value to set
***@Para->value: set value
***Return: 0 if success or -1 otherwise
*/
int datajs_set_sound_item_value_separately(datajs_item_sound_t which, uint8_t value);

/*
*/
int datasys_security_start(int *num_array, int num_length);

/*
*/
int datasys_security_listen(int num);

/*
*/
int datasys_security_unlisten(int num);



#ifdef __cplusplus
}
#endif // __cplusplus


#endif // !_DATA_TENCELL_H
