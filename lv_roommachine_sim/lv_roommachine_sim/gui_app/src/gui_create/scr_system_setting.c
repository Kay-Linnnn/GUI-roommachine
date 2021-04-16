#include "scr_system_setting.h"
#include "../../gui_app.h"

#include "config_project.h"
#include "data_tencell.h"

#ifdef _VS_SIMULATOR
#include "ex_need/temp_misc.h"

# pragma warning (disable:4819)
#else
#include "lth_voip_media.h"
#endif // _VS_SIMULATOR

#define SYSTEM_INFO_TAG_NUM 7
#define ADDRESS_TAG_NUM		AddrSet_index_max

typedef void(*first_page_free_fun)(void);
typedef void(*void_fun_void)(void);

#if 0
typedef struct free_queue_style {
	void *data;
	struct free_queue_style *next;
}_free_queue_style;
#endif

typedef struct {
	lv_obj_t *match_ta;
	char *inser_str;
}_list_btn_fun_widget;

//system information
typedef struct {
	system_info_index_t index;
	char *str_tag;
	char *ch_data;
	lv_obj_t *refresh_label;
}_system_info_secpage_type;

//address
typedef struct {
	char *str_tag;
	uint8_t index;
	lv_obj_t *refresh_label[ADDRESS_TAG_NUM];
	struct address_set_type *addr_data;
}_address_secpage_type;

typedef struct {
	lv_obj_t **ta_array;
	lv_obj_t *selectd_ta;
}_address_page_data_type;

//system info data
_system_info_type system_info_d;//d means data

//address data
_address_set_type address_set_d;

//systme password
_system_password_data_type system_password_d;

//general password
_general_password_data_type general_password_d;

//Time & Date
typedef uint8_t time_am_pm_t;
enum {
	time_t_am = 1,
	time_t_pm
};
typedef struct {
	time_am_pm_t am_or_pm;
	uint16_t time_hour;
	uint16_t time_min;
	lv_calendar_date_t date;
}_time_and_date_data_type;
_time_and_date_data_type time_date_d;


//screen protect
_screen_protect_data_type screen_protect_d;
#if 0
typedef struct {
	_screen_protect_data_type *scr_pro_data;
	lv_obj_t *time_box[screen_protect_time_max];
	lv_obj_t *mode_box[screen_protect_mode_max];
	_free_queue_style *free_queue_cb;
}_screen_protect_common_t;
#endif
//sound

_sound_data_type sound_d;

#define RING_NAME_MAX_LEN 20
typedef struct {
	_sound_data_type *sound_data;
	char ring_txt[ring_index_max][RING_NAME_MAX_LEN];
	lv_obj_t *choosen_list_btn;
}_sound_common_t;

//system
typedef struct {
	_MyStack *page_stack;
	uint8_t temp;
	lv_obj_t *pwd_ta;
	lv_obj_t *pwd_bg;
	_list_btn_fun_widget *lb_widget;
	lv_obj_t *label_ip;
	lv_obj_t *label_port;
}_system_setting_data_dynamic;

typedef struct {
	lv_obj_t *this_scr;
	char local_ip[20];
	char local_port[10];
	_system_info_type *sys_info;
	_address_set_type *addr_set;
	_system_password_data_type *sys_pwd;
	_general_password_data_type *genl_pwd;
	_time_and_date_data_type *time_date;
	_screen_protect_data_type *screen_protect;
	_sound_data_type *sound;
}_system_setting_data_memory;
_system_setting_data_memory  SysSet_data_memory;

typedef struct {
	_system_setting_data_dynamic *dynamic;
	_system_setting_data_memory *memory;
}_system_settting_data;
_system_settting_data SysSet_local;

typedef struct {
	lv_obj_t *to_del;
	uint8_t time_out_peroid;
}_system_setting_task;

typedef uint8_t obj_index;
typedef struct {
	obj_index index;
	void *data;
}_obj_user_data_type;

//system password
typedef int pwd_change_type;
enum {
	pwd_change_old = 0,
	pwd_change_new,
	pwd_change_again,
	pwd_change_max,
};

#if 0
typedef struct{
	pwd_change_type index;
	char *sys_pwd;
	char pwd_txt[pwd_change_max][PASSWORD_MAX_LEN];
	lv_obj_t *pwd_ta[pwd_change_max];
	//_free_queue_style *free_queue_ta_obj;
}_pwd_sys_ta_type;

//general password
typedef struct{
	pwd_change_type index;
	pwd_mode_type pwd_mode;
	char *pwd_addr[pwd_mode_max];
	char pwd_txt[pwd_change_max][PASSWORD_MAX_LEN];
	lv_obj_t *pwd_ta[pwd_change_max];
	_free_queue_style *free_queue_obj_btn_mode;
}_pwd_genl_ta_common_t;
#endif

//time & date
typedef uint8_t time_type;
enum {
	time_hour_t = 0,
	time_min_t,
};

typedef struct {
	lv_obj_t *time_roller[2];
	lv_obj_t *time_am_pm_btn;
	lv_obj_t *calendar;
	lv_obj_t *time_label[2];
	lv_obj_t *date_label;
	_time_and_date_data_type *time_date;
}_time_and_date_common_t;

#ifdef _VS_SIMULATOR
#pragma execution_character_set("utf-8")
LV_IMG_DECLARE(lovelyghost_6464);
#define temp_img_setting_widget lovelyghost_6464
#else
LV_IMG_DECLARE(widget_temp_classfiy_64_64);
#define temp_img_setting_widget widget_temp_classfiy_64_64
#endif

//font declare
//LV_FONT_DECLARE(SC_MicroSoft_YaHei_R16);
//LV_FONT_DECLARE(SC_MicroSoft_YaHei_R10);

enum {
	s_operation_apply,
	s_operation_clear,
};
typedef int static_operation_t;

static lv_obj_t* get_a_default_bg(void);

static void SysSet_change_scr(scr_change_type type,_ui_scr_id scr_id);

static void SysSet_return_btn_event_handle(lv_obj_t *obj,lv_event_t e);

static void sset_initM_ui_create(void);

static int system_command_pack__block_information(system_info_index_t signal, const char *string_value, char *des_s, char *aux_str);

static int AddrSet_data_formalize(_address_set_type *addr_data);

/*sound*/
enum {
	sound_item_vol_voice,
	sound_item_vol_bell,
	sound_item_vol_touch,
	sound_item_vol_microphone,
	sound_item_music_signal,
	sound_item_music_ring,
};
typedef int sset_sound_item_t;

typedef struct {
	lv_obj_t *bar;
	lv_obj_t *label_percent;
}_sound_widget_item;

typedef struct {
	lv_obj_t *list;
	lv_obj_t *label_music;
	sset_sound_item_t which;
}_sound_list_widget;

typedef struct {
	_sound_widget_item *widget[4];
	_sound_list_widget *w_list[2];
}_sound_page_data;
static _sound_page_data *sset_sound_data_manually(static_operation_t operate);

/*date time*/
typedef struct {
	lv_obj_t *jump_bg;
	lv_obj_t *roller[3];
	lv_obj_t *label_title;
	lv_obj_t *label_display;
	int which_item;
	int roller_cnt;
}_date_time_widget;

typedef struct {
	_date_time_widget *widget[2];
}_date_time_page_mem;

static _date_time_page_mem* sset_date_time_mem_manually(static_operation_t operate);

/*Address*/
static _address_page_data_type *sset_address_data_manually(static_operation_t operation);

/*Password*/
typedef struct {
	int index;
	lv_obj_t *ta;
	struct pwd_class *all;
}_pwd_widget;

typedef struct pwd_class {
	_pwd_widget *widget[4];
	lv_obj_t *btnm_bg;
	bool kb_exist;
}_pwd_class;

static _pwd_class *sset_pwd_data_manually(static_operation_t operation);

static int temp_read_ip_addr(char *str_addr);


#define volume_to_percent(volume, max, _des)	sprintf(_des, "%2d%%", volume * 100 / max)
/******************************
******DATA LAYER*******
******************************/
static bool dynamic_is_null(_system_setting_data_dynamic *p)
{
	if (p == NULL) return true;
	else return false;
}

static uint32_t* get_dynamic_pointer_32bits(_system_settting_data *local_data)
{
	if (local_data->dynamic == NULL)
	{
		local_data->dynamic = (_system_setting_data_dynamic *)general_malloc(sizeof(_system_setting_data_dynamic));
	}
	return (uint32_t *)local_data->dynamic;
}

static uint32_t *get_local_pointer(_data_pointer_type p_type)
{
	if (p_type == p_data_dynamic) {
		return get_dynamic_pointer_32bits(&SysSet_local);
	}
	else if (p_type == p_data_memory) {
		return (uint32_t *)SysSet_local.memory;
	}
	return NULL;
}

//list btn widget dynamic
static _list_btn_fun_widget* setting_data_malloc_lbwidget()
{
	_list_btn_fun_widget *lbw = (_list_btn_fun_widget *)general_malloc(sizeof(_list_btn_fun_widget));
	if (is_null_ptr(lbw)) return NULL;

	SysSet_local.dynamic->lb_widget = lbw;
	return lbw;
}

static void setting_data_free_lbwidget()
{
	if (is_null_ptr(SysSet_local.dynamic->lb_widget))
		return;
	general_free(SysSet_local.dynamic->lb_widget);
	SysSet_local.dynamic->lb_widget = NULL;

}

static _list_btn_fun_widget *setting_data_get_lbwidget()
{
	return SysSet_local.dynamic->lb_widget;
}

static void dynamic_data_free()
{
	if(is_null_ptr(SysSet_local.dynamic)) return;

	general_free(SysSet_local.dynamic);
	SysSet_local.dynamic = NULL;
}

static int SysSet_data_set_info_local_data(system_info_index_t index, char *str_value)
{
	char command[100] = {0};
	int b_command = -1;
	char  *aux_str = NULL;
	//sset_info_commun_signal_set_datajs
	_system_setting_data_memory *p_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);
	//set local data
	strcpy(&p_memory->sys_info->SIN[0]+index*sizeof(char)*SYSTEM_INFO_ITEM_STRING_LEN,str_value);
	//write data to json file
	datajs_set_system_info_item_value_separately(index, str_value);

	if(index == system_info_index_ip)
	{
		aux_str = p_memory->sys_info->SUBMASK;
	}
	//whether run commnad or not
	if(0 == system_command_pack__block_information(index, str_value, command, aux_str))
	{
		datajs_command_universal(command);
	}

	return 0;	
}
/*=================
*****Page Stack****
==================*/
static int sset_page_stack_Init(_system_setting_data_dynamic *p_dynamic)
{
	_MyStack *page_stack = (_MyStack *)general_malloc(sizeof(_MyStack));
	if (page_stack == NULL)
	{
		ui_printf_plus("page_stack init fail!");
		return -1;
	}
	my_stack_init(page_stack, 3);//page stack size is 3
	
	p_dynamic->page_stack = page_stack;
	return 0;
}

static void sset_page_stack_deInit(_system_setting_data_dynamic *p_dynamic)
{
	if (p_dynamic->page_stack != NULL)
	{
		my_stack_destory(p_dynamic->page_stack);
		general_free(p_dynamic->page_stack);
		p_dynamic->page_stack = NULL;
	}
}

static _MyStack* sset_page_get_stack(void)
{
	_system_setting_data_dynamic *p_dynamic = (_system_setting_data_dynamic *)get_local_pointer(p_data_dynamic);
	
	return p_dynamic->page_stack;
}

static bool sset_page_stack_is_empty()
{
	_MyStack *stack = sset_page_get_stack();
	if (my_stack_is_empty(stack) == Stack_empty)
		return true;

	return false;
}

static int sset_page_stack_push(int page)
{
	int top;
	_MyStack *stack = sset_page_get_stack();
	if (my_stack_get_top(stack, &top) != Stack_empty)
	{
		if (top == page)// do not repeate push the same menu page into stack, only menu page happens
			return 0;
	}

	if (my_stack_push(stack, page) != Stack_ok)
	{
		return -1;
	}
	
	return 0;
}

static int sset_page_stack_pop()
{
	_MyStack *stack = sset_page_get_stack();
	
	ElmType page;
	StackState ret = my_stack_pop(stack, &page);
	
	if (ret != Stack_ok)
		return ret;
	
	return page;
}


/*=========================================
Additional,  data implemention of all block
===========================================*/

/*Information*/
static int sset_info_data_read_all(_system_info_type *info_addr)
{
	//_system_info_type info;
	//memset(&info_addr, 0, sizeof(_system_info_type));
	if (datajs_get_system_info_all_item_value(info_addr) == -1)
	{
		ui_printf_plus("[sset_info_data_read_all] error!");
		//to do, read json error
		return -1;
	}
	return 0;
}

static int sset_info_data_write_seperately(int signal, const char *value)
{
	//to do
	return 0;
}

/*Address*/
static int sset_addr_data_read_all(char *out_addr[])
{
	int i = 0;
	char *addr_set = SysSet_data_memory.addr_set->region;
	if (is_null_ptr(addr_set)) return -1;
	//char temp[5];
	
	for (i = 0; i < ADDRESS_ITEM_NUM; ++i)
	{
		strcpy(out_addr[i], addr_set);
		//strcpy(temp, addr_set);
		addr_set += ADDRESS_DATA_STRING_LEN;
	}
	return 0;
}

static int sset_addr_data_set(const char *addr[])
{
	int i = 0;
	char addr_ip[20] = { 0 };
	_address_set_type *p_addr_set = SysSet_data_memory.addr_set;
	char *addr_set = p_addr_set->region;
	if (addr_set == NULL)
		return -1;

	//set local data
	
	for (i = 0; i < ADDRESS_ITEM_NUM; i++)
	{
		strcpy(addr_set, addr[i]);
		addr_set += ADDRESS_DATA_STRING_LEN;
	}

	//set json data
	AddrSet_data_formalize(p_addr_set); //formarlize first
	datajs_set_address_all_item_value(p_addr_set);

	if (datajs_get_address_ip(p_addr_set, addr_ip) == 0)
	{
		//got the addr ip
		//change json file info->ip
		datajs_set_system_info_item_value_separately(system_info_index_ip, addr_ip);
	}
	datajs_command_universal("reboot");

	return 0;
}

/*PASSWORD*/
enum {
	sset_pwd_mode_system,
	sset_pwd_mode_security,
};
typedef int sset_pwd_mode_t;
static int sset_pwd_get(sset_pwd_mode_t mode, char *des)
{
	if (mode == sset_pwd_mode_system)
	{
		_system_password_data_type *pwd = (_system_password_data_type *)SysSet_data_memory.sys_pwd;
		strcpy(des, pwd->pwd);
	}
	else if (mode == sset_pwd_mode_security)
	{
		char *pwd = &SysSet_data_memory.genl_pwd->pwd[0][0];
		strcpy(des, pwd);
	}
	return 0;
}

