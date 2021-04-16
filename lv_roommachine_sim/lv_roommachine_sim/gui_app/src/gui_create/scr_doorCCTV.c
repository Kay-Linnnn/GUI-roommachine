#include "scr_doorCCTV.h"
#include "../../gui_app.h"

#include "scr_system_setting.h"
#include "data_tencell.h"

#ifndef _VS_SIMULATOR
#include "video_sys/include/video_sys.h"
#endif

#ifdef _VS_SIMULATOR
#pragma execution_character_set("utf-8")
#endif

typedef struct{
  uint32_t *dailing_task_data_addr;
  lv_obj_t **door_widget_array; //array of door widget
}_doorCCTV_data_dynamic;

typedef struct{
  lv_obj_t *this_scr;
}_doorCCTV_data_memory;
_doorCCTV_data_memory doorCCTV_data_memory;

typedef struct{
	_doorCCTV_data_dynamic *dynamic;
	_doorCCTV_data_memory *memory;
}_scr_doorCCTV_data;
_scr_doorCCTV_data doorCCTV_local;

typedef struct {
	lv_obj_t *temp_bg;
	uint8_t time_out_peroid;
}_doorCCTV_task;

enum {
	door_widget_index_1 = 1,
	door_widget_index_2,
	door_widget_index_3,
	door_widget_index_4,
};
typedef uint8_t door_widget_index_t;

#define CCTV_DOOR_WIDGET_COUNT_MAX	4

LV_FONT_DECLARE(SC_MicroSoft_YaHei_R16);

static void doorCCTV_change_scr(scr_change_type type,_ui_scr_id scr_id);

LV_EVENT_CB_DECLARE(unit_door_click_event_handle);


void scr_doorCCTV_init()
{
	memset(&doorCCTV_data_memory,0,sizeof(_doorCCTV_data_memory));

	doorCCTV_local.dynamic = NULL;
	doorCCTV_local.memory = &doorCCTV_data_memory;
}

static void dynamic_data_free()
{
	if(is_null_ptr(doorCCTV_local.dynamic)) return;
	
	general_free(doorCCTV_local.dynamic);
	doorCCTV_local.dynamic = NULL;
}

/******************************
******local data control*******
******************************/
static bool dynamic_is_null(_doorCCTV_data_dynamic *p)
{
	if (p == NULL) return true;
	else return false;
}

static uint32_t* get_dynamic_pointer_32bits(_scr_doorCCTV_data *local_data)
{
	bool null_true = dynamic_is_null(local_data->dynamic);
	if (null_true) {
		local_data->dynamic = (_doorCCTV_data_dynamic *)general_malloc(sizeof(_doorCCTV_data_dynamic));
	}
	return (uint32_t *)local_data->dynamic;
}

static uint32_t *get_local_pointer(_data_pointer_type p_type)
{
	if (p_type == p_data_dynamic) {
		return get_dynamic_pointer_32bits(&doorCCTV_local);
	}
	else if (p_type == p_data_memory) {
		return (uint32_t *)doorCCTV_local.memory;
	}
	return NULL;
}
/*============================
******Communication Layer *****
==============================*/
static void cctv_communication_out(Sys_Commu_type comm_type, SysCommu_message_type message, void *data)
{
	Sys_Commu_Direction comdir = SysCommuDir_scr2ex;
	//gui_sys_commu_event_send(SysCommuDir_scr2ex,comm_type,message,data);
	if (comm_type == SysCommu_type_sipmessage)
		comdir = SysCommuDir_scr2ex;
	else if (comm_type == SysCommu_type_callmessage)
		comdir = SysCommuDir_toCall;

	ui_printf("[dial_communication_out]message:%d\n", message);
	gui_sys_commu_event_send(comdir, comm_type, message, data);
}

static void cctv_interior_call_request(char *ip_addr)
{
	ui_printf_plus("[cctv_interior_call_request] ip_addr:%s", ip_addr);
	cctv_communication_out(SysCommu_type_callmessage, message_call_cctv, NULL); //inner message to change screen

	cctv_communication_out(SysCommu_type_sipmessage, message_sip_local_call, ip_addr); //sip message to request connect
}


