#include "scr_home.h"
#include "sigma_star_theme.h"
#include "../../gui_app.h"
#include "scr_VoiceSetting.h"

#ifdef _VS_SIMULATOR
#pragma execution_character_set("utf-8")
#endif

typedef struct {
	uint8_t temp;
}_home_data_dynamic;

typedef struct {
	lv_obj_t *this_scr;
}_home_data_memory;
_home_data_memory home_data_memory;

typedef struct {
	_home_data_dynamic *dynamic;
	_home_data_memory  *memory;
}_scr_home_data;
_scr_home_data home_local;


static void home_change_scr(scr_change_type type,_ui_scr_id scr_id);


LV_IMG_DECLARE(icon_voice_48_42);
LV_IMG_DECLARE(icon_camera_65_82);
//image declare
LV_IMG_DECLARE(icon_setting_4848);
LV_IMG_DECLARE(icon_setting_8080);
LV_IMG_DECLARE(icon_phone_call_6464);
LV_IMG_DECLARE(icon_security_8080);
LV_IMG_DECLARE(icon_SOS_8080);
LV_IMG_DECLARE(icon_elevator_6464);
LV_IMG_DECLARE(icon_visitor_profile_2424);
LV_IMG_DECLARE(icon_voice_5050);
LV_IMG_DECLARE(icon_cctv_6464);
LV_IMG_DECLARE(icon_message_3232);

//10 inch delcare
LV_IMG_DECLARE(icon_home_dial_35_34);
LV_IMG_DECLARE(icon_home_visitor_37_33);
LV_IMG_DECLARE(icon_home_sos_37_37);
LV_IMG_DECLARE(icon_home_setting_36_36);
LV_IMG_DECLARE(icon_home_security_35_35);
LV_IMG_DECLARE(icon_home_cctv_44_40);
LV_IMG_DECLARE(icon_home_volume_37_33);
LV_IMG_DECLARE(icon_home_elv_down_41_48);
LV_IMG_DECLARE(icon_home_elv_up_41_48);
LV_IMG_DECLARE(icon_home_visitor_circle_28_28);

//icon img declare
#define HOME_ICON_SETTING		icon_home_setting_36_36
#define HOME_ICON_VOICE			icon_home_volume_37_33
#define HOME_ICON_DIAL			icon_home_dial_35_34
#define HOME_ICON_CCTV			icon_home_cctv_44_40
#define HOME_ICON_SECURITY		icon_home_security_35_35
#define HOME_ICON_SOS			icon_home_sos_37_37
#define HOME_ICON_ELEVATOR		icon_elevator_6464
#define HOME_ICON_VISITOR		icon_home_visitor_37_33
#define HOME_ICON_MESSAGE		icon_home_visitor_circle_28_28
#define HOME_ICON_ELEVATOR_DOWN	icon_home_elv_down_41_48
#define HOME_ICON_ELEVATOR_UP	icon_home_elv_up_41_48
//#define HOME_INFO_VISITOR_MSG	icon_home_visitor_circle_28_28

//color make declare
#define HOME_COLOR_DIAL_BG			LV_COLOR_MAKE(0x86, 0x73, 0xe6)
#define HOME_COLOR_BLOCK_INFO_BG	LV_COLOR_MAKE(0x16, 0x97, 0xc2)
#define HOME_COLOR_VISITOR_BG		LV_COLOR_MAKE(0x64, 0xd0, 0xf4)
#define HOME_COLOR_SOS_BG			LV_COLOR_MAKE(0x38, 0x71, 0xfc)
#define HOME_COLOR_SETTING_BG		LV_COLOR_MAKE(0x00, 0xae, 0x8b)
#define HOME_COLOR_SECURITY_BG		LV_COLOR_MAKE(0x1e, 0xbe, 0xc7)
#define HOME_COLOR_CCTV_BG			HOME_COLOR_SECURITY_BG
#define	HOME_COLOR_VOLUME_BG		LV_COLOR_MAKE(0x16, 0x97, 0xc2)
#define	HOME_COLOR_ELEVATOR_DOWN	LV_COLOR_MAKE(0x48, 0xb2, 0xf0)
#define	HOME_COLOR_ELEVATOR_UP		LV_COLOR_MAKE(0x00, 0xa1, 0x81)

