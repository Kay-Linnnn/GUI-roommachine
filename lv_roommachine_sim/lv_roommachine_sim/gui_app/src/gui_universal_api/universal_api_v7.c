#include "universal_api_v7.h"

#include"sigma_star_theme.h"
#include "gui_app.h"

#ifdef _VS_SIMULATOR
#pragma execution_character_set("utf-8")
static const char * bg_picture_path = "W:gui_app/src/pic_font_file/screen_bg_7inch_32b.bin";
static const char * return_imgbtn_rel_path = "W:gui_app/src/pic_font_file/return_btn_114_76_32b.bin";//return_btn_114_76_32b.bin
static const char * return_imgbtn_pre_path = "W:gui_app/src/pic_font_file/return_btn_pre_114_76_32b.bin";
#else
static const char * bg_picture_path = "W:";
static const char * return_imgbtn_rel_path = "W:";
static const char * return_imgbtn_pre_path = "W:";
#endif

static const char * tencell_num_character_btnm_map_line_1[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "" };
static const lv_btnmatrix_ctrl_t tencell_numc_btnm_map_1[] = { 1,1,1,1,1,1,1,1,1,1 };

static const char * tencell_num_character_btnm_map_line_2[] = { "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "" };
static const lv_btnmatrix_ctrl_t tencell_numc_btnm_map_2[] = { 1,1,1,1,1,1,1,1,1,1 };

static const char * tencell_num_character_btnm_map_line_3[] = { "A", "S", "D", "F", "G", "H", "J", "K", "L", "" };
static const lv_btnmatrix_ctrl_t tencell_numc_btnm_map_3[] = { 1,1,1,1,1,1,1,1,1 };

static const char * tencell_num_character_btnm_map_line_4[] = { "Z", "X", "C", "V", "B", "N", "M", "" };
static const lv_btnmatrix_ctrl_t tencell_numc_btnm_map_4[] = { 1,1,1,1,1,1,1 };

#define Exec_NOT_NULL_FUNC(_ptr, Func)\
do {\
	if (_ptr != NULL)\
		Func; \
} while (0);

#define Exec_Theme_Apply(_obj, _Theme)\
do{\
	if(_Theme != tencell_theme_NULL)\
		lv_theme_apply(_obj, (lv_theme_style_t)_Theme);\
} while (0)

/*=======================
*******Misc function*****
========================*/
//rm: roommachine
lv_obj_t *universal_misc_rm_bg_default_create()
{
	#if 0
	lv_obj_t *bg = lv_obj_create(lv_scr_act(),NULL);
	screen_adaption_lvgl_obj_set_size(bg, SCREEN_BASE_RESOLUTION_HOR, SCREEN_BASE_RESOLUTION_VER);
	screen_adaption_lvgl_obj_set_pos(bg, 0, 0);
	//lv_theme_apply(bg, (lv_theme_style_t)sigma_star_theme_default_bg);
	lv_obj_set_style_local_bg_opa(bg, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);

	return bg;
	#endif
	return universal_misc_rm_pic_bg_create();
}

lv_obj_t *universal_misc_rm_pic_bg_create()
{
#ifdef _VS_SIMULATOR
	//const char * bg_picture_path = "W:gui_app/src/pic_font_file/screen_bg_7inch.bin";
#else
	//const char * bg_picture_path = "W:";
#endif // _VS_SIMULATOR

	lv_obj_t *pic_bg = lv_img_create(lv_scr_act(), NULL);
	lv_obj_set_size(pic_bg, LV_HOR_RES, LV_VER_RES);
	lv_img_set_src(pic_bg, bg_picture_path);

	return pic_bg;
}

lv_obj_t *universal_misc_rm_head_title(lv_obj_t *parent,const char *head_str, lv_event_cb_t return_event)
{
#if 0
	lv_obj_t *head_cont = lv_cont_create(parent, NULL);
	screen_adaption_lvgl_obj_set_pos(head_cont, 0, 0);
	lv_cont_set_fit(head_cont, LV_FIT_NONE);
	screen_adaption_lvgl_obj_set_size(head_cont, SCREEN_BASE_RESOLUTION_HOR, 60);
	//screen_adaption_lvgl_obj_set_size(head_cont, SCREEN_BASE_RESOLUTION_HOR, 50);
	lv_cont_set_layout(head_cont, LV_LAYOUT_CENTER);
	lv_theme_apply(head_cont, (lv_theme_style_t)sigma_star_theme_title);
#endif
	uint16_t label_top_space = 20;

	lv_obj_t *obj_label = lv_label_create(parent, NULL);//head_cont
	lv_theme_apply(obj_label, (lv_theme_style_t)sigma_star_theme_label_chinese_normal);
	lv_label_set_text(obj_label, head_str);
	screen_adaption_lvgl_obj_align(obj_label, NULL, LV_ALIGN_IN_TOP_MID, 0, label_top_space);

	lv_obj_t *obj_return = lv_imgbtn_create(parent, NULL);
	lv_imgbtn_set_src(obj_return, LV_STATE_DEFAULT, return_imgbtn_rel_path);
	lv_imgbtn_set_src(obj_return, LV_BTN_STATE_PRESSED, return_imgbtn_pre_path);
	screen_adaption_lvgl_obj_align(obj_return, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
	Exec_NOT_NULL_FUNC(return_event, lv_obj_set_event_cb(obj_return, return_event));

	//return head_cont;
	return obj_return;
}

/*
***Return: kb background
*/
lv_obj_t *universal_tencell_ascall_kb_create(const _universal_obj_element *elm_kb_bg, lv_event_cb_t kb_event, void *kb_user_data)
{
	//size calculation
	lv_coord_t h_bg = screen_adaption_lvgl_ver_calculate(elm_kb_bg->height);
	lv_coord_t h_num = (lv_coord_t)(h_bg / 5.8); //take h_num as the basic size, then, h_char is 1.5 * h_num, h_space = 0.1 * h_num, and there is 3 h_char and 3 h_space
									//so there is equal: h_bg = h_num + 3 * h_char + 3 * h_space = h_num + 3 * 1.5 * h_num + 3 * 0.1 * h_hum = 5.8 * h_num;
	lv_coord_t h_char = (lv_coord_t)(1.5 * h_num), h_space = (lv_coord_t)(0.1 * h_num);
	
	lv_coord_t hor_inner_space = 5;//kb btnm inner space
	lv_coord_t width_single_btn = screen_adaption_lvgl_hor_calculate(elm_kb_bg->width) / 10;//single btnm btn, calculate from the longest kb(number one), 10 btns
	//ui_printf_plus("kb_bg_height:%d, calculate height:%d", h_bg, h_num + 3 * h_char + 3 * h_space);
	//ui_printf_plus("h_num:%d, h_char:%d, h_space:%d", h_num, h_char, h_space);

	static lv_style_t style_btnm_bg;
	lv_style_init(&style_btnm_bg);
	lv_style_set_pad_inner(&style_btnm_bg, LV_STATE_DEFAULT, hor_inner_space);
	//lv_style_set_bg_color(&style_btnm_bg, LV_STATE_DEFAULT, LV_COLOR_GREEN);
	lv_style_set_bg_opa(&style_btnm_bg, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_style_set_pad_top(&style_btnm_bg, LV_STATE_DEFAULT, 1);
	lv_style_set_pad_bottom(&style_btnm_bg, LV_STATE_DEFAULT, 1);
	lv_style_set_pad_left(&style_btnm_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_btnm_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_border_width(&style_btnm_bg, LV_STATE_DEFAULT, 0);
	lv_style_set_border_width(&style_btnm_bg, LV_STATE_PRESSED, 0);
	//lv_style_set_border_width(&style_btnm_bg, LV_STATE_CHECKED, 0);
	lv_style_set_border_width(&style_btnm_bg, LV_STATE_FOCUSED, 0);

	static lv_style_t style_btnm_num;
	lv_style_init(&style_btnm_num);
	lv_style_set_bg_color(&style_btnm_num, LV_STATE_DEFAULT, LV_COLOR_GRAY);
	lv_style_set_radius(&style_btnm_num, LV_STATE_DEFAULT, 10);
	lv_style_set_text_color(&style_btnm_num, LV_STATE_DEFAULT, LV_COLOR_BLACK);

	static lv_style_t style_btnm_char;
	lv_style_copy(&style_btnm_char, &style_btnm_num);
	lv_style_set_bg_color(&style_btnm_char, LV_STATE_DEFAULT, TENCELL_RM_COLOR_KB_BTN_GRAY);
	#if 0
	_universal_obj_element elm_obj;
	elm_obj.parent = parent;
	elm_obj.width = 609;
	elm_obj.height = 311;
	#endif

	static lv_style_t style_obj;
	lv_style_init(&style_obj);
	lv_style_set_bg_opa(&style_obj, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_style_set_pad_all(&style_obj, LV_STATE_DEFAULT, 0);
	lv_style_set_border_width(&style_obj, LV_STATE_DEFAULT, 0);
	lv_style_set_border_width(&style_obj, LV_STATE_FOCUSED, 0);
	//lv_style_set_pad_left(&style_obj, LV_STATE_DEFAULT, 0);
	//lv_style_set_pad_right(&style_obj, LV_STATE_DEFAULT, 0);
	//lv_obj_t *kb_bg = universal_cont_create(elm_kb_cont, NULL, LV_LAYOUT_PRETTY_MID, LV_FIT_NONE);//universal_obj_create(&elm_obj, NULL);
	//lv_theme_apply(kb_bg, (lv_theme_style_t)tencell_theme_obj_bg);
	//lv_theme_apply(kb_bg, (lv_theme_style_t)sigma_star_theme_dial_kb_cont_bg);
	lv_obj_t *kb_bg = universal_obj_create(elm_kb_bg, tencell_theme_NULL);
	lv_obj_add_style(kb_bg, LV_OBJ_PART_MAIN, &style_obj);
	

	_universal_obj_element elm_btnm;
	Uni_Element_init(elm_btnm);
	//memset(&elm_btnm, 0, sizeof(_universal_obj_element));
	//Universal_Element_Apply(_universal_obj_element, elm_btnm);
	elm_btnm.parent = kb_bg;
	elm_btnm.width = lv_obj_get_width(kb_bg);
	elm_btnm.height = h_num;
	elm_btnm.event_cb = kb_event;
	elm_btnm.user_data = kb_user_data;

	lv_obj_t *kb = NULL;
	lv_obj_t *kb_last = NULL;
	
	//kb = lv_btnmatrix_create(kb_bg, NULL);
	kb = universal_btnm_create(&elm_btnm, tencell_num_character_btnm_map_line_1, tencell_numc_btnm_map_1, tencell_theme_NULL);
	lv_obj_add_style(kb, LV_BTNMATRIX_PART_BTN, &style_btnm_num);
	lv_obj_add_style(kb, LV_BTNMATRIX_PART_BG, &style_btnm_bg);
	screen_adaption_lvgl_obj_set_pos(kb, 0, 0);
	kb_last = kb;

	elm_btnm.height = h_char;
	kb = universal_btnm_create(&elm_btnm, tencell_num_character_btnm_map_line_2, tencell_numc_btnm_map_2, tencell_theme_NULL);
	lv_obj_add_style(kb, LV_BTNMATRIX_PART_BTN, &style_btnm_char);
	lv_obj_add_style(kb, LV_BTNMATRIX_PART_BG, &style_btnm_bg);
	screen_adaption_lvgl_obj_align(kb, kb_last, LV_ALIGN_OUT_BOTTOM_LEFT, 0, h_space);
	kb_last = kb;

	elm_btnm.width -= width_single_btn;
	kb = universal_btnm_create(&elm_btnm, tencell_num_character_btnm_map_line_3, tencell_numc_btnm_map_3, tencell_theme_NULL);
	lv_obj_add_style(kb, LV_BTNMATRIX_PART_BTN, &style_btnm_char);
	lv_obj_add_style(kb, LV_BTNMATRIX_PART_BG, &style_btnm_bg);
	screen_adaption_lvgl_obj_align(kb, kb_last, LV_ALIGN_OUT_BOTTOM_LEFT, width_single_btn / 2, h_space);
	kb_last = kb;

	elm_btnm.width -= width_single_btn * 2;
	kb = universal_btnm_create(&elm_btnm, tencell_num_character_btnm_map_line_4, tencell_numc_btnm_map_4, tencell_theme_NULL);
	lv_obj_add_style(kb, LV_BTNMATRIX_PART_BTN, &style_btnm_char);
	lv_obj_add_style(kb, LV_BTNMATRIX_PART_BG, &style_btnm_bg);
	screen_adaption_lvgl_obj_align(kb, kb_last, LV_ALIGN_OUT_BOTTOM_LEFT, width_single_btn , h_space);

	return kb_bg;
}

lv_obj_t *universal_keyboard_design(lv_obj_t *parent, lv_coord_t bg_width, lv_coord_t bg_height, lv_coord_t top_space, lv_coord_t top_left_space, lv_event_cb_t kb_event, void *kb_user_data, keyboard_design_mode_t align,										lv_coord_t kb_tight_height)
{
	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = parent;
	elm_obj.width = bg_width;
	elm_obj.height = bg_height;

	lv_obj_t *kb_visual_bg = universal_obj_create(&elm_obj, tencell_theme_obj_bg_cont_kb);
	
	//_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = kb_visual_bg;
	elm_obj.width = lv_obj_get_width(kb_visual_bg) - top_left_space * 2;
	elm_obj.height = lv_obj_get_height(kb_visual_bg) - top_space * 2;

	if (align == Keyboard_design_align_top)
	{
		elm_obj.height = kb_tight_height;
	}

	lv_obj_t *kb_bg = universal_tencell_ascall_kb_create(&elm_obj, kb_event, kb_user_data);
	screen_adaption_lvgl_obj_set_pos(kb_bg, top_left_space, top_space);

	return kb_visual_bg;
}

/*========================
************Button********
==========================*/
lv_obj_t *universal_btn_chinese_create(lv_obj_t *parent,uint16_t btn_width,uint16_t btn_height,char *txt)
{
	lv_obj_t *btn = lv_btn_create(parent, NULL);
	screen_adaption_lvgl_obj_set_size(btn, btn_width, btn_height);

	lv_obj_t *label_of_btn = lv_label_create(btn, NULL);
	lv_label_set_text(label_of_btn, txt);
	lv_theme_apply(label_of_btn, (lv_theme_style_t)sigma_star_theme_label_chinese);

	return btn;
}

#if 0
lv_obj_t *universal_btnm_create(lv_obj_t *parent,lv_obj_t *copy,uint16_t width,uint16_t height,const char *map[])
{
	lv_obj_t *btnm = lv_btnmatrix_create(parent,NULL);
	screen_adaption_lvgl_obj_set_size(btnm,width,height);
	lv_btnmatrix_set_map(btnm,map);

	return btnm;
}
#endif
lv_obj_t *universal_btn_label_chineses_create(_universal_obj_element *elm_btn_label,char *txt)
{
	lv_obj_t *btn = lv_btn_create(elm_btn_label->parent,NULL);
	screen_adaption_lvgl_obj_set_size(btn,elm_btn_label->width, elm_btn_label->height);

	lv_obj_t *label = lv_label_create(btn,NULL);
	lv_label_set_text(label, txt);
	lv_theme_apply(label, (lv_theme_style_t)sigma_star_theme_label_chinese);

	return btn;
}

lv_obj_t *universal_btn_label_create(_universal_obj_element *elm_btn_label,const char *txt, int theme)
{
	lv_obj_t *btn = lv_btn_create(elm_btn_label->parent, elm_btn_label->copy);
	screen_adaption_lvgl_obj_set_size(btn, elm_btn_label->width, elm_btn_label->height);
	btn->user_data = elm_btn_label->user_data;
	Exec_NOT_NULL_FUNC(elm_btn_label->event_cb, lv_obj_set_event_cb(btn, elm_btn_label->event_cb));

	if (theme != tencell_theme_NULL)
		lv_theme_apply(btn, (lv_theme_style_t)theme);

	if (!txt) return btn;

	lv_obj_t *label = lv_label_create(btn, NULL);
	lv_label_set_text(label, txt);

	return btn;
}

/*===============================
************Button Matrix********
================================*/
lv_obj_t *universal_btnm_create(_universal_obj_element *elm_btnm, const char *map[], const  lv_btnmatrix_ctrl_t *ctrl_map, int theme)
{
	lv_obj_t *btnm = lv_btnmatrix_create(elm_btnm->parent, NULL);
	screen_adaption_lvgl_obj_set_size(btnm,elm_btnm->width, elm_btnm->height);
	lv_btnmatrix_set_map(btnm, map);
	lv_obj_set_event_cb(btnm, elm_btnm->event_cb);

	Exec_NOT_NULL_FUNC(elm_btnm->user_data, btnm->user_data = elm_btnm->user_data);

	Exec_NOT_NULL_FUNC(ctrl_map, lv_btnmatrix_set_ctrl_map(btnm, ctrl_map));

	Exec_Theme_Apply(btnm, theme);

	return btnm;
}

/*========================
************Textarea********
==========================*/
lv_obj_t *universal_ta_pwd_create(_universal_obj_element *elm_ta, bool enable_click,char *placeholder_txt, int theme)
{
	lv_obj_t *ta = lv_textarea_create(elm_ta->parent, elm_ta->copy);
	screen_adaption_lvgl_obj_set_size(ta, elm_ta->width, elm_ta->height);
	lv_textarea_set_one_line(ta, true);
	lv_textarea_set_cursor_hidden(ta, true);
	lv_textarea_set_pwd_mode(ta, true);
	lv_textarea_set_text(ta, "");
	lv_obj_set_click(ta,enable_click);
	
	//if (!is_null_ptr(placeholder_txt))
		//lv_textarea_set_placeholder_text(ta, placeholder_txt);

	Exec_NOT_NULL_FUNC(placeholder_txt, lv_textarea_set_placeholder_text(ta, placeholder_txt));

	Exec_Theme_Apply(ta, theme);

	return ta;
}

lv_obj_t *universal_ta_create(_universal_obj_element *elm_ta, const char *init_txt, const char *placeholder_txt, bool enable_click, bool enable_pwd, bool hide_cursor, int theme)
{
	lv_obj_t *ta = lv_textarea_create(elm_ta->parent, elm_ta->copy);
	screen_adaption_lvgl_obj_set_size(ta, elm_ta->width, elm_ta->height);
	lv_textarea_set_one_line(ta, true);
	lv_textarea_set_cursor_hidden(ta, hide_cursor);
	lv_textarea_set_pwd_mode(ta, enable_pwd);
	lv_textarea_set_text(ta, "");
	lv_obj_set_click(ta, enable_click);
	lv_obj_set_event_cb(ta, elm_ta->event_cb);
	ta->user_data = elm_ta->user_data;

	Exec_NOT_NULL_FUNC(init_txt, lv_textarea_set_text(ta, init_txt));
	Exec_NOT_NULL_FUNC(placeholder_txt, lv_textarea_set_placeholder_text(ta, placeholder_txt));

	Exec_Theme_Apply(ta, theme);

	return ta;
}
/*========================
************Obj************
==========================*/
lv_obj_t *universal_obj_create(const _universal_obj_element *elm_obj, int theme)
{
	lv_obj_t *obj = lv_obj_create(elm_obj->parent, elm_obj->copy);
	//screen_adaption_lvgl_obj_set_size(obj,elm_obj->width,elm_obj->height);
	screen_adaption_lvgl_obj_set_size(obj, elm_obj->width, elm_obj->height);

	Exec_Theme_Apply(obj, theme);

	return obj;
}

void universal_obj_add_style(lv_obj_t *obj, lv_style_t *style_main)
{
	lv_obj_add_style(obj, LV_OBJ_PART_MAIN, style_main);
}

/*==========================
**********Container*********
===========================*/
lv_obj_t *universal_cont_create(const _universal_obj_element *elm_cont, lv_layout_t layout, lv_fit_t fit, int theme)
{
	lv_obj_t *cont = lv_cont_create(elm_cont->parent, elm_cont->copy);
	screen_adaption_lvgl_obj_set_size(cont, elm_cont->width, elm_cont->height);
	lv_obj_set_event_cb(cont, elm_cont->event_cb);
	cont->user_data = elm_cont->user_data;

	Exec_Theme_Apply(cont, theme);

	lv_cont_set_layout(cont,layout);
	lv_cont_set_fit(cont,fit);

	return cont;
}

/*=========================
************Image**********
==========================*/
lv_obj_t *universal_image_create(_universal_obj_element *elm_img, const void *src_img, int theme)
{
	lv_obj_t *obj_img = lv_img_create(elm_img->parent, elm_img->copy);
	lv_img_set_src(obj_img, src_img);

	Exec_Theme_Apply(obj_img, theme);

	return obj_img;
}

/*==========================
**********Roller************
===========================*/
#if 0
lv_obj_t *universal_roller_create(_universal_roller_element *elm_roller,lv_obj_t *copy,const char *roller_option,lv_roller_mode_t roller_mode)
{
	_universal_obj_element *elm_base = elm_roller->base;
	lv_obj_t *roller = lv_roller_create(elm_base->parent,copy);
	screen_adaption_lvgl_obj_set_size(roller, elm_base->width, elm_base->height);
	lv_roller_set_options(roller, roller_option, roller_mode);
	
	if (elm_roller->row_cnt != 0)
		lv_roller_set_visible_row_count(roller, elm_roller->row_cnt);

	return roller;
}
#endif

lv_obj_t *universal_roller_create(_universal_obj_element *elm_roller, const char *roller_option, _universal_roller_value_t *v_roller, int theme)
{
	lv_obj_t *roller = lv_roller_create(elm_roller->parent, elm_roller->copy);
	screen_adaption_lvgl_obj_set_size(roller, elm_roller->width, elm_roller->height);
	lv_roller_set_options(roller, roller_option, v_roller->mode);	
	
	lv_roller_set_visible_row_count(roller, v_roller->row_cnt);
	lv_obj_set_event_cb(roller, elm_roller->event_cb);
	
	Exec_Theme_Apply(roller, theme);

	return roller;
}

/*=========================
************Bar************
==========================*/
lv_obj_t *universal_bar_create(const _universal_obj_element *elm_bar, const _universal_bar_value_t *value, int theme)
{
	lv_obj_t *bar = lv_bar_create(elm_bar->parent, elm_bar->copy);
	screen_adaption_lvgl_obj_set_size(bar, elm_bar->width, elm_bar->height);
	lv_bar_set_range(bar, value->min, value->max);
	//lv_bar_set_start_value(bar, 50, true); //value->enable_anim
	lv_bar_set_value(bar, value->start_value, value->enable_anim);
	bar->user_data = elm_bar->user_data;

	Exec_Theme_Apply(bar, theme);

	return bar;
}

/*=========================
**********Label************
==========================*/
//LV_FONT_DECLARE(SC_MicroSoft_YaHei_R12);
lv_obj_t *universal_label_chinese_create(lv_obj_t *parent,const char *txt, universal_font_size_t font_size)
{
#if 0
	static lv_style_t style_label;
	lv_style_init(&style_label);
	lv_style_set_text_font(&style_label, LV_STATE_DEFAULT, &SC_MicroSoft_YaHei_R12);
	lv_style_set_text_color(&style_label, LV_STATE_DEFAULT, LV_COLOR_WHITE);
#endif
	lv_obj_t *label = lv_label_create(parent, NULL);
	lv_label_set_text(label, txt);

	if (font_size == universal_font_S)
		lv_theme_apply(label, (lv_theme_style_t)sigma_star_theme_label_chinese_small);
	else if (font_size == universal_font_Common)
		//lv_obj_add_style(label, LV_LABEL_PART_MAIN, &style_label);
		lv_theme_apply(label, (lv_theme_style_t)sigma_star_theme_label_chinese_normal);
	//else if (font_size == universal_font_L)
		//lv_theme_apply(label, (lv_theme_style_t));

	return label;
}

lv_obj_t *universal_label_create(_universal_obj_element *elm_label, const char *txt, int theme)
{
	lv_obj_t *label = lv_label_create(elm_label->parent, elm_label->copy);
	lv_label_set_text(label, txt);

	Exec_Theme_Apply(label, theme);

	return label;
}

/*=======================
***********LIST**********
=======================*/
lv_obj_t *universal_list_create(_universal_obj_element *elm_list, int theme)
{
	lv_obj_t *list = lv_list_create(elm_list->parent, elm_list->copy);
	screen_adaption_lvgl_obj_set_size(list, elm_list->width, elm_list->height);
	lv_obj_set_event_cb(list, elm_list->event_cb);
	list->user_data = elm_list->user_data;

	Exec_Theme_Apply(list, theme);

	return list;
}


/*========================
************Task********
==========================*/
typedef int(*int_fun_voidP)(void *);
typedef struct{
	uint8_t time_out_period;
	uint32_t *fun_addr;
	void *data;
}_task_timer_fun_t;
static void task_100ms_fun_callback(lv_task_t *task)
{
	if(is_null_ptr(task)) return;

	if(is_null_ptr(task->user_data)) return;

	_task_timer_fun_t *task_timer = (_task_timer_fun_t *)task->user_data;

	if(is_null_ptr(task_timer->fun_addr)) return;

	if(task_timer->time_out_period > 1){
		task_timer->time_out_period--;
		return;
		}
	else{
		int ret = ((int_fun_voidP)task_timer->fun_addr)(task_timer->data);

		//to do,ret
		general_free(task_timer);
		lv_task_del(task);
		}
	
	
}

uint32_t *universal_task_100ms_timer_create(uint8_t period_cnt,uint32_t *fun_addr,void *data)
{
	_task_timer_fun_t *task_timer = (_task_timer_fun_t *)general_malloc(sizeof(_task_timer_fun_t));
	task_timer->fun_addr = fun_addr;
	task_timer->data = data;
	task_timer->time_out_period = period_cnt;
	
	lv_task_t *task_t = lv_task_create(task_100ms_fun_callback,100,LV_TASK_PRIO_MID,task_timer);
	return (uint32_t *)task_timer;
}

/*========================
****UI Screen Adaption****
=========================*/
#define ADAPTION_BASE_WIDTH		800
#define ADAPTION_BASE_HEIGHT	480

#ifdef _SCREEN_SIZE_10_INCH
#define ADAPTION_WIDTH		1024
#define	ADAPTION_HEIGHT		600

#elif defined _SCREEN_SIZE_7_INCH
#define ADAPTION_WIDTH		800
#define	ADAPTION_HEIGHT		480
#endif // _SCREEN_SIZE_10_INCH

lv_coord_t screen_adaption_lvgl_hor_calculate(lv_coord_t hor)
{
	return hor * ADAPTION_WIDTH / ADAPTION_BASE_WIDTH;
}

lv_coord_t screen_adaption_lvgl_ver_calculate(lv_coord_t ver)
{
	return ver * ADAPTION_HEIGHT / ADAPTION_BASE_HEIGHT;
}

void screen_adaption_lvgl_obj_set_size(lv_obj_t *obj,lv_coord_t w,lv_coord_t h)
{
	lv_coord_t width = 0, height = 0;

	width = w * ADAPTION_WIDTH / ADAPTION_BASE_WIDTH;
	height = h * ADAPTION_HEIGHT / ADAPTION_BASE_HEIGHT;

	lv_obj_set_size(obj, width, height);
}

void screen_adaption_lvgl_obj_align(lv_obj_t *obj,lv_obj_t *base,lv_align_t align,lv_coord_t x_ofs,lv_coord_t y_ofs)
{
	x_ofs = x_ofs * ADAPTION_WIDTH / ADAPTION_BASE_WIDTH;
	y_ofs = y_ofs * ADAPTION_HEIGHT / ADAPTION_BASE_HEIGHT;

	lv_obj_align(obj, base, align, x_ofs, y_ofs);
}

void screen_adaption_lvgl_obj_set_pos(lv_obj_t *obj,lv_coord_t x, lv_coord_t y)
{
	x = x * ADAPTION_WIDTH / ADAPTION_BASE_WIDTH;
	y = y * ADAPTION_HEIGHT / ADAPTION_BASE_HEIGHT;

	lv_obj_set_pos(obj, x, y);
}

