#include "data_tencell.h"

#include "fnc_DataJsonAnalytic.h"

#include "scr_system_setting.h"
//#include "message_mechanism.h"

#ifndef _VS_SIMULATOR
#include <unistd.h>

#endif // !_VS_SIMULATOR
#include <fcntl.h>


/*Path Declare*/
static const char *path_str_addressbook = "ui_data/addressbook.json";
static const char *path_str_system_setting = "ui_data/systemcfg.json";
static const char *path_str_guestbook = "ui_data/guestbook.json";

/*Item String Declare*/
//system information
#if 0
static const char itme_str_name_system_info[][50] = {"local_machine_id",
											"machine_hw_ether",
											"machine_IP",
											"machine_submask",
											"machine_gateway",
											"server_ip",
											"software_version"};
#endif

#define item_num_system_info	8
static const char *object_name_system_info_of_systemcfg = "system information";

static const char * item_name_of_system_info_object[] = { "host_address",
																"server_ip",
																"machine_hw_ether",																
																"software_version"
																"machine_ip",
																"machine_submask",
																"machine_gateway",
																"machine_sin"};

//address
static const char *array_name_address_book = "addressinfo";

static const char *item_str_name_address_addrnum = "addressnum";

static const char *object_name_address_of_systemcfg = "address";
static const char * item_str_name_address[] = {"region",
													"building",
													"unit",
													"floor",
													"room",
													"branch"};

//sound
#define item_num_sound		5
static const char *object_name_sound_of_systemcfg = "sound";

static const char * item_name_of_sound_object[] = { "voice_value",
													"bell_value",
													"touch_value",
													"microphone_value",
													"ring_type" };


//screen protect
#define item_num_screen_protect		2
static const char *object_name_sceen_protect_of_systemcfg = "screen protect";

static const char * item_name_of_screen_protect_object[] = { "time","mode" };

//system password
#define item_num_system_pwd		1
static const char * item_name_of_system_pwd_object[] = { "admin_password" };

//general password
#define item_num_general_pwd	1
static const char *object_name_general_pwd_of_systemcfg = "general password";

static const char * item_name_of_general_pwd_object[] = {"security"};

//message mechanism
#define item_num_mesg_mech		6

static const char *array_name_of_visitor = "guestbookinfo";

static const char * array_item_name_of_mesg_mech[] = { "index","date","time","b_read","type","file_prefix" };
enum {
	array_item_msg_index,
	array_item_msg_date,
	array_item_msg_time,
	array_item_msg_b_read,
	array_item_msg_type,
	array_item_meg_file_prefix,
};


#define ExecFunction_int(func, _ret_) \
do{\
	int int_TempRet = 0;	\
	int_TempRet = func;		\
	if(int_TempRet != _ret_) \
	{\
		return -1;\
	}\
} while (0);

#define is_system_info_index(index)		(0<= index < system_info_max)

/*Format:
***Prefix: datajs_
***Get data: datajs_get
***Set json local file: datajs_set
***System Command: datajs_command_
*/

/*=======================
********Command**********
========================*/
/*
*****Descr: universal command, for linux, use "system" function to run command
***@Para->command: command
*/
int datajs_command_universal(const char *command)
{
	if (command == NULL) return -1;

#ifndef _VS_SIMULATOR
	system(command);
#else
	ui_printf_plus("datajs command:%s", command);
#endif // !_VS_SIMULATOR

	return 0;
}

/*=============================
******SYSTEM SETTING DATA******
==============================*/
/*Format:
***Prefix:datajs_***_system
*/

/*----------------------------
**********Add Module**********
-----------------------------*/
/*
*****Descr: add system informatio module in system book
*/
int datajs_add_system_information_in_system_book()
{
	const char *info_item_value[] = { "01区01栋01单元0001房01分机",
										"192.168.1.108",
										"00:70:27:00:10:01",
										"1.0.6",
										"192.168.24.18",
										"255.255.0.0",
										"192.168.1.1",										
										"50010011010201"
												 };

	ExecFunction_int(fncjson_AddObject(path_str_system_setting, object_name_system_info_of_systemcfg, item_name_of_system_info_object, info_item_value, item_num_system_info),
		0);

	return 0;
}