/*UI frame size define
*/
#define frame_left_sapce 19  //the space between edge of frame and leftest widget,like visitor message and voice ...
#define frame_right_space 18//frame_left_sapce
#define widget_left_right_space 12
#define widget_up_down_space 9
#define frame_bottom_space 15

#define widget_txt_left_space 5 //leave some space the align_left_below txt with it's widget bg

//7 inch

#define widget_dial_width		299
#define widget_block_width		453
#define widget_below_width		143
#define widget_common_height	122

#define widget_pic_txt_space	10 //space between pic and text, it colud be used as left-right or top-bottom space
#define widget_icon_center_space	-19

#ifdef _SCREEN_SIZE_7_INCH
LV_FONT_DECLARE(SC_MicroSoft_YaHei_R16);
#define	HOME_FONT_NORMAL	SC_MicroSoft_YaHei_R16
#else
LV_FONT_DECLARE(SC_MicroSoft_YaHei_R18);
#define	HOME_FONT_NORMAL	SC_MicroSoft_YaHei_R18
#endif // _SCREEN_SIZE_7_INCH	

void scr_home_init()
{
	memset(&home_data_memory, 0, sizeof(_home_data_memory));
	home_data_memory.this_scr = NULL;

	home_local.dynamic = NULL;
	home_local.memory = &home_data_memory;
}

LV_EVENT_CB_DECLARE(widget_event_blockinfo)
{
	if (e == LV_EVENT_CLICKED) {
		home_change_scr(scr_chtype_change, scr_block);
	}
}

LV_EVENT_CB_DECLARE(scr_home_eventcb_cctv)
{
	if (e == LV_EVENT_CLICKED) {
		home_change_scr(scr_chtype_change,scr_doorCCTV);
	}
}

LV_EVENT_CB_DECLARE(scr_home_eventcb_voice)
{
	if (e == LV_EVENT_CLICKED) {
		home_change_scr(scr_chtype_change,scr_VoiceSetting);
	}
}

LV_EVENT_CB_DECLARE(scr_home_eventcb_dial)
{
	if (e == LV_EVENT_CLICKED) {
		home_change_scr(scr_chtype_change,scr_Dial);
	}
}

LV_EVENT_CB_DECLARE(scr_home_eventcb_setting)
{
	if (e == LV_EVENT_CLICKED) {
		home_change_scr(scr_chtype_change,scr_system_setting);
	}
}

LV_EVENT_CB_DECLARE(scr_home_eventcb_security)
{
	if (e == LV_EVENT_CLICKED) {
		home_change_scr(scr_chtype_change, scr_security);
	}
}

LV_EVENT_CB_DECLARE(scr_home_eventcb_visitor)
{
	if (e == LV_EVENT_CLICKED) {
		home_change_scr(scr_chtype_change, scr_visitor);
	}
}
/*=============================
********Screen Home UI ********
=============================*/
/*Format:
***Prefix: scr_home_
***UI draw: scr_home_ui_
***event_cb: scr_home_eventcb_
*/
static lv_obj_t *scr_home_ui_chinese_label(lv_obj_t *parent,char *txt)
{
	static lv_style_t style_label;
	lv_style_init(&style_label);
	lv_style_set_text_font(&style_label,LV_STATE_DEFAULT,&HOME_FONT_NORMAL);
	lv_style_set_text_color(&style_label,LV_STATE_DEFAULT, LV_COLOR_WHITE);//LV_COLOR_BLACK
	lv_style_set_text_opa(&style_label, LV_STATE_DEFAULT, LV_OPA_100);

	lv_obj_t *label = lv_label_create(parent, NULL);
	lv_label_set_text(label, txt);
	lv_obj_add_style(label,LV_LABEL_PART_MAIN,&style_label);

	return label;
}