static int sset_pwd_set(sset_pwd_mode_t mode, const char *src_pwd)
{
	if (mode == sset_pwd_mode_system)
	{
		_system_password_data_type *pwd = (_system_password_data_type *)SysSet_data_memory.sys_pwd;
		strcpy(pwd->pwd, src_pwd);
		//to do, write to json
		datajs_set_system_password_item_value_separately(src_pwd);
	}
	else if (mode == sset_pwd_mode_security)
	{
		char *pwd = &SysSet_data_memory.genl_pwd->pwd[0][0];
		strcpy(pwd, src_pwd);
		//to do
		datajs_set_general_password_item_value_separately(pwd_mode_security, src_pwd);
	}
	return 0;
}

/*Sound*/

static int sset_sound_volume_get(sset_sound_item_t which)
{
	_sound_data_type tmp_sound;
	memset(&tmp_sound, 0, sizeof(_sound_data_type));
	if (datajs_get_sound_all_item_value(&tmp_sound) == -1)
	{
		//get item value fail
		return -1;
	}
	uint8_t *p_vol_voice = &tmp_sound.volume_m.vol_voice;

	return (int)*(p_vol_voice + which);
}

static int sset_sound_volume_set(sset_sound_item_t which, uint8_t value)
{
	if (which >= sound_item_vol_voice && which <= sound_item_vol_microphone)
	{
		uint8_t item_sound_js[] = { item_sound_voice, item_sound_bell, item_sound_touch, item_sound_microphone };
		return datajs_set_sound_item_value_separately(item_sound_js[which], value);
	}
	return 0;	
}

static int sset_sound_music_get(sset_sound_item_t which, char *out_str)
{
	if (which >= sound_item_music_signal && which <= sound_item_music_ring)
	{
		strcpy(out_str, "bell.wav");
		return 0;
	}
	return -1;
}

/**
 * return the number of music in library
*/
static int sset_sound_music_get_library(sset_sound_item_t which, char *out_arr[])
{
	if (which >= sound_item_music_signal && which <= sound_item_music_ring)
	{
		int i = 0;
		for (i = 0; i < 4; ++i)
		{
			//strcpy(out_arr[i], "bell.wav");
			sprintf(out_arr[i], "%d-ring.wav", i);
		}		
		
		return 4;
	}
	return -1;
}

static int sset_sound_music_set(sset_sound_item_t which, const char *music)
{
	if (which >= sound_item_music_signal && which <= sound_item_music_ring)
	{
		ui_printf_plus("Choose %s", music);
	}
	return 0;
}

enum {
	sset_date_time_item_time,
	sset_date_time_item_date,
	sset_date_time_item_time_syntax,
};
typedef int sset_date_time_item_t;
static int sset_date_time_get(sset_date_time_item_t which, char *out_str)
{
	switch (which)
	{
	case sset_date_time_item_time:
		sprintf(out_str, "20:52");
		break;

	case sset_date_time_item_date:
		sprintf(out_str, "2021年1月30日");
		break;

	case sset_date_time_item_time_syntax:
		sprintf(out_str, "24小时");
		break;

	default:
		break;
	}
	return 1;
}

static int sset_date_set(sset_date_time_item_t which, ...)
{
	return 0;
}

static int sset_ip_address_refresh()
{
	//to do
	//refresh ip by address after address changed
	return 0;
}

/*
*****Descr: write command for system information set
***@Para-> signal: tell which command
***@Para-> string value: command value
***@Para-> des_s: string of destination
***@Para-> aux_str: auxiliary string to help complete command, for example: when set ip, netmask also need to set both simultaneously, netmask there is a aux_str
//				set it to NULL if no use
***@Return : 0 if pack command or -1 if no
*/
static int system_command_pack__block_information(system_info_index_t signal, const char *string_value, char *des_s, char *aux_str)
{
	if (is_null_ptr(des_s) || is_null_ptr(string_value)) return -1;

	char *command = des_s;

	switch (signal)
	{
	case system_info_index_sin:
		break;

	case system_info_index_mac:
	{
	#if 0
		char pre_command[] = "ifconfig eth0 hw ether";
		sprintf(command, "%s %s", pre_command, string_value);
		//b_command = true;
		return 0;
	#endif		
	}
	break;

	case system_info_index_ip:
		{
			char ip_command[] = "ifconfig eth0";
			sprintf(command, "%s %s netmask %s", ip_command, string_value, aux_str);
			//b_command = true;
			return 0;
		}
		break;

	case system_info_index_submask:
	{
		char submask_command[] = "ifconfig eth0 netmask";
		sprintf(command, "%s %s", submask_command, string_value);
		//b_command = true;
		return 0;
	}
	break;

	case system_info_index_gateway:
		break;

	case system_info_index_server_addr:
		break;

	case system_info_index_soft_version:
		break;

	default:
		break;
	}
	return -1;
}

void scr_system_setting_init()
{
	memset(&SysSet_data_memory,0,sizeof(_system_setting_data_memory));
	if (SysSet_data_memory.this_scr == NULL) ui_printf("system_setting_data_memory,this scr is NULL\r\n");
	else ui_printf("system_setting_data_memory,this scr is not NULL\r\n");

	char local_ip[] = "192.168.1.196";
	char local_port[] = "5060";

	strcpy(&SysSet_data_memory.local_ip[0], local_ip);
	strcpy(&SysSet_data_memory.local_port[0], local_port);

//address
	//datajs_add_system_object_address();//do not repeate this operation
	memset(&address_set_d, 0, sizeof(_address_set_type));

	if (datajs_get_address_all_item_value(&address_set_d) != 0)
	{
		ui_printf_plus("system init,address get value fail");
		char init_region[] = "01";
		char init_building[] = "001";
		char init_unit[] = "1";
		char init_floor[] = "01";
		char init_room[] = "02";
		char init_branch[] = "01";


		strcpy(&address_set_d.region[0], init_region);
		strcpy(&address_set_d.building[0], init_building);
		strcpy(&address_set_d.unit[0], init_unit);
		strcpy(address_set_d.floor, init_floor);
		strcpy(&address_set_d.room[0], init_room);
		strcpy(&address_set_d.branch[0], init_branch);

		//datajs_set_address_all_item_value(&address_set_d);//test set address data in system setting book
	}

	char temp_get_addr_ip[20] = { 0 };
	if (datajs_get_address_ip(&address_set_d, temp_get_addr_ip) == 0)
		ui_printf_plus("test,get address ip:%s", temp_get_addr_ip);
#if 0 //turn on if want to get ip from addressbook, this will change ip of system_book
	datajs_set_system_info_item_value_separately(system_info_index_ip, temp_get_addr_ip);
#endif

//system info	
	memset(&system_info_d, 0, sizeof(_system_info_type));

	//datajs_add_system_information_in_system_book();//add system info module in system book, do not repeat this operation any more if already run
	//datajs_del_system_information_in_system_book();//undo add operation above

	if (datajs_get_system_info_all_item_value(&system_info_d) == -1) // get system info bookf fail
	{
		ui_printf_plus("get system info book fail");
		char init_SIN[] = "123456789";
		char init_MAC[] = "123456789";
		//char init_IP[] = "192.168.1.121";
		char init_IP[20] = { 0 };
		char init_SUBMASK[] = "255.255.0.0";
		char init_GATEWAY[] = "192.168.1.1";
		char init_SERVERIP[] = "192.168.1.102";
		char init_SOFTVER[] = CFG_P_software_version_num;

		temp_read_ip_addr(init_IP);//auto get local previous ip

		strcpy(&system_info_d.SIN[0], init_SIN);
		strcpy(&system_info_d.MAC[0], init_MAC);
		strcpy(&system_info_d.IP[0], init_IP);
		strcpy(&system_info_d.SUBMASK[0], init_SUBMASK);
		strcpy(&system_info_d.GATEWAY[0], init_GATEWAY);
		strcpy(&system_info_d.SERVER_ADDR[0], init_SERVERIP);
		//strcpy(&system_info_d.SOFTWARE_VERSION[0], init_SOFTVER);
	}
	strcpy(&system_info_d.SOFTWARE_VERSION[0], CFG_P_software_version_num);
	
	char command[100] = { 0 };
	int info_index = 0;
	char *p_info = &system_info_d.SIN[0];
	char *aux_str = NULL;
	for (info_index = system_info_index_sin; info_index < system_info_index_max; info_index++)//system_info_index_max
	{
		aux_str = NULL;
		if (info_index == system_info_index_ip)
			aux_str = system_info_d.SUBMASK;

		if (0 == system_command_pack__block_information(info_index, p_info + SYSTEM_INFO_ITEM_STRING_LEN * info_index, command, aux_str))
			datajs_command_universal(command);
	}	
	
	
//password
	memset(&system_password_d, 0, sizeof(_system_password_data_type));
	if (0 != datajs_get_system_password_item_value(&system_password_d))
	{
		ui_printf_plus("system init,system password get value fail");
		char init_system_pwd[] = "123456";
		strcpy(&system_password_d.pwd[0], init_system_pwd);
		//datajs_set_system_password_item_value_separately(init_system_pwd); //test set system password
	}	

	//datajs_add_general_pwd_in_system_book();//add general pws, do not repeat this operation any more if already run
	//datajs_del_general_pwd_in_system_book();//undo add operation above
	if (0 != datajs_get_general_password_all_item_value(&general_password_d))
	{
		ui_printf_plus("system init, get general pwd fail");
		char init_security_pwd[] = "123456";
		strcpy(&general_password_d.pwd[pwd_mode_security][0], init_security_pwd);
		//datajs_set_general_password_item_value_separately(pwd_mode_security, general_password_d.pwd[pwd_mode_security]);//test set security pwd
	}
	ui_printf_plus("init, general password[0]:%s",general_password_d.pwd[0]);

//time and date
	memset(&time_date_d,0,sizeof(_time_and_date_data_type));
	time_date_d.time_min = 10;
	time_date_d.time_hour = 10;
	time_date_d.date.day = 12;
	time_date_d.date.month = 1;
	time_date_d.date.year = 2021;
	time_date_d.am_or_pm = time_t_am;

//screen protect
	//datajs_add_screen_protect_in_system_book(); //add screen protect module in system book, do not repeat this operation any more if already run;
	//datajs_del_screen_protect_in_system_book(); //undo add operation above

	memset(&screen_protect_d, 0, sizeof(_screen_protect_data_type));

	if (datajs_get_screen_protect_all_item_value(&screen_protect_d) != 0)
	{
		ui_printf_plus("system init,get screen protect value fail");
		screen_protect_d.time = screen_protect_time_2min;
		screen_protect_d.mode = screen_protect_mode_clock;
	}
	ui_printf_plus("screen protect, time:%d, mode:%d", screen_protect_d.time, screen_protect_d.mode);
	
	//datajs_set_screen_protect_item_value_separately(screen_protect_index_mode, 1);//test set screen protect value in system book

//sound
	//datajs_add_sound_in_system_book(); // add sound module, do not repeat this operation anymore
	//datajs_del_sound_in_system_book();//del, undo add operation up

	memset(&sound_d,0,sizeof(_sound_data_type));
	sound_d.ring_cnt = ring_index_max;
	if (datajs_get_sound_all_item_value(&sound_d) != 0)
	{
		ui_printf_plus("system init,get sound value fail");
		sound_d.ring_choosen = ring_index_2;
		
		sound_d.volume_m.vol_voice = 10;
		sound_d.volume_m.vol_bell = 10;
		sound_d.volume_m.vol_touch = 10;
		sound_d.volume_m.vol_microphone = 10;
	}	

	SysSet_local.dynamic = NULL;
	SysSet_data_memory.sys_info = &system_info_d;
	SysSet_data_memory.addr_set = &address_set_d;
	SysSet_data_memory.sys_pwd = &system_password_d;
	SysSet_data_memory.genl_pwd = &general_password_d;
	SysSet_data_memory.time_date = &time_date_d;
	SysSet_data_memory.screen_protect = &screen_protect_d;
	SysSet_data_memory.sound = &sound_d;
	SysSet_local.memory = &SysSet_data_memory;
}

/*************************
*********task**************
****************************/
static void system_setting_task_tick(uint8_t *time_tick, bool e_zero)
{
	if (time_tick == NULL || *time_tick < 2) return;

	if (!e_zero)
		(*time_tick)--;
	else
		*time_tick = 1;
}

static void system_setting_callback(lv_task_t *task)
{
	if (task == NULL) return;

	_system_setting_task *task_data = (_system_setting_task *)task->user_data;
	if (task_data->to_del == NULL) return;

	if (task_data->time_out_peroid > 1) {
		system_setting_task_tick(&task_data->time_out_peroid,false);
		return;
	}
	else {
		lv_obj_del(task_data->to_del);
		lv_task_del(task);
		general_free(task_data);
	}
}

static uint32_t *system_setting_task_create(lv_obj_t *to_del)
{
	_system_setting_task *SysSet_task = (_system_setting_task *)general_malloc(sizeof(_system_setting_task));
	SysSet_task->to_del = to_del;
	SysSet_task->time_out_peroid = 2;

	lv_task_t *task_t = lv_task_create(system_setting_callback,1000,LV_TASK_PRIO_MID, SysSet_task);
	return (uint32_t *)SysSet_task;
}

/*
****Temp File Operation
*/
static int temp_write_ip_addre(char *ip_str)
{
	FILE *file = fopen("ip_addr.txt","w");
	if (file == NULL)
	{
		ui_printf("open ip_addr.txt error\n");
		return -1;
	}
	fputs(ip_str, file);
	fclose(file);
	return 0;
}

static int temp_set_ip(char *ip_str)
{
	char command[100] = {0};
	char ip_command[] = "ifconfig eth0 ";

	sprintf(command,"%s%s netmask 255.255.0.0",ip_command,ip_str);

	ui_printf("[temp set ip]command:%s\n",command);
#ifndef _VS_SIMULATOR
	system(command);
#endif // _VS_SIMULATOR
	return 0;
}

static int temp_read_ip_addr(char *str_addr)
{
#if 1
	int ret = 0;
	char buf[50] = { 0 };

	FILE *file = fopen("ip_addr.txt", "r");
	if (file == NULL)
	{
		ui_printf("read,open ip_addr.txt error\n");
		ret = lthmedia_getlocalip("eth0", str_addr);
		return -1;
	}	

	fgets(buf, 50, file);
	strcpy(str_addr, buf);
	fclose(file);
	//return 0;
#endif
	temp_set_ip(str_addr);
	
	return ret;
}

/*========================
******special operation
=========================*/
#if 0
typedef struct {
	uint8_t index;
	void *upper_data;
}_ta_user_data;

typedef struct {
	uint8_t index;
	void *upper_data;
}_obj_user_data;

_system_info_secpage_type *system_info_dynamic = NULL;
_address_secpage_type *address_dynamic = NULL;

