#include "scr_system_setting.h"
#include "../../gui_app.h"


#define SYSTEM_INFO_TAG_NUM 7
#define ADDRESS_TAG_NUM		5

#define PASSWORD_MAX_LEN 7 //This must be 1 more than true max_len of pwd given of char type


typedef void(*first_page_free_fun)(void);
typedef void(*void_fun_void)(void);

typedef struct free_queue_style {
	void *data;
	struct free_queue_style *next;
}_free_queue_style;

typedef struct {
	lv_obj_t *match_ta;
	char *inser_str;
}_list_btn_fun_widget;

typedef struct {
	char *str_tag;
	char *ch_data;
	lv_obj_t *refresh_label;
}_system_info_secpage_type;

typedef enum addrset_index{
	AddrSet_index_region = 0,
	AddrSet_index_building,
	AddrSet_index_unit,
	AddrSet_index_room,
	AddrSet_index_branch,
}_addrset_index;

typedef struct {
	char *str_tag;
	uint8_t index;
	//char *ch_data;
	//lv_obj_t *refresh_label;
	//lv_obj_t *match_ta;
	lv_obj_t *refresh_label[ADDRESS_TAG_NUM];
	struct address_set_type *addr_data;
}_address_secpage_type;

typedef struct {
	char SIN[20];
	char MAC[20];
	char IP[20];
	char SUBMASK[20];
	char GATEWAY[20];
	char SERVER_ADDR[20];
	char SOFTWARE_VERSION[10];
}_system_info_type;
_system_info_type system_info_d;//d means data

typedef struct address_set_type{
	char region[5];
	char building[5];
	char unit[5];
	char room[5];
	char branch[5];
}_address_set_type;
_address_set_type address_set_d;

typedef struct{
	char pwd[PASSWORD_MAX_LEN];
}_system_password_data_type;
_system_password_data_type system_password_d;