//widget bg, it's a base obj, and set clickable
static lv_obj_t *scr_home_ui_widget_bg(lv_obj_t *parent,uint16_t width,uint16_t height, lv_style_t *style_color)
{
	static lv_style_t obj_style;
	lv_style_init(&obj_style);
	//lv_style_set_bg_color(&obj_style,LV_STATE_DEFAULT,color);
	//lv_style_set_radius(&obj_style, LV_STATE_DEFAULT, 15);
	//lv_style_set_text_color(&obj_style,LV_STATE_DEFAULT,LV_COLOR_WHITE);

	_universal_obj_element elm_obj;
	Uni_Element_init(elm_obj);
	elm_obj.parent = parent;
	elm_obj.width = width;
	elm_obj.height = height;

	lv_obj_t *bg_obj = universal_obj_create(&elm_obj, tencell_theme_NULL);
	lv_obj_add_style(bg_obj, LV_OBJ_PART_MAIN, &obj_style);
	lv_obj_add_style(bg_obj, LV_OBJ_PART_MAIN, style_color);
	lv_obj_set_click(bg_obj,true);

	return bg_obj;
}

static lv_obj_t *scr_home_ui_visitor_message()
{
	uint16_t width = widget_below_width, height = widget_common_height;
	lv_obj_t *bg = lv_scr_act();

	static lv_style_t style_bg;
	lv_style_init(&style_bg);
	lv_style_set_bg_color(&style_bg, LV_STATE_DEFAULT, HOME_COLOR_VISITOR_BG);

	lv_obj_t *widget = scr_home_ui_widget_bg(bg, width, height,&style_bg);
	lv_obj_set_event_cb(widget, scr_home_eventcb_visitor);	

	lv_obj_t *obj_img = lv_img_create(widget, NULL);
	lv_img_set_src(obj_img, &HOME_ICON_VISITOR);
	screen_adaption_lvgl_obj_align(obj_img, NULL, LV_ALIGN_CENTER, 0, widget_icon_center_space);

	lv_obj_t *label = scr_home_ui_chinese_label(widget, "访客留言");
	screen_adaption_lvgl_obj_align(label, obj_img, LV_ALIGN_OUT_BOTTOM_MID, 0, widget_pic_txt_space);

	lv_obj_t *obj_msg = lv_img_create(widget, NULL);
	lv_img_set_src(obj_msg, &HOME_ICON_MESSAGE);//HOME_ICON_MESSAGE
	//screen_adaption_lvgl_obj_align(obj_msg, label, LV_ALIGN_OUT_RIGHT_MID, 0, 0);
	screen_adaption_lvgl_obj_align(obj_msg, NULL, LV_ALIGN_IN_TOP_RIGHT, -5, 5);

	static lv_style_t style_text;
	lv_style_init(&style_text);
	lv_style_set_text_color(&style_text, LV_STATE_DEFAULT, LV_COLOR_WHITE);

	lv_obj_t *num_label = lv_label_create(obj_msg, NULL);
	screen_adaption_lvgl_obj_set_pos(num_label, 10, 0);
	lv_obj_add_style(num_label, LV_LABEL_PART_MAIN, &style_text);
	lv_label_set_text(num_label, "1");

	return widget;
}

static lv_obj_t *scr_home_ui_block_info()
{
	uint16_t width = widget_block_width, height = widget_common_height;
	lv_obj_t *bg = lv_scr_act();

	static lv_style_t style_bg;
	lv_style_init(&style_bg);
	lv_style_set_bg_color(&style_bg, LV_STATE_DEFAULT, HOME_COLOR_BLOCK_INFO_BG);

	lv_obj_t *widget = scr_home_ui_widget_bg(bg, width, height, &style_bg);
	lv_obj_set_event_cb(widget, widget_event_blockinfo);

	lv_obj_t *label = scr_home_ui_chinese_label(widget, "小区信息");
	screen_adaption_lvgl_obj_align(label,NULL,LV_ALIGN_IN_LEFT_MID,0,0);

	return widget;
}

static lv_obj_t *scr_home_ui_voice()
{
	uint16_t width = widget_below_width, height = widget_common_height;
	lv_obj_t *bg = lv_scr_act();

	static lv_style_t style_bg;
	lv_style_init(&style_bg);
	lv_style_set_bg_color(&style_bg, LV_STATE_DEFAULT, HOME_COLOR_VOLUME_BG);
	
	lv_obj_t *widget = scr_home_ui_widget_bg(bg, width, height, &style_bg);
	lv_obj_set_event_cb(widget,scr_home_eventcb_voice);	

	lv_obj_t *obj_img = lv_img_create(widget, NULL);
	lv_img_set_src(obj_img, &HOME_ICON_VOICE);
	screen_adaption_lvgl_obj_align(obj_img, NULL, LV_ALIGN_CENTER, 0, widget_icon_center_space);

	lv_obj_t *label = scr_home_ui_chinese_label(widget, "音量");
	screen_adaption_lvgl_obj_align(label, obj_img, LV_ALIGN_OUT_BOTTOM_MID, 0, widget_pic_txt_space);

	return widget;
}