/*
*****Descr: delete system informatio module in system book
*/
int datajs_del_system_information_in_system_book()
{
	ExecFunction_int(fncjson_DetachObjectItem(path_str_system_setting, object_name_system_info_of_systemcfg),
		0);

	return 0;
}

/*
*****Descr: add address module in system book
*/
int datajs_add_system_object_address()
{
	//if (addr_data == NULL) return -1;

	char object_name[] = "address";
	const char *address_item_name[] = {"region",
										"building",
										"unit",
										"floor",
										"room",
										"branch"};

	const char *address_item_value[] = {"01",
												"001",
												"1",
												"01",
												"01",
												"01"};
	
	fncjson_AddObject(path_str_system_setting, object_name, address_item_name, address_item_value, 6);

	return 0;
}

/*
*****Descr: add sound module in system book
*/
int datajs_add_sound_in_system_book()
{
	//char object_name[] = "sound";
	//int item_num = 4;
#if 0
	const char * const sound_item_nam[] = { "voice_value",
											"bell_value",
											"microphone_value",
											"ring_type" };
#endif
	const char *sound_item_value[] = { "10","15","2","15","1" };

	ExecFunction_int(fncjson_AddObject(path_str_system_setting, object_name_sound_of_systemcfg, item_name_of_sound_object, sound_item_value, item_num_sound),
		0);

	return 0;
}

/*
*****Descr: delete sound module in system book
*/
int datajs_del_sound_in_system_book()
{
	//char object_name[] = "sound";

	ExecFunction_int(fncjson_DetachObjectItem(path_str_system_setting, object_name_sound_of_systemcfg),
		0);

	return 0;
}

/*
*****Descr: add screen protect module in system book
*/
int  datajs_add_screen_protect_in_system_book()
{
	int item_num = 2;
	
	const char * screen_protect_item_value[] = { "1","1" };

	ExecFunction_int(fncjson_AddObject(path_str_system_setting, object_name_sceen_protect_of_systemcfg, item_name_of_screen_protect_object, screen_protect_item_value, item_num),
		0);

	return 0;
}

/*
*****Descr: delete screen protect module in system book
*/
int datajs_del_screen_protect_in_system_book()
{
	ExecFunction_int(fncjson_DetachObjectItem(path_str_system_setting, object_name_sceen_protect_of_systemcfg),
		0);

	return 0;
}

/*
*****Descr: add general password module in system book
*/
int datajs_add_general_pwd_in_system_book()
{
	const char * general_pwd_item_value[] = { "888888" };

	ExecFunction_int(fncjson_AddObject(path_str_system_setting, object_name_general_pwd_of_systemcfg, item_name_of_general_pwd_object, general_pwd_item_value, item_num_general_pwd),
		0);

	return 0;
}

/*
*****Descr:s delete general password module in system book
*/
int datajs_del_general_pwd_in_system_book()
{
	ExecFunction_int(fncjson_DetachObjectItem(path_str_system_setting, object_name_general_pwd_of_systemcfg),
		0);

	return 0;
}


/*------------Block-------------
**********Device Infomation*****
-------------------------------*/
#if 0
/*
******Descr: Init, get all device information datas at first
***@Para->info_addr: address of memory data of system infomation
*/
int datajs_get_system_init_all(_system_info_type *info_addr)
{
	if (info_addr == NULL) return -1;

	int out_str_num = system_info_index_max;
	//char f_name_str_tab[][50] = { "admin_password","security_password","CueTone_volume","machine_IP","machine_hw_ether" };
	char out_str[7][50] = { 0 };
	
	ExecFunction_int(fncjson_GetObjectItem_multistrValue(path_str_system_setting, itme_str_name_system_info, out_str, out_str_num), 
						0);

	memcpy(info_addr, out_str, sizeof(_system_info_type));

	return 0;
}


/*
*****Descr: set value of system infomation item, like sin, mac, ip, etc...,only one each time
***@Para->item_index: index of system information item, system_info_index_sin,ect..., see "system_info_index_t"
***@Para->set_string: string value to set
*/
int datajs_set_system_item_value(system_info_index_t item_index,const char *set_string)
{
	if (set_string == NULL) return -1;

	ExecFunction_int(fncjson_SetArrayString_By_ObjectItem(path_str_system_setting, NULL, itme_str_name_system_info[item_index], set_string),
						0);
	return 0;
}
#endif

