#include "scr_VoiceSetting.h"
#include "gui_app.h"

#ifdef _VS_SIMULATOR
#pragma execution_character_set("utf-8")
#endif

#define widget_cont_w   560
#define widget_cont_h   80
#define widget_left_offset  (SCREEN_BASE_RESOLUTION_HOR - widget_cont_w)/2
#define widget_head_offset 50

typedef struct{
	uint8_t temp;
}_VoiceSetting_data_dynamic;

typedef struct{
	lv_obj_t *this_scr;
	uint8_t voice_value;
	uint8_t bell_value;
	uint8_t microphone_value;
	uint8_t ring_type;
}_VoiceSetting_data_memory;
_VoiceSetting_data_memory VoiceSetting_data_memory;

typedef struct{
	_VoiceSetting_data_dynamic *dynamic;
	_VoiceSetting_data_memory  *memory;
}_scr_VoiceSetting_data;
_scr_VoiceSetting_data VoiceSetting_local;

typedef struct {
	uint8_t *bar_value;
	uint8_t btn_flag;
	lv_obj_t *obj_bar;
}_Voice_widget;

typedef struct {
	lv_obj_t *btn_left;
	lv_obj_t *btn_right;
	lv_obj_t *btn_cont;
	lv_obj_t *ring_choosen;
}_Ring_Widget;

enum btn_flag_id {
	voice_plus = 1,
	voice_minus,
	bell_plus,
	bell_minus,
	microphone_plus,
	microphone_minus,
};

/*==========================
**********Data Layer********
===========================*/
void scr_VoiceSetting_init()
{
  memset(&VoiceSetting_data_memory,0,sizeof(_VoiceSetting_data_memory));
  VoiceSetting_data_memory.this_scr = NULL;
  VoiceSetting_data_memory.voice_value = 3;
  VoiceSetting_data_memory.bell_value = 3;
  VoiceSetting_data_memory.microphone_value = 3;
  VoiceSetting_data_memory.ring_type = 1;

  VoiceSetting_local.dynamic = NULL;
  VoiceSetting_local.memory = &VoiceSetting_data_memory;
}


static void voice_set_data_free()
{
	if (is_null_ptr(VoiceSetting_local.dynamic)) return;

	general_free(VoiceSetting_local.dynamic);
	VoiceSetting_local.dynamic = NULL;
}

LV_EVENT_CB_DECLARE(btn_event_handle)
{
	if (e == LV_EVENT_CLICKED) {

		_Voice_widget *custom_widget = (_Voice_widget *)obj->user_data;
		if (custom_widget == NULL) {
			return;
		}
		uint8_t btn_flag = custom_widget->btn_flag;
		if(btn_flag <= 1) //always >= 0 because of data-type
		{
			if (btn_flag == 0 && *custom_widget->bar_value<9) {
				(*custom_widget->bar_value)++;
			}
			else if(btn_flag == 1 && *custom_widget->bar_value>1){
				(*custom_widget->bar_value)--;
			}
			lv_bar_set_value(custom_widget->obj_bar, *custom_widget->bar_value,LV_ANIM_ON);
		}

		else if (btn_flag >= 2 && btn_flag <= 3)
		{
			if (btn_flag == 2 && *custom_widget->bar_value < 9) {
				(*custom_widget->bar_value)++;
			}
			else if (btn_flag == 3 && *custom_widget->bar_value > 1) {
				(*custom_widget->bar_value)--;
			}
			lv_bar_set_value(custom_widget->obj_bar, *custom_widget->bar_value, LV_ANIM_ON);
		}

		else if (btn_flag >= 4 && btn_flag <= 5)
		{
			if (btn_flag == 4 && *custom_widget->bar_value < 9) {
				(*custom_widget->bar_value)++;
			}
			else if (btn_flag == 5 && *custom_widget->bar_value > 1) {
				(*custom_widget->bar_value)--;
			}
			lv_bar_set_value(custom_widget->obj_bar, *custom_widget->bar_value, LV_ANIM_ON);
		}
	}
}