static lv_obj_t *scr_home_ui_call_elevator_down()
{
	uint16_t width = widget_below_width, height = widget_common_height * 2 + widget_left_right_space;
	lv_obj_t *bg = lv_scr_act();

	static lv_style_t style_bg;
	lv_style_init(&style_bg);
	lv_style_set_bg_color(&style_bg, LV_STATE_DEFAULT, HOME_COLOR_ELEVATOR_DOWN);

	lv_obj_t *widget = scr_home_ui_widget_bg(bg, width, height, &style_bg);	

	lv_obj_t *obj_img = lv_img_create(widget, NULL);
	lv_img_set_src(obj_img, &HOME_ICON_ELEVATOR_DOWN);
	screen_adaption_lvgl_obj_align(obj_img, NULL, LV_ALIGN_CENTER, 0, widget_icon_center_space);

	lv_obj_t *label = scr_home_ui_chinese_label(widget, "下行");
	screen_adaption_lvgl_obj_align(label, obj_img, LV_ALIGN_OUT_BOTTOM_MID, 0, widget_pic_txt_space);

	return widget;
}

static lv_obj_t *scr_home_ui_call_elevator_up()
{
	uint16_t width = widget_below_width, height = widget_common_height * 2 + widget_left_right_space;
	lv_obj_t *bg = lv_scr_act();

	static lv_style_t style_bg;
	lv_style_init(&style_bg);
	lv_style_set_bg_color(&style_bg, LV_STATE_DEFAULT, HOME_COLOR_ELEVATOR_UP);

	lv_obj_t *widget = scr_home_ui_widget_bg(bg, width, height, &style_bg);

	lv_obj_t *obj_img = lv_img_create(widget, NULL);
	lv_img_set_src(obj_img, &HOME_ICON_ELEVATOR_UP);
	screen_adaption_lvgl_obj_align(obj_img, NULL, LV_ALIGN_CENTER, 0, widget_icon_center_space);

	lv_obj_t *label = scr_home_ui_chinese_label(widget, "上行");
	screen_adaption_lvgl_obj_align(label, obj_img, LV_ALIGN_OUT_BOTTOM_MID, 0, widget_pic_txt_space);

	return widget;
}

static lv_obj_t *scr_home_ui_dial()
{
	uint16_t width = widget_dial_width, height = widget_common_height;
	lv_obj_t *bg = lv_scr_act();

	static lv_style_t style_bg;
	lv_style_init(&style_bg);
	lv_style_set_bg_color(&style_bg, LV_STATE_DEFAULT, HOME_COLOR_DIAL_BG);

	lv_obj_t *widget = scr_home_ui_widget_bg(bg, width, height, &style_bg);
	lv_obj_set_event_cb(widget, scr_home_eventcb_dial);	

	lv_obj_t *obj_img = lv_img_create(widget, NULL);
	lv_img_set_src(obj_img, &HOME_ICON_DIAL);
	screen_adaption_lvgl_obj_align(obj_img, NULL, LV_ALIGN_CENTER, widget_icon_center_space, 0);

	lv_obj_t *label = scr_home_ui_chinese_label(widget, "拨号");
	screen_adaption_lvgl_obj_align(label, obj_img, LV_ALIGN_OUT_RIGHT_MID, widget_pic_txt_space, 0);

	return widget;
}

static lv_obj_t *scr_home_ui_cctv()
{
	uint16_t width = widget_below_width, height = widget_common_height;
	lv_obj_t *bg = lv_scr_act();

	static lv_style_t style_bg;
	lv_style_init(&style_bg);
	lv_style_set_bg_color(&style_bg, LV_STATE_DEFAULT, HOME_COLOR_CCTV_BG);

	lv_obj_t *widget = scr_home_ui_widget_bg(bg, width, height, &style_bg);
	lv_obj_set_event_cb(widget, scr_home_eventcb_cctv);	

	lv_obj_t *obj_img = lv_img_create(widget, NULL);
	lv_img_set_src(obj_img, &HOME_ICON_CCTV);
	screen_adaption_lvgl_obj_align(obj_img, NULL, LV_ALIGN_CENTER, 0, widget_icon_center_space);

	lv_obj_t *label = scr_home_ui_chinese_label(widget, "监控");
	screen_adaption_lvgl_obj_align(label, obj_img, LV_ALIGN_OUT_BOTTOM_MID, 0, widget_pic_txt_space);

	return widget;
}