int datajs_get_system_info_all_item_value(_system_info_type *info_addr)
{
	if (is_null_ptr(info_addr)) return -1;

	ui_printf_plus("[datajs_get_system_info_all_item_value]");

	char out_str[item_num_system_info][50] = { 0 };

	ExecFunction_int(fncjson_GetObjectItem_StringValue(path_str_system_setting, object_name_system_info_of_systemcfg, item_name_of_system_info_object, out_str, item_num_system_info),
		0);

	memcpy(info_addr, out_str, sizeof(_system_info_type));

	return 0;
}

int datajs_set_system_info_item_value_separately(system_info_index_t item_index, const char *set_string)
{
	if (set_string == NULL) return -1;

	int item_num = 1;

	const char * item_set_value[] = { set_string };

	const char * item_name[] = { item_name_of_system_info_object[item_index] };

	ExecFunction_int(fncjson_SetStringValue_by_object(path_str_system_setting, object_name_system_info_of_systemcfg, item_name, item_set_value, item_num),
		0);

	return 0;
}

/*=========BLOCK================
********Address Data***********
=============================*/
/*
*****Desc: turn address to addressnum: 01 region,001 building,01 unit,01 floor,01 room, 01 branch ->50010011010101
*/
static int address_get_addressnum_by_desc(const _address_set_type * desc, char *addrnum)
{
	if (desc == NULL || addrnum == NULL)
		return -1;

	//to do
	//is there any need to check whether "desc" is valid or not?

	char room_machine_tag[] = "50";
	sprintf(addrnum, "%s%2s%3s%1s%2s%2s%2s", room_machine_tag, desc->region, desc->building, desc->unit, desc->floor, desc->room, desc->branch);
	//sprintf(addrnum, "%s%s%s%s%s%s%s", room_machine_tag, desc->region, desc->building, desc->unit, desc->floor, desc->room, desc->branch);
	return 0;
}
/*Format:
***Prefix:datajs_***_address_
*/

/*
*****Descr: get ip from address
***@Para->address_data: address memory data
***@Para->ip_addr: address to get ip value
***@Return: 0 if success or -1 otherwise
*/
int datajs_get_address_ip(const _address_set_type *address_data, char *ip_addr)
{
	char str_adddr_num[30] = { 0 };
	char out_str[3][50] = { 0 };

	ExecFunction_int(address_get_addressnum_by_desc(address_data, str_adddr_num),
						0);
	ui_printf_plus("Got addrnum:%s", str_adddr_num);

	ExecFunction_int(fncjson_GetArrayChildObjectItem(path_str_addressbook, array_name_address_book, item_str_name_address_addrnum, str_adddr_num, out_str),
						0);

	strcpy(ip_addr, out_str[1]);// the second of out_str is ip

	return 0;
}

/*
*****Descr: get ip from address_num, you should turn address to number self
***@Para->addr_num: address number
***@Para->ip_addr: address to get ip value
***@Return: 0 if success or -1 otherwise
*/
int datajs_get_address_ip_by_addrnum(const char *addr_num, char *ip_addr)
{
	char out_str[3][50] = { 0 };

	ExecFunction_int(fncjson_GetArrayChildObjectItem(path_str_addressbook, array_name_address_book, item_str_name_address_addrnum, addr_num, out_str),
		0);

	strcpy(ip_addr, out_str[1]);// the second of out_str is ip

	return 0;
}

/*
*****Descr: get all item value of address object in systemcfg book, such as region,building,unit, ect...
***@Para-> p_addr: pointer to address memory
***@Return: 0 if success or -1 otherwise
*/
int datajs_get_address_all_item_value(_address_set_type *p_addr)
{
	int out_str_num = 6;
	
	char out_str[6][50] = { 0 };
#if 0
	out_str[0][0] = '0';
	out_str[1][0] = '1';
	out_str[2][0] = '2';
	out_str[3][0] = '3';
	out_str[4][0] = '4';
#endif
	ui_printf_plus("[datajs_get_address_all_item_value]");
	ExecFunction_int(fncjson_GetObjectItem_StringValue(path_str_system_setting, object_name_address_of_systemcfg, item_str_name_address, out_str, out_str_num),
						0);
#if 1
	int i = 0;
	for (i = 0; i < 6; i++)//p_addr is 5 bytes each item, while out_str is 50 bytes
	{
		strcpy(p_addr->region + i * sizeof(char) * 5, out_str[i]);
		//ui_printf_plus("out_str:%s", out_str[i]);
	}
#endif
	return 0;
}

