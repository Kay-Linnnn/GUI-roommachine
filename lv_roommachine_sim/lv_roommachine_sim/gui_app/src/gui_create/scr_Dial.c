#include "scr_Dial.h"
#include "../../gui_app.h"

#include "data_tencell.h"
#include "scr_system_setting.h"


typedef struct {
	lv_obj_t *dial_ta;
	lv_obj_t *default_kb_cont;
	lv_obj_t *cap_kb_cont;
	struct calling_data *task_data;
}_Dial_data_dynamic;

typedef struct {
	lv_obj_t *this_scr;
}_Dial_data_memory;
_Dial_data_memory Dial_data_memory;

typedef struct {
	_Dial_data_dynamic *dynamic;
	_Dial_data_memory  *memory;
}_scr_dial_data;
_scr_dial_data Dial_data;

typedef struct calling_data{
	uint8_t time_out_peroid;
	lv_obj_t *obj_bg;
}_calling_task;

enum {
	dial_type_invalid,
	dial_type_call,
	dial_type_unit_door,
	dial_type_block_door,
	dial_type_adm_center,
};
typedef uint8_t dial_type_t;

#ifdef _VS_SIMULATOR
#pragma execution_character_set("utf-8")
#endif

LV_FONT_DECLARE(SC_MicroSoft_YaHei_R16);

#if 0
#define Dial_Keyboard_Ctrl_Btn_Flags  (LV_BTNMATRIX_CTRL_NO_REPEAT | LV_BTNMATRIX_CTRL_CLICK_TRIG)

static const char * const default_btnm_map[] = {"1","2","3","小区门口","\n",
                                                 "4","5","6","返回","\n",
                                                 "7","8","9","呼叫","\n",
                                                 "*","0","#","ABC",""};

static const lv_btnmatrix_ctrl_t default_btnm_ctrl_map[] = { 1,1,1,Dial_Keyboard_Ctrl_Btn_Flags | 1,
                                                             1,1,1,Dial_Keyboard_Ctrl_Btn_Flags | 1,
                                                             1,1,1,Dial_Keyboard_Ctrl_Btn_Flags | 1,
                                                             1,1,1,Dial_Keyboard_Ctrl_Btn_Flags | 1
};

static const char * const kb_r_btnm_map[] = {"单元\n门口","\n",
                                             "管理\n中心",""};


static const char *const cap_btnm_map[] = {"A","B","C","D","E","F","G","\n",
                                            "H","I","J","K","L","M","N","\n",
                                            "O","P","Q","R","S","T","首页","\n",
                                            "U","V","W","X","Y","Z","123",""};

static const lv_btnmatrix_ctrl_t  cap_btnm_ctrl_map[] = {1,1,1,1,1,1,1,
                                                         1,1,1,1,1,1,1,
                                                         1,1,1,1,1,1,Dial_Keyboard_Ctrl_Btn_Flags | 1,
                                                         1,1,1,1,1,1,Dial_Keyboard_Ctrl_Btn_Flags | 1
};
#endif
static void scr_dial_data_dynamic_free(void);

static _calling_task* dial_calling_task_create(lv_obj_t *obj_bg);
static void calling_task_tick(uint8_t *time_tick,bool e_zero);


static void dial_change_scr(scr_change_type type,_ui_scr_id scr_id);


/*=======================
********DATA LAYER*******
=========================*/
void scr_dial_init()
{
	memset(&Dial_data_memory,0,sizeof(_Dial_data_memory));
	Dial_data_memory.this_scr = NULL;

	Dial_data.dynamic = NULL;
	Dial_data.memory = &Dial_data_memory;
}

void scr_dial_quit(void)
{
	scr_dial_data_dynamic_free();
}

static void scr_dial_data_dynamic_free()
{
	general_free(Dial_data.dynamic);
	Dial_data.dynamic = NULL;
}

static void scr_dial_data_dynamic_init()  //malloc memory for  dynamic data
{
	if (Dial_data.dynamic != NULL) {
		scr_dial_data_dynamic_free();
	}
	Dial_data.dynamic = (_Dial_data_dynamic *)general_malloc(sizeof(_Dial_data_dynamic));
}

