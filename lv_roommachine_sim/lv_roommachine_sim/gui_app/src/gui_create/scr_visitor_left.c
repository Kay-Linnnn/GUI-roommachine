#include "scr_visitor_left.h"

#include "gui_app.h"

#include "message_mechanism.h"


#ifdef _VS_SIMULATOR
#pragma execution_character_set("utf-8")
#include "ex_need//temp_misc.h"
#else
#include "image_module.h"

#include <unistd.h> 
#endif

/*Calculation
*/
#define	MESSAGE_BG_HEIGHT	(75)
#define MESSAGE_PARENT_CONT_HEIGHT	MESSAGE_BG_HEIGHT * 5 + 1 //don't miss the "1", it's the border width of bottom of message bg

/*
***Local UI Global Data
*/
enum {
	ui_page_first,
	ui_page_second,
};
typedef uint8_t ui_page_index_t;

typedef struct {
	uint8_t temp;
	lv_obj_t *page[2];
	lv_obj_t *info_bg_box;
}_visitor_data_dynamic;

typedef struct {
	lv_obj_t *this_scr;	
}_visitor_data_memory;
_visitor_data_memory visitor_data_memory;

typedef struct {
	_visitor_data_dynamic *dynamic;
	_visitor_data_memory *memory;
}_visitor_data;
_visitor_data visitor_local;
#if 0
typedef struct visitor_info_t{
	uint16_t index;
	bool b_read;
	_ui_date_time_t time;
	struct visitor_info_t *next;
	struct visitor_info_t *pre;
}_visitor_info_t;
#endif

enum {
	info_node_index = 0,
	info_node_tag,
	info_node_date,
	info_node_time,
};
typedef uint8_t info_node_t;

//declare
static void visitor_ui_info_message_page_show(lv_obj_t *parent);

static const char *visitor_left_head_title_txt = "访客留言";

/*===========================
***********DATA LAYER********
============================*/
static void visitor_copy_json_file()
{
	mesg_mech_retrieve_list_data(message_mech_type_visitor);
}

static bool dynamic_is_null(_visitor_data_dynamic *p)
{
	if (p == NULL) return true;
	else return false;
}

static uint32_t* get_dynamic_pointer_32bits(_visitor_data *local_data)
{
	bool null_true = dynamic_is_null(local_data->dynamic);
	if (null_true) {
		local_data->dynamic = (_visitor_data_dynamic *)general_malloc(sizeof(_visitor_data_dynamic));
	}
	return (uint32_t *)local_data->dynamic;
}

static uint32_t *get_local_pointer(_data_pointer_type p_type)
{
	if (p_type == p_data_dynamic) {
		return get_dynamic_pointer_32bits(&visitor_local);
	}
	else if (p_type == p_data_memory) {
		return (uint32_t *)visitor_local.memory;
	}
	return NULL;
}

static void visitor_data_free()
{
	if (is_null_ptr(visitor_local.dynamic)) return;

	general_free(visitor_local.dynamic);
	visitor_local.dynamic = NULL;
}

void visitor_data_init()
{
	memset(&visitor_local,0,sizeof(_visitor_data));
	
	memset(&visitor_data_memory,0,sizeof(_visitor_data_memory));

	visitor_local.dynamic = NULL;
	visitor_local.memory = &visitor_data_memory;

	visitor_copy_json_file();
}

/*====================
*******UI Page *******
**********************/
/*
***@Para-> page_index
***@Para-> en: true to hide, false to unhide
*/
static void visitor_page_hide_set(ui_page_index_t page_index,bool en)
{
	_visitor_data_dynamic *p_dynamic = (_visitor_data_dynamic *)get_local_pointer(p_data_dynamic);
	
	if (p_dynamic->page[page_index] != NULL)
	{
		lv_obj_set_hidden(p_dynamic->page[page_index], en);
	}
}

static void visitor_page_del(ui_page_index_t page_index)
{
	_visitor_data_dynamic *p_dynamic = (_visitor_data_dynamic *)get_local_pointer(p_data_dynamic);

	if (p_dynamic->page[page_index] != NULL)
	{
		lv_obj_del(p_dynamic->page[page_index]);
		p_dynamic->page[page_index] = NULL;
	}
}