/*
*****Descr: set address all item value in system setting book
*/
int datajs_set_address_all_item_value(const _address_set_type *p_addr)
{
	if (is_null_ptr(p_addr)) return -1;

	const char * set_value[] = { p_addr->region,p_addr->building,p_addr->unit,p_addr->floor,p_addr->room,p_addr->branch };
	
	ExecFunction_int(fncjson_SetStringValue_by_object(path_str_system_setting, object_name_address_of_systemcfg, item_str_name_address, set_value, AddrSet_index_max),
						0);

	return 0;
}

/*===========Block==============
***********Screen Protect*******
===============================*/
/*
*****Descr: get screen protect all value in system book
*/
int datajs_get_screen_protect_all_item_value(_screen_protect_data_type *screen_protect_address)
{
	if (is_null_ptr(screen_protect_address)) return -1;

	ui_printf_plus("[datajs_get_screen_protect_all_item_value]");
	char out_str[2][50] = { 0 };

	ExecFunction_int(fncjson_GetObjectItem_StringValue(path_str_system_setting, object_name_sceen_protect_of_systemcfg, item_name_of_screen_protect_object, out_str, item_num_screen_protect),
		0);
#if 0
	ui_printf_plus("strlen(out_str[0]):%d",strlen(out_str[0]));
	ui_printf_plus("strlen(out_str[1]):%d", strlen(out_str[1]));
	ui_printf_plus("1th:%d,2th:%d", out_str[0][0], out_str[0][1]);
	ui_printf_plus("1th:%d,2th:%d", out_str[1][0], out_str[1][1]);
#endif
	screen_protect_address->time = atoi(out_str[0]);
	screen_protect_address->mode = atoi(out_str[1]);

	return 0;
}

/*
*****Descr: set screen protect item value separately
***@Para-> index: time or mode,"screen_protect_index_time"/"screen_protect_index_mode"
***@Prar-> set_value: set_value of intergar type
***@Return: 0 if success or -1 othersise
*/
int datajs_set_screen_protect_item_value_separately(screen_protect_index index, uint8_t set_value)
{
	int item_num = 1;
	char str_value[5] = { 0 };
	//_itoa(set_value, str_value, 10); //turn int value to string
	sprintf(str_value, "%d", set_value);

	const char *const item_set_value[] = {str_value};

	const char *const item_name[] = { item_name_of_screen_protect_object[index] }; //take out the name of item

	ExecFunction_int(fncjson_SetStringValue_by_object(path_str_system_setting, object_name_sceen_protect_of_systemcfg, item_name, item_set_value, item_num),
		0);

	return 0;
}

/*=========Block=============
********Passwod**************
============================*/
/*
*****Descr: Get system pasword from system book
***@Para-> addr_system_pwd: address of memory data of system passwod
***@Return: 0 if success or -1 othersise
*/
int datajs_get_system_password_item_value(_system_password_data_type *addr_system_pwd)
{
	if (is_null_ptr(addr_system_pwd)) return -1;

	ui_printf_plus("[datajs_get_system_password_item_value]");

	char out_str[1][50] = { 0 };
	
	ExecFunction_int(fncjson_GetObjectItem_StringValue(path_str_system_setting, NULL, item_name_of_system_pwd_object, out_str, item_num_system_pwd),
		0);

	strcpy(addr_system_pwd->pwd,out_str[0]);
	ui_printf_plus("system pwd:%s", addr_system_pwd->pwd);

	return 0;
}