static lv_obj_t *scr_home_ui_sos()
{
	uint16_t width = widget_below_width, height = widget_common_height;
	lv_obj_t *bg = lv_scr_act();

	static lv_style_t style_bg;
	lv_style_init(&style_bg);
	lv_style_set_bg_color(&style_bg, LV_STATE_DEFAULT, HOME_COLOR_SOS_BG);

	lv_obj_t *widget = scr_home_ui_widget_bg(bg, width, height, &style_bg);	

	lv_obj_t *obj_img = lv_img_create(widget, NULL);
	lv_img_set_src(obj_img, &HOME_ICON_SOS);
	screen_adaption_lvgl_obj_align(obj_img, NULL, LV_ALIGN_CENTER, 0, widget_icon_center_space);

	lv_obj_t *label = scr_home_ui_chinese_label(widget, "求救");
	screen_adaption_lvgl_obj_align(label, obj_img, LV_ALIGN_OUT_BOTTOM_MID, 0, widget_pic_txt_space);

	return widget;
}

static lv_obj_t *scr_home_ui_security()
{
	uint16_t width = widget_below_width, height = widget_common_height;
	lv_obj_t *bg = lv_scr_act();

	static lv_style_t style_bg;
	lv_style_init(&style_bg);
	lv_style_set_bg_color(&style_bg, LV_STATE_DEFAULT, HOME_COLOR_SECURITY_BG);

	lv_obj_t *widget = scr_home_ui_widget_bg(bg, width, height, &style_bg);
	lv_obj_set_event_cb(widget, scr_home_eventcb_security);	

	lv_obj_t *obj_img = lv_img_create(widget, NULL);
	lv_img_set_src(obj_img, &HOME_ICON_SECURITY);
	screen_adaption_lvgl_obj_align(obj_img, NULL, LV_ALIGN_CENTER, 0, widget_icon_center_space);

	lv_obj_t *label = scr_home_ui_chinese_label(widget, "安防");
	screen_adaption_lvgl_obj_align(label, obj_img, LV_ALIGN_OUT_BOTTOM_MID, 0, widget_pic_txt_space);

	return widget;
}

static lv_obj_t *scr_home_ui_setting()
{
	uint16_t width = widget_below_width, height = widget_common_height;
	lv_obj_t *bg = lv_scr_act();

	static lv_style_t style_bg;
	lv_style_init(&style_bg);
	lv_style_set_bg_color(&style_bg, LV_STATE_DEFAULT, HOME_COLOR_SETTING_BG);

	lv_obj_t *widget = scr_home_ui_widget_bg(bg, width, height, &style_bg);
	lv_obj_set_event_cb(widget, scr_home_eventcb_setting);	

	lv_obj_t *obj_img = lv_img_create(widget,NULL);
	lv_img_set_src(obj_img, &HOME_ICON_SETTING);
	screen_adaption_lvgl_obj_align(obj_img, NULL, LV_ALIGN_CENTER, 0, widget_icon_center_space);

	lv_obj_t *label = scr_home_ui_chinese_label(widget, "设置");
	screen_adaption_lvgl_obj_align(label, obj_img, LV_ALIGN_OUT_BOTTOM_MID, 0, widget_pic_txt_space);

	return widget;
}

static void scr_home_ui_title_create(lv_obj_t *parent)
{
	lv_obj_t *label_date = universal_label_chinese_create(parent, "2021-01-14", universal_font_Common);//lv_label_create(parent, NULL);
	screen_adaption_lvgl_obj_align(label_date, NULL, LV_ALIGN_IN_TOP_MID, 200, 30);

	lv_obj_t *label_time = universal_label_chinese_create(parent, "6:00 PM", universal_font_Common);//lv_label_create(parent, NULL);
	screen_adaption_lvgl_obj_align(label_time, label_date, LV_ALIGN_OUT_RIGHT_TOP, 20, 0);
}