LV_EVENT_CB_DECLARE(ring_btn_event_handle)
{
	if (e == LV_EVENT_CLICKED)
	{
		_Ring_Widget *user_data = (_Ring_Widget *)obj->user_data;
		if (user_data->ring_choosen == NULL) {
			return;
		}
		lv_obj_t *next_type = NULL;
		if (obj == user_data->btn_left) {
			next_type = lv_obj_get_child(user_data->btn_cont,user_data->ring_choosen);
			if (next_type != NULL) {
				lv_btn_set_state(user_data->ring_choosen, LV_BTN_STATE_RELEASED);
				lv_btn_set_state(next_type, LV_BTN_STATE_CHECKED_RELEASED);
				VoiceSetting_local.memory->ring_type--;
				user_data->ring_choosen = next_type;
			}
		}
		else if (obj == user_data->btn_right) {
			next_type = lv_obj_get_child_back(user_data->btn_cont,user_data->ring_choosen);
			if (next_type != NULL) {
				lv_btn_set_state(user_data->ring_choosen,LV_BTN_STATE_RELEASED);
				lv_btn_set_state(next_type, LV_BTN_STATE_CHECKED_RELEASED);
				VoiceSetting_local.memory->ring_type++;
				user_data->ring_choosen = next_type;
			}
		}
		
	}
}

LV_EVENT_CB_DECLARE(home_return_btn_event_handle)
{
	if (e == LV_EVENT_CLICKED) {
		ui_printf("home_return btn event clicked\r\n");
		gui_sys_screen_event_send(scr_chtype_return,scr_Home);
	}
}
#if 1
LV_EVENT_CB_DECLARE(ring_type_btn_test)
{
	switch (e)
	{
	case LV_EVENT_PRESSED:
		ui_printf("Pressed\n");
		break;

	case LV_EVENT_SHORT_CLICKED:
		ui_printf("Short clicked\n");
		break;

	case LV_EVENT_CLICKED:
		ui_printf("Clicked\n");
		break;

	case LV_EVENT_LONG_PRESSED:
		ui_printf("Long press\n");
		break;

	case LV_EVENT_LONG_PRESSED_REPEAT:
		ui_printf("Long press repeat\n");
		break;

	case LV_EVENT_RELEASED:
		ui_printf("Released\n");
		break;
	}
	uint8_t btn_state = lv_btn_get_state(obj);
	ui_printf("ring type btn clicked,btn:%p,btn state:%d\r\n", obj, btn_state);
}
#endif
/*======================================
**********UI Layer**********************
=======================================*/
/*Format:
***@prefix: voice_set_
***@ui: voice_set_ui_
***@event_cb :voice_set_eventcb
*/
static lv_obj_t *voice_set_ui_head_title(lv_obj_t *parent, char *head_str)
{
	return universal_misc_rm_head_title(parent, head_str, NULL);
}

static lv_obj_t *voice_set_ui_return_create(lv_obj_t *parent, lv_event_cb_t event_cb)
{
	_universal_obj_element elm_btn;
	elm_btn.parent = parent;
	elm_btn.width = 120;
	elm_btn.height = 50;

	lv_obj_t *btn = universal_btn_label_chineses_create(&elm_btn, "返回");
	lv_theme_apply(btn, (lv_theme_style_t)sigma_star_theme_Btn_home_return);
	lv_obj_set_event_cb(btn, event_cb);

	screen_adaption_lvgl_obj_align(btn, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);

	return btn;
}

