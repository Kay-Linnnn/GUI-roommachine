#ifndef  _UNIVERSAL_API_V7_H
#define _UNIVERSAL_API_V7_H


#ifdef __cplusplus
extern "C" {
#endif


#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

typedef struct {
	lv_coord_t width;
	lv_coord_t height;
	lv_event_cb_t event_cb;
	lv_obj_t *parent;
	lv_obj_t *copy;
	void *user_data;
	int theme;
}_universal_obj_element;

typedef struct {
	uint8_t row_cnt;
	_universal_obj_element *base;
}_universal_roller_element;

/*Bar*/
typedef struct {
	bool enable_anim;
	int16_t min;
	int16_t max;
	int16_t start_value;
}_universal_bar_value_t;

/*Roller*/
typedef struct {
	lv_roller_mode_t mode;
	uint8_t row_cnt;
}_universal_roller_value_t;
#if 0
#define Universal_Element_Apply(_type, _elemnet)\
do{\
_type _element;\
memset(&_element, 0, sizeof(_type));\
}while(0);
#endif
#define Uni_Element_init(_element)	memset(&_element, 0, sizeof(_universal_obj_element))

#define Uni_Elemet_Roller_init(_roller)	memset(&roller, 0, sizeof(_universal_roller_element))

/*
Universal Option
*/
enum {
	universal_font_S,
	universal_font_Common,
	universal_font_L,
};
typedef uint8_t universal_font_size_t;

/*PS
***:Different obj uses different element,but there are three basic elements used in any obj of universal fun
***Which name them as "basic",that's parent,width,height
***for furture feature,use "Parent" if basic do not need
*/

/*=======================
*******Misc function*****
========================*/
/*
***Create A RoomMachine Default Background
*/
lv_obj_t *universal_misc_rm_bg_default_create(void);

/*
***Create A RoomMachine picture Background
*/
lv_obj_t *universal_misc_rm_pic_bg_create(void);

/*
*** Create a head title default
***Return: img btn for setting event handle
*/
lv_obj_t *universal_misc_rm_head_title(lv_obj_t *parent,const char *head_str, lv_event_cb_t return_event);

/*
***create a keyboard with ascall and numbers
***@Para->elm_kb_bg: bg to hold all btnm, it's a transparent obj, it's a tight obj which kb will automate calculate it's size and fit it warpped
***@Para->kb_event: event for all btnm
***@Para->kb_user_data: user data insert in kb->user_data
***Return: kb background
*/
lv_obj_t *universal_tencell_ascall_kb_create(const _universal_obj_element *elm_kb_bg, lv_event_cb_t kb_event, void *kb_user_data);

enum {
	Keyboard_design_align_center,
	Keyboard_design_align_top,
};

typedef uint8_t keyboard_design_mode_t;
/*
***Design a keyboard, actuallt it called fun 'universal_tencell_ascall_kb_create', there a two obj, one visual bg for apperance, one transprant for holds all btnms as one union
***@Para->parent
***@Para->bg_widht: visual bg width
***@Para->bg_height: visual bg height
***@Para->top_space: the vertical space between real-btnm-bg and top border of visual bg
***@Para->top_left_space: the horizational space between real-btnm-bg and left border of visual bg
***@kb_event: kb event
***@kb_user_data: user data insert in kb->user_data
***Note:the real transprant kb bg will set pos as (top_left_space, top_space)
***@Para->align: real_kb_kb align mode, see 'keyboard_design_mode_t'
***@Para->kb_tight_height: if align is Keyboard_design_align_top, then the real kb height will set to @kb_tight_height
***Return: visual bg
*/
lv_obj_t *universal_keyboard_design(lv_obj_t *parent, lv_coord_t bg_width, lv_coord_t bg_height, lv_coord_t top_space, lv_coord_t top_left_space, lv_event_cb_t kb_event, void *kb_user_data, keyboard_design_mode_t align,										lv_coord_t kb_tight_height);

/*========================
************Button********
==========================*/
lv_obj_t *universal_btn_chinese_create(lv_obj_t *parent, uint16_t btn_width, uint16_t btn_height, char *txt);

/*===
***@Para->elm_btn_label:basic
***@Para-> txt:label txt
*/
lv_obj_t *universal_btn_label_chineses_create(_universal_obj_element *elm_btn_label, char *txt);

/*===
***@Para->elm_btn_label:basic
***@Para-> txt:label txt
*/
lv_obj_t *universal_btn_label_create(_universal_obj_element *elm_btn_label, const char *txt, int theme);
//btnm
/*===============================
************Button Matrix********
===============================*/

/*===
***@Para-> elm_btnm:basic,event_cb,user_data(if not NULL)
***@Para-> map:as you see
*/
lv_obj_t *universal_btnm_create(_universal_obj_element *elm_btnm, const char *map[], const  lv_btnmatrix_ctrl_t *ctrl_map, int theme);

//textarea
/*============================
************Textarea**********
=============================*/
/*===
***Descr:default feature: one line mode, curson hidden, and set to pwd mode
***@parameter-> elm_ta:basic 
***@parameter-> placeholder_txt: NULL if not use
===*/
lv_obj_t *universal_ta_pwd_create(_universal_obj_element *elm_ta, bool enable_click, char *placeholder_txt, int theme);

/*===
***Descr:
*/
lv_obj_t *universal_ta_create(_universal_obj_element *elm_ta, const char *init_txt, const char *placeholder_txt, bool enable_click, bool enable_pwd, bool hide_cursor, int theme);

/*========================
************Obj************
==========================*/

/*===
***@Para-> elm_obj:basic
***@Para-> theme
*/
lv_obj_t *universal_obj_create(const _universal_obj_element *elm_obj, int theme);

void universal_obj_add_style(lv_obj_t *obj, lv_style_t *style_main);

/*==========================
**********Container*********
===========================*/
/*===
***@Para->elm_cont:basic
*/
lv_obj_t *universal_cont_create(const _universal_obj_element *elm_cont, lv_layout_t layout, lv_fit_t fit, int theme);

/*=========================
************Image**********
==========================*/
lv_obj_t *universal_image_create(_universal_obj_element *elm_img, const void *src_img, int theme);

/*==========================
**********Roller************
===========================*/
/*===
***@Para->elm_roller: base:basic
***@Para:->roller_option
***@Para-> v_roller: roller mode, visible row
*/
lv_obj_t *universal_roller_create(_universal_obj_element *elm_roller, const char *roller_option, _universal_roller_value_t *v_roller, int theme);

/*=========================
************Bar************
==========================*/
/*===
***@Para->elm_bar: basic
***@Para->value: specific value for bar, like: start_value, range(min, max), anim ....
*/
lv_obj_t *universal_bar_create(const _universal_obj_element *elm_bar,const  _universal_bar_value_t *value, int theme);

/*=========================
**********Label************
==========================*/
/* Label display Chinese Character
*/
lv_obj_t *universal_label_chinese_create(lv_obj_t *parent, const char *txt, universal_font_size_t font_size);

/*Default Label*/
lv_obj_t *universal_label_create(_universal_obj_element *elm_label, const char *txt, int theme);

/*=======================
***********LIST**********
=======================*/
lv_obj_t *universal_list_create(_universal_obj_element *elm_list, int theme);

//task
/*==========================
************Task************
==========================*/
/*===
***@Para-> period_cnt: 100ms cnt
***@Para-> fun_addr: your function auto call when timer over
***@Para-> data: data send to your function
*/
uint32_t *universal_task_100ms_timer_create(uint8_t period_cnt,uint32_t *fun_addr,void *data);


/*========================
****UI Screen Adaption****
=========================*/

/*
***Auto Calculate Horizatal Size
*/
lv_coord_t screen_adaption_lvgl_hor_calculate(lv_coord_t size);

/*
***Auto Calculate Vertical Size
*/
lv_coord_t screen_adaption_lvgl_ver_calculate(lv_coord_t ver);

/*
***Size
*/
void screen_adaption_lvgl_obj_set_size(lv_obj_t *obj, lv_coord_t w, lv_coord_t h);

/*
***Align
*/
void screen_adaption_lvgl_obj_align(lv_obj_t *obj, lv_obj_t *base, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

/*
***Position
*/
void screen_adaption_lvgl_obj_set_pos(lv_obj_t *obj, lv_coord_t x, lv_coord_t y);


#ifdef __cplusplus
}
#endif


#endif // ! _UNIVERSAL_API_V7_H
