#include "scr_security.h"

#include "gui_app.h"

#ifdef _VS_SIMULATOR
#pragma execution_character_set("utf-8")
#endif

/*
***Local UI Global Data
*/

typedef struct {
	uint8_t temp;
}_security_data_dynamic;

typedef struct {
	lv_obj_t *this_scr;
}_security_data_memory;
_security_data_memory security_data_memory;

typedef struct {
	_security_data_dynamic *dynamic;
	_security_data_memory *memory;
}_security_data;
_security_data security_local;

/*IMG & Font Declare
*/
LV_IMG_DECLARE(icon_fire_4848);
LV_IMG_DECLARE(icon_door_mag_5848);
LV_IMG_DECLARE(icon_emergency_btn_4848);
LV_IMG_DECLARE(icon_infrared_4848);
LV_IMG_DECLARE(icon_smoke_4848);
LV_IMG_DECLARE(icon_gas_53_48);
LV_IMG_DECLARE(icon_window_mag_48_48);

#define DEFENCE_ICON_FIRE			icon_fire_4848
#define DEFENCE_ICON_DOOR_MAG		icon_door_mag_5848
#define DEFENCE_ICON_EMERGENCY_BTN	icon_emergency_btn_4848
#define DEFENCE_ICON_INFRARED		icon_infrared_4848
#define DEFENCE_ICON_SMOKE			icon_smoke_4848
#define DEFENCE_ICON_GAS			icon_gas_53_48
#define DEFENCE_ICON_WINDOW_MAG		icon_window_mag_48_48



/*===========================
***********DATA LAYER********
============================*/
static bool dynamic_is_null(_security_data_dynamic *p)
{
	if (p == NULL) return true;
	else return false;
}

static uint32_t* get_dynamic_pointer_32bits(_security_data *local_data)
{
	bool null_true = dynamic_is_null(local_data->dynamic);
	if (null_true) {
		local_data->dynamic = (_security_data_dynamic *)general_malloc(sizeof(_security_data_dynamic));
	}
	return (uint32_t *)local_data->dynamic;
}

static uint32_t *get_local_pointer(_data_pointer_type p_type)
{
	if (p_type == p_data_dynamic) {
		return get_dynamic_pointer_32bits(&security_local);
	}
	else if (p_type == p_data_memory) {
		return (uint32_t *)security_local.memory;
	}
	return NULL;
}

static void scr_security_data_free()
{
	if (is_null_ptr(security_local.dynamic)) return;

	general_free(security_local.dynamic);
	security_local.dynamic = NULL;
}

void  scr_security_data_init()
{
	memset(&security_local, 0, sizeof(_security_data));

	memset(&security_data_memory, 0, sizeof(_security_data_memory));
	
	security_local.dynamic = NULL;
	security_local.memory = &security_data_memory;
}

/*===============================
***** Interior Interface API*****
================================*/
/*Format:
*** Prefix:security_interior_
*/
static void security_interior_change_scr(scr_change_type type, _ui_scr_id scr_id)
{
	gui_sys_screen_event_send(type, scr_id);
}


/*====================
******UI layer********
====================*/
static lv_obj_t *scr_security_ui_head_title(lv_obj_t *parent, char *head_str, lv_event_cb_t event)
{
	return universal_misc_rm_head_title(parent, head_str, event);
}

