#ifndef _SCR_SYSTEM_SETTING_H
#define _SCR_SYSTEM_SETTING_H


#ifdef __cplusplus
extern "C"{
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif


//#include "data_tencell.h"

//system info data
#if 0
typedef struct {
	char SIN[20];
	char MAC[20];
	char IP[20];
	char SUBMASK[20];
	char GATEWAY[20];
	char SERVER_ADDR[20];
	char SOFTWARE_VERSION[10];
}_system_info_type;
#endif
#if 1
#define SYSTEM_INFO_ITEM_STRING_LEN	50
typedef struct {
	char HOST_ADDRESS[SYSTEM_INFO_ITEM_STRING_LEN];
	char SERVER_ADDR[SYSTEM_INFO_ITEM_STRING_LEN];
	char MAC[SYSTEM_INFO_ITEM_STRING_LEN];
	char SOFTWARE_VERSION[SYSTEM_INFO_ITEM_STRING_LEN];
	char IP[SYSTEM_INFO_ITEM_STRING_LEN];
	char SUBMASK[SYSTEM_INFO_ITEM_STRING_LEN];
	char GATEWAY[SYSTEM_INFO_ITEM_STRING_LEN];
	char SIN[SYSTEM_INFO_ITEM_STRING_LEN];	
}_system_info_type;
#endif

enum {
	system_info_index_host_addr = 0,
	system_info_index_server_addr,
	system_info_index_mac,
	system_info_index_soft_version,	
	system_info_index_ip,
	system_info_index_submask,
	system_info_index_gateway,
	system_info_index_sin,
	system_info_index_max,	
};
typedef uint8_t system_info_index_t;

//Address
#define ADDRESS_DATA_STRING_LEN		5
#define ADDRESS_ITEM_NUM	6
typedef struct address_set_type {
	char region[ADDRESS_DATA_STRING_LEN];
	char building[ADDRESS_DATA_STRING_LEN];
	char unit[ADDRESS_DATA_STRING_LEN];
	char floor[ADDRESS_DATA_STRING_LEN];
	char room[ADDRESS_DATA_STRING_LEN];
	char branch[ADDRESS_DATA_STRING_LEN];
}_address_set_type;

typedef enum addrset_index {
	AddrSet_index_region = 0,
	AddrSet_index_building,
	AddrSet_index_unit,
	AddrSet_index_floor,
	AddrSet_index_room,
	AddrSet_index_branch,
	AddrSet_index_max,
}_addrset_index;

//screen protect
enum {
	screen_protect_index_time,
	screen_protect_index_mode,
};
typedef uint8_t screen_protect_index;

enum {
	screen_protect_time_30s = 0,
	screen_protect_time_1min,
	screen_protect_time_2min,
	screen_protect_time_5min,
	screen_protect_time_max,
};
typedef uint8_t screen_protect_time_t;

enum {
	screen_protect_mode_black,
	screen_protect_mode_clock,
	screen_protect_mode_photo,
	screen_protect_mode_max,
};
typedef uint8_t screen_protect_mode_t;

typedef struct {
	screen_protect_time_t time;
	screen_protect_mode_t mode;
}_screen_protect_data_type;

//system password
#define PASSWORD_MAX_LEN 7 //This must be 1 more than true max_len of pwd given of char type

typedef struct {
	char pwd[PASSWORD_MAX_LEN];
}_system_password_data_type;


//general password
#define GENERAL_PWD_MODE_MAX 1 //kepp suit with "pwd_mode_max" below
typedef struct {
	char pwd[GENERAL_PWD_MODE_MAX][PASSWORD_MAX_LEN];
}_general_password_data_type;

typedef struct {
	char pwd[2][PASSWORD_MAX_LEN];
}_password_data_type;

enum {
	pwd_mode_security,
	pwd_mode_more,
	pwd_mode_max,
};

typedef int pwd_mode_type;
//end of general password

//sound
enum {
	ring_index_1,
	ring_index_2,
	ring_index_3,
	ring_index_4,
	ring_index_5,
	ring_index_max,
};
typedef uint8_t ring_index_t;

typedef struct {
	uint8_t vol_voice;
	uint8_t vol_bell;
	uint8_t vol_touch;
	uint8_t vol_microphone;
}_sound_volume_t;

typedef struct {
	uint8_t ring_cnt;
	ring_index_t ring_choosen;
	_sound_volume_t volume_m;
}_sound_data_type;
//end of sound


typedef enum {
	btnm_ctrl_null = 0,
	btnm_ctrl_add = 1,
	btnm_ctrl_del,
	btnm_ctrl_check,
}_btnm_ctrl_type;


	void scr_system_setting_init(void);

	bool scr_sysytem_setting_register(void);

	int SysSet_get_local_machine_address(_address_set_type *des_addr);

/*
*****Descr: get sound volume from js_memory data when init
***Return: volume of item or -1 if wrong
*/
int SysSet_get_memory_sound_volume(uint8_t item);

	



#ifdef __cplusplus
}
#endif

#endif