//para:btn_flg: voice->0 , bell->1, microphone->2
//p_bar_value: value for bar,as pointer
static lv_obj_t* voice_set_ui_common_cont_add(lv_obj_t *parent, uint8_t btn_flag, uint8_t *p_bar_value, const void *scr_img)
{
	lv_obj_t *obj_cont = lv_cont_create(parent, NULL);
	lv_cont_set_fit(obj_cont, LV_FIT_NONE);
	lv_cont_set_layout(obj_cont, LV_LAYOUT_OFF);
	screen_adaption_lvgl_obj_set_size(obj_cont, widget_cont_w, widget_cont_h);
	lv_theme_apply(obj_cont, (lv_theme_style_t)sigma_star_theme_VoiceSetting_part);
	lv_style_list_t *list = lv_obj_get_style_list(obj_cont, LV_CONT_PART_MAIN);

	lv_obj_t *icon_img = lv_img_create(obj_cont, NULL);
	lv_img_set_src(icon_img, scr_img);
	screen_adaption_lvgl_obj_align(icon_img, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);

	lv_obj_t *obj_bar = lv_bar_create(obj_cont, NULL);
	screen_adaption_lvgl_obj_set_size(obj_bar, 270, 30);
	screen_adaption_lvgl_obj_align(obj_bar, NULL, LV_ALIGN_CENTER, 40, 0);
	lv_bar_set_range(obj_bar, 1, 9);
	lv_bar_set_start_value(obj_bar, 0, LV_ANIM_ON);
	lv_bar_set_value(obj_bar, *p_bar_value, LV_ANIM_ON);
	uint16_t bar_y = lv_obj_get_y(obj_cont);

	_Voice_widget *wt_voice_btnp = (_Voice_widget *)general_malloc(sizeof(_Voice_widget));
	wt_voice_btnp->obj_bar = obj_bar;
	wt_voice_btnp->btn_flag = btn_flag * 2;

	_Voice_widget *wt_voice_btnm = (_Voice_widget *)general_malloc(sizeof(_Voice_widget));
	wt_voice_btnm->obj_bar = obj_bar;
	wt_voice_btnm->btn_flag = btn_flag * 2 + 1;

	wt_voice_btnm->bar_value = wt_voice_btnp->bar_value = p_bar_value;

	lv_obj_t *obj_btn, *btn_label;
	obj_btn = lv_btn_create(obj_cont, NULL);
	screen_adaption_lvgl_obj_set_size(obj_btn, 45, 45);
	screen_adaption_lvgl_obj_align(obj_btn, obj_bar, LV_ALIGN_OUT_LEFT_MID, -25, 0);
	obj_btn->user_data = wt_voice_btnp;
	lv_obj_set_event_cb(obj_btn, btn_event_handle);

	btn_label = lv_label_create(obj_btn, NULL);
	lv_label_set_text(btn_label, LV_SYMBOL_PLUS);

	obj_btn = lv_btn_create(obj_cont, NULL);
	screen_adaption_lvgl_obj_set_size(obj_btn, 45, 45);
	screen_adaption_lvgl_obj_align(obj_btn, obj_bar, LV_ALIGN_OUT_RIGHT_MID, 25, 0);
	obj_btn->user_data = wt_voice_btnm;
	lv_obj_set_event_cb(obj_btn, btn_event_handle);

	btn_label = lv_label_create(obj_btn, NULL);
	lv_label_set_text(btn_label, LV_SYMBOL_MINUS);


	return obj_cont;
}
#if 1
static lv_obj_t* voice_set_ui_ring_cont_add(lv_obj_t *parent, const void *scr_img)
{
	lv_obj_t *cont = lv_cont_create(parent, NULL);
	lv_cont_set_fit(cont, LV_FIT_NONE);
	lv_cont_set_layout(cont, LV_LAYOUT_OFF);
	screen_adaption_lvgl_obj_set_size(cont, widget_cont_w, widget_cont_h);
	lv_theme_apply(cont, (lv_theme_style_t)sigma_star_theme_VoiceSetting_part);

	lv_obj_t *icon_img = lv_img_create(cont, NULL);
	lv_img_set_src(icon_img, scr_img);
	screen_adaption_lvgl_obj_align(icon_img, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0);

	lv_obj_t *btn_cont = lv_cont_create(cont, NULL);
	lv_cont_set_fit(btn_cont, LV_FIT_TIGHT);
	lv_cont_set_layout(btn_cont, LV_LAYOUT_ROW_MID);
	//screen_adaption_lvgl_obj_align(btn_cont,icon_img,LV_ALIGN_OUT_RIGHT_TOP,30,0);
	lv_theme_apply(btn_cont, (lv_theme_style_t)sigma_star_theme_VoiceSetting_ring);

	char text_1[] = { "1" };
	char text_2[] = { "2" };
	char text_3[] = { "3" };
	char text_4[] = { "4" };
	char text_5[] = { "5" };
	char text_6[] = { "6" };
	char *label_text[6] = { text_1,text_2,text_3,text_4,text_5,text_6 };
	//char label_text[] = {'1','2','3','4','5','6'};
	uint8_t i;
	lv_obj_t *obj_btn, *obj_label;
	lv_obj_t *ring_choosen = NULL;
	for (i = 0; i < 6; i++) {
		obj_btn = lv_btn_create(btn_cont, NULL);
		screen_adaption_lvgl_obj_set_size(obj_btn, 45, 30);
		lv_btn_set_checkable(obj_btn, true);

		lv_obj_set_event_cb(obj_btn, ring_type_btn_test);
		if (VoiceSetting_local.memory->ring_type == i + 1) {
			ring_choosen = obj_btn;
		}
		lv_theme_apply(obj_btn, (lv_theme_style_t)sigma_star_theme_Btn_opa);

		obj_label = lv_label_create(obj_btn, NULL);
		lv_label_set_text(obj_label, label_text[i]);
	}
	screen_adaption_lvgl_obj_align(btn_cont, NULL, LV_ALIGN_CENTER, 40, 0);
	if (ring_choosen != NULL) {  //color the choosen button

		lv_btn_set_state(ring_choosen, LV_BTN_STATE_CHECKED_RELEASED);
	}

	_Ring_Widget *ring_widget = (_Ring_Widget *)general_malloc(sizeof(_Ring_Widget));
	ring_widget->btn_cont = btn_cont;
	ring_widget->ring_choosen = ring_choosen;

	lv_obj_t *btn_control = lv_btn_create(cont, NULL);
	screen_adaption_lvgl_obj_set_size(btn_control, 45, 45);
	screen_adaption_lvgl_obj_align(btn_control, btn_cont, LV_ALIGN_OUT_LEFT_MID, -25, 0);
	ring_widget->btn_left = btn_control;
	btn_control->user_data = ring_widget;
	lv_obj_set_event_cb(btn_control, ring_btn_event_handle);

	lv_obj_t *label_control = lv_label_create(btn_control, NULL);
	lv_label_set_text(label_control, LV_SYMBOL_LEFT);

	btn_control = lv_btn_create(cont, NULL);
	screen_adaption_lvgl_obj_set_size(btn_control, 45, 45);
	screen_adaption_lvgl_obj_align(btn_control, btn_cont, LV_ALIGN_OUT_RIGHT_MID, 25, 0);
	ring_widget->btn_right = btn_control;
	btn_control->user_data = ring_widget;
	lv_obj_set_event_cb(btn_control, ring_btn_event_handle);

	label_control = lv_label_create(btn_control, NULL);
	lv_label_set_text(label_control, LV_SYMBOL_RIGHT);

	return cont;

}
#endif