static void visitor_page_set(ui_page_index_t page_index, lv_obj_t *page)
{
	_visitor_data_dynamic *p_dynamic = (_visitor_data_dynamic *)get_local_pointer(p_data_dynamic);

	p_dynamic->page[page_index] = page;
}

/*==================================
*******Infomation Data Control******
===================================*/
/*Format:
***@ prefix: visitor_info_
***@ list: visitor_info_list_
*/
#define INFO_LIST_PAGE_ROW_MAX	5
enum {
	list_get_head = 0,
	list_get_tail,
	list_get_page_now_head,
};
typedef uint8_t list_get_node_t;

_mesg_mech_node *visitor_info_list_get_node(list_get_node_t type)
{
	_mesg_mech_list_t *visitor_mesg_list = message_box_list_get(message_mech_type_visitor);

	if (type == list_get_head)
	{
		return visitor_mesg_list->head;
	}
	else if (type == list_get_tail)
	{
		return visitor_mesg_list->tail;
	}
	else if (type == list_get_page_now_head)
	{
		return mesg_mech_list_get_page_reverse_head_node(visitor_mesg_list);
	}
	else
	{
		return NULL;
	}
}

static bool visitor_info_list_page_turn_note(mesg_display_mech_page_cmd_t page_cmd)
{
	_mesg_mech_list_t *visitor_mesg_list = message_box_list_get(message_mech_type_visitor);

	return mesg_mech_list_page_turn(page_cmd, visitor_mesg_list);
}

static void visitor_info_list_node_del(_mesg_mech_node *del_node)
{
	if (is_null_ptr(del_node))
		return;
	_mesg_mech_list_t *visitor_mesg_list = message_box_list_get(message_mech_type_visitor);
	mesg_mech_list_del_node(del_node, visitor_mesg_list, message_mech_type_visitor);
}

static void visitor_data_sim()
{
	uint8_t i = 0;
	int minute_t = 12;
	char dir[] = "VisitorMesg";
	char path[100] = { 0 };
	for (i = 0; i < 10; i++)
	{
		minute_t += i;
		sprintf(path, "%s/A_20210109_10%02d12_1.wav", dir, minute_t);
		mesg_mech_receive_path_come_in(path);
		sprintf(path, "%s/A_20210109_10%02d12_1.jpg", dir, minute_t);
		mesg_mech_receive_path_come_in(path);
		ui_printf_plus("path%s", path);
	}
}

/*===============================
***** Interior Interface API*****
================================*/
/*Format:
*** Prefix:visitor_interior_
*/
static void visitor_interior_change_scr(scr_change_type type, _ui_scr_id scr_id)
{
	gui_sys_screen_event_send(type, scr_id);
}

static void visitor_interior_display_image()
{
	int coor_x, coor_y, img_width, img_height;
	coor_x = 16;
	coor_y = 112;
	img_width = 478;//SCREEN_BASE_RESOLUTION_HOR / 4 * 3;
	img_height = 353;//300;

	const char img_test[] = "./img_dir/dog.jpg";
	ui_printf_plus("[visitor_interior_display_image]");
	#ifdef _VS_SIMULATOR
	_visitor_data_dynamic *p_dynamic = (_visitor_data_dynamic *)get_local_pointer(p_data_dynamic);

	_universal_obj_element elm_img;
	Uni_Element_init(elm_img);
	elm_img.parent = p_dynamic->page[ui_page_second];

	lv_obj_t *obj_image = universal_image_create(&elm_img, bin_path_img_guest_win_test, tencell_theme_NULL);
	screen_adaption_lvgl_obj_set_pos(obj_image, coor_x, coor_y);
	#else
	image_dispaly(img_test, coor_x, coor_y, img_width, img_height);
	#endif // _VS_SIMULATOR	
}

static void visitor_interiot_del_image()
{
	image_display_stop();
}


/*======================================
**********UI Layer**********************
=======================================*/

/***********************Block*****************************
********Second Page: Message & Picture Visitor leave******
*/
/*commonn repeat operation
*/
static lv_obj_t *visitor_ui_head_title(lv_obj_t *parent,const char *head_str, lv_event_cb_t event)
{
	return universal_misc_rm_head_title(parent, head_str, event);
}

static lv_obj_t *visitor_ui_return_create(lv_obj_t *parent,lv_event_cb_t event_cb)
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