/*
*****Descr: set system password in system book
***Note: cause system pws is item of rootfs, and has only one item, so group "item_name_of_system_pwd_object" used as separate item, cause it only stores one item
***@Para-> string value of password want
***@Return: 0 if success or -1 othersise
*/
int datajs_set_system_password_item_value_separately(const char *set_value)
{
	if (is_null_ptr(set_value)) return -1;

	const char * item_set_value[] = { set_value };

	ExecFunction_int(fncjson_SetStringValue_by_object(path_str_system_setting, NULL, item_name_of_system_pwd_object, item_set_value, item_num_system_pwd),
		0);

	return 0;
}

/*
*****Descr: get general pwd in system book
***@Para-> general_pwd: two dimension memory data of general password
***@Return: 0 if success or -1 othersise
*/
//int datajs_get_general_password_all_item_value(_general_password_data_type (*general_pwd)[PASSWORD_MAX_LEN])
int datajs_get_general_password_all_item_value(_general_password_data_type *general_pwd)
{
	if (is_null_ptr(general_pwd)) return -1;

	ui_printf_plus("[datajs_get_general_password_all_item_value]");
	char out_str[1][50] = { 0 };

	ExecFunction_int(fncjson_GetObjectItem_StringValue(path_str_system_setting, object_name_general_pwd_of_systemcfg, item_name_of_general_pwd_object, out_str, item_num_general_pwd),
		0);

	uint8_t i = 0;
	for (i = 0; i < item_num_general_pwd; i++)
	{
		//strcpy(general_pwd[i], out_str[i]);
		strcpy(general_pwd->pwd[i], out_str[i]);
	}

	return 0;
}

/*
*****Descr: set general password item value
***@Para->index: password mode
***@Para->set pwd: string value of pwd to set
***@Return: 0 if success or -1 otherwise
*/
int datajs_set_general_password_item_value_separately(pwd_mode_type index, const char *set_pwd)
{
	if (is_null_ptr(set_pwd)) return -1;

	const char *const item_set_value[] = { set_pwd };

	const char *const item_name[] = { item_name_of_general_pwd_object[index] }; //take out the name of item

	ExecFunction_int(fncjson_SetStringValue_by_object(path_str_system_setting, object_name_general_pwd_of_systemcfg, item_name, item_set_value, 1),
		0);

	return 0;
}

/*=========Block=====================
*********Visitor Left****************
===================================*/
typedef struct {
	char index[10];
	char date[20];
	char time[15];
	char b_read[10];
	char type[10];
	char file_prefix[25];
}_str_mesg_mech_node_t;

static void misc_turn_mesg_mech_node_2_strValue(_str_mesg_mech_node_t *des,const _mesg_mech_node *node)
{
	if(des == NULL) return;
	memset(des, 0, sizeof(_str_mesg_mech_node_t));

	sprintf(des->index, "%d", (int)node->index);
	//sprintf(des->date, "%04d-%02d-%02d", node->_time.date.year, node->_time.date.month, node->_time.date.day);
	//sprintf(des->time, "%02d:%02d:%02d", node->_time.time.hour, node->_time.time.minute, node->_time.time.second);
	strcpy(des->date,node->_time.date);
	strcpy(des->time, node->_time.time);
	sprintf(des->b_read, "%d", (int)node->b_read);
	sprintf(des->type, "%d", (int)node->type);
	strcpy(des->file_prefix, node->file_prefix);
}

/*
*****Descr: for message mechanism,once message come in, tail insert message data information to seleted array of roorjs, if array not exist, then array will create automatically
***@Para-> node: message mechanism node, bring in all useful information like: index,date&time, b_read, type
***@Para-> onwer: visitor or block meesage,see "message_mech_type_visitor"/"message_mech_type_block"
***@Return: 0 if success or -1 otherwise
*/
int datajs_message_mech_array_come_in(const _mesg_mech_node *node, message_mech_type_t owner)
{
	if (node == NULL) return -1;

	char *path = NULL;
	_str_mesg_mech_node_t node_str_value;
	
	misc_turn_mesg_mech_node_2_strValue(&node_str_value, node);
#if 1
	const char * item_value[] = { node_str_value.index,
										node_str_value.date,
										node_str_value.time,
										node_str_value.b_read, 
										node_str_value.type,
										node_str_value.file_prefix};

#if 0
	if (owner == message_mech_type_visitor)
		path = path_str_guestbook;
	else if (owner == message_mech_type_block)
		path = NULL;
	else
		return -1;
#endif
	if (owner == message_mech_type_visitor) {
		ExecFunction_int(fncjson_AddItem_IntoArray_OfRootjs(path_str_guestbook, array_name_of_visitor, array_item_name_of_mesg_mech, item_value, item_num_mesg_mech),
			0);
	}
	else if (owner == message_mech_type_block) {
		ExecFunction_int(fncjson_AddItem_IntoArray_OfRootjs(NULL, NULL, array_item_name_of_mesg_mech, item_value, item_num_mesg_mech),
			0);
	}
	else
		return -1;

	return 0;
#endif
}