void voice_set_ui_create(void)
{

	if (VoiceSetting_local.memory->this_scr == NULL) {
		lv_obj_t *obj_scr = lv_obj_create(NULL, NULL);
		VoiceSetting_local.memory->this_scr = obj_scr;
	}
	lv_scr_load(VoiceSetting_local.memory->this_scr);
	ui_printf("voice_set_ui_create\r\n");

	lv_obj_t *page_bg = universal_misc_rm_bg_default_create();

	voice_set_ui_head_title(page_bg,"音量设置");

	voice_set_ui_return_create(page_bg, home_return_btn_event_handle);
	
	LV_IMG_DECLARE(icon_voice_48_42);

	lv_obj_t *widget_cont = voice_set_ui_common_cont_add(lv_scr_act(), 0, &VoiceSetting_local.memory->voice_value, &icon_voice_48_42);
	screen_adaption_lvgl_obj_set_pos(widget_cont, widget_left_offset, widget_head_offset);

	lv_obj_t *bell_cont = voice_set_ui_common_cont_add(lv_scr_act(), 1, &VoiceSetting_local.memory->bell_value, &icon_voice_48_42);
	screen_adaption_lvgl_obj_align(bell_cont, widget_cont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

	lv_obj_t *microphone_cont = voice_set_ui_common_cont_add(lv_scr_act(), 2, &VoiceSetting_local.memory->microphone_value, &icon_voice_48_42);
	screen_adaption_lvgl_obj_align(microphone_cont, bell_cont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

	lv_obj_t *ring_cont = voice_set_ui_ring_cont_add(lv_scr_act(), &icon_voice_48_42);
	screen_adaption_lvgl_obj_align(ring_cont, microphone_cont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10);

	lv_obj_t *home_btn = lv_btn_create(lv_scr_act(), NULL);
	screen_adaption_lvgl_obj_set_size(home_btn, 100, 60);
	screen_adaption_lvgl_obj_align(home_btn, ring_cont, LV_ALIGN_OUT_BOTTOM_RIGHT, -60, 10);
	lv_theme_apply(home_btn, (lv_theme_style_t)sigma_star_theme_Btn_home_return);
	lv_obj_set_event_cb(home_btn, home_return_btn_event_handle);

	lv_obj_t *obj_label = lv_label_create(home_btn, NULL);
	lv_label_set_text(obj_label, "Home");
}

/*=======================================
*** Exterior Interface with screen system
========================================*/
/*Format:
***@ prefix: voice_setting_ex_interface
*/
static void voice_setting_ex_interface_quit(void)
{
	voice_set_data_free();
	lv_obj_clean(VoiceSetting_local.memory->this_scr);
}

bool scr_VoiceSetting_register()
{
	bool is_true = gui_sys_screen_register(voice_set_ui_create, 
											voice_setting_ex_interface_quit, 
											NULL, 
											NULL, 
											NULL, 
											scr_VoiceSetting);

    if(is_true) return true;
	else return false;
}