/*Format:
***Prefix: visitor_message_
*/
static void visitor_message_refresh_info_page()
{
	_visitor_data_dynamic *p_dynamic = (_visitor_data_dynamic *)get_local_pointer(p_data_dynamic);
	lv_obj_clean(p_dynamic->info_bg_box);
	visitor_ui_info_message_page_show(p_dynamic->info_bg_box);	
}

LV_EVENT_CB_DECLARE(visitor_message_eventcb_del)
{
	if (e == LV_EVENT_CLICKED)
	{
		visitor_interiot_del_image();
		lv_obj_set_style_local_bg_opa(lv_scr_act(), LV_OBJMASK_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
		
		_mesg_mech_node *node = (_mesg_mech_node *)obj->user_data;
		visitor_info_list_node_del(node);
		visitor_page_del(ui_page_second);
		visitor_page_hide_set(ui_page_first, false);
		visitor_message_refresh_info_page();
	}
}

LV_EVENT_CB_DECLARE(visitor_message_eventcb_return)
{
	if (e == LV_EVENT_CLICKED)
	{
		visitor_interiot_del_image();
		lv_obj_set_style_local_bg_opa(lv_scr_act(), LV_OBJMASK_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
		visitor_page_del(ui_page_second);
		visitor_page_hide_set(ui_page_first, false);
		visitor_message_refresh_info_page();
	}
}

static lv_obj_t *visitor_message_ui_fun_btn_duplicate(lv_obj_t *parent, lv_event_cb_t event_cb, const void *default_img_src, const void *pressed_img_src)
{
	lv_obj_t *img_btn = lv_imgbtn_create(parent, NULL);
	lv_imgbtn_set_src(img_btn, LV_STATE_DEFAULT, default_img_src);
	lv_imgbtn_set_src(img_btn, LV_BTN_STATE_PRESSED, pressed_img_src);
	lv_obj_set_event_cb(img_btn, event_cb);

	return img_btn;
}

static lv_obj_t *visitor_message_ui_picture_display(lv_obj_t *parent)
{
	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = parent;
	elm_obj.width = SCREEN_BASE_RESOLUTION_HOR / 4 * 3;
	elm_obj.height = 300;
	
	return universal_obj_create(&elm_obj, tencell_theme_NULL);
}

static lv_obj_t *visitor_message_ui_audio_display(lv_obj_t *parent, int16_t start_value)
{
	_universal_obj_element elm_bar;
	Uni_Element_init(elm_bar);
	elm_bar.parent = parent;
	elm_bar.width = 275;
	elm_bar.height = 40;
	
	_universal_bar_value_t bar_v;
	memset(&bar_v, 0, sizeof(_universal_bar_value_t));
	bar_v.min = 1;
	bar_v.max = 100;
	bar_v.start_value = start_value;
	bar_v.enable_anim = false;
	
	lv_obj_t *bar = universal_bar_create(&elm_bar, &bar_v, tencell_theme_bar_volume);
	
	return bar;
}

static void visitor_message_ui_create(_mesg_mech_node *node)
{
	lv_obj_set_style_local_bg_opa(lv_scr_act(), LV_OBJMASK_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	mesg_mech_set_node_read(node, message_mech_type_visitor);

	lv_obj_t *page_bg = universal_misc_rm_bg_default_create();
	visitor_page_set(ui_page_second, page_bg);	

	visitor_ui_head_title(page_bg, visitor_left_head_title_txt, visitor_message_eventcb_return);

	uint16_t pic_x = 16, pic_y = 105;
	uint16_t audio_bg_x = 507, audio_bg_y = 209;
	uint16_t label_obj_space = 5;
	
	uint16_t btn_play_x = 507, btn_play_y = 325;
	uint16_t btn_hor_space = 28;

	visitor_interior_display_image();

	lv_obj_t *label_pic = universal_label_chinese_create(page_bg, "图片", universal_font_Common);
	screen_adaption_lvgl_obj_set_pos(label_pic, pic_x, pic_y - 30);

	int16_t audio_value = 50;
	lv_obj_t *audio_obj = visitor_message_ui_audio_display(page_bg, audio_value);
	screen_adaption_lvgl_obj_set_pos(audio_obj, audio_bg_x, audio_bg_y);

	lv_obj_t *label_audio = universal_label_chinese_create(page_bg, "语音", universal_font_Common);
	screen_adaption_lvgl_obj_align(label_audio, audio_obj, LV_ALIGN_OUT_TOP_LEFT, 0, -label_obj_space);

	lv_obj_t *btn_audio_play = visitor_message_ui_fun_btn_duplicate(page_bg, NULL, bin_path_img_message_play_icon_default,
																	bin_path_img_message_play_icon_pressed);//visitor_message_ui_fun_btn_duplicate(page_bg, "播放", LV_SYMBOL_PAUSE, NULL);
	screen_adaption_lvgl_obj_set_pos(btn_audio_play, btn_play_x, btn_play_y);

	lv_obj_t *btn_del = visitor_message_ui_fun_btn_duplicate(page_bg, visitor_message_eventcb_del, bin_path_img_message_del_icon_default,
																bin_path_img_message_del_icon_pressed);//visitor_message_ui_fun_btn_duplicate(page_bg, "删除", LV_SYMBOL_CLOSE, NULL);
	screen_adaption_lvgl_obj_align(btn_del, btn_audio_play, LV_ALIGN_OUT_RIGHT_TOP, btn_hor_space, 0);
	btn_del->user_data = node;
}

LV_EVENT_CB_DECLARE(visitor_eventcb_messge_enter)
{
	if (e == LV_EVENT_CLICKED)
	{
		visitor_page_hide_set(ui_page_first, true);
		visitor_message_ui_create((_mesg_mech_node *)obj->user_data);
	}
}

static lv_obj_t *visitor_ui_page_turn_btn_duplicate(lv_obj_t *parent,char *txt)
{
	_universal_obj_element elm_btn;
	Uni_Element_init(elm_btn);
	elm_btn.parent = parent;
	elm_btn.width = 60;
	elm_btn.height = 40;

	lv_obj_t *btn = universal_btn_label_create(&elm_btn, txt, tencell_theme_NULL);

	return btn;
}

static lv_obj_t *visitor_ui_simple_label(lv_obj_t *parent, const char *txt, bool is_chinese)
{
	lv_obj_t *label = NULL;
	if (!is_chinese) {
		label = lv_label_create(parent, NULL);
		lv_label_set_text(label, txt);
	}
	else {
		label = universal_label_chinese_create(parent, txt, universal_font_Common);
	}

	return label;
}

static void visitor_data_info_extract_time(const _mesg_mech_node *info_node, char *str_addr, info_node_t info_type)
{
	if (is_null_ptr(info_node)) return;
	
	switch (info_type)
	{
	case info_node_index:
		sprintf(str_addr,"%d",info_node->index);
		break;

	case info_node_tag:
		if (info_node->b_read)
			sprintf(str_addr, "已读");
		else
			sprintf(str_addr,"未读");
		break;

	case info_node_date:
		sprintf(str_addr, "%s", info_node->_time.date);
		break;

	case info_node_time:
		sprintf(str_addr, "%s", info_node->_time.time);
		break;

	default:
		break;
	}
	
}

static lv_obj_t *visitor_ui_info_message_show(lv_obj_t *parent, _mesg_mech_node *info_node)
{
	//char txt_index[10] = { 0 };
	char txt_tag[10] = { 0 };
	char txt_date[50] = { 0 };
	char txt_time[50] = { 0 };

	//visitor_data_info_extract_time(info_node, txt_index, info_node_index);
	visitor_data_info_extract_time(info_node, txt_tag, info_node_tag);
	visitor_data_info_extract_time(info_node, txt_date, info_node_date);
	visitor_data_info_extract_time(info_node, txt_time, info_node_time);

	static lv_style_t style_obj_visitor;
	lv_style_init(&style_obj_visitor);
	lv_style_set_bg_color(&style_obj_visitor, LV_STATE_DEFAULT, TENCELL_RM_COLOR_HIGHLIGHT_BLUE);
	lv_style_set_bg_opa(&style_obj_visitor, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_style_set_text_color(&style_obj_visitor, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_pad_left(&style_obj_visitor, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_obj_visitor, LV_STATE_DEFAULT, 0);
	lv_style_set_border_side(&style_obj_visitor, LV_STATE_DEFAULT, LV_BORDER_SIDE_BOTTOM);
	lv_style_set_border_width(&style_obj_visitor, LV_STATE_DEFAULT, 1);
	lv_style_set_border_color(&style_obj_visitor, LV_STATE_DEFAULT, TENCELL_RM_COLOR_WATER_BLUE);
	lv_style_set_border_opa(&style_obj_visitor, LV_STATE_DEFAULT, LV_OPA_30);

	lv_style_set_bg_opa(&style_obj_visitor, LV_STATE_PRESSED, LV_OPA_COVER);

	static lv_style_t style_label_read;
	lv_style_init(&style_label_read);
	lv_style_set_text_color(&style_label_read, LV_STATE_DEFAULT, TENCELL_RM_COLOR_WORD_BLUE);

	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = parent;
	elm_obj.width = lv_obj_get_width_fit(parent);
	elm_obj.height = MESSAGE_BG_HEIGHT;
	
	lv_obj_t *obj_bg = universal_obj_create(&elm_obj, tencell_theme_NULL);
	lv_obj_add_style(obj_bg, LV_OBJ_PART_MAIN, &style_obj_visitor);
	lv_obj_set_click(obj_bg, true);
	lv_obj_set_event_cb(obj_bg, visitor_eventcb_messge_enter);
	obj_bg->user_data = info_node;

	uint16_t pos_hor_shift_box = 20, pos_hor_shift_tag = 680;
	uint16_t pos_hor_space_date_beside_box = 20, pos_hor_space_time_beside_date = 20, pos_hor_space_arror_beside_tag = 30;

	_universal_obj_element elm_img;
	Uni_Element_init(elm_img);
	elm_img.parent = obj_bg;
	lv_obj_t *img_box = universal_image_create(&elm_img, bin_path_img_message_box_icon, tencell_theme_NULL);
	screen_adaption_lvgl_obj_align(img_box, NULL, LV_ALIGN_IN_LEFT_MID, pos_hor_shift_box, 0);

	lv_obj_t *label_date = visitor_ui_simple_label(obj_bg, txt_date, false); //date & time
	//screen_adaption_lvgl_obj_align(label_time, NULL, LV_ALIGN_IN_LEFT_MID, 400, 0);
	screen_adaption_lvgl_obj_align(label_date, img_box, LV_ALIGN_OUT_RIGHT_TOP, pos_hor_space_date_beside_box, 0);

	lv_obj_t *label_time = visitor_ui_simple_label(obj_bg, txt_time, false);
	screen_adaption_lvgl_obj_align(label_time, label_date, LV_ALIGN_OUT_RIGHT_TOP, pos_hor_space_time_beside_date, 0);
	
	lv_obj_t *label_tag = visitor_ui_simple_label(obj_bg, txt_tag, true);// read or not
	screen_adaption_lvgl_obj_align(label_tag, NULL, LV_ALIGN_IN_LEFT_MID, pos_hor_shift_tag, 0);
	lv_obj_add_style(label_tag, LV_LABEL_PART_MAIN, &style_label_read);

	lv_obj_t *label_arrow = visitor_ui_simple_label(obj_bg, LV_SYMBOL_RIGHT, false);
	screen_adaption_lvgl_obj_align(label_arrow, label_tag, LV_ALIGN_OUT_RIGHT_TOP, pos_hor_space_arror_beside_tag, 5);

	return obj_bg;
}

static void visitor_ui_info_message_page_show(lv_obj_t *parent)
{
	int i = 0;
	_mesg_mech_node *info_node = visitor_info_list_get_node(list_get_page_now_head);
	while (i < INFO_LIST_PAGE_ROW_MAX && info_node != NULL)
	{
		visitor_ui_info_message_show(parent, info_node);
		i++;
		info_node = info_node->pre;
	}
}

static lv_obj_t *visitor_ui_get_info_bg(lv_obj_t *parent)
{
	static lv_style_t style_cont;
	lv_style_init(&style_cont);
	lv_style_set_bg_color(&style_cont, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_bg_opa(&style_cont,LV_STATE_DEFAULT,LV_OPA_TRANSP);//LV_OPA_30
	lv_style_set_pad_top(&style_cont, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_left(&style_cont, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_right(&style_cont, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_bottom(&style_cont, LV_STATE_DEFAULT, 0);
	lv_style_set_pad_inner(&style_cont, LV_STATE_DEFAULT, 0);

	_universal_obj_element elm_cont;
	Uni_Element_init(elm_cont);
	elm_cont.parent = parent;
	elm_cont.width = SCREEN_BASE_RESOLUTION_HOR;
	elm_cont.height = MESSAGE_PARENT_CONT_HEIGHT;

	lv_obj_t *obj_cont = universal_cont_create(&elm_cont, LV_LAYOUT_COLUMN_MID, LV_FIT_NONE, tencell_theme_NULL);
	lv_obj_add_style(obj_cont, LV_CONT_PART_MAIN, &style_cont);
	
	return obj_cont;
}

LV_EVENT_CB_DECLARE(visitor_eventcb_next_page)
{
	if (e == LV_EVENT_CLICKED)
	{
		if (visitor_info_list_page_turn_note(mesg_display_mech_page_cmd_next))
		{
			lv_obj_clean((lv_obj_t *)obj->user_data);
			visitor_ui_info_message_page_show((lv_obj_t *)obj->user_data);
		}
	}
}

LV_EVENT_CB_DECLARE(visitor_eventcb_pre_page)
{
	if (e == LV_EVENT_CLICKED)
	{
		if (visitor_info_list_page_turn_note(mesg_display_mech_page_cmd_pre))
		{
			lv_obj_clean((lv_obj_t *)obj->user_data);
			visitor_ui_info_message_page_show((lv_obj_t *)obj->user_data);
		}
	}
}

LV_EVENT_CB_DECLARE(visitor_eventcb_return)
{
	if (e == LV_EVENT_CLICKED)
	{
		visitor_interior_change_scr(scr_chtype_return, scr_Null);
	}
}

static bool b_visior_sim_data = false;
static void visitor_ui_create(void)
{
	_visitor_data_memory *p_memory = (_visitor_data_memory *)get_local_pointer(p_data_memory);
	_visitor_data_dynamic *p_dynamic = (_visitor_data_dynamic *)get_local_pointer(p_data_dynamic);
	
	if (p_memory->this_scr == NULL) {
		lv_obj_t *obj_scr = lv_obj_create(NULL, NULL);
		p_memory->this_scr = obj_scr;
	}
	lv_scr_load(p_memory->this_scr);

	lv_obj_t *bg = universal_misc_rm_bg_default_create();
	visitor_page_set(ui_page_first, bg);

	visitor_ui_head_title(bg, visitor_left_head_title_txt, visitor_eventcb_return);

//message
	uint16_t message_bg_x = 0, message_bg_y = 65;
	lv_obj_t *info_bg = visitor_ui_get_info_bg(bg); //message bg
	screen_adaption_lvgl_obj_set_pos(info_bg, message_bg_x, message_bg_y);
	p_dynamic->info_bg_box = info_bg;

	if (!b_visior_sim_data)
	{
		b_visior_sim_data = true;
	}

	visitor_ui_info_message_page_show(info_bg);

//turn btn
	uint16_t btn_mid_space = 80, btn_height_space = 3;
	lv_obj_t *btn_pre = visitor_ui_page_turn_btn_duplicate(bg, LV_SYMBOL_UP);
	screen_adaption_lvgl_obj_align(btn_pre, info_bg, LV_ALIGN_OUT_BOTTOM_MID, -btn_mid_space, btn_height_space);
	lv_obj_set_event_cb(btn_pre, visitor_eventcb_pre_page);
	btn_pre->user_data = info_bg;

	lv_obj_t *btn_next = visitor_ui_page_turn_btn_duplicate(bg, LV_SYMBOL_DOWN);
	screen_adaption_lvgl_obj_align(btn_next,info_bg,LV_ALIGN_OUT_BOTTOM_MID, btn_mid_space, btn_height_space);
	lv_obj_set_event_cb(btn_next, visitor_eventcb_next_page);
	btn_next->user_data = info_bg;
}

/*=======================================
*** Exterior Interface with screen system
========================================*/
/*Format:
***@ prefix: visitor_ex_interface_
*/
void visitor_ex_interface_commu(int mesg, void *data)
{
	mesg_mech_receive_path_come_in((char* )data);
	general_free(data);
}

static void visitor_quit(void)
{
	visitor_data_free();
	lv_obj_clean(visitor_local.memory->this_scr);
}

bool visitor_ex_interface_register()
{
	bool is_true = gui_sys_screen_register(visitor_ui_create, 
											visitor_quit, 
											NULL, 
											NULL, 
											visitor_ex_interface_commu, 
											scr_visitor);
	if (is_true) return true;
	else return false;
}