/*================================
**************UI Layer************
=================================*/
/*Format:
***prefix: cctv_
***UI:	cctv_ui_
***eventcb:	cctv_eventcb_
*/

static void doorCCTV_head_title_create(char *head_str)
{
	lv_obj_t *head_cont = lv_cont_create(lv_scr_act(), NULL);
	lv_obj_set_pos(head_cont, 0, 0);
	lv_cont_set_fit(head_cont, LV_FIT_NONE);
	lv_obj_set_size(head_cont, LV_HOR_RES, 40);
	lv_cont_set_layout(head_cont, LV_LAYOUT_CENTER);
	//lv_obj_add_style(head_cont, LV_CONT_PART_MAIN, &title_style);
	lv_theme_apply(head_cont, (lv_theme_style_t)sigma_star_theme_title);

	lv_obj_t *obj_label = lv_label_create(head_cont, NULL);
	lv_label_set_text(obj_label, head_str);
}

static lv_obj_t **cctv_door_widget_memory_apply()
{
	return (lv_obj_t **)general_malloc(sizeof(lv_obj_t **) * CCTV_DOOR_WIDGET_COUNT_MAX);
}

static void cctv_door_widegt_memory_free(lv_obj_t **obj_array)
{
	int i = 0;
	for (i = 0; i < CCTV_DOOR_WIDGET_COUNT_MAX; ++i)//CCTV_DOOR_WIDGET_COUNT_MAX
	{		
		if (*obj_array != NULL)
		{
			general_free((*obj_array)->user_data);
		}
		obj_array++;
	}
}

static int cctv_door_widget_data(lv_obj_t *obj, door_widget_index_t index, lv_obj_t **widget_array)
{
	uint8_t *widget_index = (uint8_t *)general_malloc(sizeof(uint8_t));
	if (is_null_ptr(widget_index))
		return -1;

	*widget_index = index;

	obj->user_data = widget_index;

	//widget_array[index - 1] = obj; //index is begin from 1, while array is begin with 0
	*widget_array = obj;
	
	return 0;
}

static int cctv_turn_address_to_num(_address_set_type *machine_addr,uint8_t index, char *addr_num)
{
	sprintf(addr_num, "30%s%s%s%s00%02d", machine_addr->region, machine_addr->building, machine_addr->unit, machine_addr->floor, index);

	return 0;
}

LV_EVENT_CB_DECLARE(unit_door_click_event_handle)
{
	if (e == LV_EVENT_CLICKED) 
	{
		uint8_t door_index = *(uint8_t *)obj->user_data;
		char addr_num[50] = { 0 };

		_address_set_type machine_addr;
		SysSet_get_local_machine_address(&machine_addr);
		cctv_turn_address_to_num(&machine_addr, door_index, addr_num);
		
		char *door_ip = (char *)general_malloc(sizeof(char) * 20);
		if (0 != datajs_get_address_ip_by_addrnum(addr_num, door_ip))//no ip got
		{
			//to do, any warning?
			ui_printf_plus("cctv, get ip fail");
			return;
		}

		cctv_interior_call_request(door_ip);
	}
}

static lv_obj_t* cctv_ui_add_door_widget(lv_obj_t *parent, const char *machine_txt)
{
	_universal_obj_element elm_cont;
	Uni_Element_init(elm_cont);
	elm_cont.parent = parent;
	elm_cont.width = 133;
	elm_cont.height = 68;

	static lv_style_t style_cont;
	lv_style_init(&style_cont);
	lv_style_set_bg_color(&style_cont, LV_STATE_DEFAULT, TENCELL_RM_COLOR_WATER_BLUE);
	lv_style_set_radius(&style_cont, LV_STATE_DEFAULT, 10);
	lv_style_set_text_font(&style_cont, LV_STATE_DEFAULT, &THEME_CHINESE_FONT_NORMAL);
	lv_style_set_text_color(&style_cont, LV_STATE_DEFAULT, LV_COLOR_WHITE);

	lv_obj_t *cont = universal_cont_create(&elm_cont, LV_LAYOUT_CENTER, LV_FIT_NONE, tencell_theme_NULL);
	lv_obj_add_style(cont, LV_CONT_PART_MAIN, &style_cont);
	lv_obj_set_click(cont, true);
	lv_obj_set_event_cb(cont, unit_door_click_event_handle);

	lv_label_set_text(lv_label_create(cont, NULL), machine_txt);

	return cont;
}