static void scr_home_create(void)
{
	if (home_local.memory->this_scr == NULL) {
		lv_obj_t *obj_scr = lv_obj_create(NULL, NULL);
		home_local.memory->this_scr = obj_scr;
	}
	lv_scr_load(home_local.memory->this_scr);

//bg
	universal_misc_rm_pic_bg_create();

	lv_obj_t *bg = universal_misc_rm_bg_default_create();		
	
//title
	scr_home_ui_title_create(bg);
//widgets
	uint16_t widget_call_y = 79, widget_dial_x = frame_left_sapce;
	//dial
	lv_obj_t *widget_dial = scr_home_ui_dial();
	screen_adaption_lvgl_obj_set_pos(widget_dial, widget_dial_x, widget_call_y);

	//block info
	lv_obj_t *widget_block_info = scr_home_ui_block_info();
	screen_adaption_lvgl_obj_align(widget_block_info, widget_dial, LV_ALIGN_OUT_RIGHT_TOP, widget_left_right_space, 0);

	//visitor message
	lv_obj_t *widget_visitor_mes = scr_home_ui_visitor_message();
	screen_adaption_lvgl_obj_align(widget_visitor_mes, widget_dial, LV_ALIGN_OUT_BOTTOM_LEFT, 0, widget_up_down_space);

	//sos
	lv_obj_t *widget_sos = scr_home_ui_sos();
	screen_adaption_lvgl_obj_align(widget_sos, widget_visitor_mes, LV_ALIGN_OUT_RIGHT_TOP, widget_left_right_space, 0);

	//cctv
	lv_obj_t *widget_cctv = scr_home_ui_cctv();
	screen_adaption_lvgl_obj_align(widget_cctv, widget_sos, LV_ALIGN_OUT_RIGHT_TOP, widget_left_right_space, 0);

	//setting
	lv_obj_t *widget_setting = scr_home_ui_setting();
	screen_adaption_lvgl_obj_align(widget_setting, widget_visitor_mes, LV_ALIGN_OUT_BOTTOM_LEFT, 0, widget_up_down_space);

	//security
	lv_obj_t *widget_security = scr_home_ui_security();
	screen_adaption_lvgl_obj_align(widget_security, widget_setting, LV_ALIGN_OUT_RIGHT_TOP, widget_left_right_space, 0);

	//voice
	lv_obj_t *widget_voice = scr_home_ui_voice();
	screen_adaption_lvgl_obj_align(widget_voice, widget_security, LV_ALIGN_OUT_RIGHT_TOP, widget_left_right_space, 0);

	//call elevator down
	lv_obj_t *widget_elevator_down = scr_home_ui_call_elevator_down();
	screen_adaption_lvgl_obj_align(widget_elevator_down, widget_cctv, LV_ALIGN_OUT_RIGHT_TOP, widget_left_right_space, 0);

	//call elevator up
	lv_obj_t *widget_elevator_up = scr_home_ui_call_elevator_up();
	screen_adaption_lvgl_obj_align(widget_elevator_up, widget_elevator_down, LV_ALIGN_OUT_RIGHT_TOP, widget_left_right_space, 0);

}

void home_theme_init()
{
	lv_theme_t * th = sigma_star_theme_init(LV_COLOR_BLACK, LV_COLOR_BLACK,
		0, &lv_font_montserrat_14, &lv_font_montserrat_22,
		&lv_font_montserrat_28, &lv_font_montserrat_32);
	lv_theme_set_act(th);
}

/*===========================================
***   Exterior  Interface with screen system
=============================================*/
static void home_change_scr(scr_change_type type,_ui_scr_id scr_id)
{
  gui_sys_screen_event_send(type,scr_id);
}


#if 1
static void home_exit_fun(void)
{
  ui_printf("home exit fun\r\n");
  lv_obj_clean(home_local.memory->this_scr);
}

bool scr_home_register()
{
	bool is_true = gui_sys_screen_register(scr_home_create, home_exit_fun, NULL, NULL, NULL, scr_Home);
	if(is_true){
		//home_theme_init();
		//scr_home_init();
		return true;
	}
	else return false;
}
#endif