/*
*****Free Style Queue Data Structure,for freeing memory of any kind of data type
*/

_free_queue_style *addr_set_free_queue_list = NULL;
_free_queue_style *addr_set_textarea_list = NULL;
typedef uint8_t free_queue_index;
enum {
	free_queue_list_AddrSet_first_page_user_data = 1,
	free_queue_list_AddrSet_textarea,
};
static _free_queue_style* free_queue_get_list(free_queue_index index)
{
	switch (index)
	{
	case free_queue_list_AddrSet_first_page_user_data:
		return addr_set_free_queue_list;
		break;

	case free_queue_list_AddrSet_textarea:
		return addr_set_textarea_list;
		break;

	default:
		return NULL;
		break;
	}
}

static _free_queue_style* free_queue_new_node(void *data)
{
	_free_queue_style *node = (_free_queue_style *)general_malloc(sizeof(_free_queue_style));
	if (!is_null_ptr(node)) {
		node->data = data;
		node->next = NULL;
	}

	return  node;
}

static void free_queue_tail_insert(_free_queue_style **list_head_addr, void *data)
{
	_free_queue_style *node = free_queue_new_node(data);
	if (is_null_ptr(node))
		return;

	_free_queue_style *tail = *list_head_addr;
	if (is_null_ptr(tail))
		*list_head_addr = node;
	else
	{
		while (tail->next != NULL)
		{
			tail = tail->next;
		}
		tail->next = node;
	}
}

static void free_queue_free(_free_queue_style *list_head)
{
	if (is_null_ptr(list_head))
		return;
	ui_printf("[free_queue_free] called\n");

	_free_queue_style *free_node = NULL;

	while (list_head != NULL)
	{
		general_free(list_head->data);
		free_node = list_head;
		list_head = list_head->next;		
		general_free(free_node);
	}
}

typedef struct {
	uint8_t choosen_index;
	lv_obj_t *match_ta;
	_free_queue_style *ta_list;
	void *upper_data;
}_AddrSet_ta_data_type;

static _AddrSet_ta_data_type *Temp_ta_data = NULL;
static void free_queue_free_without_data(_free_queue_style *list_head)
{
	if (is_null_ptr(list_head))
		return;

	ui_printf("[free_queue_free_without_data] called\n");

	_free_queue_style *free_node = NULL;

	while (list_head != NULL)
	{
		//general_free(list_head->data);		
		free_node = list_head;
		list_head = list_head->next;
		general_free(free_node);
	}
}

//free queue的释放分两种，本身这个就是为了任意存放和释放数据用的
//在使用中因为扩展了一些功能，对释放增加一种without_data的情况，即释放的时候只释放queue本身为node申请的内存，不用去释放node下的内存
//这种情况发生比如说数据送入的是创建的obj对象，这种本身是lv内部的内存不需要自己手动释放，在删除整个界面时会自动删除
static void free_queue_free_all(_free_queue_style *list_head, bool without_data)
{
	if (is_null_ptr(list_head))
		return;

	if (without_data) {
		free_queue_free_without_data(list_head);
	}
	else {
		free_queue_free(list_head);
	}
}
#endif

/*============================
 ****** UI LAYER**************
==============================*/
static lv_obj_t* get_a_default_bg()
{
	return universal_misc_rm_pic_bg_create();
}

static lv_obj_t *get_a_active_bg()
{
	lv_obj_t * act_bg = lv_obj_create(lv_scr_act(), NULL);
	screen_adaption_lvgl_obj_set_size(act_bg, SCREEN_BASE_RESOLUTION_HOR, SCREEN_BASE_RESOLUTION_VER);
	screen_adaption_lvgl_obj_set_pos(act_bg, 0, 0);
	lv_obj_set_style_local_bg_opa(act_bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);

	return act_bg;
}

static lv_obj_t* setting_get_cover_bg()
{
	lv_obj_t *bg_top = lv_obj_create(lv_scr_act(), NULL);
	lv_theme_apply(bg_top, (lv_theme_style_t)sigma_star_theme_obj_bg_cover_black);
	screen_adaption_lvgl_obj_set_size(bg_top, SCREEN_BASE_RESOLUTION_HOR, SCREEN_BASE_RESOLUTION_VER);
	screen_adaption_lvgl_obj_set_pos(bg_top, 0, 0);

	return bg_top;
}

static lv_obj_t * setting_ui_head_title_create(lv_obj_t *parent,const char *head_str, lv_event_cb_t event)
{
#if 0
	lv_obj_t *head_cont = lv_cont_create(parent, NULL);
	screen_adaption_lvgl_obj_set_pos(head_cont, 0, 0);
	lv_cont_set_fit(head_cont, LV_FIT_NONE);
	screen_adaption_lvgl_obj_set_size(head_cont, SCREEN_BASE_RESOLUTION_HOR, 60);
	lv_cont_set_layout(head_cont, LV_LAYOUT_CENTER);
	//lv_obj_add_style(head_cont, LV_CONT_PART_MAIN, &title_style);
	lv_theme_apply(head_cont, (lv_theme_style_t)sigma_star_theme_title);

	lv_obj_t *obj_label = lv_label_create(head_cont, NULL);
	lv_label_set_text(obj_label, head_str);

	return head_cont;
#endif
	return universal_misc_rm_head_title(parent, head_str, event);
}

static lv_obj_t *setting_ui_head_return_create(lv_obj_t *parent, char *txt,lv_event_cb_t event)
{
	_universal_obj_element btn_label;
	btn_label.parent = parent;
	btn_label.width = 120;
	btn_label.height = 60;
	
	lv_obj_t *btn = universal_btn_label_chineses_create(&btn_label,txt);
	lv_theme_apply(btn,(lv_theme_style_t)sigma_star_theme_Btn_home_return);
	lv_obj_set_event_cb(btn,event);

	screen_adaption_lvgl_obj_align(btn, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

	return btn;
}

enum sset_page_index {
	sset_page_menu,
	sset_page_info,
	sset_page_screen_protect,
	sset_page_upgrade,
	sset_page_address,
	sset_page_date_time,
	sset_page_password,
	sset_page_ring,
	sset_page_pwd_check,
};
#define compare_cal(a, b)	((a) > (b))
//static lv_obj_t *sset_page_img_bg = NULL;  //nothing to do with img_bg
static lv_obj_t *sset_page_active = NULL;
static void sset_ui_page_bg_img_create()
{
	lv_obj_t *bg = get_a_default_bg();//just a page of bg
	//sset_page_img_bg = bg;
}

static void sset_ui_page_clear()
{
	//to do, see whether to delete screen there
	//sset_page_img_bg = NULL;
	sset_page_active = NULL;
}

/* actuall we can set all return btn event using this one, no matter menu or not*/
//once we got the page pop out from stack_page, we can decide what to do with this page
LV_EVENT_CB_DECLARE(sset_ui_eventcb_fun_page_quit)
{
	if (e == LV_EVENT_CLICKED)
	{
		int page = sset_page_stack_pop();
		ui_printf_plus("sset_ui_page_quit, page:%d", page);
		switch (page)//do some clean page memory work
		{
		case sset_page_menu:
			sset_ui_page_clear();
			break;

		case sset_page_info:
			break;

		case sset_page_address:
			//sset_addr_page_free();
			sset_address_data_manually(s_operation_clear);
			break;

		case sset_page_password:
			//sset_pwd_page_clear();
			sset_pwd_data_manually(s_operation_clear);
			break;

		case sset_page_ring:
			sset_sound_data_manually(s_operation_clear);
			break;

		case sset_page_date_time:
			sset_date_time_mem_manually(s_operation_clear);
			break;

		case sset_page_pwd_check:
			sset_ui_page_clear();
			break;

		default:
			break;
		}

		if (page == sset_page_menu || page == sset_page_pwd_check)
			SysSet_change_scr(scr_chtype_return, scr_Null);
		else
			sset_initM_ui_create();
	}
}

static lv_obj_t *sset_ui_page_enter(enum sset_page_index page)
{
	const char *title[] = { "设置", "系统信息","屏保设置", "系统升级", "房号设置", "时间设置", "密码设置", "铃声设置" ,"输入密码"};
	#if 0
	lv_event_cb_t return_event[] = { SysSet_return_btn_event_handle, 
										sset_ui_eventcb_fun_page_quit };
	#endif

	if (sset_page_active != NULL)
		lv_obj_del(sset_page_active);

	lv_obj_t *bg = get_a_active_bg();
	sset_page_active = bg;

	//lv_obj_t *obj_head = setting_ui_head_title_create(bg, title[page], return_event[(int)compare_cal(page, sset_page_menu)]);
	lv_obj_t *obj_head = setting_ui_head_title_create(bg, title[page], sset_ui_eventcb_fun_page_quit);

	int ret = sset_page_stack_push((int) page);
	if (ret == -1)
	{
		//to do, push fail
	}

	return bg;
}

/*******************BELOW************************************
*************System Setting-System Info Block****************
************************************************************/
/*Foramt:
***@ Prefix:sset_info_
***@ UI draw: sset_info_ui_
***@ event_cb: sset_info_eventcb_
***@ communication:sset_info_commun_
*/

/*---------------BELOW-------------------
*********System Info Second Page*********
----------------------------------------*/
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

static void sset_info_commun_signal_set_datajs(system_info_index_t signal, const char *string_value)
{
	char command[100] = { 0 };
	int b_command = -1; // need to set true if want to use command
	char *aux_str = NULL;
	_system_setting_data_memory *p_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);
	//note: what if p_memory is NULL
	if (signal == system_info_index_ip)
	{
		aux_str = p_memory->sys_info->SUBMASK;
	}
	b_command = system_command_pack__block_information(signal, string_value, command, aux_str);
#if 0
	switch (signal)
	{
	case system_info_index_sin:		
	break;

	case system_info_index_mac:
	{
		char pre_command[] = "ifconfig eth0";
		sprintf(command, "%s %s", pre_command, string_value);
		b_command = true;
	}
		break;

	case system_info_index_ip:
		if (!is_null_ptr(p_memory)) 
		{
			char ip_command[] = "ifconfig eth0";
			sprintf(command, "%s %s netmask %s", ip_command, string_value, p_memory->sys_info->SUBMASK);
			b_command = true;
		}		
		break;

	case system_info_index_submask:
		{
			char submask_command[] = "ifconfig eth0";
			sprintf(command, "%s %s", submask_command, string_value);
			b_command = true;
		}
		break;

	case system_info_index_gateway:
		break;

	case system_info_index_server_addr:
		break;

	case system_info_index_soft_version:
		break;

	default:
		break;
	}
#endif
	datajs_set_system_info_item_value_separately(signal, string_value);
	if (b_command == 0)
		datajs_command_universal(command);
}

static int sset_info_data_block_take_txt(_system_info_type *info, char *data_array[])
{
	int ret = sset_info_data_read_all(info);
	if (ret == -1)
	{
		//to do, read fail
		return -1;
	}
	data_array[0] = info->HOST_ADDRESS;
	data_array[1] = info->SERVER_ADDR;
	data_array[2] = info->MAC;	
	data_array[3] = info->SOFTWARE_VERSION;
	data_array[4] = info->IP;
	data_array[5] = info->SUBMASK;
	data_array[6] = info->GATEWAY;
	data_array[7] = info->SIN;

	return 0;
}

static lv_obj_t *sset_info_ui_data_block_duplicate(lv_obj_t *parent, const char *txt)
{
	uint16_t label_hor_left_space = 22;

	static lv_style_t style_obj;
	lv_style_init(&style_obj);
	lv_style_set_bg_opa(&style_obj, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_obj, LV_STATE_DEFAULT, TENCELL_RM_COLOR_BLACK_BLUE_SET_INFO_WIDGET);
	lv_style_set_radius(&style_obj, LV_STATE_DEFAULT, 10);

	static lv_style_t style_label;
	lv_style_init(&style_label);
	lv_style_set_text_color(&style_label, LV_STATE_DEFAULT, TENCELL_RM_COLOR_GARY_WHITE_WORD);

	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = parent;
	elm_obj.width = 566;
	elm_obj.height = 46;	

	lv_obj_t *obj = universal_obj_create(&elm_obj, tencell_theme_NULL);
	//lv_obj_add_style(obj, LV_OBJ_PART_MAIN, &style_obj);
	universal_obj_add_style(obj, &style_obj);

	_universal_obj_element elm_label;
	Uni_Element_init(elm_label);
	elm_label.parent = obj;

	lv_obj_t *label = universal_label_create(&elm_label, txt, sigma_star_theme_label_chinese_normal);
	screen_adaption_lvgl_obj_align(label, NULL, LV_ALIGN_IN_LEFT_MID, label_hor_left_space, 0);	
	universal_obj_add_style(label, &style_label);

	return obj;
}

static void sset_info_ui_tag_label_duplicate(lv_obj_t *parent, const char *txt, lv_obj_t *base)
{
	uint16_t label_hor_out_shift_space = 15;

	_universal_obj_element elm_label;
	Uni_Element_init(elm_label);
	elm_label.parent = parent;

	lv_obj_t *label = universal_label_create(&elm_label, txt, sigma_star_theme_label_chinese_normal);
	screen_adaption_lvgl_obj_align(label, base, LV_ALIGN_OUT_LEFT_MID, -label_hor_out_shift_space, 0);
}

/*
 * 系统信息界面创建
*/
static void sset_info_ui_page_create(void)
{
	int block_cnt = 0;
	uint16_t block_coor_x = 173, block_coor_y = 73;
	lv_coord_t block_ver_shift = screen_adaption_lvgl_ver_calculate(123 - 73);
	lv_obj_t *obj_block = NULL;
	lv_obj_t *page_bg = sset_ui_page_enter(sset_page_info);

	_system_info_type *info = (_system_info_type *)general_malloc(sizeof(_system_info_type));
	char *info_data_txt[8];// = (char *)general_malloc(sizeof(char) * 8);
	if (sset_info_data_block_take_txt(info, info_data_txt) == -1)
		return;

	const char *tag_txt[] = {"本机地址:",
							"服务器地址:",
							"MAC地址:",
							"软件版本:",
							"IP地址:",
							"子网掩码:",
							"网关:",
							"设备ID:"};

	for (block_cnt = 0; block_cnt < 8; ++block_cnt)
	{
		obj_block = sset_info_ui_data_block_duplicate(page_bg, info_data_txt[block_cnt]);
		lv_obj_set_pos(obj_block, block_coor_x, block_coor_y);
		block_coor_y += block_ver_shift;
		sset_info_ui_tag_label_duplicate(page_bg, tag_txt[block_cnt], obj_block);
	}

	general_free(info);	
}

/*******************BELOW************************************
*****************System Setting-Address Block****************
************************************************************/
/*Foramt:
***@ Prefix:sset_addr_
***@ UI draw: sset_addr_ui_
***@ event_cb: sset_addr_eventcb_
*/