/*
*****@Descr: for message mechanism, delete seleted node information of array in rootjs by index of array, the "index" item of array will reorder automatically
***@Para-> node_index: node index of list, given index of "message_mech_node" is begin with 1, so there do node_index decrese 1 "node_index--" auto
***@Para-> onwer: visitor or block meesage,see "message_mech_type_visitor"/"message_mech_type_block"
***@Return: 0 if success or -1 otherwise
***Note: set "auto_del_array" true if you want to delete array auto when array is empty
*/
int datajs_del_message_mech_array(int node_index, message_mech_type_t owner)
{
	node_index--;
	bool auto_del_array = false;
	const char *change_item_name = array_item_name_of_mesg_mech[0]; //index
	
	if (owner == message_mech_type_visitor) {
		ExecFunction_int(fncjson_DeleteItem_InArray_OfRootjs_by_ArrayIndex_AutoReorder(path_str_guestbook, array_name_of_visitor, change_item_name, node_index, auto_del_array),
			0);
	}
	else if (owner == message_mech_type_block) {
		ExecFunction_int(fncjson_DeleteItem_InArray_OfRootjs_by_ArrayIndex_AutoReorder(NULL, NULL, change_item_name, node_index, auto_del_array),
			0);
	}
	else
		return -1;

	return 0;
}
#if 0
//2021-01-01 to 2021 01 01
static void datajs_message_mech_date_2_value(const char *date, _ui_date_t *des_value)
{
	char str_year[5] = { 0 }, str_month[3] = { 0 }, str_day[3] = { 0 };

	memcpy(str_year, date + 0, sizeof(char) * 4);
	//str_year[4] = '\0'; //question: memcpy only copy the before 4 characters, will "memcpy" overshadow the "\0" end of char variable which supposed to be the first character?
	memcpy(str_month, date + sizeof(str_year), sizeof(char) * 2);
	memcpy(str_day, date + sizeof(str_year) + sizeof(str_month), sizeof(char) * 2);

	//ui_printf_plus("strlen(str_year):%d", strlen(str_year));
	//ui_printf_plus("str_year[4]:%d", str_year[4]);

	des_value->year = atoi(str_year);
	des_value->month = atoi(str_month);
	des_value->day = atoi(str_day);
}

//10:12:01 to 10 12 01
static void datajs_message_mech_time_2_value(const char *time, _ui_time_t *des_value)
{
	char str_hour[3] = { 0 }, str_minute[3] = { 0 }, str_second[3] = { 0 };

	memcpy(str_hour, time + 0, sizeof(char) * 2);
	memcpy(str_minute, time + sizeof(str_hour), sizeof(char) * 2);
	memcpy(str_second, time + sizeof(str_second), sizeof(char) * 2);

	des_value->hour = atoi(str_hour);
	des_value->minute = atoi(str_minute);
	des_value->second = atoi(str_second);
}
#endif
/*
*****Descr: get message mechanism array item by index
***@Para-> node: address to taks array item information
***@Para-> node_index: node index in list
***@Para-> owner: visitor or block meesage,see "message_mech_type_visitor"/"message_mech_type_block"
***@Para-> des_prefix: address to get file_prefix
***Return: 0 if success or -1 otherwise
*/
#if 1
int datajs_get_message_mech_array_item_by_index(_mesg_mech_node *node, int node_index, message_mech_type_t owner, char *des_prefix)
{
	node_index--;
	char out_str[item_num_mesg_mech][50] = { 0 };
	
	if (owner == message_mech_type_visitor) {
		ExecFunction_int(fncjson_GetItemGroup_InArray_OfRootjs_by_ArrayIndex(path_str_guestbook, array_name_of_visitor, array_item_name_of_mesg_mech, out_str, item_num_mesg_mech, node_index),
			0);
	}
	else
	{
		return -1;
	}	

	//datajs_message_mech_date_2_value(out_str[array_item_msg_date], &node->_time.date); //date
	//datajs_message_mech_time_2_value(out_str[array_item_msg_time], &node->_time.time);  //time
	strcpy(node->_time.date, out_str[array_item_msg_date]);//get date
	strcpy(node->_time.time, out_str[array_item_msg_time]);//get time
	node->b_read = (bool)atoi(out_str[array_item_msg_b_read]);
	node->type = (message_file_type)atoi(out_str[array_item_msg_type]);
	strcpy(des_prefix, out_str[array_item_meg_file_prefix]);

	return 0;
}
#endif