static void doorCCTV_return_btn_event_handle(lv_obj_t *obj, lv_event_t e)
{
  if(e == LV_EVENT_CLICKED){
	  _doorCCTV_data_dynamic *p_dynamic = (_doorCCTV_data_dynamic *)get_local_pointer(p_data_dynamic);

	  cctv_door_widegt_memory_free(p_dynamic->door_widget_array);
	  p_dynamic->door_widget_array = NULL;

  	  doorCCTV_change_scr(scr_chtype_return,scr_Null);
  	}
}

static void scr_doorCCTV_create(void)
{
	if (doorCCTV_local.memory->this_scr == NULL) {
		lv_obj_t *obj_scr = lv_obj_create(NULL, NULL);
		doorCCTV_local.memory->this_scr = obj_scr;
	}
	lv_scr_load(doorCCTV_local.memory->this_scr);

	const char *head_title = "监控";
	const char *door_index_txt[] = { " ","门口机1" , "门口机2" , "门口机3" , "门口机4" };

	lv_obj_t *bg_top = universal_misc_rm_pic_bg_create();

	lv_obj_t *return_btn = universal_misc_rm_head_title(bg_top, head_title, doorCCTV_return_btn_event_handle);

	_doorCCTV_data_dynamic *p_dynamic = (_doorCCTV_data_dynamic *)get_local_pointer(p_data_dynamic);	

	lv_obj_t **obj_array = cctv_door_widget_memory_apply();
	if (obj_array == NULL) {
		ui_printf_plus("apply memory for door widget fail");
		return;
	}
	p_dynamic->door_widget_array = obj_array;

	uint8_t inner_space = 64;
	uint16_t base_x = 40, base_y = 220;
	lv_obj_t *door1_cont = cctv_ui_add_door_widget(lv_scr_act(), door_index_txt[door_widget_index_1]);
	screen_adaption_lvgl_obj_set_pos(door1_cont, base_x, base_y);
	cctv_door_widget_data(door1_cont, door_widget_index_1, obj_array);

	obj_array++;
	lv_obj_t *door2_cont = cctv_ui_add_door_widget(lv_scr_act(), door_index_txt[door_widget_index_2]);
	screen_adaption_lvgl_obj_align(door2_cont, door1_cont, LV_ALIGN_OUT_RIGHT_TOP, inner_space, 0);
	cctv_door_widget_data(door2_cont, door_widget_index_2, obj_array);

	obj_array++;
	lv_obj_t *door3_cont = cctv_ui_add_door_widget(lv_scr_act(), door_index_txt[door_widget_index_3]);
	screen_adaption_lvgl_obj_align(door3_cont, door2_cont, LV_ALIGN_OUT_RIGHT_TOP, inner_space, 0);
	cctv_door_widget_data(door3_cont, door_widget_index_3, obj_array);

	obj_array++;
	lv_obj_t *door4_cont = cctv_ui_add_door_widget(lv_scr_act(), door_index_txt[door_widget_index_4]);
	screen_adaption_lvgl_obj_align(door4_cont, door3_cont, LV_ALIGN_OUT_RIGHT_TOP, inner_space, 0);
	cctv_door_widget_data(door4_cont, door_widget_index_4, obj_array);
}

static void scr_doorCCTV_quit(void)
{
	dynamic_data_free();
	ui_printf("[scr_doorCCTV_quit]\n");
}

/*===========================================
***   Exterior  Interface with screen system
=============================================*/
static void doorCCTV_change_scr(scr_change_type type,_ui_scr_id scr_id)
{
  gui_sys_screen_event_send(type,scr_id);
}

bool scr_doorCCTV_register()
{
    bool is_true = gui_sys_screen_register(scr_doorCCTV_create,
											scr_doorCCTV_quit,
											NULL,
											NULL,
											NULL,
											scr_doorCCTV);
    if(is_true) return true;
	else return false;
}