#if 0
LV_EVENT_CB_DECLARE(security_eventcb_widget_cont)
{
	if (e == LV_EVENT_CLICKED)
	{
		ui_printf("cont state:%d\n",lv_obj_get_state(obj,LV_CONT_PART_MAIN));
		lv_obj_set_state(obj, LV_STATE_DEFAULT);
	}
}
static lv_obj_t *security_ui_defence_widget_duplicate(lv_obj_t *parent,char *txt,void *img_src)
{
	static lv_style_t style_cont;
	lv_style_init(&style_cont);
	lv_style_set_bg_color(&style_cont, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_bg_opa(&style_cont, LV_STATE_DEFAULT, LV_OPA_50);
	lv_style_set_radius(&style_cont, LV_STATE_DEFAULT, 10);

	lv_style_set_border_color(&style_cont, LV_STATE_FOCUSED, LV_COLOR_RED);
	
	//lv_style_set_border_color(&style_cont, LV_STATE_CHECKED, LV_COLOR_GREEN);
	lv_style_set_shadow_width(&style_cont, LV_STATE_CHECKED, 50);
	lv_style_set_shadow_color(&style_cont, LV_STATE_CHECKED, LV_COLOR_CYAN);

	_universal_obj_element elm_cont;
	Uni_Element_init(elm_cont);
	elm_cont.parent = parent;
	elm_cont.height = 110;
	elm_cont.width = 90;

	lv_obj_t *cont = universal_cont_create(&elm_cont, LV_LAYOUT_COLUMN_MID, LV_FIT_NONE, tencell_theme_NULL);
	lv_obj_add_style(cont, LV_CONT_PART_MAIN, &style_cont);
	lv_obj_set_click(cont,true);
	lv_obj_set_event_cb(cont, security_eventcb_widget_cont);
	lv_obj_set_state(cont, LV_STATE_CHECKED);
	
	lv_obj_t *img = lv_img_create(cont, NULL);
	lv_img_set_src(img, img_src);

	lv_obj_t *label = universal_label_chinese_create(cont, txt, universal_font_Common);

	return cont;
}

LV_EVENT_CB_DECLARE(security_eventcb_btn_defence_mode)
{
	if (e == LV_EVENT_CLICKED)
	{
		ui_printf("btn state:%d\n",lv_btn_get_state(obj));
	}
}

static lv_obj_t *security_ui_defence_mode_btn_duplicate(lv_obj_t *parent,char *txt)
{

	static lv_style_t style_btn;
	lv_style_init(&style_btn);
	lv_style_set_bg_color(&style_btn, LV_STATE_DEFAULT, LV_COLOR_GRAY);
	//lv_style_set_text_font(&style_btn, LV_STATE_DEFAULT, &SC_MicroSoft_YaHei_R12);

	_universal_obj_element elm_btn;
	elm_btn.parent = parent;
	elm_btn.width = lv_obj_get_width(parent);
	elm_btn.height = 60;

	lv_obj_t *btn = universal_btn_label_chineses_create(&elm_btn, txt);
	lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style_btn);
	lv_btn_set_checkable(btn, true);
	lv_obj_set_event_cb(btn, security_eventcb_btn_defence_mode);

	return btn;
	
}
static lv_obj_t *scr_security_ui_defence_mode()
{
	lv_obj_t *parent = lv_scr_act();
	
	static lv_style_t style_obj_bg;
	lv_style_init(&style_obj_bg);
	lv_style_set_bg_color(&style_obj_bg, LV_STATE_DEFAULT, LV_COLOR_GRAY);
	
	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = parent;
	elm_obj.width = 150;
	elm_obj.height = LV_VER_RES - 60;
	
	lv_obj_t *bg_obj = universal_obj_create(&elm_obj, tencell_theme_NULL);
	lv_obj_add_style(bg_obj, LV_OBJ_PART_MAIN, &style_obj_bg);

	lv_obj_t *btn_un_defence = security_ui_defence_mode_btn_duplicate(bg_obj, "撤防");
	
	lv_obj_t *btn_home_defence = security_ui_defence_mode_btn_duplicate(bg_obj,"在家布防");
	lv_obj_align(btn_home_defence, btn_un_defence, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
	
	lv_obj_t *btn_leave_home_defence = security_ui_defence_mode_btn_duplicate(bg_obj, "离家布防");
	lv_obj_align(btn_leave_home_defence, btn_home_defence, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	lv_obj_t *btn_report = security_ui_defence_mode_btn_duplicate(bg_obj,"报警记录");
	lv_obj_align(btn_report, btn_leave_home_defence, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

	return bg_obj;
	
}

static lv_obj_t *scr_security_ui_return(lv_obj_t *parent, lv_event_cb_t event_cb)
{
	_universal_obj_element elm_btn;
	elm_btn.parent = parent;
	elm_btn.width = 120;
	elm_btn.height = 60;

	lv_obj_t *btn = universal_btn_label_chineses_create(&elm_btn, "返回");
	lv_theme_apply(btn, (lv_theme_style_t)sigma_star_theme_Btn_home_return);
	lv_obj_set_event_cb(btn, event_cb);

	lv_obj_align(btn, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

	return btn;
}

LV_EVENT_CB_DECLARE(security_set_return)
{
	if (e == LV_EVENT_CLICKED)
	{
		lv_obj_del(lv_obj_get_parent(obj));
	}
}



static void scr_security_setting_ui_create(void)
{	
	lv_obj_t *bg = universal_misc_rm_bg_default_create();

	scr_security_ui_head_title(bg, "安防设置", security_set_return);
//return
	//scr_security_ui_return(bg, security_set_return);
//defence mode
	lv_obj_t *defence_bg = scr_security_ui_defence_mode();
	lv_obj_set_pos(defence_bg, 0, 60);
//widget
	uint16_t widget_base_x = 200, widget_base_base_y = 100;
	uint16_t widget_left_right_sapce = 30;
	uint16_t widget_up_down_space = 30;

	lv_obj_t *cont_smoke = security_ui_defence_widget_duplicate(bg, "烟雾",&DEFENCE_ICON_SMOKE);
	lv_obj_set_pos(cont_smoke, 200, 100);

	lv_obj_t *cont_infrared = security_ui_defence_widget_duplicate(bg, "红外", &DEFENCE_ICON_INFRARED);
	lv_obj_align(cont_infrared, cont_smoke, LV_ALIGN_OUT_RIGHT_TOP, widget_left_right_sapce, 0);

	lv_obj_t *cont_door_mag = security_ui_defence_widget_duplicate(bg, "门磁", &DEFENCE_ICON_DOOR_MAG);
	lv_obj_align(cont_door_mag, cont_infrared, LV_ALIGN_OUT_RIGHT_TOP, widget_left_right_sapce, 0);

	lv_obj_t *cont_thermal_sensor = security_ui_defence_widget_duplicate(bg, "热感", &DEFENCE_ICON_FIRE);
	lv_obj_align(cont_thermal_sensor, cont_door_mag, LV_ALIGN_OUT_RIGHT_TOP, widget_left_right_sapce, 0);

	lv_obj_t *cont_emergency = security_ui_defence_widget_duplicate(bg, "紧急按钮", &DEFENCE_ICON_EMERGENCY_BTN);
	lv_obj_align(cont_emergency, cont_thermal_sensor, LV_ALIGN_OUT_RIGHT_TOP, widget_left_right_sapce, 0);

	lv_obj_t *cont_gas = security_ui_defence_widget_duplicate(bg, "燃气", &DEFENCE_ICON_GAS);
	lv_obj_align(cont_gas, cont_smoke, LV_ALIGN_OUT_BOTTOM_LEFT, 0, widget_up_down_space);

	lv_obj_t *cont_window_mag = security_ui_defence_widget_duplicate(bg, "窗磁", &DEFENCE_ICON_WINDOW_MAG);
	lv_obj_align(cont_window_mag, cont_gas, LV_ALIGN_OUT_RIGHT_TOP, widget_left_right_sapce, 0);
}

LV_EVENT_CB_DECLARE(security_eventcb_set_btn)
{
	if (e == LV_EVENT_CLICKED)
	{
		scr_security_setting_ui_create();
	}
}

LV_EVENT_CB_DECLARE(security_eventcb_return_home)
{
	if (e == LV_EVENT_CLICKED)
	{
		security_interior_change_scr(scr_chtype_return, scr_Null);
	}
}

static lv_obj_t *security_ui_btn_create(lv_obj_t *parent,uint16_t width, uint16_t height, char *str_txt)
{
	static lv_style_t style_secur_btn;
	lv_style_init(&style_secur_btn);
	lv_style_set_bg_opa(&style_secur_btn, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_style_set_border_color(&style_secur_btn, LV_STATE_DEFAULT, LV_COLOR_BLACK);

	_universal_obj_element elm_btn;
	elm_btn.parent = parent;
	elm_btn.width = width;
	elm_btn.height = height;
	
	lv_obj_t * obj_btn = universal_btn_label_chineses_create(&elm_btn, str_txt);
	lv_obj_add_style(obj_btn, LV_BTN_PART_MAIN, &style_secur_btn);

	return obj_btn;
}


static lv_obj_t *security_ui_pwd_jump(lv_obj_t *parent)
{
	//init
	lv_coord_t bg_width = 482, bg_height = 256;
	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = parent;
	elm_obj.width = 482;
	elm_obj.height = 256;

	lv_obj_t *obj_bg = universal_obj_create(&elm_obj, tencell_theme_temp_obj);
	screen_adaption_lvgl_obj_align(obj_bg, NULL, LV_ALIGN_CENTER, 0, 0);

	//ta
	Uni_Element_init(elm_obj);
	elm_obj.parent = obj_bg;
	elm_obj.width = 304;
	elm_obj.height = 46;
	
	lv_obj_t *ta = universal_ta_create(&elm_obj, "", "请输入密码", false, true, true, tencell_theme_pwd_ta);
	return obj_bg;

}
#endif

LV_EVENT_CB_DECLARE(security_eventcb_home_return)
{
	if (e == LV_EVENT_CLICKED)
	{
		security_interior_change_scr(scr_chtype_return, scr_Null);
	}
}

static void scr_security_ui_create(void)
{
	int i = 0;
	lv_coord_t btn_base_x = 39, btn_base_y = 205;
	lv_coord_t btn_hor_space = 76;

	_security_data_memory *p_memory = (_security_data_memory *)get_local_pointer(p_data_memory);

	if (p_memory->this_scr == NULL) {
		lv_obj_t *obj_scr = lv_obj_create(NULL, NULL);
		p_memory->this_scr = obj_scr;
	}
	lv_scr_load(p_memory->this_scr);

	lv_obj_t *bg = universal_misc_rm_bg_default_create();

	scr_security_ui_head_title(bg, "安防", security_eventcb_home_return);
	//
	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = bg;
	elm_obj.width = 123;
	elm_obj.height = 97;

	lv_obj_t *obj_btn;
	const char *btn_text[4] = { "撤防", "在家布防", "离家布防", "安防设置" };
	for (i = 0; i < 4; ++i)
	{
		obj_btn = universal_btn_label_create(&elm_obj, btn_text[i], tencell_theme_btn_chinese_normal);
		screen_adaption_lvgl_obj_set_pos(obj_btn, btn_base_x + i * (btn_hor_space + elm_obj.width),  btn_base_y);
	}
}


/*=======================================
*** Exterior Interface with screen system
========================================*/
/*Format:
***@ prefix: scr_security_interface_
*/
static void scr_security_quit(void)
{
	scr_security_data_free();
	lv_obj_clean(security_local.memory->this_scr);
}

bool scr_security_interface_register()
{
	bool is_true = gui_sys_screen_register(scr_security_ui_create, 
											scr_security_quit, 
											NULL, 
											NULL, 
											NULL, 
											scr_security);
	if (is_true) return true;
	else return false;
}