/*---------------BELOW----------------------
************Address Second Page*************
-------------------------------------------*/
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

static int AddrSet_data_formalize(_address_set_type *addr_data)
{
	int i = 0;
	int value[AddrSet_index_max] = { 0 };
	for (i = 0; i < AddrSet_index_max; i++)
	{
		value[i] = atoi(&addr_data->region[0] + i * 5);
	}
	sprintf(addr_data->region, "%02d", value[AddrSet_index_region]);
	sprintf(addr_data->building, "%03d", value[AddrSet_index_building]);
	sprintf(addr_data->unit, "%01d", value[AddrSet_index_unit]);
	sprintf(addr_data->floor, "%02d", value[AddrSet_index_floor]);
	sprintf(addr_data->room, "%02d", value[AddrSet_index_room]);
	sprintf(addr_data->branch, "%02d", value[AddrSet_index_branch]);

	return 0;
}
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*------------------BELOW------------------
************Address Set First Page********
-------------------------------------------*/

LV_EVENT_CB_DECLARE(sset_addr_evnetcb_confirm)
{
	if (e == LV_EVENT_CLICKED && obj->user_data != NULL)
	{
		int i = 0;
		_address_page_data_type *addr_page_d = (_address_page_data_type *)obj->user_data;
		lv_obj_t **ta_array = addr_page_d->ta_array;

		
		const char *ta_txt[ADDRESS_ITEM_NUM] = { 0 };
		const char *txt = lv_textarea_get_text(*ta_array);
		ui_printf_plus("txt:%s", txt);

		for (i = 0; i < ADDRESS_ITEM_NUM; ++i)
		{
			ta_txt[i] = lv_textarea_get_text(*(ta_array + i));
		}

		sset_addr_data_set(ta_txt);
	}
}

LV_EVENT_CB_DECLARE(sset_addr_eventcb_del_btn)
{
	if (e == LV_EVENT_CLICKED && obj->user_data != NULL)
	{
		_address_page_data_type *addr_page_data = (_address_page_data_type *)obj->user_data;
		lv_obj_t *match_ta = addr_page_data->selectd_ta;

		lv_textarea_del_char(match_ta);
	}
}

LV_EVENT_CB_DECLARE(sset_addr_eventcb_kb)
{
	if (e != LV_EVENT_VALUE_CHANGED && !is_null_ptr(obj->user_data))
		return;

	_address_page_data_type *addr_page_data = (_address_page_data_type *)obj->user_data;
	lv_obj_t *match_ta = addr_page_data->selectd_ta;
	
	const char *c = lv_btnmatrix_get_active_btn_text(obj);
	
	lv_textarea_add_text(match_ta, c);
}

//hide and unhide the pre-match_ta and match_ta
LV_EVENT_CB_DECLARE(sset_addr_ta_eventcb)
{
	if (e == LV_EVENT_CLICKED)
	{
		if (is_null_ptr(obj->user_data))
			return;

		_address_page_data_type *page_data = (_address_page_data_type *)obj->user_data;
		
		if (is_null_ptr(page_data->ta_array))
			return;
		
		if (obj == page_data->selectd_ta) //do nothing if click curent active textarea
			return;

		lv_textarea_set_cursor_hidden(obj, false);
		lv_textarea_set_cursor_hidden(page_data->selectd_ta, true);

		page_data->selectd_ta = obj;
	}
}

#if 0
static _address_page_data_type *sset_page_data_addr = NULL;
static _address_page_data_type *address_page_malloc()
{
	_address_page_data_type *addr_page_d = (_address_page_data_type *)general_malloc(sizeof(_address_page_data_type));
	memset(addr_page_d, 0, sizeof(_address_page_data_type));
	lv_obj_t **ta_array = (lv_obj_t **)general_malloc(sizeof(lv_obj_t *) * 6);
	addr_page_d->ta_array = ta_array;

	sset_page_data_addr = addr_page_d;

	return addr_page_d;
}

static void sset_addr_page_free()
{
	if (sset_page_data_addr)
	{
		general_free(sset_page_data_addr->ta_array);
		general_free(sset_page_data_addr);
		sset_page_data_addr = NULL;
	}
}
#endif

static _address_page_data_type *sset_address_data_manually(static_operation_t operation)
{
	_address_page_data_type *s_addr_data = NULL;
	if (s_operation_apply == operation)
	{
		s_addr_data = (_address_page_data_type *)general_malloc(sizeof(_address_page_data_type));
		if (is_null_ptr(s_addr_data))
		{
			ui_printf_malloc_error("address page data fail");
			return NULL;
		}
		memset(s_addr_data, 0, sizeof(_address_page_data_type));
		lv_obj_t **ta_array = (lv_obj_t **)general_malloc(sizeof(lv_obj_t *) * 6);
		
		s_addr_data->ta_array = ta_array;
	}
	else if (s_operation_clear == operation)
	{
		if (s_addr_data)
		{
			general_free(s_addr_data->ta_array);
			general_free(s_addr_data);
			s_addr_data = NULL;
		}
	}

	return s_addr_data;
}

static int sset_addr_data_get_ta_value(int size, char* value_addr, int size_of_2D)
{
	int i = 0;
	static const char *temp[] = { "01", "001", "1", "01", "03", "01" };
	
	char *p_value[ADDRESS_ITEM_NUM] = { 0 };
	for (i = 0; i < size; ++i)
		p_value[i] = value_addr + i * size_of_2D;
	//for (i = 0; i < size; ++i)
		//value_addr[i] = temp[i];
	sset_addr_data_read_all(p_value);

	return 0;
}

/*
 * 房号界面设置
*/
static void sset_addr_ui_page_create(void)
{
	int i = 0;
	uint8_t ta_size = 6;
	uint16_t ta_bg_coor_y = SCREEN_TENCELL_7inch_TITLE_HEIGHT + 12;
	const char *ta_name[] = { "区", "栋", "单元", "楼", "房", "分机" };
	//char *ta_txt[6];//value
	char ta_txt[6][5];

	lv_obj_t *page_bg = sset_ui_page_enter(sset_page_address);

	_address_page_data_type *page_data = sset_address_data_manually(s_operation_apply);//address_page_malloc();
	if (is_null_ptr(page_data))
	{
		//to do
		ui_printf_plus("address page, apply memory for page fail");
		return;
	}

	//textarea bg
	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = page_bg;
	elm_obj.width = SCREEN_BASE_RESOLUTION_HOR;
	elm_obj.height = 50;

	lv_obj_t *ta_bg = universal_obj_create(&elm_obj, tencell_theme_obj_transp);
	screen_adaption_lvgl_obj_set_pos(ta_bg, 0, ta_bg_coor_y);

	sset_addr_data_get_ta_value(ADDRESS_ITEM_NUM, &ta_txt[0][0], 5);//6: ta_txt size

	//delete btn
	_universal_obj_element elm_btn_del;
	Uni_Element_init(elm_btn_del);
	elm_btn_del.parent = ta_bg;
	elm_btn_del.width = 60;
	elm_btn_del.height = lv_obj_get_height(ta_bg);
	elm_btn_del.event_cb = sset_addr_eventcb_del_btn;
	elm_btn_del.user_data = page_data;

	lv_obj_t *del_btn = universal_btn_label_create(&elm_btn_del, NULL, tencell_theme_btn_chinese_normal);
	screen_adaption_lvgl_obj_align(del_btn, NULL, LV_ALIGN_IN_RIGHT_MID, -10, 0);
	
	Uni_Element_init(elm_btn_del);
	elm_btn_del.parent = del_btn;
	universal_image_create(&elm_btn_del, bin_path_img_del_icon, tencell_theme_NULL);

	//textarea
	lv_obj_t **ta_array = page_data->ta_array;
	_universal_obj_element elm_ta;
	Uni_Element_init(elm_ta);
	elm_ta.parent = ta_bg;
	elm_ta.width = 80;
	elm_ta.height = lv_obj_get_height(ta_bg) - 10;
	elm_ta.user_data = page_data;
	elm_ta.event_cb = sset_addr_ta_eventcb;

	lv_coord_t shift_plus[6] = {0, 0, 0, 20, 20, 20};//compensation shift	

	lv_obj_t *ta = NULL;
	lv_obj_t *ta_label = NULL;
	for (i = 0; i < ta_size; ++i)
	{
		ta = universal_ta_create(&elm_ta, ta_txt[i], NULL, true, false, true, sigma_star_theme_dial_ta);
		lv_textarea_set_max_length(ta, 4);
		screen_adaption_lvgl_obj_set_pos(ta, 15 + (800 - 30) / 7 * i + shift_plus[i], 0);
		*(ta_array + i) = ta;

		ta_label = lv_label_create(ta_bg, NULL);
		lv_label_set_text(ta_label, ta_name[i]);
		lv_theme_apply(ta_label, (lv_theme_style_t)sigma_star_theme_label_chinese_normal);
		screen_adaption_lvgl_obj_align(ta_label, ta, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
	}
	lv_textarea_set_cursor_hidden(ta, false); //choose the last one as the default seleted ta
	page_data->selectd_ta = ta;

	//keyboard
	lv_coord_t kb_bg_width = 769, kb_bg_height = 346;
	lv_coord_t kb_top_space = 12, kb_left_space = 30;
	lv_coord_t kb_coor_x = 16, kb_coor_y = 128;
	lv_coord_t kb_tight_height = 264;
	
	lv_obj_t *kb_bg = universal_keyboard_design(page_bg, kb_bg_width, kb_bg_height, kb_top_space, kb_left_space, sset_addr_eventcb_kb, page_data, Keyboard_design_align_top, kb_tight_height);
	screen_adaption_lvgl_obj_set_pos(kb_bg, kb_coor_x, kb_coor_y);

	//two btns
	_universal_obj_element elm_btn;
	Uni_Element_init(elm_btn);
	elm_btn.parent = kb_bg;
	elm_btn.width = 133;
	elm_btn.height = 47;
	
	const char *btn_txt[2] = { "确认", "取消" };
	//cancel
	lv_obj_t *btn_cancel = universal_btn_label_create(&elm_btn, btn_txt[1], tencell_theme_btn_chinese_normal);
	screen_adaption_lvgl_obj_align(btn_cancel, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -20, -10);
	//confirm
	elm_btn.user_data = page_data;
	elm_btn.event_cb = sset_addr_evnetcb_confirm;
	lv_obj_t *btn_confirm = universal_btn_label_create(&elm_btn, btn_txt[0], tencell_theme_btn_chinese_normal);
	screen_adaption_lvgl_obj_align(btn_confirm, btn_cancel, LV_ALIGN_OUT_LEFT_TOP, -20, 0);
	
}

/******************BELOW**********************************
************System Setting-System Password Block**********
*********************************************************/
/*Format:
***@ Prefix: sset_sys_pwd_
***@ UI draw: sset_sys_pwd_ui_
***@ event_cb: sset_sys_pwd_evnetcb_
*/

/*------------------BELOW----------------------
*************System Password First Page********
----------------------------------------------*/
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
int sset_sys_pwd_ui_del_warning(void *obj_label)
{
	lv_obj_del((lv_obj_t *)obj_label);

	return 0;
}

static void sset_sys_pwd_ui_wrong_warning(char *warning_txt)
{
	lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label, warning_txt);
	lv_theme_apply(label, (lv_theme_style_t)sigma_star_theme_label_chinese);
	screen_adaption_lvgl_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 80);

	universal_task_100ms_timer_create(6, (uint32_t *)&sset_sys_pwd_ui_del_warning, label);
}

/*******************BLOCK************************************
*************System Setting->General Password Set************
************************************************************/
/*Format:
***@ Prefix:sset_genl_pwd_
***@ UI draw:sset_genl_pwd_ui_
***@ event_cb:sset_genl_pwd_eventcb_
*/