/*
*****Descr: for message mechanism, get size of seleted array in rootjs
***@Para-> owner:visitor or block meesage,see "message_mech_type_visitor"/"message_mech_type_block"
***Return: array size, -1 if wrong
*/
int datajs_get_message_mech_array_size(message_mech_type_t owner)
{
	if (owner == message_mech_type_visitor) {
		return fncjson_get_array_size(path_str_guestbook, array_name_of_visitor);
	}
	else
	{
		return -1;
	}

	return -1;	
}

/*
*****Descr: set message mech node as read
***@Para: node_index: index of node
***Return: 0 if success or -1 otherwise
*/
int datajs_set_message_mech_read_item(message_mech_type_t owner, int node_index)
{
	node_index--;

	if (owner == message_mech_type_visitor) 
	{
		const char * const item_name[] = { array_item_name_of_mesg_mech[array_item_msg_b_read] };
		const char * const item_value[] = { "1" };//means true
		
		ExecFunction_int(fncjson_SetStringValue_InArrayItem__OfRootjs_byArrayIndex(path_str_guestbook, array_name_of_visitor, item_name, item_value, 1, node_index),
			0);
		return 0;
	}
	else if (owner == message_mech_type_block)
	{
		return -1;
	}
	return -1;
}

/*============Block============
***********Dial & CCTV ********
=============================*/
//just use datajs_get_address_ip is OK


/*============Block============
*************Sound*************
==============================*/
/*
*****Descr: Get sound all item value 
***@Para->ptr_sound: address of sound data to get value 
***@Return: 0 if success or -1 otherwise
*/
int datajs_get_sound_all_item_value(_sound_data_type *ptr_sound)
{
	if (ptr_sound == NULL) return -1;

	ui_printf_plus("[datajs_get_sound_all_item_value]");
	char out_str[item_num_sound][50] = { 0 };

	ExecFunction_int(fncjson_GetObjectItem_StringValue(path_str_system_setting, object_name_sound_of_systemcfg, item_name_of_sound_object, out_str, item_num_sound),
		0);

	ptr_sound->ring_choosen = atoi(out_str[item_sound_type]);
	ptr_sound->volume_m.vol_voice = atoi(out_str[item_sound_voice]);
	ptr_sound->volume_m.vol_bell = atoi(out_str[item_sound_bell]);
	ptr_sound->volume_m.vol_touch = atoi(out_str[item_sound_touch]);
	ptr_sound->volume_m.vol_microphone = atoi(out_str[item_sound_microphone]);

	return 0;
}

/*
******Descr: set sound item value separately
***@Para->which: tell which item's value to set
***@Para->value: set value
***Return: 0 if success or -1 otherwise
*/
int datajs_set_sound_item_value_separately(datajs_item_sound_t which, uint8_t value)
{
	if (which >= item_sound_max) return -1;

	char str_value[5] = { 0 };	
	sprintf(str_value, "%d", (int)value);

	const char * const item_name[] = { item_name_of_sound_object[which] };
	const char * const item_value[] = { str_value };

	ExecFunction_int(fncjson_SetStringValue_by_object(path_str_system_setting, object_name_sound_of_systemcfg, item_name, item_value, 1),
		0);

	return 0;
}

/*===================
********GPIO*********
=====================*/
#ifndef _VS_SIMULATOR