/*============================
******Communication Layer *****
==============================*/
static void dial_communication_out(Sys_Commu_type comm_type,SysCommu_message_type message,void *data)
{
	Sys_Commu_Direction comdir = SysCommuDir_scr2ex;
	//gui_sys_commu_event_send(SysCommuDir_scr2ex,comm_type,message,data);
	if(comm_type == SysCommu_type_sipmessage)
		comdir = SysCommuDir_scr2ex;
	else if(comm_type == SysCommu_type_callmessage)
		comdir = SysCommuDir_toCall;

	ui_printf("[dial_communication_out]message:%d\n",message);
	gui_sys_commu_event_send(comdir,comm_type,message,data);
}

static void dial_communication_in(int message,void *in_data)
{
	ui_printf("[dial_communication_in]\n");
	switch(message)
	{
		case message_sip_ex_recv:
			calling_task_tick(&Dial_data.dynamic->task_data->time_out_peroid,true);
			break;

		default:
		break;
	}
}

static void dial_send_sip_message(SysCommu_message_type message, char *ip_addr)
{
	dial_communication_out(SysCommu_type_sipmessage, message, ip_addr);
}

static void dial_send_call_message(SysCommu_message_type message)
{
	dial_communication_out(SysCommu_type_callmessage,message,NULL);
}


/*======================
  **** TASK LAYER *******
========================*/
static void calling_task_tick(uint8_t *time_tick,bool e_zero)
{
	if (time_tick == NULL || *time_tick < 2) return;

	if (!e_zero)
		(*time_tick)--;
	else
		*time_tick = 1;
}

void calling_task_callback(lv_task_t *task)
{
	if (task->user_data == NULL) return;

	_calling_task *calling_data = (_calling_task *)task->user_data;
	if (calling_data->obj_bg == NULL) return;

	if (calling_data->time_out_peroid > 1) {
		//calling_data->time_out_peroid--;
		calling_task_tick(&calling_data->time_out_peroid,false);
		return;
	}
	else {
		lv_obj_del(calling_data->obj_bg);
		lv_task_del(task);
		general_free(calling_data);
	}

}

static _calling_task* dial_calling_task_create(lv_obj_t *obj_bg)
{
	_calling_task *calling_task = (_calling_task *)general_malloc(sizeof(_calling_task));
	calling_task->obj_bg = obj_bg;
	calling_task->time_out_peroid = 20*10;

	lv_task_t *task_t = lv_task_create(calling_task_callback,200,LV_TASK_PRIO_LOW,calling_task);
	return calling_task;
}

/*======================================
 *******  UI LAYER **********
=======================================*/
/*
*****Descr: turn dail num to address, invalid num_length: 4,5,8,10
***@Para->dial_num: dial numbe called
***@Para-> des_addr: address pointer to hold address information decode from dial num, make sure this data is get from local machine firstly
***Return: 0 if success or -1 otherwise
*/
static int dial_call_address_number_decode(const char *dial_num, _address_set_type *des_addr)
{
	if (is_null_ptr(dial_num) || is_null_ptr(des_addr))
		return -1;

	int num_size = (int)strlen(dial_num);

	if (num_size < 4 || num_size > 10)
		return -1;

	memcpy(des_addr->room, dial_num + sizeof(char)*(num_size - 2), sizeof(char) * 2);//room
	memcpy(des_addr->floor, dial_num + sizeof(char)*(num_size - 4), sizeof(char) * 2);//floor
	//uncomment "return 0" below if dial can only call the same unit clients
	return 0;
	if (num_size - 4 != 0)
	{
		memcpy(des_addr->unit, dial_num + sizeof(char)*(num_size - 5), sizeof(char) * 1);//unit
		if (num_size - 5 != 0 && num_size >= 8)
		{
			memcpy(des_addr->building, dial_num + sizeof(char)*(num_size - 8), sizeof(char) * 3);//building
			if (num_size == 10)
			{
				memcpy(des_addr->region, dial_num, sizeof(char) * 2);//region
				return 0;
			}
			else // 9 or more than 10 nums
			{
				return -1;
			}
			
		}
		else // 6/7 nums, invalid
		{
			return -1;
		}
		//return 0;//5 nums
	}
	return 0;
}