/*----------------BELOW----------------------
*************General Password First Page*****
--------------------------------------------*/
/*->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

#if 0
static _pwd_class *g_pwd_class_data = NULL;
static _pwd_class* sset_pwd_data_apply(void)
{
	_pwd_class *all = (_pwd_class *)general_malloc(sizeof(_pwd_class));
	if (is_null_ptr(all))
	{
		ui_printf_malloc_error("sset pwd data apply");
		return NULL;
	}
	memset(all, 0, sizeof(_pwd_class));
	return all;
}

static void sset_pwd_page_clear()
{
	if (is_null_ptr(g_pwd_class_data))
	{
		return;
	}

	int i = 0;
	for (i = 0; i < 4; ++i)
	{
		general_free(g_pwd_class_data->widget[i]);
	}
	general_free(g_pwd_class_data);
	g_pwd_class_data = NULL;	
}
#endif

static _pwd_class *sset_pwd_data_manually(static_operation_t operation)
{
	static _pwd_class *s_pwd_page = NULL;
	
	if (s_operation_apply == operation)
	{
		s_pwd_page = (_pwd_class *)general_malloc(sizeof(_pwd_class));
		if (is_null_ptr(s_pwd_page))
		{
			ui_printf_malloc_error("sset pwd data apply");
			return NULL;
		}
		memset(s_pwd_page, 0, sizeof(_pwd_class));
	}
	else if (s_operation_clear == operation)
	{
		if (is_null_ptr(s_pwd_page))
			return NULL;

		int i = 0;
		for (i = 0; i < 4; ++i)
			general_free(s_pwd_page->widget[i]);

		general_free(s_pwd_page);
		s_pwd_page = NULL;
	}

	return s_pwd_page;
}

LV_EVENT_CB_DECLARE(sset_pwd_eventcb_btn_out_kb)
{
	if (e == LV_EVENT_CLICKED)
	{
		_pwd_class *all = (_pwd_class *)((_pwd_widget *)obj->user_data)->all;
		
		lv_obj_del(all->btnm_bg);
		all->kb_exist = false;
		all->btnm_bg = NULL;
	}
}

LV_EVENT_CB_DECLARE(sset_pwd_eventcb_btnm)
{
	if (e != LV_EVENT_VALUE_CHANGED) return;

	const char *txt = lv_btnmatrix_get_active_btn_text(obj);

	//lv_textarea_add_char((lv_obj_t *)obj->user_data, txt);
	lv_textarea_add_text((lv_obj_t *)obj->user_data, txt);
}

static lv_obj_t *sset_pwd_ui_kb_jump(lv_obj_t *parent, _pwd_widget *widget)
{
	//coordinate
	lv_coord_t width_bg_obj = 350, height_bg_ob = 182;
	lv_coord_t width_btnm = 338, height_btnm = 105;
	lv_coord_t width_btn = 80, height_btn = 40;

	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = parent;
	elm_obj.width = width_bg_obj;
	elm_obj.height = height_bg_ob;
	
	//bg
	lv_obj_t *obj_bg = universal_obj_create(&elm_obj, tencell_theme_pwd_kb_obj_bg);

	//kb
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	elm_obj.width = width_btnm;
	elm_obj.height = height_btnm;
	elm_obj.user_data = widget->ta;
	elm_obj.event_cb = sset_pwd_eventcb_btnm;

	static const char *kb_map[] = { "1", "2", "3", "4", "5", "\n",
							"6", "7", "8", "9", "0", "" };

	const lv_btnmatrix_ctrl_t kb_ctrl_map[] = { 1, 1, 1, 1, 1,
												1, 1, 1, 1, 1 };
	
	lv_obj_t *btnm = universal_btnm_create(&elm_obj, kb_map, kb_ctrl_map, tencell_theme_pwd_kb);
	screen_adaption_lvgl_obj_align(btnm, NULL, LV_ALIGN_IN_TOP_LEFT, (width_bg_obj - width_btnm) / 2, 17);

	//btn
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	elm_obj.width = width_btn;
	elm_obj.height = height_btn;
	elm_obj.user_data = widget;
	elm_obj.event_cb = sset_pwd_eventcb_btn_out_kb;
	lv_obj_t *btn_cancel = universal_btn_label_create(&elm_obj, "取消", tencell_theme_pwd_btn_below_kb);
	screen_adaption_lvgl_obj_align(btn_cancel, NULL, LV_ALIGN_IN_BOTTOM_MID, -40 - 10, -5);
	
	lv_obj_t *btn_confirm = universal_btn_label_create(&elm_obj, "确定", tencell_theme_pwd_btn_below_kb);
	screen_adaption_lvgl_obj_align(btn_confirm, NULL, LV_ALIGN_IN_BOTTOM_MID, 40 + 10, -5);

	//take out data
	_pwd_class *all = (_pwd_class *)widget->all;
	all->btnm_bg = obj_bg;

	return obj_bg;	
}

LV_EVENT_CB_DECLARE(sset_pwd_eventcb_save_btn)
{
	if (e == LV_EVENT_CLICKED && !is_null_ptr(obj->user_data))
	{
		_pwd_widget *d_widget = (_pwd_widget *)obj->user_data;
		_pwd_class *all = (_pwd_class *)d_widget->all;
		pwd_mode_type mode[2] = { sset_pwd_mode_system, sset_pwd_mode_security };
		//not again
		if (d_widget->index % 2 != 1)
			return;

		const char *txt_set = lv_textarea_get_text(all->widget[d_widget->index - 1]->ta);
		const char *txt_again = lv_textarea_get_text(d_widget->ta);

		if (strcmp(txt_set, txt_again) != 0)
		{
			sset_sys_pwd_ui_wrong_warning("新密码不一致");
			return;
		}
		if (strlen(txt_set) != PASSWORD_MAX_LEN - 1)
		{
			sset_sys_pwd_ui_wrong_warning("请输入6位密码");
			return;
		}

		if (sset_pwd_set(mode[(int)d_widget->index / 2], txt_again) != 0)
		{
			ui_printf_plus("pwd change, set error");
			sset_sys_pwd_ui_wrong_warning("Error!");
			//to do
			return;
		}
		lv_textarea_set_text(all->widget[d_widget->index - 1]->ta, "");
		lv_textarea_set_text(d_widget->ta, "");
		if (all->kb_exist)
		{
			lv_obj_del(all->btnm_bg);
			all->kb_exist = false;
		}
		
	}
}

LV_EVENT_CB_DECLARE(sset_pwd_eventcb_del_btn)
{
	if (e == LV_EVENT_CLICKED && !is_null_ptr(obj->user_data))
	{
		_pwd_widget *widget = (_pwd_widget *)obj->user_data;
		
		lv_textarea_del_char(widget->ta);
	}
}

LV_EVENT_CB_DECLARE(sset_pwd_eventcb_ta)
{
	if (e == LV_EVENT_CLICKED && !is_null_ptr(obj->user_data))//!is_null_ptr(obj->user_data)
	{
		ui_printf_plus("sset_pwd_eventcb_ta");
		_pwd_widget *d_widget = (_pwd_widget *)obj->user_data;
		_pwd_class *all = (_pwd_class *)((_pwd_widget *)obj->user_data)->all;
		if (all->kb_exist) {
			ui_printf_plus("kb exist");
			return;
		}
		lv_align_t align[2] = { LV_ALIGN_OUT_BOTTOM_LEFT, LV_ALIGN_OUT_TOP_LEFT };
		lv_coord_t ver_space[2] = { 10, -10 };

		lv_obj_t *page_bg = lv_obj_get_parent(lv_obj_get_parent(obj));
		lv_obj_t *kb_bg = sset_pwd_ui_kb_jump(page_bg, (_pwd_widget *)obj->user_data);
		//screen_adaption_lvgl_obj_align(kb_bg, obj, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
		screen_adaption_lvgl_obj_align(kb_bg, obj, align[(int)(d_widget->index / 2)], 0, ver_space[(int)(d_widget->index / 2)]);
		
		all->kb_exist = true;
	}
}

static lv_obj_t* sset_pwd_ui_widget_duplicate(lv_obj_t *parent, int index, _pwd_class *pwd_data)
{
	const char *tag_txt[] = { "系统设置密码:", "再次输入确认:", "安防密码:", "再次输入确认:" };
	#if 0
	_obj_user_data_type *obj_data = (_obj_user_data_type *)general_malloc(sizeof(_obj_user_data_type));
	if (is_null_ptr(obj_data))
	{
		ui_printf_plus("pwd ui widget duplicate, ob_data apply fail");
		return NULL;
	}
	obj_data->index = index;
	#endif
	_pwd_widget *d_widget = (_pwd_widget *)general_malloc(sizeof(_pwd_widget));
	if (is_null_ptr(d_widget))
	{
		//to do,throw out the error signal
		ui_printf_malloc_error("pwd_widget data");
		return NULL;
	}
	d_widget->all = pwd_data;
	d_widget->index = index;
	pwd_data->widget[index] = d_widget;

	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = parent;
	elm_obj.width = 740;
	elm_obj.height = 55;

	//union bg to hold all objs
	lv_obj_t *obj_bg = universal_obj_create(&elm_obj, tencell_theme_pwd_obj_bg);//tencell_theme_pwd_obj_bg

	//textarea
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	elm_obj.width = 350;
	elm_obj.height = 46;
	elm_obj.event_cb = sset_pwd_eventcb_ta;
	elm_obj.user_data = d_widget;
	lv_obj_t *ta = universal_ta_create(&elm_obj, "", "请输入密码", true, true, true, tencell_theme_pwd_ta);//tencell_theme_pwd_ta
	screen_adaption_lvgl_obj_set_pos(ta, 190 - 40, 2);
	d_widget->ta = ta;
	
	//label tag
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	lv_obj_t *label_tag = universal_label_create(&elm_obj, tag_txt[index], sigma_star_theme_label_chinese_normal);
	screen_adaption_lvgl_obj_align(label_tag, ta, LV_ALIGN_OUT_LEFT_MID, -10, 0);

	//del btn
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	elm_obj.width = 101;
	elm_obj.height = 47;
	elm_obj.user_data = d_widget;
	elm_obj.event_cb = sset_pwd_eventcb_del_btn;
	
	lv_obj_t *btn_del = universal_btn_label_create(&elm_obj, NULL, tencell_theme_btn_chinese_normal);
	screen_adaption_lvgl_obj_align(btn_del, ta, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
	
	Uni_Element_init(elm_obj);
	elm_obj.parent = btn_del;
	
	lv_obj_t *img = universal_image_create(&elm_obj, bin_path_img_del_icon, tencell_theme_NULL);

	//save btn
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	elm_obj.width = 111;
	elm_obj.height = 47;
	elm_obj.user_data = d_widget;
	elm_obj.event_cb = sset_pwd_eventcb_save_btn;
	
	lv_obj_t *save_btn = universal_btn_label_create(&elm_obj, "保存", tencell_theme_btn_chinese_normal);
	screen_adaption_lvgl_obj_align(save_btn, btn_del, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

	return obj_bg;
}

/*
 * 密码设置界面创建
*/
static void sset_genl_pwd_ui_page_create(void)
{
	//wideget_heiget: see "sset_pwd_ui_widget_duplicate", the height of widget bg
	int i;
	lv_coord_t widget_base_x = 30, widget_base_y = 128;
	lv_coord_t widget_height = 55, widget_inner_space = 15;
	lv_obj_t *page_bg = sset_ui_page_enter(sset_page_password);

	_pwd_class *pwd_data = sset_pwd_data_manually(s_operation_apply);//sset_pwd_data_apply();
	if (is_null_ptr(pwd_data))
	{
		//to do
		//throw out the error signal 
		return;
	}
	memset(pwd_data, 0, sizeof(_pwd_class));

	for (i = 0; i < 4; ++i)
	{
		lv_obj_t *widget_sys = sset_pwd_ui_widget_duplicate(page_bg, i, pwd_data);
		screen_adaption_lvgl_obj_set_pos(widget_sys, widget_base_x, widget_base_y + i * widget_height + i * widget_inner_space); //128
	}	
}
/*<<<<<<<<<<<<<<<<<End Of General Password Page*/

/*********************BLOCK**********************
************System Setting->Screen Set*********** 
************************************************/
/*Format:
***@ Prefix: sset_screen_
***@ UI draw: sset_screen_ui_
***@ event_cb: sset_screen_eventcb_
*/

/****Below
***Screen Set
***/
/*->>>>>>>>>>>>>>>*/

LV_EVENT_CB_DECLARE(sset_screen_protect_eventcb_list_btn)
{
	if (e == LV_EVENT_CLICKED)
	{
		lv_obj_del(lv_obj_get_parent(lv_obj_get_parent(obj)));
	}
}

static void sset_screen_ui_time_list_create(lv_obj_t *time_btn)
{
	int i = 0;
	lv_obj_t *list_btn = NULL;
	lv_obj_t *bg = lv_obj_get_parent(time_btn);
	const char *time_mode[] = { "30秒", "1分钟", "2分钟", "5分钟" };

	static lv_style_t style_list;
	lv_style_init(&style_list);
	lv_style_set_bg_color(&style_list, LV_STATE_DEFAULT, TENCELL_COLOR_DARK_BLUE);
	lv_style_set_pad_all(&style_list, LV_STATE_DEFAULT, 3);
	lv_style_set_pad_inner(&style_list, LV_STATE_DEFAULT, 10);
	//lv_style_set_bg_opa(&style_list, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_style_set_border_width(&style_list, LV_STATE_DEFAULT, 0);
	lv_style_set_radius(&style_list, LV_STATE_DEFAULT, 5);

	_universal_obj_element elm_list;
	Uni_Element_init(elm_list);
	elm_list.parent = bg;
	elm_list.width = lv_obj_get_width(time_btn);
	elm_list.height = lv_obj_get_height(time_btn) * 5;

	lv_obj_t *list = universal_list_create(&elm_list, tencell_theme_NULL);
	//screen_adaption_lvgl_obj_align(list, time_btn, LV_ALIGN_OUT_TOP_LEFT, 0, 0);
	screen_adaption_lvgl_obj_set_pos(list, lv_obj_get_x(time_btn), lv_obj_get_y(time_btn));
	lv_obj_add_style(list, LV_LIST_PART_BG, &style_list);

	for (i = 0; i < 4; i++)
	{
		list_btn = lv_list_add_btn(list, NULL, time_mode[i]);
		lv_theme_apply(list_btn, tencell_theme_btn_list_screen_time);
		lv_obj_set_event_cb(list_btn, sset_screen_protect_eventcb_list_btn);
	}
}

LV_EVENT_CB_DECLARE(sset_screen_eventcb_time_btn)
{
	if (e == LV_EVENT_CLICKED)
	{
		sset_screen_ui_time_list_create(obj);
	}
}

/*
 * 屏保界面创建
*/
static void sset_screen_ui_page_create(void)
{
	lv_coord_t time_bg_x = 201, time_bg_y = 150;
	lv_coord_t photo_bg_x = time_bg_x, photo_bg_y = 287;
	lv_coord_t left_label_space = 10;
	lv_coord_t left_inner_space = 20;
	lv_obj_t *obj_btn = NULL, *obj_label = NULL;
	const char *time_mode[] = { "30秒", "1分钟", "2分钟", "5分钟" };

	lv_obj_t *page_bg = sset_ui_page_enter(sset_page_screen_protect);

	/*Time*/
	_universal_obj_element elm_btn;
	Uni_Element_init(elm_btn);
	elm_btn.parent = page_bg;
	elm_btn.width = 531;
	elm_btn.height = 46;
	elm_btn.event_cb = sset_screen_eventcb_time_btn;

	_universal_obj_element elm_label;
	Uni_Element_init(elm_label);
	//main cont btn
	obj_btn = universal_btn_label_create(&elm_btn, NULL, tencell_theme_btn_dark_bg);
	lv_btn_set_layout(obj_btn, LV_LAYOUT_OFF);
	screen_adaption_lvgl_obj_set_pos(obj_btn, time_bg_x, time_bg_y);
	//content label
	elm_label.parent = obj_btn;
	obj_label = universal_label_create(&elm_label, time_mode[0], sigma_star_theme_label_chinese_normal);
	screen_adaption_lvgl_obj_align(obj_label, NULL, LV_ALIGN_IN_LEFT_MID, left_inner_space, 0);
	
	//to do
	//a down arror there
	elm_label.parent = page_bg;
	obj_label = universal_label_create(&elm_btn, "关屏时间设置:", sigma_star_theme_label_chinese_normal);
	screen_adaption_lvgl_obj_align(obj_label, obj_btn, LV_ALIGN_OUT_LEFT_MID, -left_label_space, 0);

	/*Photo*/
	elm_btn.event_cb = NULL;
	elm_btn.width = 361;
	elm_btn.height = 46;
	//main obj bg
	lv_obj_t *obj_bg = universal_obj_create(&elm_btn, tencell_theme_obj_bg_cont_kb);
	screen_adaption_lvgl_obj_set_pos(obj_bg, photo_bg_x, photo_bg_y);
	//content label
	elm_label.parent = obj_bg;
	obj_label = universal_label_create(&elm_label, "test.jpg", sigma_star_theme_label_chinese_normal);
	screen_adaption_lvgl_obj_align(obj_label, NULL, LV_ALIGN_IN_LEFT_MID, left_inner_space, 0);
	//choose btn
	elm_btn.width = 133;
	elm_btn.height = 47;
	obj_btn = universal_btn_label_create(&elm_btn, "选择图片", tencell_theme_btn_chinese_normal);
	screen_adaption_lvgl_obj_align(obj_btn, obj_bg, LV_ALIGN_OUT_RIGHT_TOP, 35, 0);

	elm_label.parent = page_bg;
	obj_label = universal_label_create(&elm_btn, "屏保图片选择:", sigma_star_theme_label_chinese_normal);
	screen_adaption_lvgl_obj_align(obj_label, obj_bg, LV_ALIGN_OUT_LEFT_MID, -left_label_space, 0);
}