static int fs_gpio_request(int volatile num)
{
    int gpiofd = -1;
    char buf[3];
    sprintf(buf, "%d", num);

    gpiofd = open("/sys/class/gpio/export", O_WRONLY);

    if(gpiofd == -1)
    {
        // fail to open gpio device
        ui_printf_plus("fail to device!");
        return -1;
    }

    ui_printf_plus("open success!");

    write(gpiofd, buf, sizeof(buf));
    ui_printf_plus("gpio_request success,num = %d", num);

    close(gpiofd);
    return 0;
}

static int fs_gpio_set_direct(int volatile num, char* direct)
{
	int fd = -1;
    char path[60];
    sprintf(path, "/sys/class/gpio/gpio%d/direction", num);
    ui_printf_plus("path = %s", path);

    fd = open(path, O_WRONLY);

    if(fd == -1)
    {
        // fail to open watchdog device
        ui_printf_plus("fail to device!\n");
        return -1;
    }

    write(fd, direct, sizeof(direct));
    ui_printf_plus("fs_gpio_set_direct is %s\n", direct);

    close(fd);
    return 0;
}

static int fs_gpio_get_value(int volatile num)
{
	FILE* file = NULL;
    char buf[10] = {0};
    char path[50] = {0};
    sprintf(path, "/sys/class/gpio/gpio%d/value", num);

	if(access(path, F_OK) == -1 || access("/sys/class/gpio/gpio12", F_OK) == -1)
	{
		ui_printf_plus("path:%s not exist", path);
		return -1;
	}

    file = fopen(path, "rb");

    if(file == NULL)
    {
        // fail to open watchdog device
        ui_printf_plus("fs_gpio_get_value: fail to open!");
        return -1;
    }

    fseek(file, 0, SEEK_END); //文件指针转到文件末尾
    int len = ftell(file);
    rewind(file);

    fread(buf, 1, len, file);
    //printf("fs_gpio_get_value:gpio_value is %s\n", buf);

    fclose(file);

    return atoi(buf);
	
}

static int fs_gpio_cancel(int volatile num)
{
	int gpiofd = -1;
	char buf[3] = {0};
	sprintf(buf, "%d", num);

	gpiofd = open("/sys/class/gpio/unexport", O_WRONLY);

	if(gpiofd == -1)
	{
		ui_printf_plus("no this gpio!");
		return -1;
	}

	write(gpiofd, buf, sizeof(buf));
	ui_printf_plus("GPIO %d cancel !", num);
	close(gpiofd);

	return 0;
}

//static int first_start = 0;
static int fs_security_get_gpio_value(int num)
{
	
	int io_value = fs_gpio_get_value(num);
#if 0 //if not call start below, then uncomment this for gpio12
	if(first_start == 0 && num == 12)
	{
		io_value = -1;
		first_start = 1;
	}
#endif
	if(io_value == -1)
	{
		ui_printf_plus("gpio%d is not exist, request now", num);
		if(fs_gpio_request(num) != -1)
		{
			if(fs_gpio_set_direct(num, "in") != -1)
				return fs_gpio_get_value(num);

			ui_printf_plus("set gpio%d direction fail", num);
		}
		
		ui_printf_plus("request gpio%d fail", num);
		return -1;
	}

	return io_value;
}
#endif // !_VS_SIMULATOR

int datasys_security_start(int *num_array, int num_length)
{
#ifndef _VS_SIMULATOR
	int i = 0;

	for (i = 0; i < num_length; i++)
	{
		if (fs_gpio_request(*num_array) != -1)
		{
			if (fs_gpio_set_direct(*num_array, "in") == -1)
				ui_printf_plus("gpio%d set direction fail", *num_array);

		}
		else
			ui_printf_plus("gpio%d request fail", *num_array);

		num_array++;
	}
	return 0;
#else
	return 0;

#endif // !_VS_SIMULATOR

	
	
}

int datasys_security_listen(int num)
{
#ifndef _VS_SIMULATOR
	return fs_security_get_gpio_value(num);
#else
	return 0;
#endif // !_VS_SIMULATOR

	
}

int datasys_security_unlisten(int num)
{
#ifndef _VS_SIMULATOR
	return fs_gpio_cancel(num);
#else
	return 0;
#endif // !_VS_SIMULATOR

	
}