LV_EVENT_CB_DECLARE(dial_call_cancel_btn_event_handle)
{
	if (e == LV_EVENT_CLICKED) {
		calling_task_tick(&Dial_data.dynamic->task_data->time_out_peroid,true);
	}
}

static void dial_calling_create(char *txt_account)
{
	LV_IMG_DECLARE(dial_hand_green_200_200);

	lv_obj_t *calling_bg = lv_obj_create(lv_scr_act(),NULL);
	lv_obj_set_style_local_bg_opa(calling_bg,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_OPA_COVER);
	lv_obj_set_style_local_bg_color(calling_bg,LV_OBJ_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_GRAY);
	lv_obj_set_size(calling_bg,LV_HOR_RES,LV_VER_RES);
	lv_obj_set_pos(calling_bg,0,0);

	lv_obj_t *call_img = lv_img_create(calling_bg,NULL);
	lv_img_set_src(call_img, &dial_hand_green_200_200);//LV_SYMBOL_CALL
	lv_obj_align(call_img, NULL, LV_ALIGN_IN_LEFT_MID, 50, 0);

	lv_obj_t *obj_cont = lv_cont_create(calling_bg,NULL);
	uint16_t c_height = 200;
	uint16_t c_y = LV_VER_RES / 2 - c_height / 2;
	lv_obj_set_pos(obj_cont,640, c_y);
	lv_obj_set_size(obj_cont,160, c_height);
	lv_cont_set_fit(obj_cont,LV_FIT_TIGHT);
	lv_cont_set_layout(obj_cont,LV_LAYOUT_COLUMN_MID);
	lv_theme_apply(obj_cont,(lv_theme_style_t)sigma_star_theme_cont_bg_transparent);

	static lv_style_t btn_style;
	lv_style_init(&btn_style);
	lv_style_set_bg_color(&btn_style,LV_STATE_DEFAULT, LV_COLOR_RED);
	//lv_style_set_bg_grad_color(&btn_style,LV_STATE_DEFAULT,LV_COLOR_YELLOW);
	lv_style_set_bg_opa(&btn_style,LV_STATE_DEFAULT,LV_OPA_90);
	lv_style_set_text_font(&btn_style,LV_STATE_DEFAULT,&SC_MicroSoft_YaHei_R16);

	lv_obj_t *btn = lv_btn_create(obj_cont,NULL);
	lv_obj_set_size(btn,120,80);
	lv_obj_set_event_cb(btn,dial_call_cancel_btn_event_handle);
	lv_obj_add_style(btn,LV_BTN_PART_MAIN,&btn_style);

	lv_obj_t *label = lv_label_create(btn,NULL);	
	lv_theme_apply(label, (lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_label_set_text(label,"挂断");
	
	Dial_data.dynamic->task_data = dial_calling_task_create(calling_bg);
}

static int  dial_ui_invalid_number_del(void *obj)
{
	lv_obj_del((lv_obj_t*)obj);

	return 0;
}

static void dial_ui_invalid_number_warning(char *warn_tag)
{
	static lv_style_t style_obj_bg;
	lv_style_init(&style_obj_bg);
	lv_style_set_bg_opa(&style_obj_bg, LV_STATE_DEFAULT, LV_OPA_60);
	lv_style_set_bg_color(&style_obj_bg, LV_STATE_DEFAULT, LV_COLOR_GRAY);

	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = lv_scr_act();
	elm_obj.width = SCREEN_BASE_RESOLUTION_HOR / 4;
	elm_obj.height = elm_obj.width / 2;
	
	lv_obj_t *obj_bg = universal_obj_create(&elm_obj, tencell_theme_NULL);
	lv_obj_add_style(obj_bg, LV_OBJ_PART_MAIN, &style_obj_bg);
	screen_adaption_lvgl_obj_align(obj_bg, NULL, LV_ALIGN_CENTER, 0, -20);

	lv_obj_t *label = lv_label_create(obj_bg, NULL);
	lv_label_set_text(label,warn_tag);
	lv_theme_apply(label,(lv_theme_style_t)sigma_star_theme_label_chinese);
	screen_adaption_lvgl_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);

	universal_task_100ms_timer_create(5, (uint32_t *)dial_ui_invalid_number_del, obj_bg);
}

static void dial_function_call_signal_schedule(dial_type_t signal)
{
	switch (signal)
	{
		case dial_type_call:
		{
			const char *call_txt = lv_textarea_get_text(Dial_data.dynamic->dial_ta);
			//dial_calling_create(call_txt);
			ui_printf_plus("call number:%s", call_txt);

			_address_set_type call_addr;
			if (SysSet_get_local_machine_address(&call_addr) != 0) //get local address
				return;

			if (dial_call_address_number_decode(call_txt, &call_addr) != 0) //invalid number
			{
				//to do, invalid warning
				dial_ui_invalid_number_warning("错误号码");
				return;
			}
			//char ip_addr[15] = { 0 };
			char *ip_addr = (char *)general_malloc(sizeof(char) * 20);//remembe to free this in system control when used

			if (datajs_get_address_ip(&call_addr, ip_addr) != 0)
			{
				//to do, get ip failed
				general_free(ip_addr);
				dial_ui_invalid_number_warning("无号码");
				return;
			}
			ui_printf_plus("dial, get ip;%s", ip_addr);

			dial_send_call_message(message_call_dial);//enter screen call
			dial_send_sip_message(message_sip_local_call, ip_addr);//send call request
		}
			break;

		case dial_type_unit_door:
			break;

		case dial_type_block_door:
			break;

		case dial_type_adm_center:
			dial_calling_create("管理中心");
			break;

		default:
			break;
	}
}

LV_EVENT_CB_DECLARE(dial_eventcb_function_call)
{
	int i = 0;
	const char *label_txt = NULL;
	const char *txt[] = {" ", "呼叫", "单元门口机", "小区门口", "管理中心机" };
	dial_type_t signal = 0;
	if (e == LV_EVENT_CLICKED)
	{
		label_txt = lv_label_get_text(lv_obj_get_child(obj, NULL));
		for (i = 1; i < 5; i++)
		{
			if (strcmp(label_txt, txt[i]) == 0)
			{
				signal = (dial_type_t)i;
				break;
			}				
		}
		dial_function_call_signal_schedule(signal);
	}
}

LV_EVENT_CB_DECLARE(dial_keyboard_event_handle)
{
	if (e != LV_EVENT_VALUE_CHANGED)
		return;

	lv_obj_t *ta = (lv_obj_t *)obj->user_data;

	const char *c = lv_btnmatrix_get_active_btn_text(obj);
	
	//lv_textarea_add_char(ta, c);
	lv_textarea_add_text(ta, c);
}

LV_EVENT_CB_DECLARE(dial_del_btn_event_handle)
{
	if (e == LV_EVENT_CLICKED) {
		lv_textarea_del_char(Dial_data.dynamic->dial_ta);
	}
}

static lv_obj_t *dial_keyboard_create(lv_obj_t *parent, lv_event_cb_t kb_event, void *kb_data)
{
	lv_coord_t top_space = 18;
	lv_coord_t top_left_space = 5;
	lv_obj_t *bg = parent;

	lv_coord_t bg_width = 609, bg_height = 311;
	
	lv_obj_t *kb_visual_bg = universal_keyboard_design(parent, bg_width, bg_height, top_space, top_left_space, kb_event, kb_data, Keyboard_design_align_center, 0);
	
	return kb_visual_bg;
}

extern const char *bin_path_img_del_icon;
static void dial_content_create()
{
	lv_obj_t *bg = lv_scr_act();

	_universal_obj_element elm_ta;
	Uni_Element_init(elm_ta);
	elm_ta.parent = bg;
	elm_ta.width = 610;
	elm_ta.height = 46;

	lv_obj_t *ta = universal_ta_pwd_create(&elm_ta, false, "请输入", sigma_star_theme_dial_ta);
	lv_textarea_set_pwd_mode(ta, false);
	screen_adaption_lvgl_obj_set_pos(ta, 25, 97);
	Dial_data.dynamic->dial_ta = ta;

	_universal_obj_element elm_btn;
	Uni_Element_init(elm_btn);
	elm_btn.parent = bg;
	elm_btn.width = 133;
	elm_btn.height = 47;
	elm_btn.event_cb = dial_del_btn_event_handle;

	uint16_t left_right_space = 8;
	uint16_t up_down_space = 10;
	const char *label_text[] = { " ", "呼叫", "单元门口机", "小区门口", "管理中心机" };
	lv_obj_t *del_btn = universal_btn_label_create(&elm_btn, NULL, tencell_theme_btn_chinese_normal);
	screen_adaption_lvgl_obj_align(del_btn, ta, LV_ALIGN_OUT_RIGHT_TOP, left_right_space, 0);
	//lv_theme_apply(del_btn, (lv_theme_style_t)tencell_theme_btn_chinese_normal);

	lv_obj_t *img = lv_img_create(del_btn, NULL);
	lv_img_set_src(img, bin_path_img_del_icon);

	elm_btn.height = 67;
	elm_btn.event_cb = dial_eventcb_function_call;
	lv_obj_t *call_btn = universal_btn_label_create(&elm_btn, label_text[1], tencell_theme_btn_chinese_normal);
	screen_adaption_lvgl_obj_align(call_btn, del_btn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, up_down_space);

	up_down_space += 5;
	lv_obj_t *door_btn = universal_btn_label_create(&elm_btn, label_text[2], tencell_theme_btn_chinese_normal);
	screen_adaption_lvgl_obj_align(door_btn, call_btn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, up_down_space);

	lv_obj_t *block_btn = universal_btn_label_create(&elm_btn, label_text[3], tencell_theme_btn_chinese_normal);
	screen_adaption_lvgl_obj_align(block_btn, door_btn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, up_down_space);

	lv_obj_t *center_btn = universal_btn_label_create(&elm_btn, label_text[4], tencell_theme_btn_chinese_normal);
	screen_adaption_lvgl_obj_align(center_btn, block_btn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, up_down_space);

	lv_obj_t *kb_block = dial_keyboard_create(bg, dial_keyboard_event_handle, ta);
	screen_adaption_lvgl_obj_align(kb_block, ta, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
}

LV_EVENT_CB_DECLARE(dial_eventcb_return)
{
	if (e == LV_EVENT_CLICKED)
	{
		dial_change_scr(scr_chtype_return, 0);
	}
}

static void dial_head_title_create(char *head_str)
{
	lv_obj_t *head_cont = lv_cont_create(lv_scr_act(), NULL);
	lv_obj_set_pos(head_cont, 0, 0);
	lv_cont_set_fit(head_cont, LV_FIT_NONE);
	lv_obj_set_size(head_cont, LV_HOR_RES, 40);
	lv_cont_set_layout(head_cont, LV_LAYOUT_CENTER);
	//lv_obj_add_style(head_cont, LV_CONT_PART_MAIN, &title_style);
	lv_theme_apply(head_cont, (lv_theme_style_t)sigma_star_theme_title);

	lv_obj_t *obj_label = lv_label_create(head_cont, NULL);
	lv_label_set_text(obj_label, head_str);//"户户通"
}

static void scr_dial_create(void)
{
	if (Dial_data.memory->this_scr == NULL) {
		lv_obj_t *obj_scr = lv_obj_create(NULL,NULL);
		Dial_data.memory->this_scr = obj_scr;
	}
	lv_scr_load(Dial_data.memory->this_scr);
	scr_dial_data_dynamic_init();

	lv_obj_t *bg_top = universal_misc_rm_pic_bg_create();

	lv_obj_t *return_btn = universal_misc_rm_head_title(bg_top, "拨号", dial_eventcb_return);

	dial_content_create();
}


/*===========================================
***   Exterior  Interface with screen system
=============================================*/
static void dial_change_scr(scr_change_type type,_ui_scr_id scr_id)
{
  gui_sys_screen_event_send(type,scr_id);
}

bool scr_dial_register()
{
    bool is_true = gui_sys_screen_register(scr_dial_create,
											scr_dial_quit,
											NULL,NULL,
											dial_communication_in,
											scr_Dial);
    if(is_true) return true;
	else return false;
}