/***************BLOCK******************************
**************System setting->Date & Time Set******
***************************************************/
/*Format:
***@ Prefix: sset_date_
***@ UI draw: ssete_date_ui_
***@ event_cb: sset_date_eventcb_
*/

/***Below
>>Date & Time
*/
/*>>>>>>>>>>>>>>>>>>>>>>>>>*/


static _date_time_page_mem* sset_date_time_mem_manually(static_operation_t operate)
{
	static _date_time_page_mem *s_date_time = NULL;

	if (s_operation_apply == operate)
	{
		s_date_time = (_date_time_page_mem *)general_malloc(sizeof(_date_time_page_mem));
		if (is_null_ptr(s_date_time))
		{
			//to do
			ui_printf_malloc_error("date time page memory fail");
			return NULL;
		}
		return s_date_time;
	}
	else if (s_operation_clear == operate)
	{
		if (!is_null_ptr(s_date_time))
		{
			int i;
			for (i = 0; i < 2; ++i)
			{
				if (!is_null_ptr(s_date_time->widget[i]))
					general_free(s_date_time->widget[i]);
			}
			general_free(s_date_time);
			s_date_time = NULL;
		}
	}
	return NULL;
}

static void sset_date_time_refresh_text(sset_date_time_item_t which_item, char *des, const _date_time_widget *d_widget)
{
	switch (which_item)
	{
	case sset_date_time_item_time:
	{
		ui_printf_plus("refresh date, time");
		char hour_str[4] = { 0 }, minute_str[4] = { 0 };
		lv_roller_get_selected_str(d_widget->roller[0], hour_str, 4);
		lv_roller_get_selected_str(d_widget->roller[1], minute_str, 4);
		sprintf(des, "%s:%s", hour_str, minute_str);
	}		
		break;

	case sset_date_time_item_date:
	{
		ui_printf_plus("refresh date, date");
		char year_str[10] = { 0 }, month_str[8] = { 0 }, day_str[8] = { 0 };
		lv_roller_get_selected_str(d_widget->roller[0], year_str, 10);
		lv_roller_get_selected_str(d_widget->roller[1], month_str, 8);
		lv_roller_get_selected_str(d_widget->roller[2], day_str, 8);
		sprintf(des, "%s%s%s", year_str, month_str, day_str);
	}
		break;

	default:
		break;
	}
}

LV_EVENT_CB_DECLARE(sset_date_time_eventcb_btn_confirm)
{
	if (e == LV_EVENT_CLICKED && !is_null_ptr(obj->user_data))
	{
		char hour_str[4] = { 0 }, minute_str[4] = { 0 };
		char title_str[30] = { 0 };
		_date_time_widget *d_widget = (_date_time_widget *)obj->user_data;
		//lv_roller_get_selected_str(d_widget->roller[0], hour_str, 4);
		//lv_roller_get_selected_str(d_widget->roller[1], minute_str, 4);
		//sprintf(title_str, "%s:%s", hour_str, minute_str);
		sset_date_time_refresh_text(d_widget->which_item, title_str, d_widget);
		lv_label_set_text(d_widget->label_title, title_str);
		lv_label_set_text(d_widget->label_display, title_str);
		lv_obj_del(d_widget->jump_bg);
	}
}

LV_EVENT_CB_DECLARE(sset_date_time_eventcb_btn_cancel)
{
	if (e == LV_EVENT_CLICKED)
	{
		lv_obj_del(lv_obj_get_parent(obj));
	}
}