typedef struct {
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

typedef uint8_t pwd_change_type;
enum {
	pwd_change_old = 0,
	pwd_change_new,
	pwd_change_again,
};

typedef struct{
	pwd_change_type index;
	char *sys_pwd;
	char pwd_txt[3][PASSWORD_MAX_LEN];
	//lv_obj_t *match_ta;
	lv_obj_t *pwd_ta[3];
	_free_queue_style *free_queue_ta_obj;
}_pwd_sys_ta_type;

#ifdef _VS_SIMULATOR
#pragma execution_character_set("utf-8")
#endif


static lv_obj_t* get_a_default_bg(void);

static void SysSet_change_scr(scr_change_type type,_ui_scr_id scr_id);

//LV_EVENT_CB_DECLARE(pwd_btnm_event_handle);
static void SysSet_return_btn_event_handle(lv_obj_t *obj,lv_event_t e);

static lv_obj_t *system_set_ui_first_page_title_create(lv_obj_t *parent, const char *title_txt);

static void sset_initN_ui_create(void);
static void sset_root_ui_page_create(void);

static const char  *const pwd_btnm_map[] = { "1","2","3","删除","\n",
											 "4","5","6","0","\n",
											 "7","8","9","确认","" };


static const char  *const lbw_btnm_map[] = { "1","2","3","删除","\n",
											 "4","5","6","0","\n",
											 "7","8","9",".","确认","" };

static const char * const addr_set_num_btnm_map[] = {"0","1","2","3","4","\n",
												 "5","6","7","8","9",""};

static const char * const addr_set_ctrl_btnm_map[] = {"删除","确认",""};

static const char * const btnm_map_sset_pwd_change[] = {"1","2","3","\n",
														"4","5","6","\n",
														"7","8","9","\n",
														"*","0",LV_SYMBOL_TRASH,""};


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
	bool null_true = dynamic_is_null(local_data->dynamic);
	if (null_true) {
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
	//bool null_true = dynamic_is_null(SysSet_local.dynamic);
	//if (!null_true) return;
	if(is_null_ptr(SysSet_local.dynamic)) return;

	general_free(SysSet_local.dynamic);
	SysSet_local.dynamic = NULL;
}

void scr_system_setting_init()
{
	memset(&SysSet_data_memory,0,sizeof(_system_setting_data_memory));
	if (SysSet_data_memory.this_scr == NULL) ui_printf("system_setting_data_memory,this scr is NULL\r\n");
	else ui_printf("system_setting_data_memory,this scr is not NULL\r\n");

	char local_ip[] = "192.168.1.196";
	char local_port[] = "5060";

	memset(&system_info_d,0,sizeof(_system_info_type));

	strcpy(&SysSet_data_memory.local_ip[0],local_ip);
	strcpy(&SysSet_data_memory.local_port[0],local_port);

	char init_SIN[] = "123456789";
	char init_MAC[] = "123456789";
	char init_IP[] = "192.168.1.121";
	char init_SUBMASK[] = "255.255.0.0";
	char init_GATEWAY[] = "192.168.1.1";
	char init_SERVERIP[] = "192.168.1.101";
	char init_SOFTVER[] = "1.0.0";

	strcpy(&system_info_d.SIN[0],init_SIN);
	strcpy(&system_info_d.MAC[0],init_MAC);
	strcpy(&system_info_d.IP[0],init_IP);
	strcpy(&system_info_d.SUBMASK[0],init_SUBMASK);
	strcpy(&system_info_d.GATEWAY[0],init_GATEWAY);
	strcpy(&system_info_d.SERVER_ADDR[0],init_SERVERIP);
	strcpy(&system_info_d.SOFTWARE_VERSION[0],init_SOFTVER);

	char init_region[] = "A";
	char init_building[] = "16";
	char init_unit[] = "99";
	char init_room[] = "9999";
	char init_branch[] = "01";

	strcpy(&address_set_d.region[0],init_region);
	strcpy(&address_set_d.building[0],init_building);
	strcpy(&address_set_d.unit[0],init_unit);
	strcpy(&address_set_d.room[0],init_room);
	strcpy(&address_set_d.branch[0], init_branch);

	char init_system_pwd[] = "123456";
	strcpy(&system_password_d.pwd[0],init_system_pwd);

	SysSet_local.dynamic = NULL;
	SysSet_data_memory.sys_info = &system_info_d;
	SysSet_data_memory.addr_set = &address_set_d;
	SysSet_data_memory.sys_pwd = &system_password_d;
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

/*========================
******special operation
=========================*/
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
	#if 0
		while (tail != NULL)
		{
			tail = tail->next;
		}
		tail = node;
	#endif
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

static lv_obj_t *pwd_page = NULL;
/*============================
 ****** UI LAYER**************
==============================*/
static lv_obj_t* get_a_default_bg()
{
	lv_obj_t *bg_top = lv_obj_create(lv_scr_act(), NULL);
	lv_theme_apply(bg_top, (lv_theme_style_t)sigma_star_theme_default_bg);
	lv_obj_set_size(bg_top, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_pos(bg_top, 0, 0);

	return bg_top;
}

static lv_obj_t* setting_get_cover_bg()
{
	lv_obj_t *bg_top = lv_obj_create(lv_scr_act(), NULL);
	lv_theme_apply(bg_top, (lv_theme_style_t)sigma_star_theme_obj_bg_cover_black);
	lv_obj_set_size(bg_top, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_pos(bg_top, 0, 0);

	return bg_top;
}

static lv_obj_t * setting_ui_head_title_create(lv_obj_t *parent,char *head_str)
{
	lv_obj_t *head_cont = lv_cont_create(parent, NULL);
	lv_obj_set_pos(head_cont, 0, 0);
	lv_cont_set_fit(head_cont, LV_FIT_NONE);
	lv_obj_set_size(head_cont, LV_HOR_RES, 60);
	lv_cont_set_layout(head_cont, LV_LAYOUT_CENTER);
	//lv_obj_add_style(head_cont, LV_CONT_PART_MAIN, &title_style);
	lv_theme_apply(head_cont, (lv_theme_style_t)sigma_star_theme_title);

	lv_obj_t *obj_label = lv_label_create(head_cont, NULL);
	lv_label_set_text(obj_label, head_str);

	return head_cont;
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

	lv_obj_align(btn, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

	return btn;
}



static void ip_port_label_refresh()
{
	_system_setting_data_dynamic *local_dynamic = (_system_setting_data_dynamic *)get_local_pointer(p_data_dynamic);
	_system_setting_data_memory *local_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);

	lv_label_set_text(local_dynamic->label_ip,local_memory->local_ip);
	lv_label_set_text(local_dynamic->label_port,local_memory->local_port);

	char command[50];
	char ip_commamd[] = "ifconfig eth0 ";
	strcpy(command,ip_commamd);
	strcat(command,local_memory->local_ip);
	ui_printf("command:%s\n",&command[0]);

	system(command);
}



LV_EVENT_CB_DECLARE(list_btn_widget_return)
{
	if (e == LV_EVENT_CLICKED) {
		lv_obj_t *bg = lv_obj_get_parent(obj);
		lv_obj_del(bg);
		setting_data_free_lbwidget();
	}
}
#if 0
static lv_obj_t * settting_ui_duplicate_page_create(lv_obj_t *parent, char *title_str, bool b_ip, char *ta_txt)
{
	lv_obj_t *page_bg = NULL;	

	//page_bg = parent;
	static lv_style_t style_cover_bg;
	lv_style_init(&style_cover_bg);
	lv_style_set_bg_opa(&style_cover_bg,LV_STATE_DEFAULT,LV_OPA_COVER);
	lv_style_set_bg_color(&style_cover_bg,LV_STATE_DEFAULT,LV_COLOR_BLACK);

	lv_obj_t *new_bg = get_a_default_bg();
	lv_obj_add_style(new_bg,LV_OBJ_PART_MAIN,&style_cover_bg);
	page_bg = new_bg;

	lv_obj_t *obj_label = lv_label_create(page_bg,NULL);
	lv_theme_apply(obj_label, (lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_label_set_text(obj_label, title_str);
	lv_obj_align(obj_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 80);

	uint16_t ta_width = LV_HOR_RES / 4 * 3;
	uint16_t ta_height = 60;
	uint16_t btnm_height = LV_VER_RES / 2;
	uint16_t base_x = LV_HOR_RES / 8;
	lv_obj_t *obj_ta = lv_textarea_create(page_bg, NULL);
	lv_theme_apply(obj_ta, (lv_theme_style_t)sigma_star_theme_ta);
	//lv_textarea_set_one_line(obj_ta,true);
	lv_textarea_set_cursor_hidden(obj_ta, true);
	lv_textarea_set_text_align(obj_ta, LV_LABEL_ALIGN_CENTER);
	lv_textarea_set_accepted_chars(obj_ta, "0123456789.");
	lv_textarea_set_max_length(obj_ta, 15);
	lv_obj_set_size(obj_ta, ta_width, ta_height);
	lv_obj_set_pos(obj_ta, base_x, 120);
	//lv_textarea_set_text(obj_ta, "");
	lv_textarea_set_text(obj_ta,ta_txt);
	//lv_textarea_set_pwd_mode(obj_ta, true);

	lv_obj_t *obj_btnm = lv_btnmatrix_create(page_bg, NULL);
	lv_obj_set_size(obj_btnm, ta_width, btnm_height);
	lv_obj_align(obj_btnm, obj_ta, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
	if(b_ip)
		lv_btnmatrix_set_map(obj_btnm, &lbw_btnm_map[0]);
	else
		lv_btnmatrix_set_map(obj_btnm, &pwd_btnm_map[0]);
	lv_theme_apply(obj_btnm, (lv_theme_style_t)sigma_star_theme_btnmatrix_pwd);
	lv_obj_set_event_cb(obj_btnm, lbw_btnm_event_handle);

	lv_obj_t *obj_btn = lv_btn_create(page_bg, NULL);
	lv_obj_set_size(obj_btn, 150, 50);
	lv_obj_set_pos(obj_btn, LV_HOR_RES - 150 - 30, 30);
	lv_theme_apply(obj_btn, (lv_theme_style_t)sigma_star_theme_Btn_home_return);
	lv_obj_set_event_cb(obj_btn, list_btn_widget_return);

	lv_obj_t *obj_label_1 = lv_label_create(obj_btn, NULL);
	lv_label_set_text(obj_label_1, "返回");
	
	

	return obj_ta;
}

LV_EVENT_CB_DECLARE(list_btn_ip_event)
{
	if (e == LV_EVENT_CLICKED) {
		_list_btn_fun_widget *lb_widget = setting_data_malloc_lbwidget();
		_system_setting_data_memory *local_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);

		lv_obj_t *ta = settting_ui_duplicate_page_create(NULL,"IP",true, local_memory->local_ip);
		lb_widget->match_ta = ta;
		lb_widget->inser_str = &local_memory->local_ip[0];
	}
}

LV_EVENT_CB_DECLARE(list_btn_port_event)
{
	if (e == LV_EVENT_CLICKED) {
		_list_btn_fun_widget *lb_widget = setting_data_malloc_lbwidget();
		_system_setting_data_memory *local_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);

		lv_obj_t *ta = settting_ui_duplicate_page_create(NULL, "PORT", false,local_memory->local_port);
		lb_widget->match_ta = ta;
		lb_widget->inser_str = &local_memory->local_port[0];
	}
}



static lv_obj_t * setting_ui_list_create()
{
	_system_setting_data_memory *local_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);
	_system_setting_data_dynamic *local_dynamic = (_system_setting_data_dynamic *)get_local_pointer(p_data_dynamic);

	static lv_style_t list_style_bg;
	lv_style_init(&list_style_bg);
	lv_style_set_bg_opa(&list_style_bg,LV_STATE_DEFAULT,LV_OPA_TRANSP);
	//lv_style_set_pad_inner(&list_style_bg,LV_STATE_DEFAULT,10);

	static lv_style_t list_style_scrl;
	lv_style_init(&list_style_scrl);
	lv_style_set_pad_inner(&list_style_scrl,LV_STATE_DEFAULT,10);

	lv_obj_t *list = lv_list_create(lv_scr_act(),NULL);
	lv_obj_set_width(list,LV_HOR_RES);
	lv_obj_add_style(list,LV_LIST_PART_BG,&list_style_bg);
	lv_obj_add_style(list,LV_LIST_PART_SCROLLABLE,&list_style_scrl);

	lv_obj_t *list_btn = NULL;
	list_btn = lv_list_add_btn(list,NULL,"IP");
	lv_obj_set_event_cb(list_btn, list_btn_ip_event);

	static lv_style_t label_style;
	lv_style_init(&label_style);
	lv_style_set_text_color(&label_style,LV_STATE_DEFAULT,LV_COLOR_BLACK);

	lv_obj_t *label_ip = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label_ip, local_memory->local_ip);
	lv_obj_add_style(label_ip,LV_LABEL_PART_MAIN,&label_style);
	//lv_obj_align(label_ip, list,LV_ALIGN_IN_TOP_RIGHT,0,0);
	lv_obj_set_pos(label_ip,600,70);
	local_dynamic->label_ip = label_ip;

	list_btn = lv_list_add_btn(list,NULL,"PORT");
	lv_obj_set_event_cb(list_btn, list_btn_port_event);

	lv_obj_t *label_port = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label_port, local_memory->local_port);
	lv_obj_add_style(label_port, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(label_port,label_ip,LV_ALIGN_OUT_BOTTOM_LEFT,0,30);
	local_dynamic->label_port = label_port;

	return list;
}
#endif



LV_EVENT_CB_DECLARE(default_second_page_return)
{
	if (e == LV_EVENT_CLICKED)
	{
		if (!is_null_ptr(obj->user_data))
			(*(void_fun_void)obj->user_data)();

		lv_obj_t *bg = lv_obj_get_parent(obj);
		lv_obj_del(bg);
	}
}




/*
***Below
***Some repeat operation,like title,return btn and so on
*/
LV_EVENT_CB_DECLARE(system_set_eventcb_root_widget_return)
{
	if (e == LV_EVENT_CLICKED)
	{
		//first_page_free_fun p;
		//system_info_page_free();
		//p = (first_page_free_fun)obj->user_data;
		//(*p)();
		if(!is_null_ptr(obj->user_data))
			(*(first_page_free_fun)obj->user_data)();

		lv_obj_del(lv_obj_get_parent(obj));
		sset_root_ui_page_create();
	}
}

LV_EVENT_CB_DECLARE(system_set_eventcb_initN_widget_return)
{
	if (e == LV_EVENT_CLICKED)
	{
		if (!is_null_ptr(obj->user_data))
			(*(first_page_free_fun)obj->user_data)();

		lv_obj_del(lv_obj_get_parent(obj));
		sset_initN_ui_create();
	}
}

static lv_obj_t *system_set_ui_first_page_title_create(lv_obj_t *parent,const char *title_txt)
{
	lv_obj_t *label_title = lv_label_create(parent, NULL);
	lv_theme_apply(label_title, (lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_label_set_text(label_title, title_txt);
	lv_obj_align(label_title, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);

	return label_title;
}

static lv_obj_t *system_set_ui_plain_return_create(lv_obj_t *parent)
{
	static lv_style_t btn_style;
	lv_style_init(&btn_style);
	lv_style_set_bg_color(&btn_style, LV_STATE_DEFAULT, LV_COLOR_GRAY);
	lv_style_set_bg_grad_color(&btn_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_border_width(&btn_style, LV_STATE_DEFAULT, 1);

	lv_obj_t *return_btn = lv_btn_create(parent, NULL);
	lv_obj_set_size(return_btn, 120, 60);
	lv_obj_align(return_btn, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
	//lv_theme_apply(return_btn,(lv_theme_style_t)sigma_star_theme_Btn_home_return);
	lv_btn_set_layout(return_btn, LV_LAYOUT_ROW_MID);
	//lv_obj_set_event_cb(return_btn, sysset_first_page_return);
	lv_obj_add_style(return_btn, LV_BTN_PART_MAIN, &btn_style);
	//return_btn->user_data = return_free_event;

	lv_obj_t *label_p = lv_label_create(return_btn, NULL);
	lv_label_set_text(label_p, LV_SYMBOL_LEFT);

	lv_obj_t *label_r = lv_label_create(return_btn, NULL);
	lv_label_set_text(label_r, "返回");
	lv_theme_apply(label_r, (lv_theme_style_t)sigma_star_theme_label_chinese);

	return return_btn;
}

static lv_obj_t *system_set_ui_root_widget_return_create(lv_obj_t *parent, void *return_free_event)
{
	lv_obj_t *btn = system_set_ui_plain_return_create(parent);
	lv_obj_set_event_cb(btn, system_set_eventcb_root_widget_return);

	btn->user_data = return_free_event;

	return btn;
}

static lv_obj_t *system_set_ui_initN_widget_return_create(lv_obj_t *parent, void *return_free_event)
{
	lv_obj_t *btn = system_set_ui_plain_return_create(parent);
	lv_obj_set_event_cb(btn, system_set_eventcb_initN_widget_return);

	return btn;
}

/*******************BELOW************************************
*************System Setting-System Info Block****************
************************************************************/
/*Foramt:
***@ Prefix:sset_info_
***@ UI draw: sset_info_ui_
***@ event_cb: sset_info_eventcb_
*/

/*---------------BELOW-------------------
*********System Info Second Page*********
----------------------------------------*/
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
static void info_label_refresh(lv_obj_t *label , char *refresh_data)
{
	lv_label_set_text(label,refresh_data);
	//lv_label_set_text(label,"1");
}

static void info_btnm_match_ta_control(_btnm_ctrl_type ctrl_t, const char *txt, char *get_data,void *user_data)
{
	if (btnm_ctrl_null == ctrl_t) return;

	//_list_btn_fun_widget *lb_widget = setting_data_get_lbwidget();
	//if (is_null_ptr(lb_widget)) return;

	//lv_obj_t *match_ta = lb_widget->match_ta;
	lv_obj_t *match_ta = (lv_obj_t *)user_data;

	switch (ctrl_t)
	{
	case btnm_ctrl_add:
		lv_textarea_add_text(match_ta, txt);
		break;

	case btnm_ctrl_del:
		lv_textarea_del_char(match_ta);
		break;

	case btnm_ctrl_check:
	{
		const char *temp = lv_textarea_get_text(match_ta);
		//*(char **)get_data = temp;
		//strcpy(lb_widget->inser_str, temp);
		strcpy(get_data,temp);
	}
	break;

	default:
		break;
	}
}

LV_EVENT_CB_DECLARE(info_btnm_event_handle)
{
	if (e != LV_EVENT_VALUE_CHANGED) return;

	const char *txt = lv_btnmatrix_get_active_btn_text(obj);
	if (txt == NULL) return;
	//ui_printf("pwd btnm,btn clicked\r\n");
	_system_info_secpage_type *info_page = (_system_info_secpage_type *)((lv_obj_t *)obj->user_data)->user_data;

	if (strcmp(txt, "删除") == 0) {
		info_btnm_match_ta_control(btnm_ctrl_del, NULL, NULL,obj->user_data);
		return;
	}
	else if (strcmp(txt, "确认") == 0) {
		char *get_txt = NULL;
		//get_txt = (char *)((lv_obj_t *)obj->user_data)->user_data;//btnm->ta->ch_data
		get_txt = info_page->ch_data;
		info_btnm_match_ta_control(btnm_ctrl_check, NULL, get_txt, obj->user_data);
		info_label_refresh(info_page->refresh_label,get_txt);
		//sset_initN_root_pwd_confirm_control(get_txt);
		//ip_port_label_refresh();
		ui_printf("data:%s",&SysSet_local.memory->sys_info->SIN[0]);
		lv_obj_t *bg = lv_obj_get_parent(obj);
		lv_obj_del(bg);
		return;
	}
	else {
		info_btnm_match_ta_control(btnm_ctrl_add, txt, NULL, obj->user_data);
	}
}

LV_EVENT_CB_DECLARE(system_info_second_page_return)
{
	if (e == LV_EVENT_CLICKED)
	{
		lv_obj_t *bg = lv_obj_get_parent(obj);
		lv_obj_del(bg);
	}
}

static lv_obj_t *system_info_second_page_duplicate(char *tag_str,char *ch_data,void *sec_data)
{
	lv_obj_t *page_bg = NULL;

	lv_obj_t *new_bg = setting_get_cover_bg();
	page_bg = new_bg;
#if 0
	lv_obj_t *cont_bg = lv_cont_create(page_bg, NULL);
	lv_cont_set_fit(cont_bg, LV_FIT_NONE);
	lv_cont_set_layout(cont_bg, LV_LAYOUT_CENTER);
	lv_obj_set_size(cont_bg, 100, 60);
	//lv_obj_set_pos(cont_bg, 20, 100);
	lv_obj_align(cont_bg,NULL,LV_ALIGN_CENTER,0,-100);
	lv_theme_apply(cont_bg, (lv_theme_style_t)sigma_star_theme_syset_cont_trans_border);
#endif
//tag head
	lv_obj_t *label_tag = lv_label_create(page_bg,NULL);
	lv_label_set_text(label_tag,tag_str);
	lv_theme_apply(label_tag,(lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_obj_align(label_tag,NULL,LV_ALIGN_IN_TOP_MID,0,20);	
//return btn	
	lv_obj_t *obj_btn = lv_btn_create(page_bg,NULL);
	lv_obj_set_size(obj_btn,100,60);
	lv_obj_align(obj_btn,NULL,LV_ALIGN_IN_TOP_RIGHT,0,10);
	lv_obj_set_event_cb(obj_btn, system_info_second_page_return);

	lv_obj_t *obj_label = lv_label_create(obj_btn,NULL);
	lv_label_set_text(obj_label,"return");
//textarea display
	uint16_t ta_width = LV_HOR_RES / 4 * 3;
	uint16_t ta_height = 60;
	uint16_t btnm_height = LV_VER_RES / 2;
	uint16_t base_x = LV_HOR_RES / 8;
	lv_obj_t *obj_ta = lv_textarea_create(page_bg, NULL);
	lv_theme_apply(obj_ta, (lv_theme_style_t)sigma_star_theme_ta);
	//lv_textarea_set_one_line(obj_ta,true);
	lv_textarea_set_cursor_hidden(obj_ta, true);
	lv_textarea_set_text_align(obj_ta, LV_LABEL_ALIGN_CENTER);
	lv_textarea_set_accepted_chars(obj_ta, "0123456789.");
	lv_textarea_set_max_length(obj_ta, 20);
	lv_obj_set_size(obj_ta, ta_width, ta_height);
	lv_obj_set_pos(obj_ta, base_x, 120);
	lv_textarea_set_text(obj_ta, ch_data);
	//lv_textarea_set_pwd_mode(obj_ta, true);
//btnmatrix
	lv_obj_t *obj_btnm = lv_btnmatrix_create(page_bg, NULL);
	lv_obj_set_size(obj_btnm, ta_width, btnm_height);
	lv_obj_align(obj_btnm, obj_ta, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
	lv_btnmatrix_set_map(obj_btnm, lbw_btnm_map);
	lv_theme_apply(obj_btnm, (lv_theme_style_t)sigma_star_theme_btnmatrix_pwd);
	lv_obj_set_event_cb(obj_btnm, info_btnm_event_handle);

	obj_ta->user_data = sec_data;//ch_data;
	obj_btnm->user_data = obj_ta;

	return obj_ta;
}

LV_EVENT_CB_DECLARE(system_info_into_second_page)
{
	if (e == LV_EVENT_CLICKED)
	{
		//_system_setting_data_memory *p_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);
		_system_info_secpage_type *upper_info = (_system_info_secpage_type *)obj->user_data;
		if (is_null_ptr(upper_info)) {
			ui_printf("[Fail]enter second page,user_data is null\n");
			return;
		}
		//lv_obj_t * ta = system_info_second_page_duplicate("本机编码", (char *)obj->user_data);
		lv_obj_t * ta = system_info_second_page_duplicate(upper_info->str_tag, upper_info->ch_data,upper_info);
		//lv_textarea_set_accepted_chars(ta,"0123456789");
	}
}

static lv_obj_t *info_page_tag_cont_create(const lv_obj_t *parent, char *tag_str)
{
	lv_obj_t *page_bg = parent;

	lv_obj_t *cont_bg = lv_cont_create(page_bg, NULL);
	lv_cont_set_fit(cont_bg, LV_FIT_NONE);
	lv_cont_set_layout(cont_bg, LV_LAYOUT_CENTER);
	lv_obj_set_size(cont_bg, 120, 60);
	//lv_obj_set_pos(cont_bg, 20, 100);
	lv_theme_apply(cont_bg, (lv_theme_style_t)sigma_star_theme_syset_cont_trans_border);

	lv_obj_t *obj_label = lv_label_create(cont_bg, NULL);
	lv_label_set_text(obj_label, tag_str);
	lv_theme_apply(obj_label, (lv_theme_style_t)sigma_star_theme_label_chinese);

	return cont_bg;
}

static lv_obj_t *info_page_data_cont_create(const lv_obj_t *parent,const lv_obj_t *copy, _system_info_secpage_type *info_type)
{
	lv_obj_t *page_bg = parent;

	lv_obj_t *cont_new = lv_cont_create(page_bg, copy);
	lv_obj_align(cont_new, copy, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
	lv_obj_set_size(cont_new, 180, 60);
	lv_obj_set_click(cont_new, true);
	lv_obj_set_event_cb(cont_new, system_info_into_second_page);
	//cont_new->user_data = p_memory->sys_info->SIN;
	cont_new->user_data = info_type;

	lv_obj_t *obj_label = lv_label_create(cont_new, NULL);
	//lv_label_set_text(obj_label, p_memory->sys_info->SIN);
	lv_label_set_text(obj_label,info_type->ch_data);
	lv_theme_apply(obj_label, (lv_theme_style_t)sigma_star_theme_label_chinese);
	info_type->refresh_label = obj_label;

	return cont_new;
}


//system info page memory control
static _system_info_secpage_type* system_info_page_malloc()
{
	_system_info_secpage_type *info_type = (_system_info_secpage_type *)general_malloc(sizeof(_system_info_secpage_type) * SYSTEM_INFO_TAG_NUM);//7 tags
	if (info_type == NULL) {
		ui_printf("info_type malloc fail\n");
		return NULL;
	}
	system_info_dynamic = info_type;
	memset(info_type, 0, sizeof(_system_info_secpage_type) * SYSTEM_INFO_TAG_NUM);

	return system_info_dynamic;
}

static void system_info_page_free()
{
	if (!is_null_ptr(system_info_dynamic)) {
		int i = 0;
		_system_info_secpage_type *free_info = NULL;
		for (i = 0; i < SYSTEM_INFO_TAG_NUM; i++) {
			free_info = system_info_dynamic + i;
			if(!is_null_ptr(free_info->str_tag))//tag的内存是单独根据tag长度分配的，需要逐一释放先
				general_free(free_info->str_tag);
			//system_info_dynamic = system_info_dynamic + 1;
		}

		general_free(system_info_dynamic);
		system_info_dynamic = NULL;
		ui_printf("[system_info_page_free]\n");
	}
}

static void info_dynamic_data_set(char *tag,char *ch_addr,_system_info_secpage_type *info_type)
{
	if (is_null_ptr(info_type))
		return;
	
	//int tag_size = strlen(tag);
	//ui_printf("tag_size:%d\n",tag_size);
#if 1
	info_type->str_tag = (char *)general_malloc(strlen(tag) +1);
	if (is_null_ptr(info_type->str_tag))
		return;

	strcpy(info_type->str_tag,tag);
	
	info_type->ch_data = ch_addr;
#endif
}

/*
*****两难的问题，回调只能送进去一个void型用户指针，又不想在尚未进入二级界面前就为所有这些子模块分配资源浪费内存
****边敲一想分配下内存应该也不多，与其为每个子模块重复一次代码，还不如分配内存简洁些
*/
static void sset_info_ui_page_create()
{
	lv_obj_t *page_bg = NULL;
	
	lv_obj_t *new_bg = setting_get_cover_bg();
	page_bg = new_bg;

	_system_setting_data_memory *p_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);

	//system_set_ui_first_page_title_create(page_bg,"系统信息");
	setting_ui_head_title_create(page_bg, "系统信息");
	system_set_ui_root_widget_return_create(page_bg, system_info_page_free);


	_system_info_secpage_type *info_type = system_info_page_malloc();
	if (info_type == NULL) {
		ui_printf("info_type malloc fail\n");
		return;
	}

	//lv_obj_t *cont_tag = NULL;
	lv_obj_t *cont_data = NULL;
	uint16_t up_down_space = 15;
//SIN
	lv_obj_t *cont_tag_sin = info_page_tag_cont_create(page_bg, "本机编码");
	lv_obj_set_pos(cont_tag_sin,30,100);
	info_dynamic_data_set("本机编码", &p_memory->sys_info->SIN[0],info_type);

	cont_data = info_page_data_cont_create(page_bg, cont_tag_sin, info_type);

//MAC address
	info_type++;//important,dont't ignore
	lv_obj_t *cont_tag_mac = info_page_tag_cont_create(page_bg,"MAC地址");
	lv_obj_align(cont_tag_mac, cont_tag_sin,LV_ALIGN_OUT_BOTTOM_LEFT,0, up_down_space);

	info_dynamic_data_set("MAC地址", &p_memory->sys_info->MAC[0], info_type);
	
	cont_data = info_page_data_cont_create(page_bg,cont_tag_mac,info_type);

//IP
	info_type++;//important,dont't ignore
	lv_obj_t *cont_tag_ip = info_page_tag_cont_create(page_bg, "IP地址");
	lv_obj_align(cont_tag_ip,cont_tag_mac, LV_ALIGN_OUT_BOTTOM_LEFT, 0, up_down_space);

	info_dynamic_data_set("IP地址", &p_memory->sys_info->IP[0], info_type);

	cont_data = info_page_data_cont_create(page_bg, cont_tag_ip, info_type);

//SUB-Mask
	info_type++;//important,dont't ignore
	lv_obj_t *cont_tag_submask = info_page_tag_cont_create(page_bg,"子网掩码");
	lv_obj_align(cont_tag_submask,cont_tag_ip, LV_ALIGN_OUT_BOTTOM_LEFT, 0, up_down_space);


	info_dynamic_data_set("子网掩码", &p_memory->sys_info->SUBMASK[0], info_type);

	cont_data = info_page_data_cont_create(page_bg, cont_tag_submask, info_type);

//NETMASK
	info_type++;//important,dont't ignore
	lv_obj_t *cont_tag_gateway = info_page_tag_cont_create(page_bg, "网关");
	lv_obj_align(cont_tag_gateway, cont_tag_submask, LV_ALIGN_OUT_BOTTOM_LEFT, 0, up_down_space);


	info_dynamic_data_set("网关", &p_memory->sys_info->GATEWAY[0], info_type);

	cont_data = info_page_data_cont_create(page_bg, cont_tag_gateway, info_type);

//SERVER-address
	info_type++;//important,dont't ignore
	lv_obj_t *cont_tag_serverip = info_page_tag_cont_create(page_bg, "服务器地址");
	//lv_obj_align(cont_tag_serverip, cont_tag_sin, LV_ALIGN_OUT_RIGHT_TOP, up_down_space, 0);
	lv_obj_set_pos(cont_tag_serverip,430,100);


	info_dynamic_data_set("服务器地址", &p_memory->sys_info->SERVER_ADDR[0], info_type);

	cont_data = info_page_data_cont_create(page_bg, cont_tag_serverip, info_type);

//Software Version
	info_type++;//important,dont't ignore
	lv_obj_t *cont_tag_softwarever = info_page_tag_cont_create(page_bg, "软件版本");
	lv_obj_align(cont_tag_softwarever, cont_tag_serverip, LV_ALIGN_OUT_BOTTOM_LEFT, 0, up_down_space);


	info_dynamic_data_set("软件版本", &p_memory->sys_info->SOFTWARE_VERSION[0], info_type);

	cont_data = info_page_data_cont_create(page_bg, cont_tag_softwarever, info_type);
	
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
static void AddrSet_btnm_match_ta_control(_btnm_ctrl_type ctrl_t, const char *txt, lv_obj_t *ta, char *get_data)
{
	if (btnm_ctrl_null == ctrl_t) return;

	//lv_obj_t *match_ta = (lv_obj_t *)((_address_secpage_type *)(lv_obj_t *)user_data)->match_ta;
	//lv_obj_t *match_ta = (lv_obj_t *)((_address_secpage_type *)user_data)->match_ta;
	//lv_obj_t *match_ta = (lv_obj_t *)((_AddrSet_ta_data_type *)user_data)->match_ta;
	lv_obj_t *match_ta = ta;

	switch (ctrl_t)
	{
	case btnm_ctrl_add:
		lv_textarea_add_text(match_ta, txt);
		break;

	case btnm_ctrl_del:
		lv_textarea_del_char(match_ta);
		break;

	case btnm_ctrl_check:
	{
		char *ta_txt = lv_textarea_get_text(match_ta);
		strcpy(get_data, ta_txt);
	}
	break;
	default:
		break;
	}
}

LV_EVENT_CB_DECLARE(AddrSet_btnm_event_handle)
{
	if (e != LV_EVENT_VALUE_CHANGED) return;

	const char *txt = lv_btnmatrix_get_active_btn_text(obj);
	if (is_null_ptr(txt)) return;

	lv_obj_t *match_ta = ((_AddrSet_ta_data_type *)obj->user_data)->match_ta;
	AddrSet_btnm_match_ta_control(btnm_ctrl_add, txt, match_ta, NULL);
}

LV_EVENT_CB_DECLARE(AddrSet_btn_ctrl_event_handle)
{
	if (e == LV_EVENT_CLICKED)
	{
		lv_obj_t *label = lv_obj_get_child(obj, NULL);

		const char *txt = lv_label_get_text(label);
		if (is_null_ptr(txt)) return;

		if (strcmp(txt, "删除") == 0) {
			lv_obj_t *match_ta = ((_AddrSet_ta_data_type *)obj->user_data)->match_ta;
			AddrSet_btnm_match_ta_control(btnm_ctrl_del, NULL, match_ta, NULL);
		}
		else {
#if 0
			_address_secpage_type *addr_page_data = (_address_secpage_type *)obj->user_data;
			_address_set_type *addr_data = addr_page_data->addr_data;
			char *data_addr = &addr_data->region[0] + addr_page_data->index * 5;
			AddrSet_btnm_match_ta_control(btnm_ctrl_check, NULL, obj->user_data, data_addr);
			lv_label_set_text(addr_page_data->refresh_label[addr_page_data->index], data_addr);
			ui_printf("index:%d,label:%p\n", addr_page_data->index, addr_page_data->refresh_label[addr_page_data->index]);
			ui_printf("region:%s\n", addr_data->region);
			ui_printf("building:%s\n", addr_data->building);
			ui_printf("unit:%s\n", addr_data->room);
#endif
			_AddrSet_ta_data_type *ta_data = (_AddrSet_ta_data_type *)obj->user_data;
			_free_queue_style *ta_list = ta_data->ta_list;
			_address_secpage_type *upper_data = (_address_secpage_type *)ta_data->upper_data;
			int i = 0;
			char *data_addr;
			lv_obj_t *ta = NULL;
			for (i = 0; i <= AddrSet_index_branch; i++)
			{
				ta = (lv_obj_t *)ta_list->data;
				data_addr = &upper_data->addr_data->region[0] + i * 5;
				AddrSet_btnm_match_ta_control(btnm_ctrl_check, NULL, ta, data_addr);
				ta_list = ta_list->next;
				lv_label_set_text(upper_data->refresh_label[i], data_addr);
			}
		}
	}
}

static void AddrSet_ta_into_list(lv_obj_t *ta, _free_queue_style **ta_list)
{
	free_queue_tail_insert(ta_list, ta);
}

static void addr_ta_set_user_data(lv_obj_t *ta, uint8_t index, _AddrSet_ta_data_type *common_data)
{
#if 0
	_ta_user_data *ta_user = (_ta_user_data *)general_malloc(sizeof(_ta_user_data));
	if (is_null_ptr(ta_user))
		return;

	ta_user->index = index;
	ta_user->upper_data = user_data;

	ta->user_data = ta_user;

	free_queue_tail_insert(&addr_set_textarea_list, ta_user);
#endif
	_obj_user_data_type *obj_data = (_obj_user_data_type *)general_malloc(sizeof(_obj_user_data_type));
	if (is_null_ptr(obj_data))
		return;

	obj_data->index = index;
	obj_data->data = common_data;
	AddrSet_ta_into_list(ta, &common_data->ta_list);
	ta->user_data = obj_data;
#if 0
	obj_data->data = AddrSet_ta_data_malloc(upper_data);
	if (is_null_ptr(obj_data->data)) {//这一步为了防止ta_data申请失败，出现这种情况就把当前为ta申请的内存也释放掉避免内存泄漏，这一步还有待进一步研究会出现的连锁反应
		general_free(obj_data);
		return;
	}
	ta->user_data = obj_data;
#endif
}

static void addr_ta_memory_free()
{
#if 0
	free_queue_free(addr_set_textarea_list);
	addr_set_textarea_list = NULL;
#endif
	if (is_null_ptr(Temp_ta_data)) return;

	//free_queue_free(Temp_ta_data->ta_list);
	free_queue_free_without_data(Temp_ta_data->ta_list);
	general_free(Temp_ta_data);
	Temp_ta_data = NULL;
	
}

static _AddrSet_ta_data_type* AddrSet_ta_data_malloc(void *upper_data, uint8_t index)
{
	_AddrSet_ta_data_type *p = (_AddrSet_ta_data_type *)general_malloc(sizeof(_AddrSet_ta_data_type));
	if (is_null_ptr(p))
		return NULL;
	//p->ta_list = (_free_queue_style *)general_malloc(sizeof(_free_queue_style));
	memset(p, 0, sizeof(_AddrSet_ta_data_type));

	p->upper_data = upper_data;
	p->choosen_index = index;
	return p;
}

//hide and unhide the pre-match_ta and match_ta
//change the index of common_data of ta,also match_ta
LV_EVENT_CB_DECLARE(addr_ta_callback)
{
	if (e == LV_EVENT_CLICKED)
	{
		if (is_null_ptr(obj->user_data))
			return;
#if 0
		_ta_user_data *ta_user = (_ta_user_data *)obj->user_data;
		ui_printf("addr ta cliecked,index:%d\n", ta_user->index);
		lv_textarea_set_cursor_hidden(((_address_secpage_type *)ta_user->upper_data)->match_ta, true);//hide cursor of current textarea
		((_address_secpage_type *)ta_user->upper_data)->match_ta = obj;
		((_address_secpage_type *)ta_user->upper_data)->index = ta_user->index;
		lv_textarea_set_cursor_hidden(obj, false);
#endif
		_obj_user_data_type *obj_data = (_obj_user_data_type *)obj->user_data;
		if (is_null_ptr(obj_data)) return;

		_AddrSet_ta_data_type *ta_data = (_AddrSet_ta_data_type *)obj_data->data;
		if (is_null_ptr(ta_data)) return;

		lv_textarea_set_cursor_hidden(obj, false);
		lv_textarea_set_cursor_hidden(ta_data->match_ta, true);
		ui_printf("pre index:%d,now index:%d\n", ta_data->choosen_index, obj_data->index);

		ta_data->choosen_index = obj_data->index;
		ta_data->match_ta = obj;

	}
}

static lv_obj_t *addr_ta_duplicate(lv_obj_t *parent,lv_obj_t *ta_base,uint16_t obj_height,char *ta_txt,char *label_txt,bool ta_width_double)
{
	lv_obj_t *bg = parent;
	uint16_t ta_height = 60, ta_width = 60;
	uint16_t ta_y = (obj_height - ta_height) / 2;
	uint16_t ta_sapce = 50;

	if (ta_width_double)
		ta_width *= 2;

	lv_obj_t *ta = lv_textarea_create(bg, NULL);
	lv_obj_set_size(ta, ta_width, ta_height);
	//lv_obj_set_pos(ta, 20, ta_y);
	lv_textarea_set_text(ta, ta_txt);
	lv_textarea_set_one_line(ta, true);
	lv_textarea_set_cursor_hidden(ta,true);
	lv_obj_set_click(ta,true);
	lv_obj_set_event_cb(ta, addr_ta_callback);
	if (is_null_ptr(ta_base))
		lv_obj_set_pos(ta, 20, ta_y);
	else
		lv_obj_align(ta,ta_base, LV_ALIGN_OUT_RIGHT_TOP, ta_sapce, 0);

	lv_obj_t *ta_label = lv_label_create(bg, NULL);
	lv_label_set_text(ta_label, label_txt);
	lv_theme_apply(ta_label, (lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_obj_align(ta_label, ta, LV_ALIGN_OUT_RIGHT_BOTTOM, 2, 0);

	return ta;
}

static lv_obj_t *address_second_page_duplicate(void *sec_data)
{
	lv_obj_t *page_bg = NULL;
	
	lv_obj_t *new_bg = setting_get_cover_bg();
	page_bg = new_bg;

	_obj_user_data *pre_data = (_obj_user_data *)sec_data;
	uint8_t choosen_index = pre_data->index;
	
	_address_secpage_type *addr_data = (_address_secpage_type *)pre_data->upper_data;
	//general_free(pre_data);  //got the previouse data and then free memory
//tag head
	lv_obj_t *label_tag = lv_label_create(page_bg,NULL);
	//lv_label_set_text(label_tag,tag_str);
	lv_label_set_text(label_tag,"地址信息设置");
	lv_theme_apply(label_tag,(lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_obj_align(label_tag,NULL,LV_ALIGN_IN_TOP_MID,0,20);
//return 
	lv_obj_t *obj_btn = lv_btn_create(page_bg,NULL);
	lv_obj_set_size(obj_btn,100,60);
	lv_obj_align(obj_btn,NULL,LV_ALIGN_IN_TOP_RIGHT,0,10);
	lv_obj_set_event_cb(obj_btn, default_second_page_return);
	lv_theme_apply(obj_btn,(lv_theme_style_t)sigma_star_theme_Btn_home_return);
	obj_btn->user_data = addr_ta_memory_free;

	lv_obj_t *obj_label = lv_label_create(obj_btn,NULL);
	lv_label_set_text(obj_label,"返回");
	lv_theme_apply(obj_label,(lv_theme_style_t)sigma_star_theme_label_chinese);

	uint16_t obj_height = 80, obj_width = LV_HOR_RES / 4 * 3;
	uint16_t ta_height = 60, ta_width = 70;
	uint16_t ta_y = (obj_height - ta_height) / 2;

	static lv_style_t obj_bg_style;
	lv_style_init(&obj_bg_style);
	lv_style_set_bg_color(&obj_bg_style,LV_STATE_DEFAULT,LV_COLOR_GRAY);
	
	lv_obj_t *obj_obj = lv_obj_create(page_bg,NULL);
	lv_obj_set_size(obj_obj, obj_width+30, obj_height);
	lv_obj_set_pos(obj_obj,LV_HOR_RES/8-15,100);
	lv_obj_add_style(obj_obj,LV_OBJ_PART_MAIN,&obj_bg_style);

	//_AddrSet_ta_data_type *ta_common_data = (_AddrSet_ta_data_type *)general_malloc(sizeof(_AddrSet_ta_data_type));
	//if (is_null_ptr(ta_common_data)) return NULL;
	//ta_common_data->upper_data = addr_data;
	_AddrSet_ta_data_type *ta_common_data = AddrSet_ta_data_malloc(addr_data, choosen_index);
	if (is_null_ptr(ta_common_data)) return NULL;
	Temp_ta_data = ta_common_data;
//address info display
	uint8_t index_cnt = 0;
	uint16_t ta_sapce = 50;
	lv_obj_t * ta_0 = addr_ta_duplicate(obj_obj,NULL,obj_height, addr_data->addr_data->region,"区",false);
	//lv_obj_set_pos(ta_0,20,ta_y);
	addr_ta_set_user_data(ta_0,AddrSet_index_region, ta_common_data);	//addr_data

	lv_obj_t * ta_1 = addr_ta_duplicate(obj_obj,ta_0, obj_height, addr_data->addr_data->building, "栋",false);
	//lv_obj_align(ta_1,ta_0,LV_ALIGN_OUT_RIGHT_TOP,ta_sapce,0);
	addr_ta_set_user_data(ta_1, AddrSet_index_building, ta_common_data);

	lv_obj_t * ta_2 = addr_ta_duplicate(obj_obj,ta_1, obj_height, addr_data->addr_data->unit, "单元",false);
	//lv_obj_align(ta_2, ta_1, LV_ALIGN_OUT_RIGHT_TOP, ta_sapce, 0);
	addr_ta_set_user_data(ta_2, AddrSet_index_unit, ta_common_data);

	lv_obj_t * ta_3 = addr_ta_duplicate(obj_obj,ta_2, obj_height, addr_data->addr_data->room, "室",true);
	//lv_obj_align(ta_3, ta_2, LV_ALIGN_OUT_RIGHT_TOP, ta_sapce, 0);
	addr_ta_set_user_data(ta_3, AddrSet_index_room, ta_common_data);

	lv_obj_t * ta_4 = addr_ta_duplicate(obj_obj,ta_3, obj_height, addr_data->addr_data->branch, "分机",false);
	//lv_obj_align(ta_4, ta_3, LV_ALIGN_OUT_RIGHT_TOP, ta_sapce, 0);
	addr_ta_set_user_data(ta_4, AddrSet_index_branch, ta_common_data);
	
	lv_obj_t *choosen_ta = NULL;
	switch (choosen_index)
	{
		case AddrSet_index_region:
			choosen_ta = ta_0;
			break;

		case AddrSet_index_building:
			choosen_ta = ta_1;
			break;

		case AddrSet_index_unit:
			choosen_ta = ta_2;
			break;

		case AddrSet_index_room:
			choosen_ta = ta_3;
			break;

		case AddrSet_index_branch:
			choosen_ta = ta_4;
			break;

		default:
			break;
	}
	lv_textarea_set_cursor_hidden(choosen_ta,false);
	ta_common_data->match_ta = choosen_ta;
//btnm part
	uint16_t btnm_width = LV_HOR_RES / 4 * 3;
	uint16_t btnm_height = LV_VER_RES / 4;

	uint16_t ctrl_btnm_width = btnm_width, ctrl_btnm_height = btnm_height/2;

	uint16_t btnm_bg_width = btnm_width + 20, btnm_bg_height = btnm_height + ctrl_btnm_height + 40;

	static lv_style_t obj_style;
	lv_style_init(&obj_style);
	lv_style_set_bg_color(&obj_style,LV_STATE_DEFAULT,LV_COLOR_GRAY);
	lv_style_set_bg_opa(&obj_style,LV_STATE_DEFAULT,LV_OPA_60);

	lv_obj_t *obj_btnm_bg = lv_obj_create(page_bg,NULL);
	lv_obj_set_size(obj_btnm_bg, btnm_bg_width, btnm_bg_height);
	lv_obj_align(obj_btnm_bg, obj_obj, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 30);
	//lv_theme_apply(obj_btnm_bg,(lv_theme_style_t)sigma_star_theme_default_bg);
	lv_obj_add_style(obj_btnm_bg,LV_OBJ_PART_MAIN,&obj_style);

	lv_obj_t *label_info = lv_label_create(obj_btnm_bg,NULL);
	lv_label_set_text(label_info,"请输入室内机房号");
	lv_theme_apply(label_info,(lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_obj_align(label_info,NULL,LV_ALIGN_IN_TOP_MID,0,0);

	static lv_style_t style_btnm_bg_AddrSet;
	lv_style_init(&style_btnm_bg_AddrSet);
	//lv_style_set_bg_opa(&style_btnm_bg_AddrSet,LV_STATE_DEFAULT,LV_OPA_TRANSP);
	lv_style_set_pad_inner(&style_btnm_bg_AddrSet,LV_STATE_DEFAULT,30);
	

	static lv_style_t style_btnm_btn_AddrSet;
	lv_style_init(&style_btnm_btn_AddrSet);
	lv_style_set_bg_color(&style_btnm_btn_AddrSet,LV_STATE_DEFAULT, LV_COLOR_NAVY);
	lv_style_set_text_color(&style_btnm_btn_AddrSet,LV_STATE_DEFAULT,LV_COLOR_WHITE);
	lv_style_set_bg_opa(&style_btnm_btn_AddrSet,LV_STATE_DEFAULT,LV_OPA_60);
	lv_style_set_pad_left(&style_btnm_btn_AddrSet, LV_STATE_DEFAULT,1);
	lv_style_set_pad_right(&style_btnm_btn_AddrSet, LV_STATE_DEFAULT,1);
	
	lv_obj_t *btnm = lv_btnmatrix_create(obj_btnm_bg,NULL);
	lv_obj_set_size(btnm,btnm_width,btnm_height);
	//lv_obj_align(btnm,obj_obj,LV_ALIGN_OUT_BOTTOM_LEFT,0,30);
	lv_obj_set_pos(btnm,10,40);
	lv_btnmatrix_set_map(btnm,addr_set_num_btnm_map);
	//lv_theme_apply(btnm,(lv_theme_style_t)sigma_star_theme_btnmatrix_pwd);
	lv_obj_add_style(btnm,LV_BTNMATRIX_PART_BG,&style_btnm_bg_AddrSet);
	lv_obj_add_style(btnm,LV_BTNMATRIX_PART_BTN,&style_btnm_btn_AddrSet);
	lv_obj_set_event_cb(btnm, AddrSet_btnm_event_handle);
	btnm->user_data = ta_common_data;//addr_data
#if 0
	lv_obj_t *btnm_ctrl = lv_btnmatrix_create(obj_btnm_bg,NULL);
	lv_obj_set_size(btnm_ctrl, ctrl_btnm_width, ctrl_btnm_height);
	lv_btnmatrix_set_map(btnm_ctrl,addr_set_ctrl_btnm_map);
	lv_obj_align(btnm_ctrl,btnm, LV_ALIGN_OUT_BOTTOM_LEFT,0,0);
	lv_theme_apply(btnm_ctrl, (lv_theme_style_t)sigma_star_theme_btnmatrix_pwd);
#endif
	uint16_t ctrl_btn_width = btnm_width / 4, ctrl_btn_height = ctrl_btnm_height / 2;
#if 0
	lv_obj_t *btn_del = lv_btn_create(obj_btnm_bg,NULL);
	lv_obj_set_size(btn_del, btnm_width, ctrl_btn_height);
	
	lv_obj_t *label_of_btn = lv_label_create(btn_del,NULL);
	lv_label_set_text(label_of_btn,"删除");
	lv_theme_apply(label_of_btn,(lv_theme_style_t)sigma_star_theme_label_chinese);
#endif
	static lv_style_t btn_del_style,btn_confirm_style;
	lv_style_init(&btn_del_style);
	lv_style_set_bg_color(&btn_del_style,LV_STATE_DEFAULT,LV_COLOR_RED);
	lv_style_set_bg_opa(&btn_del_style,LV_STATE_DEFAULT,LV_OPA_60);

	lv_style_init(&btn_confirm_style);
	lv_style_copy(&btn_confirm_style,&btn_del_style);
	lv_style_set_bg_color(&btn_confirm_style, LV_STATE_DEFAULT, LV_COLOR_GREEN);

	lv_obj_t *btn_del = universal_btn_chinese_create(obj_btnm_bg, ctrl_btn_width, ctrl_btn_height,"删除");
	lv_obj_align(btn_del,btnm,LV_ALIGN_OUT_BOTTOM_LEFT,10,3);
	lv_obj_add_style(btn_del,LV_BTN_PART_MAIN,&btn_del_style);
	lv_obj_set_event_cb(btn_del, AddrSet_btn_ctrl_event_handle);
	btn_del->user_data = ta_common_data;//addr_data

	lv_obj_t *btn_confirm = universal_btn_chinese_create(obj_btnm_bg, ctrl_btn_width, ctrl_btn_height, "确认");
	lv_obj_align(btn_confirm,btnm,LV_ALIGN_OUT_BOTTOM_MID,10,3);
	lv_obj_add_style(btn_confirm, LV_BTN_PART_MAIN, &btn_confirm_style);
	lv_obj_set_event_cb(btn_confirm, AddrSet_btn_ctrl_event_handle);
	btn_confirm->user_data = ta_common_data;//addr_data

	return NULL;
}
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


#if 0
static void addr_user_data_in_queue(void *obj_type)
{
	if (is_null_ptr(obj_type))
		return;

	if (is_null_ptr(addr_set_free_queue_list))
		addr_set_free_queue_list = obj_type;
}
#endif

/*------------------BELOW------------------
************Address Set First Page********
-------------------------------------------*/

LV_EVENT_CB_DECLARE(address_into_second_page)
{
	if (e == LV_EVENT_CLICKED)
	{
		//_address_secpage_type *upper_info = (_address_secpage_type *)obj->user_data;
		_obj_user_data *upper_info = (_obj_user_data *)obj->user_data;
		if (is_null_ptr(upper_info)) {
			ui_printf("[Fail] enter address second page,user data is null\n");
			return;
		}
		((_address_secpage_type *)upper_info->upper_data)->index = upper_info->index;
		address_second_page_duplicate(upper_info);
	}
}

static _obj_user_data *addr_user_data_pack(uint8_t tag_index,_address_secpage_type *addr_data)
{
	_obj_user_data *obj_type = (_obj_user_data *)general_malloc(sizeof(_obj_user_data));
	if (is_null_ptr(obj_type))
		return NULL;

	obj_type->upper_data = addr_data;
	obj_type->index = tag_index;

	free_queue_tail_insert(&addr_set_free_queue_list,obj_type);

	return obj_type;
}

static lv_obj_t *address_tag_cont_create(const lv_obj_t *parent, char *tag_str)
{
	return info_page_tag_cont_create(parent,tag_str);
}

static lv_obj_t *address_data_cont_create(const lv_obj_t *parent, const lv_obj_t *copy,char *setting_data, _address_secpage_type *addr_type, _addrset_index tag_index)
{
	lv_obj_t *page_bg = parent;
	_obj_user_data *obj_type = addr_user_data_pack(tag_index,addr_type);
	if (is_null_ptr(obj_type))
		return NULL;

	lv_obj_t *cont_new = lv_cont_create(page_bg, copy);
	lv_obj_align(cont_new, copy, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
	lv_obj_set_size(cont_new, 180, 60);
	lv_obj_set_click(cont_new, true);
	lv_obj_set_event_cb(cont_new, address_into_second_page);
	cont_new->user_data = obj_type;

	lv_obj_t *obj_label = lv_label_create(cont_new, NULL);
	//lv_label_set_text(obj_label, addr_type->ch_data);
	lv_label_set_text(obj_label,setting_data);
	lv_theme_apply(obj_label, (lv_theme_style_t)sigma_star_theme_label_chinese);
	//addr_type->refresh_label = obj_label;
	addr_type->refresh_label[tag_index] = obj_label;

	return cont_new;
}


//address page memory control
static _address_secpage_type* address_page_malloc()
{
	_address_secpage_type *addr_type = (_address_secpage_type *)general_malloc(sizeof(_address_secpage_type) * ADDRESS_TAG_NUM);
	if (addr_type == NULL) {
		ui_printf("addr_type malloc fail\n");
		return NULL;
	}
	address_dynamic = addr_type;
	memset(addr_type, 0, sizeof(_address_secpage_type) * ADDRESS_TAG_NUM);

	return address_dynamic;
}
#if 0
static void address_dynamic_data_set(char *tag, _address_secpage_type *addr_type)
{
	if (is_null_ptr(addr_type))
		return;

	addr_type->str_tag = (char *)general_malloc(strlen(tag)+1);
	if (is_null_ptr(addr_type->str_tag))
		return;

	strcpy(addr_type->str_tag,tag);

	//addr_type->ch_data = ch_addr;

}
#endif

static void address_page_free()
{
	if (is_null_ptr(address_dynamic))
		return;

	general_free(address_dynamic);
	address_dynamic = NULL;

	free_queue_free_all(addr_set_free_queue_list,false);
	addr_set_free_queue_list = NULL;
}

static void sset_addr_ui_page_create()
{
	lv_obj_t *page_bg = NULL;

	lv_obj_t *new_bg = setting_get_cover_bg();
	page_bg = new_bg;

	_system_setting_data_memory *p_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);

	system_set_ui_first_page_title_create(page_bg, "地址设置");
	system_set_ui_root_widget_return_create(page_bg, address_page_free);

	_address_secpage_type *address_type = address_page_malloc();
	if (address_type == NULL) {
		ui_printf("address_type malloc fail\n");
		return;
	}
	address_type->addr_data = p_memory->addr_set;

	//lv_obj_t *cont_tag = NULL;
	lv_obj_t *cont_data = NULL;
	uint16_t up_down_space = 15;
//region
	lv_obj_t *cont_tag_region = address_tag_cont_create(page_bg, "区");
	lv_obj_set_pos(cont_tag_region, 30, 100);

	cont_data  = address_data_cont_create(page_bg, cont_tag_region, p_memory->addr_set->region, address_type, AddrSet_index_region);

//unit
	//address_type++;//important,do not ignore
	lv_obj_t *cont_tag_unit = address_tag_cont_create(page_bg, "单元");
	lv_obj_align(cont_tag_unit,cont_tag_region,LV_ALIGN_OUT_BOTTOM_LEFT,0,up_down_space);

	cont_data = address_data_cont_create(page_bg, cont_tag_unit, p_memory->addr_set->unit, address_type, AddrSet_index_unit);

//branch
	//address_type++;//important,do not ignore
	lv_obj_t *cont_tag_branch = address_tag_cont_create(page_bg, "分机");
	lv_obj_align(cont_tag_branch, cont_tag_unit, LV_ALIGN_OUT_BOTTOM_LEFT, 0, up_down_space);

	cont_data = address_data_cont_create(page_bg, cont_tag_branch, p_memory->addr_set->branch, address_type, AddrSet_index_branch);

//budiling
	//address_type++;//important,do not ignore
	lv_obj_t *cont_tag_building = address_tag_cont_create(page_bg, "栋");
	lv_obj_set_pos(cont_tag_building,430,100);
	
	cont_data = address_data_cont_create(page_bg, cont_tag_building, p_memory->addr_set->building, address_type, AddrSet_index_building);

//room
	//address_type++;//important,do not ignore
	lv_obj_t *cont_tag_room = address_tag_cont_create(page_bg,"房号");
	lv_obj_align(cont_tag_room, cont_tag_building, LV_ALIGN_OUT_BOTTOM_LEFT, 0, up_down_space);

	cont_data = address_data_cont_create(page_bg, cont_tag_room, p_memory->addr_set->room, address_type, AddrSet_index_room);
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
#if 0
static lv_obj_t *PwdChg_pwd_btnm_create(lv_obj_t *parent)
{
	uint16_t width = LV_HOR_RES, height = LV_VER_RES / 8 * 3;
	_universal_obj_element elm_btnm;
	elm_btnm.parent = parent;
	elm_btnm.width = width;
	elm_btnm.height = height;
	elm_btnm.user_data = NULL;
	elm_btnm.event_cb = NULL;

	lv_obj_t *btnm =  universal_btnm_create(&elm_btnm, pwd_btnm_map);
	lv_theme_apply(btnm,(lv_theme_style_t)sigma_star_theme_btnmatrix_pwd);

	return btnm;
}

LV_EVENT_CB_DECLARE(PwdChg_EventCB_pwd_change_cancel_btn)
{
	if (e == LV_EVENT_CLICKED)
	{
		lv_obj_t *page_bg = lv_obj_get_parent(lv_obj_get_parent(obj));
		lv_obj_del(page_bg);
	}
}

static lv_obj_t *PwdChg_pwd_ta_create(lv_obj_t *parent,lv_obj_t *copy,uint16_t width,uint16_t height)
{
	lv_obj_t *ta = lv_textarea_create(parent,NULL);
	lv_obj_set_size(ta,width,height);
	lv_textarea_set_one_line(ta,true);
	lv_textarea_set_cursor_hidden(ta,true);
	lv_textarea_set_pwd_mode(ta,true);
	lv_textarea_set_text(ta,"");
	lv_textarea_set_placeholder_text(ta,"Please insert");
	lv_obj_set_click(ta,true);

	return ta;
}

static void PWdChg_change_pwd_create(lv_obj_t *parent)
{
	lv_obj_t *page_bg = NULL;
	uint16_t base_width = LV_HOR_RES / 4 * 3, base_height = LV_VER_RES / 2;
	uint16_t base_x = LV_HOR_RES / 8, base_y = LV_VER_RES / 8;
	uint16_t ta_width = base_width / 2, ta_height = base_height / 4;
	uint16_t btn_width = base_width / 4, btn_height = ta_height;

	static lv_style_t style_obj_base;
	lv_style_init(&style_obj_base);
	lv_style_set_bg_color(&style_obj_base,LV_STATE_DEFAULT,LV_COLOR_GRAY);
	lv_style_set_border_color(&style_obj_base,LV_STATE_DEFAULT,LV_COLOR_BLUE);
	
	lv_obj_t *new_bg = setting_get_cover_bg();
	page_bg = new_bg;

	system_set_ui_first_page_title_create(page_bg,"系统密码设置");

	lv_obj_t *obj_base = lv_obj_create(page_bg,NULL);
	lv_obj_set_size(obj_base,base_width,base_height);
	lv_obj_set_pos(obj_base,base_x,base_y);
	lv_obj_add_style(obj_base,LV_OBJ_PART_MAIN,&style_obj_base);

	lv_obj_t *ta_up = PwdChg_pwd_ta_create(obj_base,NULL, ta_width,ta_height);
	lv_obj_align(ta_up,NULL,LV_ALIGN_IN_TOP_MID,0,20);
	
	lv_obj_t *tag_laebl = lv_label_create(obj_base,NULL);
	lv_theme_apply(tag_laebl,(lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_label_set_text(tag_laebl,"新密码");
	lv_obj_align(tag_laebl,ta_up,LV_ALIGN_OUT_LEFT_TOP,-3,10);
	
	lv_obj_t *ta_down = PwdChg_pwd_ta_create(obj_base, NULL, ta_width, ta_height);
	lv_obj_align(ta_down, ta_up,LV_ALIGN_OUT_BOTTOM_LEFT,0,20);

	tag_laebl = lv_label_create(obj_base, tag_laebl);
	lv_label_set_text(tag_laebl,"重复新密码");
	lv_obj_align(tag_laebl, ta_down, LV_ALIGN_OUT_LEFT_TOP, -3, 10);

	lv_obj_t *btn_cancel = universal_btn_chinese_create(obj_base, btn_width, btn_height, "取消");
	lv_obj_align(btn_cancel,ta_down,LV_ALIGN_OUT_BOTTOM_LEFT,0,10);
	lv_obj_set_event_cb(btn_cancel, PwdChg_EventCB_pwd_change_cancel_btn);
	lv_theme_apply(btn_cancel,(lv_theme_style_t)sigma_star_theme_Btn_home_return);

	lv_obj_t *btn_confirm = universal_btn_chinese_create(obj_base, btn_width, btn_height, "修改");
	lv_obj_align(btn_confirm,btn_cancel,LV_ALIGN_OUT_RIGHT_TOP,10,0);
	lv_theme_apply(btn_confirm, (lv_theme_style_t)sigma_star_theme_Btn_home_return);

	lv_obj_t *pwd_btnm = PwdChg_pwd_btnm_create(page_bg);
	//lv_obj_align(pwd_btnm,obj_base,LV_ALIGN_OUT_BOTTOM_LEFT,0,0);
	lv_obj_align_y(pwd_btnm,obj_base, LV_ALIGN_OUT_TOP_LEFT,base_y);
	//lv_obj_align_x(pwd_btnm,obj_base, LV_ALIGN_OUT_BOTTOM_LEFT,0);
}

LV_EVENT_CB_DECLARE(PWdChg_change_pwd_event)
{
	if (e == LV_EVENT_CLICKED)
	{
		PWdChg_change_pwd_create(NULL);
	}
}

static lv_obj_t *PwdChg_tag_cont_create(const lv_obj_t *parent,char *tag_txt)
{
	lv_obj_t *obj_cont = info_page_tag_cont_create(parent,tag_txt);
	lv_obj_set_click(obj_cont,true);
	lv_obj_set_size(obj_cont,150,60);

	return obj_cont;
}
#endif

int sset_sys_pwd_ui_del_warning(void *obj_label)
{
	lv_obj_del((lv_obj_t *)obj_label);

	return 0;
}
#
static void sset_sys_pwd_ui_wrong_waring(char *warning_txt)
{
	lv_obj_t *label = lv_label_create(lv_scr_act(),NULL);
	lv_label_set_text(label,warning_txt);
	lv_theme_apply(label,(lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_obj_align(label,NULL,LV_ALIGN_IN_TOP_MID,0,80);
	
	universal_task_100ms_timer_create(6,(uint32_t *)&sset_sys_pwd_ui_del_warning,label);
}

LV_EVENT_CB_DECLARE(sset_sys_pwd_eventcb_btn_confirm)
{
	if (e == LV_EVENT_CLICKED)
	{
		_pwd_sys_ta_type *ta_data = (_pwd_sys_ta_type *)obj->user_data;
		if (is_null_ptr(ta_data)) return;

		//first:make sure all three pwds are 6 characters
		const *txt_old = lv_textarea_get_text(ta_data->pwd_ta[pwd_change_old]);
		if (strlen(txt_old) != PASSWORD_MAX_LEN - 1) {
			ui_printf("old_password not 6 characters\n");
			sset_sys_pwd_ui_wrong_waring("密码错误");
			return;
		}
		const *txt_new = lv_textarea_get_text(ta_data->pwd_ta[pwd_change_new]);
		if (strlen(txt_new) != PASSWORD_MAX_LEN - 1) {
			ui_printf("new_password not 6 characters\n");
			sset_sys_pwd_ui_wrong_waring("请输入6位密码");
			return;
		}
		const *txt_again = lv_textarea_get_text(ta_data->pwd_ta[pwd_change_again]);
		if (strlen(txt_again) != PASSWORD_MAX_LEN - 1) {
			ui_printf("again_password not 6 characters\n");
			sset_sys_pwd_ui_wrong_waring("请输入6位密码");
			return;
		}
		//second:compare whether new equal again
		if (strcmp(txt_new, txt_again) != 0) {
			ui_printf("new pwd not equal again\n");
			sset_sys_pwd_ui_wrong_waring("新密码不一致");
			return;
		}
		//in the end:check whether old pwd is correct
		if (strcmp(txt_old, ta_data->sys_pwd) != 0) {
			ui_printf("old pwd not correct\n");
			sset_sys_pwd_ui_wrong_waring("密码错误");
			return;
		}
		strcpy(ta_data->sys_pwd,txt_new);
		ui_printf("system password changed!\n");
		for (int i = pwd_change_old; i <= pwd_change_again; i++)
			lv_textarea_set_text(ta_data->pwd_ta[i],"");

		sset_sys_pwd_ui_wrong_waring("密码设置成功");
	}
}

static void sset_sys_pwd_btnm_match_ta_control(_btnm_ctrl_type ctrl_t, const char *txt, lv_obj_t *ta, char *get_data)
{
	AddrSet_btnm_match_ta_control(ctrl_t,txt,ta,get_data);
}

LV_EVENT_CB_DECLARE(sset_sys_pwd_event_btnm)
{
	if (e != LV_EVENT_VALUE_CHANGED) return;
	
	const char *txt = lv_btnmatrix_get_active_btn_text(obj);
	if (is_null_ptr(txt)) return;

	_pwd_sys_ta_type *ta_data = (_pwd_sys_ta_type *)(obj->user_data);
	_btnm_ctrl_type ctrl_t = btnm_ctrl_add;

	if (strcmp(txt, LV_SYMBOL_TRASH) == 0)
		ctrl_t = btnm_ctrl_del;

	lv_obj_t *match_ta = ta_data->pwd_ta[ta_data->index];
	sset_sys_pwd_btnm_match_ta_control(ctrl_t,txt,match_ta,NULL);
}
//ta event，1：change index 2：hide/unhide cursor of ta
LV_EVENT_CB_DECLARE(sset_sys_pwd_eventcb_ta_common)
{
	if (e == LV_EVENT_CLICKED)
	{
		//_pwd_sys_ta_type *ta_data = (_pwd_sys_ta_type *)obj->user_data;
		_obj_user_data_type *obj_data = (_obj_user_data_type *)obj->user_data;
		_pwd_sys_ta_type *ta_data = (_pwd_sys_ta_type *)obj_data->data;
		if (ta_data->index == obj_data->index) return;

		lv_textarea_set_cursor_hidden(ta_data->pwd_ta[ta_data->index],true);//hide old
		lv_textarea_set_cursor_hidden(ta_data->pwd_ta[obj_data->index], false);//unhide new
		
		ta_data->index = obj_data->index;
	}
}

static int sset_sys_pwd_data_pack_ta(lv_obj_t *ta,_pwd_sys_ta_type *ta_data,pwd_change_type ta_index)
{
	_obj_user_data_type *obj_data = (_obj_user_data_type *)general_malloc(sizeof(_obj_user_data_type));
	if (is_null_ptr(obj_data)) return -1;

	ta_data->pwd_ta[ta_index] = ta;
	
	obj_data->index = ta_index;
	obj_data->data = ta_data;

	ta->user_data = obj_data;

	free_queue_tail_insert(&ta_data->free_queue_ta_obj,obj_data);

	return 0;
}

static lv_obj_t *sset_sys_pwd_ui_ta_duplicate(lv_obj_t *parent,lv_obj_t *copy,char *ta_beside_txt)
{
	_universal_obj_element elm_ta;
	elm_ta.parent = parent;
	elm_ta.width = LV_HOR_RES / 4;
	elm_ta.height = LV_VER_RES / 8;
	
	lv_obj_t *ta = universal_ta_pwd_create(&elm_ta,copy,true,"Please Insert");
	lv_obj_set_event_cb(ta, sset_sys_pwd_eventcb_ta_common);

	lv_obj_t *label = lv_label_create(parent,NULL);
	lv_label_set_text(label,ta_beside_txt);
	lv_theme_apply(label,(lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_obj_align(label,ta,LV_ALIGN_OUT_LEFT_MID,-3,0);

	return ta;
}

static lv_obj_t *sset_sys_pwd_ui_btnm_create(lv_obj_t *parent,uint16_t height)
{
	_universal_obj_element elm_btnm;
	memset(&elm_btnm,0,sizeof(_universal_obj_element));//init for avoiding unexpected situation
	elm_btnm.parent = parent;
	elm_btnm.width = LV_HOR_RES / 4 + 50;
	elm_btnm.height = height;
	elm_btnm.event_cb = NULL;

	static lv_style_t style_btnm_bg_sys_pwd;
	lv_style_init(&style_btnm_bg_sys_pwd);
	lv_style_set_bg_color(&style_btnm_bg_sys_pwd,LV_STATE_DEFAULT,LV_COLOR_GRAY);
	lv_style_set_bg_opa(&style_btnm_bg_sys_pwd,LV_STATE_DEFAULT,LV_OPA_60);
	lv_style_set_pad_inner(&style_btnm_bg_sys_pwd,LV_STATE_DEFAULT,10);
	lv_style_set_pad_left(&style_btnm_bg_sys_pwd,LV_STATE_DEFAULT,10);
	lv_style_set_pad_right(&style_btnm_bg_sys_pwd,LV_STATE_DEFAULT,10);
	lv_style_set_border_color(&style_btnm_bg_sys_pwd,LV_STATE_DEFAULT | LV_STATE_PRESSED,LV_COLOR_SILVER);

	static lv_style_t style_btnm_btn_sys_pwd;
	lv_style_init(&style_btnm_btn_sys_pwd);
	lv_style_set_bg_color(&style_btnm_btn_sys_pwd,LV_STATE_DEFAULT,LV_COLOR_GRAY);
	lv_style_set_text_color(&style_btnm_btn_sys_pwd,LV_STATE_DEFAULT,LV_COLOR_WHITE);
	
	lv_obj_t *btnm = universal_btnm_create(&elm_btnm, btnm_map_sset_pwd_change);
	lv_obj_add_style(btnm,LV_BTNMATRIX_PART_BG,&style_btnm_bg_sys_pwd);
	lv_obj_add_style(btnm,LV_BTNMATRIX_PART_BTN,&style_btnm_btn_sys_pwd);
	
	return btnm;
}

static lv_obj_t *sset_sys_pwd_ui_confirm_btn_create(lv_obj_t *parent)
{
	_universal_obj_element elm_btn;
	elm_btn.parent = parent;
	elm_btn.width = 100;
	elm_btn.height = 60;
	
	lv_obj_t *btn = universal_btn_label_chineses_create(&elm_btn,"确认");
	lv_theme_apply(btn,(lv_theme_style_t)sigma_star_theme_Btn_home_return);

	lv_obj_set_event_cb(btn, sset_sys_pwd_eventcb_btn_confirm);
	
	return btn;
}
_pwd_sys_ta_type *pwd_sys_ta_common_data = NULL;
static _pwd_sys_ta_type* sset_sys_pwd_data_common_malloc(char *sys_pwd)
{
	_pwd_sys_ta_type *common_data = (_pwd_sys_ta_type *)general_malloc(sizeof(_pwd_sys_ta_type));
	if (is_null_ptr(common_data))
		return NULL;

	memset(common_data,0,sizeof(_pwd_sys_ta_type));

	common_data->sys_pwd = sys_pwd;

	pwd_sys_ta_common_data = common_data;//this is not good

	return common_data;
}
static sset_sys_pwd_data_common_free()
{
	if (is_null_ptr(pwd_sys_ta_common_data))
		return;

	free_queue_free_all(pwd_sys_ta_common_data->free_queue_ta_obj,false);
	general_free(pwd_sys_ta_common_data);
	pwd_sys_ta_common_data = NULL;
}

static void sset_sys_pwd_ui_page_create()
{
	uint16_t pwd_ta_top_bottom_space = 20;
	lv_obj_t *page_bg = NULL;

	_system_setting_data_memory *p_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);

	lv_obj_t *new_bg = setting_get_cover_bg();
	page_bg = new_bg;

	setting_ui_head_title_create(page_bg,"密码设置");

	system_set_ui_root_widget_return_create(page_bg, sset_sys_pwd_data_common_free);

	_pwd_sys_ta_type *ta_common_d = sset_sys_pwd_data_common_malloc(&p_memory->sys_pwd->pwd[0]);
	if (is_null_ptr(ta_common_d))
	{
		ui_printf("sys_pwd,ta_common_data malloc fail\n");
		return;
	}

	static lv_style_t cont_bg_style;
	lv_style_init(&cont_bg_style);	
	lv_style_set_bg_color(&cont_bg_style,LV_STATE_DEFAULT,LV_COLOR_PURPLE);
	lv_style_set_pad_inner(&cont_bg_style,LV_STATE_DEFAULT,20);
//three textarea
	lv_obj_t *cont_bg = lv_cont_create(page_bg,NULL);
	//lv_cont_set_layout(cont_bg,LV_LAYOUT_COLUMN_MID);
	lv_cont_set_layout(cont_bg,LV_LAYOUT_OFF);
	lv_cont_set_fit(cont_bg,LV_FIT_TIGHT);
	lv_obj_align(cont_bg,NULL,LV_ALIGN_CENTER,-100,0);
	//lv_theme_apply(cont_bg,(lv_theme_style_t)sigma_star_theme_default_bg);
	lv_obj_add_style(cont_bg,LV_OBJ_PART_MAIN,&cont_bg_style);

	lv_obj_t *ta_old = sset_sys_pwd_ui_ta_duplicate(cont_bg,NULL,"旧密码:");
	lv_obj_set_pos(ta_old,10,10);
	lv_obj_realign(lv_obj_get_child(cont_bg,NULL));
	sset_sys_pwd_data_pack_ta(ta_old,ta_common_d,pwd_change_old);
	ta_common_d->index = pwd_change_old;
	lv_textarea_set_cursor_hidden(ta_old,false);

	lv_obj_t *ta_new = sset_sys_pwd_ui_ta_duplicate(cont_bg,NULL,"新密码:");
	lv_obj_align(ta_new, ta_old, LV_ALIGN_OUT_BOTTOM_LEFT, 0, pwd_ta_top_bottom_space);
	lv_obj_realign(lv_obj_get_child(cont_bg, NULL));
	sset_sys_pwd_data_pack_ta(ta_new, ta_common_d, pwd_change_new);

	lv_obj_t *ta_again = sset_sys_pwd_ui_ta_duplicate(cont_bg, NULL, "确认新密码:");
	lv_obj_align(ta_again, ta_new, LV_ALIGN_OUT_BOTTOM_LEFT, 0, pwd_ta_top_bottom_space);
	lv_obj_realign(lv_obj_get_child(cont_bg, NULL));
	sset_sys_pwd_data_pack_ta(ta_again, ta_common_d, pwd_change_again);

	lv_obj_realign(cont_bg);//realign cont_bg after ta created
//password btnmatrix
	lv_obj_t *pwd_btnm = sset_sys_pwd_ui_btnm_create(page_bg,lv_obj_get_height(cont_bg));
	lv_obj_align(pwd_btnm, cont_bg, LV_ALIGN_OUT_RIGHT_TOP, 10, 0);
	lv_obj_set_event_cb(pwd_btnm, sset_sys_pwd_event_btnm);
	pwd_btnm->user_data = ta_common_d;
//confirm btn
	lv_obj_t *btn_confirm = sset_sys_pwd_ui_confirm_btn_create(page_bg);
	lv_obj_align(btn_confirm,pwd_btnm,LV_ALIGN_OUT_BOTTOM_MID,0,0);
	btn_confirm->user_data = ta_common_d;
	
#if 0
	lv_obj_t *cont_sys_set = PwdChg_tag_cont_create(cont_bg,"系统设置密码");
	lv_obj_set_event_cb(cont_sys_set, PWdChg_change_pwd_event);

	lv_obj_t *cont_security = PwdChg_tag_cont_create(cont_bg,"安防密码");
#endif
	
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
static lv_obj_t *sset_genl_pwd_ui_pwd_mode_create(lv_obj_t *parent,char *mode_txt)
{
	_universal_obj_element elm_btn;
	elm_btn.parent = parent;
	elm_btn.width = 100;
	elm_btn.height = 50;

	static lv_style_t style_btn_pwd_mode;
	lv_style_init(&style_btn_pwd_mode);
	lv_style_set_bg_color(&style_btn_pwd_mode,LV_STATE_DEFAULT,LV_COLOR_PURPLE);
	
	lv_obj_t *btn = universal_btn_label_chineses_create(&elm_btn,mode_txt);
	lv_obj_add_style(btn,LV_BTN_PART_MAIN,&style_btn_pwd_mode);
	
	return btn;
}

static lv_obj_t *sset_genl_pwd_ui_confirm_btn_create(lv_obj_t *parent)
{
	lv_obj_t *btn = sset_sys_pwd_ui_confirm_btn_create(parent);
	lv_obj_set_event_cb(btn,NULL);

	return btn;
}

static lv_obj_t *sset_genl_pwd_ui_btnm_create(lv_obj_t *parent, uint16_t height)
{
	return sset_sys_pwd_ui_btnm_create(parent,height);
}

static lv_obj_t *sset_genl_pwd_ui_ta_duplicate(lv_obj_t *parent, lv_obj_t *copy, char *ta_beside_txt)
{
	lv_obj_t *ta = sset_sys_pwd_ui_ta_duplicate(parent,copy,ta_beside_txt);
	lv_obj_set_event_cb(ta,NULL);

	return ta;
}

static void sset_genl_pwd_ui_page_create()
{
	uint16_t pwd_ta_top_bottom_space = 20;
	lv_obj_t *page_bg = NULL;
	_system_setting_data_memory *p_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);
	
	lv_obj_t *new_bg = get_a_default_bg();
	page_bg = new_bg;
	
	setting_ui_head_title_create(page_bg,"密码设置");

	system_set_ui_initN_widget_return_create(page_bg,NULL);

	static lv_style_t cont_bg_style;
	lv_style_init(&cont_bg_style);
	lv_style_set_bg_color(&cont_bg_style, LV_STATE_DEFAULT, LV_COLOR_PURPLE);
	lv_style_set_pad_inner(&cont_bg_style, LV_STATE_DEFAULT, 20);
	lv_style_set_bg_opa(&cont_bg_style,LV_STATE_DEFAULT,LV_OPA_TRANSP);

//three textarea
	lv_obj_t *cont_bg = lv_cont_create(page_bg, NULL);
	//lv_cont_set_layout(cont_bg,LV_LAYOUT_COLUMN_MID);
	lv_cont_set_layout(cont_bg, LV_LAYOUT_OFF);
	lv_cont_set_fit(cont_bg, LV_FIT_TIGHT);
	lv_obj_align(cont_bg, NULL, LV_ALIGN_CENTER, -100, 0);
	//lv_theme_apply(cont_bg,(lv_theme_style_t)sigma_star_theme_default_bg);
	lv_obj_add_style(cont_bg, LV_OBJ_PART_MAIN, &cont_bg_style);

	lv_obj_t *ta_old = sset_genl_pwd_ui_ta_duplicate(cont_bg, NULL, "旧密码:");
	lv_obj_set_pos(ta_old, 10, 10);
	lv_obj_realign(lv_obj_get_child(cont_bg, NULL));
	lv_textarea_set_cursor_hidden(ta_old, false);

	lv_obj_t *ta_new = sset_genl_pwd_ui_ta_duplicate(cont_bg, NULL, "新密码:");
	lv_obj_align(ta_new, ta_old, LV_ALIGN_OUT_BOTTOM_LEFT, 0, pwd_ta_top_bottom_space);
	lv_obj_realign(lv_obj_get_child(cont_bg, NULL));

	lv_obj_t *ta_again = sset_genl_pwd_ui_ta_duplicate(cont_bg, NULL, "确认新密码:");
	lv_obj_align(ta_again, ta_new, LV_ALIGN_OUT_BOTTOM_LEFT, 0, pwd_ta_top_bottom_space);
	lv_obj_realign(lv_obj_get_child(cont_bg, NULL));

	lv_obj_realign(cont_bg);//realign cont_bg after ta created
//password btnmatrix
	lv_obj_t *pwd_btnm = sset_genl_pwd_ui_btnm_create(page_bg, lv_obj_get_height(cont_bg));
	lv_obj_align(pwd_btnm, cont_bg, LV_ALIGN_OUT_RIGHT_TOP, 10, 0);
	//lv_obj_set_event_cb(pwd_btnm, sset_sys_pwd_event_btnm);

	//confirm btn
	lv_obj_t *btn_confirm = sset_genl_pwd_ui_confirm_btn_create(page_bg);
	lv_obj_align(btn_confirm, pwd_btnm, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
//pwd mode switch
	lv_obj_t *btn_mode_security = sset_genl_pwd_ui_pwd_mode_create(page_bg,"安防密码");
	lv_obj_align(btn_mode_security,cont_bg,LV_ALIGN_OUT_TOP_LEFT,0,0);

	lv_obj_t *btn_mode_more = sset_genl_pwd_ui_pwd_mode_create(page_bg,"...密码");
	lv_obj_align(btn_mode_more,btn_mode_security,LV_ALIGN_OUT_RIGHT_TOP,0,0);
	
}

/*******************BELOW************************************
*************System Setting Init Page Needn't Password******* 
************************************************************/
/*Foramt:
***@ Prefix:sset_initN_
***@ UI draw: sset_initN_ui_
***@ event_cb: sset_initN_eventcb_
*/

/*****
->Below<- Password for entering project setting
********/
/*>>>>>>>>>>>>>>>>>>>>>>>>*/
static void sset_initN_root_pwd_ta_control(_btnm_ctrl_type ctrl_t,const char *txt,void *get_data)
{
	if (btnm_ctrl_null == ctrl_t) return;

	_system_setting_data_dynamic *p_dynamic = (_system_setting_data_dynamic *)get_local_pointer(p_data_dynamic);
	switch (ctrl_t)
	{
	  case btnm_ctrl_add:
		  ui_printf("pwd_ta_control,add:%s\r\n",txt);
		  lv_textarea_add_text(p_dynamic->pwd_ta,txt);
		break;

	  case btnm_ctrl_del:
		  lv_textarea_del_char(p_dynamic->pwd_ta);
		  break;

	  case btnm_ctrl_check:
		  ui_printf("pwd_ta_control,check:%s\r\n", lv_textarea_get_text(p_dynamic->pwd_ta));
		  ui_printf("get_data:%p,(char*)get_data:%p\r\n",get_data,(char *)get_data);
		  const char *temp = lv_textarea_get_text(p_dynamic->pwd_ta);
		  *(char **)get_data = temp;
		  ui_printf("get_data:%s\r\n",*(char **)get_data);
		  break;

	  default:
		  break;
	}
}

static lv_obj_t* sset_initN_root_pwd_wrong_create()
{
	lv_obj_t *bg_top = lv_obj_create(lv_scr_act(), NULL);
	lv_theme_apply(bg_top, (lv_theme_style_t)sigma_star_theme_default_bg);
	lv_obj_set_style_local_bg_opa(bg_top,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_OPA_COVER);
	lv_obj_set_size(bg_top, LV_HOR_RES, LV_VER_RES);
	lv_obj_set_pos(bg_top, 0, 0);

	lv_obj_t *obj_label = lv_label_create(bg_top,NULL);
	lv_theme_apply(obj_label,(lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_label_set_text(obj_label,"密码错误");
	lv_obj_align(obj_label,NULL,LV_ALIGN_CENTER,0,0);

	return bg_top;
}

static void sset_initN_root_pwd_confirm_control(const char *pwd_txt,lv_obj_t *pwd_bg)
{
#if 1//enable this if debug so that no need to add insert password every time
	lv_obj_del(pwd_bg);
	sset_root_ui_page_create();
#endif

#if 0 //enable this if release
	if (pwd_txt != NULL) {
		if (strcmp(pwd_txt, "123456") == 0) {
			ui_printf("password passed\r\n");
			lv_obj_del(pwd_bg);
			sset_root_ui_page_create();
		}
		else {
			ui_printf("password wrong\r\n");
			lv_obj_t *obj_bg = sset_initN_root_pwd_wrong_create();
			system_setting_task_create(obj_bg);
		}
	}
#endif
}

LV_EVENT_CB_DECLARE(sset_initN_eventcb_pwd_btnm)
{
	if (e != LV_EVENT_VALUE_CHANGED) return;

	const char *txt = lv_btnmatrix_get_active_btn_text(obj);
	if (txt == NULL) return;
	ui_printf("pwd btnm,btn clicked\r\n");

	if (strcmp(txt, "删除") == 0) {
		sset_initN_root_pwd_ta_control(btnm_ctrl_del, NULL, NULL);
		return;
	}
	else if (strcmp(txt, "确认") == 0) {
		char *get_txt = NULL;
		ui_printf("get_txt:%s,&get_txt:%p\r\n", get_txt, &get_txt);
		sset_initN_root_pwd_ta_control(btnm_ctrl_check, NULL, &get_txt);
		//ui_printf("pwd confirm,get_txt:%s\r\n",get_txt);
		sset_initN_root_pwd_confirm_control(get_txt,(lv_obj_t *)obj->user_data);
		return;
	}
	else {
		sset_initN_root_pwd_ta_control(btnm_ctrl_add, txt, NULL);
	}
}

static void sset_initN_ui_root_pwd_title_create(lv_obj_t *parent)
{
	lv_obj_t *obj_label = lv_label_create(parent, NULL);
	lv_theme_apply(obj_label, (lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_label_set_text(obj_label, "请输入密码");
	lv_obj_align(obj_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 80);
}

static void sset_initN_ui_root_pwd_add_ta(lv_obj_t *obj_ta)
{
	if (obj_ta == NULL) return;

	_system_setting_data_dynamic *p_dynamic = (_system_setting_data_dynamic *)get_local_pointer(p_data_dynamic);
	p_dynamic->pwd_ta = obj_ta;
	ui_printf("pwd add textarea successfully\r\n");
}

LV_EVENT_CB_DECLARE(sset_initN_eventcb_root_pwd_cancel)
{
	if (e == LV_EVENT_CLICKED)
	{
		lv_obj_del(lv_obj_get_parent(obj));
		sset_initN_ui_create();
	}
}

static void sset_initN_ui_root_pwd_page_create()
{
	lv_obj_t *pwd_bg = get_a_default_bg();
	pwd_page = pwd_bg;

	sset_initN_ui_root_pwd_title_create(pwd_bg);

	uint16_t ta_width = LV_HOR_RES / 4 * 3;
	uint16_t ta_height = 60;
	uint16_t btnm_height = LV_VER_RES / 2;
	uint16_t base_x = LV_HOR_RES / 8;
	lv_obj_t *obj_ta = lv_textarea_create(pwd_bg, NULL);
	lv_theme_apply(obj_ta, (lv_theme_style_t)sigma_star_theme_ta);
	//lv_textarea_set_one_line(obj_ta,true);
	lv_textarea_set_cursor_hidden(obj_ta, true);
	lv_textarea_set_text_align(obj_ta, LV_LABEL_ALIGN_CENTER);
	lv_textarea_set_accepted_chars(obj_ta, "0123456789");
	lv_textarea_set_max_length(obj_ta, 10);
	lv_obj_set_size(obj_ta, ta_width, ta_height);
	lv_obj_set_pos(obj_ta, base_x, 120);
	lv_textarea_set_text(obj_ta, "");
	lv_textarea_set_pwd_mode(obj_ta, true);

	sset_initN_ui_root_pwd_add_ta(obj_ta); //save ta to system setting pwd

	lv_obj_t *obj_btnm = lv_btnmatrix_create(pwd_bg, NULL);
	lv_obj_set_size(obj_btnm, ta_width, btnm_height);
	lv_obj_align(obj_btnm, obj_ta, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
	lv_btnmatrix_set_map(obj_btnm, pwd_btnm_map);
	lv_theme_apply(obj_btnm, (lv_theme_style_t)sigma_star_theme_btnmatrix_pwd);
	lv_obj_set_event_cb(obj_btnm, sset_initN_eventcb_pwd_btnm);
	obj_btnm->user_data = pwd_bg;

	lv_obj_t *obj_btn = lv_btn_create(pwd_bg, NULL);
	lv_obj_set_size(obj_btn, 150, 50);
	lv_obj_set_pos(obj_btn, LV_HOR_RES - 150 - 30, 30);
	lv_theme_apply(obj_btn, (lv_theme_style_t)sigma_star_theme_Btn_home_return);
	lv_obj_set_event_cb(obj_btn, sset_initN_eventcb_root_pwd_cancel);

	lv_obj_t *obj_label = lv_label_create(obj_btn, NULL);
	lv_label_set_text(obj_label, "返回");

}
/*<<<<<<<<<<<<<<<<<<<<<<<<<*/

LV_EVENT_CB_DECLARE(widget_callback_system_info)
{	
	if (e == LV_EVENT_CLICKED) 
	{
		sset_info_ui_page_create();
	}
}

LV_EVENT_CB_DECLARE(widget_callback_addr_set)
{
	if (e == LV_EVENT_CLICKED)
	{
		sset_addr_ui_page_create();
	}
}

LV_EVENT_CB_DECLARE(sset_initN_eventcb_auto)//widget_callback_auto
{
	if (e == LV_EVENT_CLICKED)
	{
		if (!is_null_ptr(obj->user_data))
		{
			((void_fun_void)obj->user_data)();
			lv_obj_del(lv_obj_get_parent(obj));
		}
	}
}

LV_EVENT_CB_DECLARE(sset_initN_eventcb_root_setting)//widget_callback_root_setting
{
	if (e == LV_EVENT_CLICKED)
	{
		sset_initN_ui_root_pwd_page_create();
		lv_obj_del(lv_obj_get_parent(obj));
	}
}

static lv_obj_t *sset_initN_ui_widget_duplicate(lv_obj_t *parent,void *scr_img,char *label_str)
{
	lv_obj_t *wt_cont = lv_cont_create(parent, NULL);
	lv_cont_set_fit(wt_cont, LV_FIT_TIGHT);
	lv_theme_apply(wt_cont, (lv_theme_style_t)sigma_star_theme_sysset_widet_cont);
	lv_obj_set_click(wt_cont, true);
	lv_cont_set_layout(wt_cont, LV_LAYOUT_CENTER);

	lv_obj_t *obj_img = lv_img_create(wt_cont, NULL);
	lv_img_set_src(obj_img, scr_img);

	lv_obj_t *img_label = lv_label_create(wt_cont, NULL);
	lv_label_set_text(img_label, label_str);
	lv_theme_apply(img_label, (lv_theme_style_t)sigma_star_theme_label_chinese);

	return wt_cont;
}

static void sset_initN_ui_create()
{
	LV_IMG_DECLARE(lovelyghost_6464);

	//lv_obj_del(pwd_page);//delete password page entering
	
	lv_obj_t *bg = get_a_default_bg();

	lv_obj_t *obj_head = setting_ui_head_title_create(bg,"系统设置");
	lv_obj_t *return_btn = setting_ui_head_return_create(bg,"返回", SysSet_return_btn_event_handle);
	//lv_obj_align(return_btn, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
#if 0
	lv_obj_t *return_btn = lv_btn_create(bg, NULL);
	lv_obj_set_size(return_btn, 120, 80);
	lv_obj_align(return_btn, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
	lv_theme_apply(return_btn, (lv_theme_style_t)sigma_star_theme_Btn_home_return);
	lv_obj_set_event_cb(return_btn, SysSet_return_btn_event_handle);

	lv_obj_t* obj_label = lv_label_create(return_btn, NULL);
	lv_label_set_text(obj_label, "返回");
	lv_theme_apply(obj_label, (lv_theme_style_t)sigma_star_theme_label_chinese);
#endif
//widgets
	uint16_t left_right_space_up = 150;
	uint16_t left_right_space_down = 200;
	//uint16_t top_below_space = 100;
	uint16_t top_x_coo = 100, top_y_coo = 120;
	uint16_t bottom_x_coo = top_x_coo + 100, bottom_y_coo = top_y_coo + 160;

	lv_obj_t *set_widget_scrpro = sset_initN_ui_widget_duplicate(bg, &lovelyghost_6464, "屏幕设置");
	lv_obj_set_pos(set_widget_scrpro, top_x_coo, top_y_coo);
	//lv_obj_align(set_widget_scrpro, set_widget, LV_ALIGN_OUT_RIGHT_TOP, inner_space, 0);

	lv_obj_t *set_widget_timer = sset_initN_ui_widget_duplicate(bg, &lovelyghost_6464, "时间设置");
	lv_obj_align(set_widget_timer, set_widget_scrpro, LV_ALIGN_OUT_RIGHT_TOP, left_right_space_up, 0);

	lv_obj_t *set_widget_password = sset_initN_ui_widget_duplicate(bg, &lovelyghost_6464, "密码设置");
	lv_obj_align(set_widget_password, set_widget_timer, LV_ALIGN_OUT_RIGHT_TOP, left_right_space_up, 0);
	lv_obj_set_event_cb(set_widget_password, sset_initN_eventcb_auto);
	set_widget_password->user_data = sset_genl_pwd_ui_page_create;

	lv_obj_t *set_widget_ring = sset_initN_ui_widget_duplicate(bg, &lovelyghost_6464, "铃声设置");
	//lv_obj_align(set_widget_ring, set_widget_password, LV_ALIGN_OUT_RIGHT_TOP, inner_space_b, 0);
	lv_obj_set_pos(set_widget_ring, bottom_x_coo, bottom_y_coo);

	lv_obj_t *set_widget_root = sset_initN_ui_widget_duplicate(bg, &lovelyghost_6464, "工程设置");
	lv_obj_align(set_widget_root, set_widget_ring, LV_ALIGN_OUT_RIGHT_TOP, left_right_space_down, 0);	
	lv_obj_set_event_cb(set_widget_root, sset_initN_eventcb_root_setting);
	//set_widget_root->user_data = system_setting_pwd_page_create;

#if 0
	lv_obj_t *set_widget = settting_widget_duplicate(lv_scr_act(),&lovelyghost_6464,"系统信息");
	lv_obj_set_pos(set_widget,80,120);
	lv_obj_set_event_cb(set_widget, widget_callback_system_info);	

	lv_obj_t *set_widget_addr = settting_widget_duplicate(lv_scr_act(), &lovelyghost_6464, "地址信息");
	lv_obj_align(set_widget_addr, set_widget_scrpro, LV_ALIGN_OUT_RIGHT_TOP, inner_space, 0);
	lv_obj_set_event_cb(set_widget_addr, widget_callback_addr_set);

	lv_obj_t *set_widget_upgrade = settting_widget_duplicate(lv_scr_act(), &lovelyghost_6464, "系统升级");
	lv_obj_align(set_widget_upgrade, set_widget_addr, LV_ALIGN_OUT_RIGHT_TOP, inner_space, 0);
#endif

}

/*******************BELOW************************************
*************System Setting root Page Need Password*******
************************************************************/
/*Foramt:
***@ Prefix:sset_root_
***@ UI draw: sset_root_ui_
***@ event_cb: sset_root_eventcb_
*/
LV_EVENT_CB_DECLARE(sset_root_eventcb_general_enter_page)
{
	if (e == LV_EVENT_CLICKED)
	{
		if (!is_null_ptr(obj->user_data)) {
			(*((void_fun_void)obj->user_data))();
			lv_obj_del(lv_obj_get_parent(obj));
		}
	}
}

LV_EVENT_CB_DECLARE(sset_root_eventcb_return)
{
	if (e == LV_EVENT_CLICKED) {
		sset_initN_ui_create();
		lv_obj_del(lv_obj_get_parent(obj));
	}	
}

static lv_obj_t *sset_root_ui_widget_duplicate(lv_obj_t *parent, void *scr_img, char *label_str)
{
	return sset_initN_ui_widget_duplicate(parent,scr_img,label_str);
}

static void sset_root_ui_page_create()
{
	LV_IMG_DECLARE(lovelyghost_6464);

	lv_obj_t *bg = get_a_default_bg();

	lv_obj_t *obj_head = setting_ui_head_title_create(bg, "系统设置");
	setting_ui_head_return_create(bg,"返回", sset_root_eventcb_return);

//widgets
	uint16_t left_right_space_up = 200;
	uint16_t left_right_space_down = left_right_space_up;
	//uint16_t top_below_space = 100;
	uint16_t top_x_coo = 100 + 100, top_y_coo = 120;
	uint16_t bottom_x_coo = top_x_coo, bottom_y_coo = top_y_coo + 160;

	lv_obj_t *root_widget_info = sset_root_ui_widget_duplicate(bg,&lovelyghost_6464,"系统信息");
	lv_obj_set_pos(root_widget_info,top_x_coo,top_y_coo);
	lv_obj_set_event_cb(root_widget_info, sset_root_eventcb_general_enter_page);
	root_widget_info->user_data = sset_info_ui_page_create;

	lv_obj_t *root_widget_address = sset_root_ui_widget_duplicate(bg, &lovelyghost_6464, "地址信息");
	lv_obj_align(root_widget_address, root_widget_info,LV_ALIGN_OUT_RIGHT_TOP,left_right_space_up,0);
	lv_obj_set_event_cb(root_widget_address, sset_root_eventcb_general_enter_page);
	root_widget_address->user_data = sset_addr_ui_page_create;

	lv_obj_t *root_widget_password = sset_root_ui_widget_duplicate(bg, &lovelyghost_6464, "系统密码设置");
	lv_obj_set_pos(root_widget_password,bottom_x_coo,bottom_y_coo);
	lv_obj_set_event_cb(root_widget_password, sset_root_eventcb_general_enter_page);
	root_widget_password->user_data = sset_sys_pwd_ui_page_create;
	
	lv_obj_t *root_widget_upgrade = sset_root_ui_widget_duplicate(bg, &lovelyghost_6464, "系统升级");
	lv_obj_align(root_widget_upgrade,root_widget_password, LV_ALIGN_OUT_RIGHT_TOP,left_right_space_down,0);
}

/****************************/

static void scr_sysytem_setting_create(void)
{
	_system_setting_data_memory *p_memory = (_system_setting_data_memory *)get_local_pointer(p_data_memory);

	if (p_memory->this_scr == NULL) {
		lv_obj_t *obj_scr = lv_obj_create(NULL,NULL);
		p_memory->this_scr = obj_scr;
	}
	lv_scr_load(p_memory->this_scr);
	ui_printf("scr system settting create\r\n");

	//get_a_default_bg();
	//system_setting_pwd_page_create();
	sset_initN_ui_create();
	
}


static void SysSet_return_btn_event_handle(lv_obj_t *obj,lv_event_t e)
{
  if(e == LV_EVENT_CLICKED){
  	SysSet_change_scr(scr_chtype_return,scr_Null);
  	}
}

/*===========================================
***   Exterior  Interface with screen system
=============================================*/
static void SysSet_change_scr(scr_change_type type,_ui_scr_id scr_id)
{
  gui_sys_screen_event_send(type,scr_id);
}

static void scr_system_setting_quit(void)
{
	dynamic_data_free();
	lv_obj_clean(SysSet_local.memory->this_scr);
}

bool scr_sysytem_setting_register()
{
#if 0
#ifdef _VS_SIMULATOR
	scr_system_setting_init();
	scr_sysytem_setting_create();

	return true;
#endif
#endif

    bool is_true = gui_sys_screen_register(scr_sysytem_setting_create,scr_system_setting_quit,NULL,NULL,NULL,scr_system_setting);
    if(is_true) return true;
	else return false;
}