static lv_obj_t *sset_date_time_ui_jump_bg_create(lv_obj_t *parent, _date_time_widget *d_widget, int roller_cnt)
{
	//init
	int i = 0;
	lv_obj_t *label_display = d_widget->label_display;
	const char *display_txt = lv_label_get_text(label_display);
	
	lv_obj_t *obj_roller = NULL;
	lv_obj_t *arr_roller[3] = { 0 };
	const char *roller_option[3] = { 0 };
	uint16_t roller_sel_opt[3] = { 0 };

	char option_1[1024] = { 0 };
	char option_2[300] = { 0 };
	char option_3[300] = { 0 };
	int last_char = 0;
	if (roller_cnt == 2)//time
	{
		char h_temp[4] = { 0 };
		for (i = 0; i < 24; ++i)//hour
		{
			sprintf(h_temp, "%02d\n", i);
			strcat(option_1, h_temp);
		}
		last_char = (int)strlen(option_1) - 1;
		option_1[last_char] = '\0';

		for (i = 0; i < 60; ++i)//minute
		{
			sprintf(h_temp, "%02d\n", i);
			strcat(option_2, h_temp);
		}
		last_char = (int)strlen(option_2) - 1;
		option_2[last_char] = '\0';

		int16_t hour_opt = (*display_txt - '0') * 10 + (*(display_txt + 1) - '0');
		int16_t minute_opt = (*(display_txt + 3) - '0') * 10 + *(display_txt + 4) - '0';
		roller_sel_opt[0] = hour_opt;
		roller_sel_opt[1] = minute_opt;
	}
	else//date
	{
		//set options
		char h_temp[10] = { 0 };
		for (i = 2000; i <= 2100; ++i)//year
		{
			sprintf(h_temp, "%d年\n", i);
			strcat(option_1, h_temp);
		}
		last_char = (int)strlen(option_1) - 1;
		option_1[last_char] = '\0';

		for (i = 1; i <= 12; ++i)
		{
			sprintf(h_temp, "%d月\n", i);
			strcat(option_2, h_temp);
		}
		last_char = (int)strlen(option_2) - 1;
		option_2[last_char] = '\0';

		for (i = 1; i <= 31; ++i)
		{
			sprintf(h_temp, "%d日\n", i);
			strcat(option_3, h_temp);
		}
		last_char = (int)strlen(option_3) - 1;
		option_3[last_char] = '\0';

		//calculate opt by display text
		const char *p_char = display_txt;
		char str_take[5];
		int16_t year_opt, month_opt, day_opt;
		memcpy(str_take, p_char, sizeof(char) * 4);
		year_opt = atoi(str_take) - 2000;
		p_char = strstr(display_txt, "月");
		uint8_t char_cnt = 0;
		p_char--;
		while (*(p_char) <= '9' && *(p_char) >= '0')
		{
			p_char--;
			char_cnt++;
		}
		p_char++;
		memcpy(str_take, p_char, char_cnt);
		str_take[char_cnt] = '\0';
		month_opt = atoi(str_take) - 1;

		p_char = strstr(display_txt, "日");
		p_char--;
		char_cnt = 0;
		while (*(p_char) <= '9' && *(p_char) >= '0')
		{
			p_char--;
			char_cnt++;
		}
		p_char++;
		//ui_printf_plus("char_cnt:%d", char_cnt);
		memcpy(str_take, p_char, char_cnt);
		str_take[char_cnt] = '\0';
		day_opt = atoi(str_take) - 1;

		roller_sel_opt[0] = year_opt;
		roller_sel_opt[1] = month_opt;
		roller_sel_opt[2] = day_opt;
	}
	roller_option[0] = option_1;
	roller_option[1] = option_2;
	roller_option[2] = option_3;
	//roller_sel_opt[0] = hour_opt;
	//roller_sel_opt[1] = minute_opt;
	
	//x:207, y:72
	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = parent;
	elm_obj.width = 386;
	elm_obj.height = 336;

	lv_obj_t *obj_bg = universal_obj_create(&elm_obj, tencell_theme_temp_obj);

	//title
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	
	lv_obj_t *label_title = universal_label_create(&elm_obj, display_txt, sigma_star_theme_label_chinese_normal);
	screen_adaption_lvgl_obj_align(label_title, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);

	static lv_style_t style_bg;
	lv_style_init(&style_bg);
	lv_style_set_bg_opa(&style_bg, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_text_font(&style_bg, LV_STATE_DEFAULT, &THEME_CHINESE_FONT_NORMAL);
	lv_style_set_text_letter_space(&style_bg, LV_STATE_DEFAULT, 3);
	lv_style_set_text_color(&style_bg, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x7a, 0x9e, 0x95));
	lv_style_set_pad_all(&style_bg, LV_STATE_DEFAULT, 20);
	lv_style_set_text_line_space(&style_bg, LV_STATE_DEFAULT, 20);

	//roller
	static lv_style_t style_roller_bg;
	lv_style_init(&style_roller_bg);
	lv_style_set_bg_opa(&style_roller_bg, LV_STATE_DEFAULT, LV_OPA_TRANSP);//LV_OPA_TRANSP
	lv_style_set_text_font(&style_roller_bg, LV_STATE_DEFAULT, &THEME_CHINESE_FONT_NORMAL);//lv_font_montserrat_28
	lv_style_set_text_letter_space(&style_roller_bg, LV_STATE_DEFAULT, 3);
	lv_style_set_text_color(&style_roller_bg, LV_STATE_DEFAULT, LV_COLOR_MAKE(0x7a, 0x9e, 0x95));
	lv_style_set_pad_inner(&style_roller_bg, LV_STATE_DEFAULT, 20);
	//lv_style_set_pad_left(&style_roller_bg, LV_STATE_DEFAULT, 10);
	lv_style_set_text_line_space(&style_roller_bg, LV_STATE_DEFAULT, 20);
	lv_style_set_border_width(&style_roller_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_border_width(&style_roller_bg, LV_STATE_FOCUSED, 0);
	lv_style_set_pad_all(&style_roller_bg, LV_STATE_DEFAULT, 20);

	static lv_style_t style_roller_sel;
	lv_style_init(&style_roller_sel);
	lv_style_set_text_letter_space(&style_roller_sel, LV_STATE_DEFAULT, 3);
	lv_style_set_text_color(&style_roller_sel, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_bg_opa(&style_roller_sel, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	//lv_style_set_bg_color(&style_roller_sel, LV_STATE_DEFAULT, TENCELL_RM_COLOR_WATER_BLUE);
	lv_style_set_text_font(&style_roller_sel, LV_STATE_DEFAULT, &THEME_CHINESE_FONT_NORMAL);//THEME_CHINESE_FONT_LARGE
	lv_style_set_border_width(&style_roller_sel, LV_STATE_DEFAULT, 0);
	lv_style_set_border_width(&style_roller_sel, LV_STATE_FOCUSED, 0);
	//lv_style_set_pad_all(&style_roller_sel, LV_STATE_DEFAULT, 10);
	
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	elm_obj.width = 100;
	elm_obj.height = 250;

	lv_obj_add_style(obj_bg, LV_OBJ_PART_MAIN, &style_bg);
	
	_universal_roller_value_t v_roller;
	memset(&v_roller, 0, sizeof(_universal_roller_value_t));
	v_roller.mode = LV_ROLLER_MODE_INIFINITE;
	v_roller.row_cnt = 5;
	
	lv_coord_t roller_space = lv_obj_get_width(obj_bg) / roller_cnt;
	lv_coord_t roller_width = 0;
	
	for (i = 0; i < roller_cnt; i++)//roller_cnt
	{
		if (roller_cnt == 3 && i == 0)
			v_roller.mode = LV_ROLLER_MODE_NORMAL;
		else
			v_roller.mode = LV_ROLLER_MODE_INIFINITE;

		obj_roller = universal_roller_create(&elm_obj, roller_option[i], &v_roller, tencell_theme_NULL);
		//screen_adaption_lvgl_obj_align(obj_roller, NULL, LV_ALIGN_IN_TOP_LEFT, 60, 50);
		
		roller_width = lv_obj_get_width(obj_roller);
		screen_adaption_lvgl_obj_set_pos(obj_roller, (roller_space - roller_width) / 2 + i * roller_space, 50);
		
		//lv_roller_set_visible_row_count(obj_roller, v_roller.row_cnt);
		arr_roller[i] = obj_roller;

		lv_obj_add_style(obj_roller, LV_ROLLER_PART_BG, &style_roller_bg);
		lv_obj_add_style(obj_roller, LV_ROLLER_PART_SELECTED, &style_roller_sel);
		
		lv_roller_set_selected(obj_roller, roller_sel_opt[i], LV_ANIM_OFF);//roller_sel_opt[i]
	}	

	//btn
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	elm_obj.width = 80;
	elm_obj.height = 40;
	elm_obj.user_data = d_widget;
	elm_obj.event_cb = sset_date_time_eventcb_btn_cancel;
	
	lv_obj_t *btn_cancel = universal_btn_label_create(&elm_obj, "取消", tencell_theme_date_time_btn_below_roller);
	screen_adaption_lvgl_obj_align(btn_cancel, NULL, LV_ALIGN_IN_BOTTOM_MID, -60, -8);

	elm_obj.event_cb = sset_date_time_eventcb_btn_confirm;
	lv_obj_t *btn_confirm = universal_btn_label_create(&elm_obj, "确定", tencell_theme_date_time_btn_below_roller);
	screen_adaption_lvgl_obj_align(btn_confirm, NULL, LV_ALIGN_IN_BOTTOM_MID, 60, -8);

	//take out data
	d_widget->jump_bg = obj_bg;
	d_widget->label_title = label_title;
	//d_widget->roller[0] = roller_hour;
	//d_widget->roller[1] = roller_minute;
	for (i = 0; i < roller_cnt; ++i)
	{
		d_widget->roller[i] = arr_roller[i];
	}

	return obj_bg;
}

static lv_obj_t *sset_date_time_ui_time_jump(lv_obj_t *parent, _date_time_widget *d_widget)
{
	int roller_cnt[] = { 2, 3 };
	lv_obj_t *bg = sset_date_time_ui_jump_bg_create(parent, d_widget, roller_cnt[d_widget->which_item]);
	screen_adaption_lvgl_obj_set_pos(bg, 207, 72);

	return bg;
}

LV_EVENT_CB_DECLARE(sset_date_time_eventcb_time_select)
{
	if (e == LV_EVENT_CLICKED)
	{
		sset_date_time_ui_time_jump(lv_obj_get_parent(lv_obj_get_parent(obj)), (_date_time_widget *)obj->user_data);
	}
}

static lv_obj_t *sset_date_time_ui_widget_duplicate(lv_obj_t *parent, int index, _date_time_page_mem *s_page_mem)
{
	//init
	const char *tag_txt[] = { "设置时间:", "设置日期:", "时间格式:" };
	const char *select_txt[] = { "选择", "选择", NULL };
	lv_event_cb_t event[3] = { sset_date_time_eventcb_time_select , sset_date_time_eventcb_time_select, NULL };
	sset_date_time_item_t item_t[3] = { sset_date_time_item_time, sset_date_time_item_date, sset_date_time_item_time_syntax };
	char display_txt[20] = { 0 };
	sset_date_time_get(item_t[index], display_txt);

	_date_time_widget *d_widget = NULL;

	if (index != 2)
	{
		d_widget = (_date_time_widget *)general_malloc(sizeof(_date_time_widget));
		if (is_null_ptr(d_widget))
		{
			//to do
			ui_printf_malloc_error("date time widget fail");
			return NULL;
		}
		d_widget->which_item = item_t[index];
		s_page_mem->widget[index] = d_widget;
	}

	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = parent;
	elm_obj.width = 640;
	elm_obj.height = 50;
	
	//transp bg holds all objs
	lv_obj_t *obj_bg = universal_obj_create(&elm_obj, tencell_theme_obj_transp);//tencell_theme_obj_transp

	//obj display info
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	elm_obj.width = 361;
	elm_obj.height = lv_obj_get_height_fit(obj_bg);
	
	lv_obj_t *obj_txt = universal_obj_create(&elm_obj, tencell_theme_common_obj_dark_gray_bg);
	screen_adaption_lvgl_obj_align(obj_txt, NULL, LV_ALIGN_IN_LEFT_MID, 110, 0);
	
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_txt;
	lv_obj_t *label_txt = universal_label_create(&elm_obj, display_txt, tencell_theme_NULL);
	screen_adaption_lvgl_obj_align(label_txt, NULL, LV_ALIGN_IN_LEFT_MID, 20, 0);
	if (index != 2)
	{
		d_widget->label_display = label_txt;
	}

	//label tag
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;

	lv_obj_t *label_tag = universal_label_create(&elm_obj, tag_txt[index], sigma_star_theme_label_chinese_normal);
	screen_adaption_lvgl_obj_align(label_tag, obj_txt, LV_ALIGN_OUT_LEFT_MID, -15, 0);

	//selece btn
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	elm_obj.width = 133;
	elm_obj.height = 47;
	elm_obj.event_cb = event[index];
	elm_obj.user_data = d_widget;

	lv_obj_t *btn_sel = universal_btn_label_create(&elm_obj, select_txt[index], tencell_theme_btn_chinese_normal);
	screen_adaption_lvgl_obj_align(btn_sel, NULL, LV_ALIGN_IN_RIGHT_MID, 0, 0);

	return obj_bg;
}

/*
 * 日期时间界面设置
*/
static void sset_date_ui_page_create(void)
{
	int i = 0;
	lv_obj_t *widget_bg = NULL;
	lv_coord_t widget_x = 73, widget_y = 155;
	lv_coord_t widget_height = 50, widget_ver_space = 96 - 50;
	lv_obj_t *page_bg = sset_ui_page_enter(sset_page_date_time);

	_date_time_page_mem *s_mem = (_date_time_page_mem *)general_malloc(sizeof(_date_time_page_mem));
	if (is_null_ptr(s_mem))
	{
		ui_printf_malloc_error("date time page memory fail");
		return;
	}

	for (i = 0; i < 3; ++i)
	{
		widget_bg = sset_date_time_ui_widget_duplicate(page_bg, i, s_mem);
		screen_adaption_lvgl_obj_set_pos(widget_bg, widget_x, widget_y + (widget_height + widget_ver_space) * i);
	}
}

/*******************BLOCK************************************
****************System Setting-> Sound***********************
************************************************************/
/*Format:
***@ Prefix:sset_sound_
***@ Ui draw: sset_sound_ui
***@ event_cb: sset_sound_eventcb_
*/

/***Below
*******Sound First Page
*/
/*->>>>>>>>>>>>>>>>>>>>*/
#if 0
/*
***@Para-> ring_type: now seleted ring_index
***Return: 0 if success or -1 invalid ring
*/

static int sset_sound_ring_data_signal(uint8_t ring_type)
{
	if (ring_type >= ring_index_max)
		return -1;

	datajs_set_sound_item_value_separately(item_sound_type, ring_type);
	return 0;
}
#endif

LV_EVENT_CB_DECLARE(sset_sound_eventcb_vol_btn_up)
{
	if (e == LV_EVENT_CLICKED && !is_null_ptr(obj->user_data))
	{
		_sound_widget_item *widget = (_sound_widget_item *)obj->user_data;
		lv_obj_t *bar = widget->bar;
		int16_t volume = lv_bar_get_value(bar);
		int16_t v_max = lv_bar_get_max_value(bar);
		if ( volume >= v_max)
			return;
		#ifdef _VS_SIMULATOR
		int index = (int)(uint8_t)bar->user_data;
		#else
		int index = (int)bar->user_data;
		#endif
		
		lv_bar_set_value(bar, ++volume, LV_ANIM_ON);

		char percent_txt[10] = { 0 };
		volume_to_percent(volume, v_max, percent_txt);
		lv_label_set_text(widget->label_percent, percent_txt);

		sset_sound_item_t item_vol[] = { item_sound_voice, item_sound_touch, item_sound_bell, item_sound_microphone }; //what's the signal voice?
		sset_sound_volume_set(item_vol[index], (uint8_t)volume);
	}
}

LV_EVENT_CB_DECLARE(sset_sound_eventcb_vol_btn_down)
{
	if (e == LV_EVENT_CLICKED && !is_null_ptr(obj->user_data))
	{
		_sound_widget_item *widget = (_sound_widget_item *)obj->user_data;
		lv_obj_t *bar = widget->bar;
		int16_t volume = lv_bar_get_value(bar);
		int16_t v_max = lv_bar_get_max_value(bar);
		if (volume <= lv_bar_get_min_value(bar))
			return;
		#ifdef _VS_SIMULATOR
		int index = (int)(uint8_t)bar->user_data;
		#else
		int index = (int)bar->user_data;
		#endif		
		
		lv_bar_set_value(bar, --volume, LV_ANIM_ON);

		char percent_txt[10] = { 0 };
		volume_to_percent(volume, v_max, percent_txt);
		lv_label_set_text(widget->label_percent, percent_txt);

		sset_sound_item_t item_vol[] = { item_sound_voice, item_sound_touch, item_sound_bell, item_sound_microphone }; //what's the signal voice?
		sset_sound_volume_set(item_vol[index], (uint8_t)volume);
	}
}



static _sound_page_data *sset_sound_data_manually(static_operation_t operate)
{
	static _sound_page_data *s_sound_page = NULL;
	if (s_operation_apply == operate)
	{
		s_sound_page = general_malloc(sizeof(_sound_page_data));
		if (is_null_ptr(s_sound_page))
		{
			ui_printf_malloc_error("sound page data fail");
			//to do, throw out the signal
			return NULL;
		}
		return s_sound_page;
	}
	else if(s_operation_clear == operate)
	{
		if (is_null_ptr(s_sound_page))
			return NULL;

		int i = 0;
		for (i = 0; i < 4; ++i)
		{
			if (!is_null_ptr(s_sound_page->widget[i]))
				general_free(s_sound_page->widget[i]);
		}
		for (i = 0; i < 2; ++i)
		{
			if (!is_null_ptr(s_sound_page->w_list[i]))
				general_free(s_sound_page->w_list[i]);
		}
		
		general_free(s_sound_page);
		s_sound_page = NULL;
	}
	return NULL;
}


static lv_obj_t *sset_sound_ui_volume_duplicate(lv_obj_t *parent, int index, _sound_page_data *s_data)
{
	//data
	_sound_widget_item *widget = (_sound_widget_item *)general_malloc(sizeof(_sound_widget_item));
	if (is_null_ptr(widget))
		return NULL;

	//tag
	const char *tag_txt[] = { "对讲喇叭音量:", "提示音音量:", "铃声音量:", "麦克风音量:" };
	sset_sound_item_t item_vol[] = { item_sound_voice, item_sound_touch, item_sound_bell, item_sound_microphone }; //what's the signal voice?
	int16_t get_volume = (int16_t)sset_sound_volume_get(item_vol[index]);
	if (get_volume < 0)
		get_volume = 0;

	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = parent;
	elm_obj.width = 530;
	elm_obj.height = 50;	

	//base bg
	lv_obj_t *obj_bg = universal_obj_create(&elm_obj, tencell_theme_obj_transp);
	
	//slider
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	elm_obj.width = 214;
	elm_obj.height = 39;
	#ifdef _VS_SIMULATOR
	elm_obj.user_data = (int *)(int8_t)index;
	#else
	elm_obj.user_data = index;
	#endif

	_universal_bar_value_t bar_v;
	bar_v.min = 1;
	bar_v.max = 30;
	bar_v.start_value = get_volume;
	bar_v.enable_anim = false;

	lv_obj_t *bar = universal_bar_create(&elm_obj, &bar_v, tencell_theme_bar_volume);
	screen_adaption_lvgl_obj_set_pos(bar, 170 - 34, (50 - 40) / 2);

	//label tag & percent
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	
	lv_obj_t *label = universal_label_create(&elm_obj, tag_txt[index], sigma_star_theme_label_chinese_normal);
	screen_adaption_lvgl_obj_align(label, bar, LV_ALIGN_OUT_LEFT_MID, -5, 0);

	char txt_percent[10];
	//sprintf(txt_percent, "%2d%%", (int)(get_volume * 100 / 30));//turn digit to percent
	//ui_printf_plus("precent txt:%s", txt_percent);
	volume_to_percent(get_volume, bar_v.max, txt_percent);
	label = universal_label_create(&elm_obj, txt_percent, sigma_star_theme_label_chinese_normal);
	screen_adaption_lvgl_obj_align(label, bar, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

	//up down btn
	widget->bar = bar;
	widget->label_percent = label;

	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	elm_obj.width = 49;
	elm_obj.height = 49;
	elm_obj.event_cb = sset_sound_eventcb_vol_btn_down;
	//elm_obj.user_data = bar;
	elm_obj.user_data = widget;
	
	lv_obj_t *btn_down = universal_btn_label_create(&elm_obj, LV_SYMBOL_DOWN, tencell_theme_sound_volume_btn);
	screen_adaption_lvgl_obj_align(btn_down, NULL, LV_ALIGN_IN_RIGHT_MID, -2, 0);
	
	elm_obj.event_cb = sset_sound_eventcb_vol_btn_up;
	lv_obj_t *btn_up = universal_btn_label_create(&elm_obj, LV_SYMBOL_UP, tencell_theme_sound_volume_btn);
	screen_adaption_lvgl_obj_align(btn_up, btn_down, LV_ALIGN_OUT_LEFT_MID, -10, 0);

	//take out value
	
	s_data->widget[index] = widget;

	return obj_bg;
}

LV_EVENT_CB_DECLARE(sset_sound_eventcb_music_list_btn)
{
	if (e == LV_EVENT_CLICKED)
	{
		
		lv_obj_t *list = (lv_obj_t *)obj->user_data;
		lv_obj_t *list_label = lv_list_get_btn_label(obj);
		_sound_list_widget *w_list = (_sound_list_widget *)list->user_data;

		const char *txt_choosen = lv_label_get_text(list_label);
		lv_obj_t *label_music = w_list->label_music;
		lv_label_set_text(label_music, txt_choosen);
		sset_sound_music_set(w_list->which, txt_choosen);
		//delete
		lv_obj_del(list);
		w_list->list = NULL;
	}
}

static lv_obj_t *sset_sound_ui_drop_list_jump(lv_obj_t *parent, const char *arr_txt[], int txt_count, _sound_list_widget *w_list)
{
	int i = 0;
	lv_obj_t *list_btn = NULL;

	static lv_style_t style_list;
	lv_style_init(&style_list);
	lv_style_set_bg_color(&style_list, LV_STATE_DEFAULT, TENCELL_RM_COLOR_CYAN_0x00604d);
	lv_style_set_radius(&style_list, LV_STATE_DEFAULT, 10);
	lv_style_set_pad_all(&style_list, LV_STATE_DEFAULT, 3);

	_universal_obj_element elm_list;
	Uni_Element_init(elm_list);
	elm_list.parent = parent;
	elm_list.width = 182;
	elm_list.height = 46 * 3;
	elm_list.user_data = w_list;

	lv_obj_t *obj_list = universal_list_create(&elm_list, tencell_theme_NULL);
	lv_obj_add_style(obj_list, LV_LIST_PART_BG, &style_list);

	for (i = 0; i < txt_count; ++i)
	{
		list_btn = lv_list_add_btn(obj_list, NULL, arr_txt[i]);
		lv_theme_apply(list_btn, (lv_theme_style_t)tencell_theme_sound_list_btn);
		lv_obj_set_event_cb(list_btn, sset_sound_eventcb_music_list_btn);
		list_btn->user_data = obj_list;
	}

	return obj_list;
}

LV_EVENT_CB_DECLARE(sset_sound_eventcb_drop_btn)
{
	//static bool b_list_exist = false;
	if (e == LV_EVENT_CLICKED && !is_null_ptr(obj->user_data))
	{
		_sound_list_widget *w_list = (_sound_list_widget *)obj->user_data;
		
		if (NULL != w_list->list)
		{
			//lv_obj_del((lv_obj_t *)obj->user_data);
			lv_obj_del(w_list->list);
			
			//b_list_exist = false;
			w_list->list = NULL;
			return;
		}
		lv_obj_t *page_bg = lv_obj_get_parent(lv_obj_get_parent(obj));
		const char *temp_text[] = { "tik.wav", "drop.wav", "di.wav", "bellring.wav" };
		char music_library[4][20] = { 0 };
		char *arr_music[] = { music_library[0], music_library[1], music_library[2], music_library[3] };
		int num = sset_sound_music_get_library(w_list->which, arr_music);
		if (num != 4)
		{
			//to do
			//array less than the acutal library
		}

		lv_obj_t *list = sset_sound_ui_drop_list_jump(page_bg, arr_music, 4, w_list);
		screen_adaption_lvgl_obj_align(list, lv_obj_get_parent(obj), LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);
		//obj->user_data = list;
		
		w_list->list = list;
		//b_list_exist = true;
	}
}

static lv_obj_t *sset_sound_ui_drop_down_duplicate(lv_obj_t *parent, _sound_page_data *s_data, int list_which)
{
	//data apply
	_sound_list_widget *w_list = (_sound_list_widget *)general_malloc(sizeof(_sound_list_widget));
	memset(w_list, 0, sizeof(_sound_list_widget));
	if (is_null_ptr(w_list))
	{
		//to to
		ui_printf_malloc_error("sound list widget apply fail");
		return NULL;
	}
	s_data->w_list[list_which] = w_list;

	static lv_style_t style_btn;
	lv_style_init(&style_btn);
	lv_style_set_text_color(&style_btn, LV_STATE_DEFAULT, TENCELL_RM_COLOR_WATER_BLUE);
	lv_style_set_text_font(&style_btn, LV_STATE_DEFAULT, &lv_font_montserrat_28);
	
	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = parent;
	elm_obj.width = 182;
	elm_obj.height = 46;
	
	//obj bg
	lv_obj_t *obj = universal_obj_create(&elm_obj, tencell_theme_common_obj_dark_gray_bg);
	
	//label
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj;

	sset_sound_item_t music_item[2] = { sound_item_music_signal, sound_item_music_ring };
	char get_music[20] = { 0 };
	if (-1 == sset_sound_music_get(music_item[list_which], get_music))
	{
		//to do,
		ui_printf_plus("sset_sound_music_get error");
	}
	
	lv_obj_t *label = universal_label_create(&elm_obj, get_music, sigma_star_theme_label_chinese_normal);
	screen_adaption_lvgl_obj_align(label, NULL, LV_ALIGN_IN_LEFT_MID, 20, 0);

	//btn drop
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj;
	elm_obj.width = 40;
	elm_obj.height = lv_obj_get_height_fit(obj);
	elm_obj.event_cb = sset_sound_eventcb_drop_btn;
	elm_obj.user_data = w_list;
	
	lv_obj_t *btn = universal_btn_label_create(&elm_obj, LV_SYMBOL_DOWN, tencell_theme_sound_arrow_btn);
	screen_adaption_lvgl_obj_align(btn, NULL, LV_ALIGN_IN_RIGHT_MID, -3, 0);
	lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_btn);

	//insert data
	w_list->label_music = label;
	w_list->which = music_item[list_which];
	
	return obj;	
}

/*
 * 音量设置界面创建
*/
static void sset_sound_ui_page_create(void)
{
	int i = 0;
	lv_coord_t widget_x = 34, widget_y = 103;
	lv_coord_t widget_ver_space = 42, widget_height = 50;
	
	lv_obj_t *widget_bg = NULL;
	lv_obj_t *page_bg = sset_ui_page_enter(sset_page_ring);

	_sound_page_data *s_sound_data = sset_sound_data_manually(s_operation_apply);
	if (is_null_ptr(s_sound_data))
		return;

	for (i = 0; i < 4; ++i)
	{
		widget_bg = sset_sound_ui_volume_duplicate(page_bg, i, s_sound_data);
		if (is_null_ptr(widget_bg))
		{
			//to do,fail
			continue;
		}
		screen_adaption_lvgl_obj_set_pos(widget_bg, widget_x, widget_y + (widget_ver_space + widget_height) * i);
	}

	lv_obj_t *obj_drop_down_signal = sset_sound_ui_drop_down_duplicate(page_bg, s_sound_data, 0);
	screen_adaption_lvgl_obj_set_pos(obj_drop_down_signal, 584, 198);
	
	lv_obj_t *obj_drop_down_ring = sset_sound_ui_drop_down_duplicate(page_bg, s_sound_data, 1);
	screen_adaption_lvgl_obj_set_pos(obj_drop_down_ring, 584, 198 + widget_ver_space + widget_height);
}


/*******************BLOCK************************************
*************System Setting Init Menu Page******************* 
************************************************************/
/*Foramt:
***@ Prefix:sset_initM_
***@ UI draw: sset_initM_ui_
***@ event_cb: sset_initM_eventcb_
*/

/*
 * 菜单栏里每个控件的回调函数，在obj的user_data里要保存每个obj按下时要跳转界面的绘制函数，
 * 在这个回调里会自动去执行界面绘制函数进入obj对应的分页面
*/
LV_EVENT_CB_DECLARE(sset_initM_eventcb_auto)//widget_callback_auto
{
	if (e == LV_EVENT_CLICKED)
	{
		if (!is_null_ptr(obj->user_data))
		{
			((void_fun_void)obj->user_data)();
			//lv_obj_del(lv_obj_get_parent(obj));
		}
	}
}

static lv_obj_t *sset_initM_ui_widget_duplicate(lv_obj_t *parent, const void *scr_img, const char *label_str, lv_event_cb_t event, const void_fun_void exe_fun)
{
	//Attention: this widget no need screen adaption, only the position differ
	//so do not use universal_elm, create by self, and use lv_obj_set_size() with size, and screen_adaption_lvgl_obj_align/pos with align/pos
	uint16_t wt_width = 103, wt_height = 134;
	uint16_t img_cont_width = wt_width, img_cont_height = 96;

	lv_obj_t *wt_base = lv_obj_create(parent, NULL);
	lv_obj_set_size(wt_base, wt_width, wt_height);
	lv_obj_set_click(wt_base, true);
	lv_theme_apply(wt_base, (lv_theme_style_t)tencell_theme_obj_transp);
	lv_obj_set_event_cb(wt_base, event);
	wt_base->user_data = exe_fun;
	
	lv_obj_t *wt_cont = lv_cont_create(wt_base, NULL);
	lv_cont_set_fit(wt_cont, LV_FIT_NONE);
	lv_obj_set_size(wt_cont, img_cont_width, img_cont_height);
	lv_theme_apply(wt_cont, (lv_theme_style_t)sigma_star_theme_sysset_widget_cont);
	//lv_obj_set_click(wt_cont, true);
	lv_cont_set_layout(wt_cont, LV_LAYOUT_CENTER);
	//lv_obj_set_event_cb(wt_cont, event);
	//wt_cont->user_data = exe_fun;
	//lv_obj_set_focus_parent(wt_cont, true);
	lv_obj_set_parent_event(wt_cont, true);
	

	lv_obj_t *obj_img = lv_img_create(wt_cont, NULL);
	lv_img_set_src(obj_img, scr_img);

	lv_obj_t *img_label = lv_label_create(wt_base, NULL);
	lv_label_set_text(img_label, label_str);
	lv_theme_apply(img_label, (lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_obj_align(img_label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

	return wt_base;
}

/*
 * 系统设置主界面创建
*/
static void sset_initM_ui_create()
{
	LV_IMG_DECLARE(lovelyghost_6464);
	
	lv_obj_t *bg = sset_ui_page_enter(sset_page_menu);
	
	int widget_cnt = 0;
	const char *title_array[] = { "系统信息", "屏保设置", "系统升级", "房号设置", "时间设置", "密码设置", "铃声设置" };
	const char *icon_img_path_array[] = {bin_path_img_setting_info,
										 bin_path_img_setting_screen_protect,
										 bin_path_img_setting_upgrade,
										 bin_path_img_setting_address,
										 bin_path_img_setting_date_time,
										 bin_path_img_setting_password,
										 bin_path_img_setting_ring};

	void(*auto_exe_fun[])(void) = {	sset_info_ui_page_create,
											sset_screen_ui_page_create ,
											NULL,
											sset_addr_ui_page_create,
											sset_date_ui_page_create ,
											sset_genl_pwd_ui_page_create ,
											sset_sound_ui_page_create };

//widgets
	uint16_t top_begin_x = 48, top_begin_y = 108;
	uint16_t shift_increment_space = 249 - top_begin_x;
	uint16_t bottom_begin_x = top_begin_x, bottom_begin_y = 290;
	uint16_t widget_coor_x = top_begin_x, widget_coor_y = top_begin_y;
	for (widget_cnt = 0; widget_cnt < 7; ++widget_cnt)
	{
		lv_obj_t *set_widget = sset_initM_ui_widget_duplicate(bg, icon_img_path_array[widget_cnt], title_array[widget_cnt], sset_initM_eventcb_auto, auto_exe_fun[widget_cnt]);
		screen_adaption_lvgl_obj_set_pos(set_widget, widget_coor_x, widget_coor_y);
		widget_coor_x += shift_increment_space;
		if (widget_cnt == 3)
		{
			widget_coor_x = bottom_begin_x;
			widget_coor_y = bottom_begin_y;
		}
	}
}

/*===============================
**********Password Check*********
================================*/

LV_EVENT_CB_DECLARE(sset_pwd_check_eventcb_kb)
{
	if (e != LV_EVENT_VALUE_CHANGED) return;

	lv_obj_t *ta = (lv_obj_t *)obj->user_data;

	//delete warning lable "error pwd"
	if (!is_null_ptr(ta->user_data))
	{
		lv_obj_del((lv_obj_t *)ta->user_data);
		ta->user_data = NULL;
	}

	const char *txt = lv_btnmatrix_get_active_btn_text(obj);

	lv_textarea_add_text(ta, txt);
}

LV_EVENT_CB_DECLARE(sset_pwd_check_btn_confirm)
{
	if (e == LV_EVENT_CLICKED)
	{
		lv_obj_t *ta = obj->user_data;
		const char *pwd = lv_textarea_get_text(ta);

		char sys_pwd[12] = { 0 };
		int ret = sset_pwd_get(sset_pwd_mode_system, sys_pwd);

		if (strcmp(pwd, sys_pwd) == 0)
		{
			sset_initM_ui_create();
		}
		else
		{
			lv_textarea_set_text(ta, "");
			//already exist, do nothing
			if (!is_null_ptr(ta->user_data))
				return;

			_universal_obj_element elm_obj;
			Uni_Element_init(elm_obj);
			elm_obj.parent = lv_obj_get_parent(obj);

			lv_obj_t *obj_label = universal_label_create(&elm_obj, "密码错误", sigma_star_theme_label_chinese_normal);
			screen_adaption_lvgl_obj_align(obj_label, ta, LV_ALIGN_IN_RIGHT_MID, -3, 0);

			ta->user_data = obj_label;
		}
	}
}

/*
 * 进入设置前密码验证界面
*/
static void sset_init_ui_pwd_page_create()
{
	//init
	lv_coord_t width_ta_bg = 600;
	lv_coord_t height_kb = 200 / 2;
	lv_obj_t *bg = sset_ui_page_enter(sset_page_pwd_check);

	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = bg;
	elm_obj.width = width_ta_bg;
	elm_obj.height = 70;

	lv_obj_t *bg_obj = universal_obj_create(&elm_obj, tencell_theme_obj_transp);
	screen_adaption_lvgl_obj_align(bg_obj, NULL, LV_ALIGN_CENTER, 0, -50);

	//pwd ta
	Uni_Element_init(elm_obj);
	elm_obj.parent = bg_obj;
	elm_obj.width = 382;
	elm_obj.height = 53;

	lv_obj_t *ta = universal_ta_create(&elm_obj, "", "请输入密码", false, false, false, tencell_theme_pwd_ta);
	screen_adaption_lvgl_obj_align(ta, NULL, LV_ALIGN_CENTER, 50, 0);

	//label tag
	Uni_Element_init(elm_obj);
	elm_obj.parent = bg_obj;
	
	lv_obj_t *label_tag = universal_label_create(&elm_obj, "输入密码:", sigma_star_theme_label_chinese_normal);
	screen_adaption_lvgl_obj_align(label_tag, ta, LV_ALIGN_OUT_LEFT_MID, -10, 0);

	//btn
	Uni_Element_init(elm_obj);
	elm_obj.parent = bg;
	elm_obj.width = 133;
	elm_obj.height = 47;

	elm_obj.event_cb = sset_ui_eventcb_fun_page_quit;
	lv_obj_t *btn_cancel = universal_btn_label_create(&elm_obj, "取消", tencell_theme_common_obj_dark_gray_bg);
	screen_adaption_lvgl_obj_align(btn_cancel, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -32, -50);

	elm_obj.user_data = ta;
	elm_obj.event_cb = sset_pwd_check_btn_confirm;
	lv_obj_t *btn_confirm = universal_btn_label_create(&elm_obj, "确认", tencell_theme_btn_chinese_normal);
	screen_adaption_lvgl_obj_align(btn_confirm, btn_cancel, LV_ALIGN_OUT_LEFT_TOP, -32, 0);

	//kb bg
	Uni_Element_init(elm_obj);
	elm_obj.parent = bg;
	elm_obj.width = 480;
	elm_obj.height = height_kb + 30;

	lv_obj_t *kb_bg = universal_obj_create(&elm_obj, tencell_theme_pwd_kb_obj_bg);
	screen_adaption_lvgl_obj_align(kb_bg, bg_obj, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

	//kb
	Uni_Element_init(elm_obj);
	elm_obj.parent = kb_bg;
	elm_obj.width = 400;//width_ta_bg / 2;
	elm_obj.height = height_kb;
	elm_obj.user_data = ta;
	elm_obj.event_cb = sset_pwd_check_eventcb_kb;

	static const char *kb_map[] = { "1", "2", "3", "4", "5", "\n",
							"6", "7", "8", "9", "0", "" };

	const lv_btnmatrix_ctrl_t kb_ctrl_map[] = { 1, 1, 1, 1, 1,
												1, 1, 1, 1, 1 };

	lv_obj_t *btnm = universal_btnm_create(&elm_obj, kb_map, kb_ctrl_map, tencell_theme_pwd_kb);
	//screen_adaption_lvgl_obj_align(btnm, bg_obj, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
	screen_adaption_lvgl_obj_align(btnm, NULL, LV_ALIGN_CENTER, 0, 0);
}

/*
 * 系统设置进入接口
*/
static void scr_sysytem_setting_create(void)
{
	_system_setting_data_memory *p_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);
	_system_setting_data_dynamic *p_dynamic = (_system_setting_data_dynamic *)get_local_pointer(p_data_dynamic);

	if (p_memory->this_scr == NULL) {
		lv_obj_t *obj_scr = lv_obj_create(NULL,NULL);
		p_memory->this_scr = obj_scr;
	}
	lv_scr_load(p_memory->this_scr);
	//Init Page Stack
	sset_page_stack_Init(p_dynamic);

	sset_ui_page_bg_img_create();

	sset_init_ui_pwd_page_create();
	
}


static void SysSet_return_btn_event_handle(lv_obj_t *obj, lv_event_t e)
{
  if(e == LV_EVENT_CLICKED){
  	SysSet_change_scr(scr_chtype_return,scr_Null);
  	}
}

/*===========================================
***   Exterior  Interface with screen system
=============================================*/
int SysSet_get_local_machine_address(_address_set_type *des_addr)
{
	if (is_null_ptr(des_addr)) return -1;

	_system_setting_data_memory *p_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);
	if (p_memory == NULL) return -1;

	memcpy(des_addr, p_memory->addr_set, sizeof(_address_set_type));

	return 0;
}

/*
*****Descr: get sound volume from js_memory data when init
***Return: volume of item or -1 if wrong
*/
int SysSet_get_memory_sound_volume(uint8_t item)
{
	_system_setting_data_memory *p_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);
	if (p_memory == NULL) return -1;

	uint8_t *p_voice = &p_memory->sound->volume_m.vol_voice;
	return (int)(*(p_voice + item));
}


static void SysSet_change_scr(scr_change_type type,_ui_scr_id scr_id)
{
  gui_sys_screen_event_send(type,scr_id);
}

static void scr_system_setting_quit(void)
{
	sset_page_stack_deInit(SysSet_local.dynamic);

	dynamic_data_free();
	lv_obj_clean(SysSet_local.memory->this_scr);
	
}

bool scr_sysytem_setting_register()
{
    bool is_true = gui_sys_screen_register(scr_sysytem_setting_create,
											scr_system_setting_quit,
											NULL,
											NULL,
											NULL,
											scr_system_setting);
    if(is_true) return true;
	else return false;
}
