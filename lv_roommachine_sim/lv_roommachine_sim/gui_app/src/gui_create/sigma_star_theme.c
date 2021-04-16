/**
 * @file lv_theme_template.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
//#include "../../lvgl.h" /*To see all the widgets*/
#include "sigma_star_theme.h"
#include "gui_app.h"

#if LV_USE_THEME_SIAMA_STAR

#include "lvgl/src/lv_misc/lv_gc.h"

#if defined(LV_GC_INCLUDE)
    #include LV_GC_INCLUDE
#endif /* LV_ENABLE_GC */

#include"../../gui_app.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_style_t bg;
    lv_style_t btn;
    lv_style_t round;
    lv_style_t color;
    lv_style_t gray;
    lv_style_t tick_line;
    lv_style_t tight;
} theme_styles_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void theme_apply(lv_theme_t * th, lv_obj_t * obj, lv_theme_style_t name);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_theme_t theme;

static lv_theme_t theme;
static theme_styles_t * styles;

static lv_style_t style_test;
static lv_style_t style_cont_internet;
static lv_style_t style_cont_message;
static lv_style_t style_cont_date;
static lv_style_t style_cont_blockinfo;
static lv_style_t style_cont_doorCCTV;
static lv_style_t style_cont_VisitorMessage;
static lv_style_t style_cont_VoiceSetting;
static lv_style_t style_cont_VoiceSetting_part;
static lv_style_t style_cont_VoiceSetting_ring;
static lv_style_t style_btn_bg_transp;
static lv_style_t style_btn_border_thin;
static lv_style_t style_cont_home_middle;
static lv_style_t style_cont_head_title;

static lv_style_t style_label_chinese;
static lv_style_t style_cont_keyboard;
/*container style*/
static lv_style_t style_cont_bg_transp;
static lv_style_t style_cont_bg_blueish;
static lv_style_t style_cont_with_inner;
static lv_style_t style_cont_with_border;
static lv_style_t style_cont_kb_bg;
static lv_style_t style_cont_grid_set_widget;

/*obj style*/
static lv_style_t style_obj_bg_just_transp;
static lv_style_t style_obj_bg_transp;
static lv_style_t style_obj_bg;
static lv_style_t style_obj_bg_cover;
static lv_style_t style_obj_bg_kb_cont; //used as bg of keyboard

static lv_style_t style_obj_bg_cover_dark_blue;

/*button style*/
static lv_style_t style_btn_bg_red;
static lv_style_t style_tencell_btn_common;
static lv_style_t style_dark_bg_keep_pad;

/*textarea style*/
static lv_style_t style_ta_bg_blue;
static lv_style_t style_ta_dial;

/*buttonmmatrix style*/
static lv_style_t style_keyboard_background;
static lv_style_t style_keyboard_btn;
static lv_style_t style_btnmatrix_btn_pwd;

/*Label Style*/
static lv_style_t style_label_dynamic_chinese_16;
static lv_style_t style_label_cn_normal;
static lv_style_t style_label_cn_large;
static lv_style_t style_label_cn_small;

static lv_style_t style_label_cn_normal_gray;

/*Bar*/
static lv_style_t style_bar_bg_transp;
static lv_style_t style_bar_indic_volume;

/***************************
*  TENCELL STYLE DECLARE
***************************/
/*Bg Color*/
static lv_style_t style_bg_temp_visual;
static lv_style_t style_bg_transp;
static lv_style_t style_bg_anti_transp;
static lv_style_t style_bg_water_blue;//usually btn bg color
static lv_style_t style_bg_gray;
static lv_style_t style_bg_gray_kb_btn;
static lv_style_t style_bg_gray_kb_btn_pre;
static lv_style_t style_bg_black_gray;

static lv_style_t style_bg_dart_blue_pre;

/*Border*/

/*Pad*/
static lv_style_t style_pad_ta_pwd;
static lv_style_t style_pad_kb_pwd;
static lv_style_t style_pad_btn_sound_list;

/*Radius*/
static lv_style_t style_bg_radius_none;
static lv_style_t style_radius_circle;
/*Text*/
static lv_style_t style_text_font_normal;
static lv_style_t style_text_font_large;
static lv_style_t style_text_font_small;

static lv_style_t style_text_font_gray;



static bool inited;

/**********************
 *      MACROS
 **********************/

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void style_init_reset(lv_style_t * style);


static void basic_init(void)
{
    style_init_reset(&styles->bg);
    lv_style_set_bg_opa(&styles->bg, LV_STATE_DEFAULT, LV_OPA_COVER);
    lv_style_set_border_width(&styles->bg, LV_STATE_DEFAULT, 1);
    lv_style_set_border_width(&styles->bg, LV_STATE_FOCUSED, 2);
    lv_style_set_border_color(&styles->bg, LV_STATE_FOCUSED, theme.color_secondary);
    lv_style_set_border_color(&styles->bg, LV_STATE_EDITED, lv_color_darken(theme.color_secondary, LV_OPA_30));
    lv_style_set_line_width(&styles->bg, LV_STATE_DEFAULT, 1);
    lv_style_set_scale_end_line_width(&styles->bg, LV_STATE_DEFAULT, 1);
    lv_style_set_scale_end_color(&styles->bg, LV_STATE_DEFAULT, theme.color_primary);
    lv_style_set_text_color(&styles->bg, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_pad_left(&styles->bg, LV_STATE_DEFAULT, LV_DPI / 10);
    lv_style_set_pad_right(&styles->bg, LV_STATE_DEFAULT, LV_DPI / 10);
    lv_style_set_pad_top(&styles->bg, LV_STATE_DEFAULT, LV_DPI / 10);
    lv_style_set_pad_bottom(&styles->bg, LV_STATE_DEFAULT, LV_DPI / 10);
    lv_style_set_pad_inner(&styles->bg, LV_STATE_DEFAULT, LV_DPI / 10);

    style_init_reset(&styles->btn);
    lv_style_set_bg_color(&styles->btn, LV_STATE_PRESSED, lv_color_hex3(0xccc));
    lv_style_set_bg_color(&styles->btn, LV_STATE_CHECKED, theme.color_primary);
    lv_style_set_bg_color(&styles->btn, LV_STATE_CHECKED | LV_STATE_PRESSED, lv_color_darken(theme.color_primary,
                                                         LV_OPA_30));
    lv_style_set_bg_color(&styles->btn, LV_STATE_DISABLED, LV_COLOR_SILVER);
    lv_style_set_text_color(&styles->btn, LV_STATE_DISABLED, LV_COLOR_GRAY);
    lv_style_set_image_recolor(&styles->btn, LV_STATE_DISABLED, LV_COLOR_GRAY);

    style_init_reset(&styles->round);
    lv_style_set_radius(&styles->round, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);

    style_init_reset(&styles->color);
    lv_style_set_bg_color(&styles->color, LV_STATE_DEFAULT, theme.color_primary);
    lv_style_set_line_color(&styles->color, LV_STATE_DEFAULT, theme.color_primary);

    style_init_reset(&styles->gray);
    lv_style_set_bg_color(&styles->gray, LV_STATE_DEFAULT, LV_COLOR_SILVER);
    lv_style_set_line_color(&styles->gray, LV_STATE_DEFAULT, LV_COLOR_SILVER);
    lv_style_set_text_color(&styles->gray, LV_STATE_DEFAULT, LV_COLOR_GRAY);

    style_init_reset(&styles->tick_line);
    lv_style_set_line_width(&styles->tick_line, LV_STATE_DEFAULT, 5);
    lv_style_set_scale_end_line_width(&styles->tick_line, LV_STATE_DEFAULT, 5);
    lv_style_set_scale_end_color(&styles->tick_line, LV_STATE_DEFAULT, theme.color_primary);

    style_init_reset(&styles->tight);
    lv_style_set_pad_left(&styles->tight, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&styles->tight, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&styles->tight, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&styles->tight, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_inner(&styles->tight, LV_STATE_DEFAULT, 0);

	lv_style_init(&style_test);
	lv_style_set_text_color(&style_test,LV_STATE_DEFAULT,LV_COLOR_WHITE);
	//lv_style_set_transform_zoom(&style_test,LV_STATE_PRESSED,200);
	lv_style_set_bg_color(&style_test,LV_STATE_DEFAULT,LV_COLOR_PURPLE);
	lv_style_set_border_color(&style_test, LV_STATE_DEFAULT,LV_COLOR_BLACK);
//home widegt
	lv_style_init(&style_cont_internet); //internet
	lv_style_set_bg_color(&style_cont_internet,LV_STATE_DEFAULT, LV_COLOR_MAROON);
	lv_style_set_border_width(&style_cont_internet,LV_STATE_DEFAULT,0);

	lv_style_init(&style_cont_message);//message box
	lv_style_set_bg_color(&style_cont_message, LV_STATE_DEFAULT, LV_COLOR_BLUE);
	lv_style_set_border_width(&style_cont_message, LV_STATE_DEFAULT, 0);

	lv_style_init(&style_cont_date);//date
	lv_style_set_bg_color(&style_cont_date, LV_STATE_DEFAULT, LV_COLOR_BLUE);
	lv_style_set_border_width(&style_cont_date, LV_STATE_DEFAULT, 0);

	lv_style_init(&style_cont_blockinfo);//block information
	lv_style_set_bg_color(&style_cont_blockinfo, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
	lv_style_set_border_width(&style_cont_blockinfo, LV_STATE_DEFAULT, 0);

	lv_style_init(&style_cont_doorCCTV);//door CCTV
	lv_style_set_bg_color(&style_cont_doorCCTV, LV_STATE_DEFAULT, LV_COLOR_CYAN);
	lv_style_set_border_width(&style_cont_doorCCTV, LV_STATE_DEFAULT, 0);

	lv_style_init(&style_cont_VisitorMessage);//Visitor Message
	lv_style_set_bg_color(&style_cont_VisitorMessage, LV_STATE_DEFAULT, SIGMA_STAR_HOME_COLOE_PINK);
	lv_style_set_border_width(&style_cont_VisitorMessage, LV_STATE_DEFAULT, 0);

	lv_style_init(&style_cont_VoiceSetting); //home->voice setting
	lv_style_set_bg_color(&style_cont_VoiceSetting, LV_STATE_DEFAULT, SIGMA_STAR_HOME_SHITYELLOW);
	lv_style_set_border_width(&style_cont_VoiceSetting, LV_STATE_DEFAULT, 0);
	LV_FONT_DECLARE(SC_MicroSoft_YaHei_R16);
	lv_style_set_text_font(&style_cont_VoiceSetting,LV_STATE_DEFAULT,&SC_MicroSoft_YaHei_R16);
	lv_style_set_text_color(&style_cont_VoiceSetting, LV_STATE_DEFAULT, LV_COLOR_WHITE);
//change there
	lv_style_init(&style_cont_home_middle);
	lv_style_set_bg_color(&style_cont_home_middle, LV_STATE_DEFAULT,LV_COLOR_BLACK);
	lv_style_set_border_width(&style_cont_home_middle,LV_STATE_DEFAULT,0);
	lv_style_set_text_font(&style_cont_home_middle,LV_STATE_DEFAULT,&SC_MicroSoft_YaHei_R16);
	lv_style_set_text_color(&style_cont_home_middle,LV_STATE_DEFAULT,LV_COLOR_WHITE);

//voice setting
	lv_style_init(&style_cont_VoiceSetting_part);//VoinceSetting->cont
	lv_style_set_bg_color(&style_cont_VoiceSetting_part, LV_STATE_DEFAULT, LV_COLOR_CYAN);
	lv_style_set_bg_opa(&style_cont_VoiceSetting_part, LV_STATE_DEFAULT,LV_OPA_TRANSP);
	lv_style_set_pad_top(&style_cont_VoiceSetting_part, LV_STATE_DEFAULT,10);
	lv_style_set_border_width(&style_cont_VoiceSetting_part, LV_STATE_DEFAULT,0);

	lv_style_init(&style_cont_VoiceSetting_ring);
	lv_style_set_bg_color(&style_cont_VoiceSetting_ring,LV_STATE_DEFAULT,LV_COLOR_BLACK);
	lv_style_set_bg_opa(&style_cont_VoiceSetting_ring,LV_STATE_DEFAULT,LV_OPA_TRANSP);
	lv_style_set_pad_inner(&style_cont_VoiceSetting_ring,LV_STATE_DEFAULT,0);
	lv_style_set_pad_left(&style_cont_VoiceSetting_ring, LV_STATE_DEFAULT,1);
	lv_style_set_pad_top(&style_cont_VoiceSetting_ring,LV_STATE_DEFAULT,1);
	lv_style_set_pad_bottom(&style_cont_VoiceSetting_ring,LV_STATE_DEFAULT,1);
	lv_style_set_pad_right(&style_cont_VoiceSetting_ring,LV_STATE_DEFAULT,1);

	lv_style_init(&style_btn_bg_transp);
	lv_style_set_bg_color(&style_btn_bg_transp, LV_STATE_DEFAULT,LV_COLOR_GRAY);//
	lv_style_set_bg_opa(&style_btn_bg_transp, LV_STATE_DEFAULT,LV_OPA_TRANSP);
	lv_style_set_bg_opa(&style_btn_bg_transp,LV_STATE_CHECKED,LV_OPA_COVER);//LV_STATE_PRESSED|   LV_BTN_STATE_CHECKED_RELEASED
	lv_style_set_text_color(&style_btn_bg_transp,LV_STATE_DEFAULT,LV_COLOR_WHITE);
	lv_style_set_text_color(&style_btn_bg_transp, LV_STATE_CHECKED, LV_COLOR_BLACK);//LV_COLOR_BLACK

	lv_style_init(&style_btn_border_thin);
	lv_style_set_bg_opa(&style_btn_border_thin,LV_STATE_DEFAULT,LV_OPA_TRANSP);
	lv_style_set_text_color(&style_btn_border_thin,LV_STATE_DEFAULT,LV_COLOR_WHITE);
	lv_style_set_border_width(&style_btn_border_thin,LV_STATE_DEFAULT,2);
	lv_style_set_border_color(&style_btn_border_thin, LV_STATE_DEFAULT,LV_COLOR_WHITE);
	lv_style_set_text_font(&style_btn_border_thin,LV_STATE_DEFAULT,&SC_MicroSoft_YaHei_R16);

	lv_style_init(&style_cont_head_title);
	lv_style_set_text_color(&style_cont_head_title,LV_STATE_DEFAULT,LV_COLOR_WHITE);
	lv_style_set_bg_color(&style_cont_head_title,LV_STATE_DEFAULT,LV_COLOR_GRAY);
	lv_style_set_bg_opa(&style_cont_head_title,LV_STATE_DEFAULT,LV_OPA_60);
	lv_style_set_text_font(&style_cont_head_title,LV_STATE_DEFAULT,&SC_MicroSoft_YaHei_R16);
	lv_style_set_text_color(&style_cont_head_title,LV_STATE_DEFAULT,LV_COLOR_WHITE);
//keyboard for dial
	lv_style_init(&style_cont_keyboard);
	lv_style_set_pad_inner(&style_cont_keyboard, LV_STATE_DEFAULT, 5);
	lv_style_set_pad_left(&style_cont_keyboard, LV_STATE_DEFAULT, 20);
	lv_style_set_pad_right(&style_cont_keyboard, LV_STATE_DEFAULT, 0);
	lv_style_set_border_opa(&style_cont_keyboard, LV_STATE_DEFAULT, LV_OPA_TRANSP);

	lv_style_init(&style_keyboard_background);//keyborad bg
	//lv_style_set_bg_color(&style_keyboard_background,LV_STATE_DEFAULT,LV_COLOR_BLACK);
	lv_style_set_bg_opa(&style_keyboard_background, LV_STATE_DEFAULT,LV_OPA_TRANSP);
	lv_style_set_text_font(&style_keyboard_background,LV_STATE_DEFAULT,&SC_MicroSoft_YaHei_R16);
	lv_style_set_pad_inner(&style_keyboard_background, LV_STATE_DEFAULT, 5);
	lv_style_set_pad_right(&style_keyboard_background,LV_STATE_DEFAULT,0);
	lv_style_set_pad_left(&style_keyboard_background, LV_STATE_DEFAULT, 0);
	lv_style_set_border_opa(&style_keyboard_background,LV_STATE_DEFAULT,LV_OPA_TRANSP);
	lv_style_set_border_width(&style_keyboard_background, LV_STATE_DEFAULT, 0);

	lv_style_init(&style_keyboard_btn); //keyboard btn
	lv_style_set_bg_color(&style_keyboard_btn,LV_STATE_DEFAULT,LV_COLOR_GRAY);
	lv_style_set_text_color(&style_keyboard_btn,LV_STATE_DEFAULT,LV_COLOR_WHITE);

	lv_style_init(&style_label_chinese);
	lv_style_set_text_font(&style_label_chinese,LV_STATE_DEFAULT,&SC_MicroSoft_YaHei_R16);
	lv_style_set_text_color(&style_label_chinese,LV_STATE_DEFAULT,LV_COLOR_WHITE);

/*container style*/
	lv_style_init(&style_cont_bg_transp);
	lv_style_set_bg_opa(&style_cont_bg_transp, LV_STATE_DEFAULT, LV_OPA_TRANSP);

	lv_style_init(&style_cont_bg_blueish);
	lv_style_set_bg_opa(&style_cont_bg_blueish,LV_STATE_DEFAULT,LV_OPA_COVER);
	lv_style_set_bg_color(&style_cont_bg_blueish,LV_STATE_DEFAULT, SIGMA_STAR_HOME_GREENBLUE);
	lv_style_set_radius(&style_cont_bg_blueish,LV_STATE_DEFAULT,20);

	lv_style_init(&style_cont_with_inner);
	lv_style_set_pad_inner(&style_cont_with_inner,LV_STATE_DEFAULT,10);

	lv_style_init(&style_cont_with_border);
	lv_style_set_border_width(&style_cont_with_border,LV_STATE_DEFAULT,2);
	lv_style_set_border_color(&style_cont_with_border,LV_STATE_DEFAULT,LV_COLOR_SILVER);

	lv_style_init(&style_cont_grid_set_widget);
	lv_style_set_bg_opa(&style_cont_grid_set_widget, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_cont_grid_set_widget, LV_STATE_DEFAULT, TENCELL_RM_COLOR_NAVY_BLUE_SET_WIDGET);
	lv_style_set_radius(&style_cont_grid_set_widget, LV_STATE_DEFAULT, 20);
	lv_style_set_border_width(&style_cont_grid_set_widget, LV_STATE_DEFAULT, 1);
	lv_style_set_border_color(&style_cont_grid_set_widget, LV_STATE_DEFAULT, TENCELL_RM_COLOR_SET_WIDGET_BORDER);

/*obj style*/
	lv_style_init(&style_obj_bg_just_transp);
	lv_style_set_bg_opa(&style_obj_bg_just_transp, LV_STATE_DEFAULT, LV_OPA_TRANSP);

	lv_style_init(&style_obj_bg_transp);
	lv_style_set_bg_opa(&style_obj_bg_transp,LV_STATE_DEFAULT,LV_OPA_TRANSP);
	lv_style_set_border_width(&style_obj_bg_transp, LV_STATE_DEFAULT,3);
	lv_style_set_border_color(&style_obj_bg_transp, LV_STATE_DEFAULT, SIGMA_STAR_HOME_GREENBLUE);

	lv_style_init(&style_obj_bg);
	lv_style_set_bg_opa(&style_obj_bg,LV_STATE_DEFAULT,LV_OPA_60);
	lv_style_set_bg_color(&style_obj_bg,LV_STATE_DEFAULT,LV_COLOR_BLACK);

	lv_style_init(&style_obj_bg_cover);
	lv_style_set_bg_opa(&style_obj_bg_cover,LV_STATE_DEFAULT,LV_OPA_COVER);
	lv_style_set_bg_color(&style_obj_bg_cover, LV_STATE_DEFAULT, LV_COLOR_BLACK);

	lv_style_init(&style_obj_bg_cover_dark_blue);
	lv_style_set_bg_opa(&style_obj_bg_cover_dark_blue, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_obj_bg_cover_dark_blue, LV_STATE_DEFAULT, TENCELL_COLOR_DARK_BLUE);
	lv_style_set_radius(&style_obj_bg_cover_dark_blue, LV_STATE_DEFAULT, 10);
	lv_style_set_border_color(&style_obj_bg_cover_dark_blue, LV_STATE_DEFAULT, TENCELL_COLOR_LIGHTER_DARK_BLUE);
	lv_style_set_border_width(&style_obj_bg_cover_dark_blue, LV_STATE_DEFAULT, 1);

	lv_style_init(&style_obj_bg_kb_cont);
	lv_style_set_bg_opa(&style_obj_bg_kb_cont, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_obj_bg_kb_cont, LV_STATE_DEFAULT, TENCELL_RM_COLOR_BLACK_BLUE_SET_INFO_WIDGET);
	lv_style_set_radius(&style_obj_bg_kb_cont, LV_STATE_DEFAULT, 10);

/*button style*/
	lv_style_init(&style_btn_bg_red);
	lv_style_set_bg_color(&style_btn_bg_red,LV_STATE_DEFAULT,LV_COLOR_RED);
	lv_style_set_bg_opa(&style_btn_bg_red,LV_STATE_DEFAULT,LV_OPA_90);

	lv_style_init(&style_tencell_btn_common);
	lv_style_set_bg_opa(&style_tencell_btn_common, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_tencell_btn_common, LV_STATE_DEFAULT, TENCELL_RM_COLOR_WATER_BLUE);
	lv_style_set_radius(&style_tencell_btn_common, LV_STATE_DEFAULT, 10);

	lv_style_set_bg_color(&style_tencell_btn_common, LV_STATE_PRESSED, TENCELL_RM_COLOR_BLACK_BLUE);

/*textarea style*/
	lv_style_init(&style_ta_bg_blue);
	lv_style_set_bg_opa(&style_ta_bg_blue,LV_STATE_DEFAULT,LV_OPA_COVER);
	lv_style_set_bg_color(&style_ta_bg_blue,LV_STATE_DEFAULT, SIGMA_STAR_HOME_GREENBLUE);
	lv_style_set_pad_top(&style_ta_bg_blue,LV_STATE_DEFAULT, screen_adaption_lvgl_ver_calculate(20));//15

	lv_style_init(&style_ta_dial);
	//lv_style_copy(&style_ta_dial,&style_ta_bg_blue);
	lv_style_set_bg_opa(&style_ta_dial, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_pad_top(&style_ta_dial,LV_STATE_DEFAULT, screen_adaption_lvgl_ver_calculate(13));
	lv_style_set_pad_bottom(&style_ta_dial, LV_STATE_DEFAULT, screen_adaption_lvgl_ver_calculate(12));
	lv_style_set_pad_left(&style_ta_dial, LV_STATE_DEFAULT, screen_adaption_lvgl_hor_calculate(20));
	lv_style_set_bg_color(&style_ta_dial, LV_STATE_DEFAULT, TENCELL_COLOR_DARK_BLUE); //LV_COLOR_MAKE(0x1c, 0x5f, 0x6f)
	lv_style_set_border_color(&style_ta_dial, LV_STATE_DEFAULT, TENCELL_COLOR_LIGHTER_DARK_BLUE);
	lv_style_set_border_width(&style_ta_dial, LV_STATE_DEFAULT, 1);
	lv_style_set_radius(&style_ta_dial, LV_STATE_DEFAULT, 10);
	lv_style_set_text_color(&style_ta_dial, LV_STATE_DEFAULT, LV_COLOR_WHITE);

/*btnmatrix style*/
	lv_style_init(&style_btnmatrix_btn_pwd);
	lv_style_set_bg_opa(&style_btnmatrix_btn_pwd, LV_STATE_DEFAULT,LV_OPA_TRANSP);
	lv_style_set_text_color(&style_btnmatrix_btn_pwd,LV_STATE_DEFAULT,LV_COLOR_WHITE);
	lv_style_set_border_width(&style_btnmatrix_btn_pwd,LV_STATE_DEFAULT,2);
	lv_style_set_border_color(&style_btnmatrix_btn_pwd,LV_STATE_DEFAULT,LV_COLOR_WHITE);

	lv_style_set_border_color(&style_btnmatrix_btn_pwd,LV_BTN_STATE_PRESSED,LV_COLOR_BLUE);

/*Label Style*/
	//LV_FONT_DECLARE(font_test_gb);
	lv_style_init(&style_label_dynamic_chinese_16);
	lv_style_set_text_font(&style_label_dynamic_chinese_16, LV_STATE_DEFAULT, &THEME_CHINESE_FONT_DYNAMIC_NORMAL);
	lv_style_set_text_color(&style_label_dynamic_chinese_16, LV_STATE_DEFAULT, LV_COLOR_WHITE);

	LV_FONT_DECLARE(THEME_CHINESE_FONT_NORMAL);
	lv_style_init(&style_label_cn_normal);
	lv_style_set_text_font(&style_label_cn_normal, LV_STATE_DEFAULT, &THEME_CHINESE_FONT_NORMAL);
	lv_style_set_text_color(&style_label_cn_normal, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	
	LV_FONT_DECLARE(THEME_CHINESE_FONT_SMALL);
	lv_style_init(&style_label_cn_small);
	lv_style_copy(&style_label_cn_small, &style_label_cn_normal);
	lv_style_set_text_font(&style_label_cn_small, LV_STATE_DEFAULT, &THEME_CHINESE_FONT_SMALL);

	lv_style_init(&style_label_cn_normal_gray);
	lv_style_copy(&style_label_cn_normal_gray, &style_label_cn_normal);
	lv_style_set_text_color(&style_label_cn_normal_gray, LV_STATE_DEFAULT, LV_COLOR_GRAY);

	/*cont*/
	lv_style_init(&style_cont_kb_bg);
	lv_style_set_bg_opa(&style_cont_kb_bg, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_cont_kb_bg, LV_STATE_DEFAULT, LV_COLOR_OLIVE);
	#if 0
	lv_style_copy(&style_cont_kb_bg, &style_obj_bg_cover_dark_blue);
	lv_style_set_pad_top(&style_cont_kb_bg, LV_STATE_DEFAULT, 20);
	lv_style_set_pad_bottom(&style_cont_kb_bg, LV_STATE_DEFAULT, 20);
	lv_style_set_pad_inner(&style_cont_kb_bg, LV_STATE_DEFAULT, 10);
	#endif

	/*bar*/
	lv_style_init(&style_bar_bg_transp);
	lv_style_set_bg_opa(&style_bar_bg_transp, LV_STATE_DEFAULT, LV_OPA_COVER);//LV_OPA_TRANSP
	lv_style_set_bg_color(&style_bar_bg_transp, LV_STATE_DEFAULT, TENCELL_RM_COLOR_VOLUME_GRAY);//TENCELL_RM_COLOR_VOLUME_GRAY
	//lv_style_set_pad_right(&style_bar_bg_transp, LV_STATE_DEFAULT, 100);
	//lv_style_set_pad_left(&style_bar_bg_transp, LV_STATE_DEFAULT, 10);
	lv_style_set_radius(&style_bar_bg_transp, LV_STATE_DEFAULT, 20);

	lv_style_init(&style_bar_indic_volume);
	lv_style_set_bg_opa(&style_bar_indic_volume, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_bar_indic_volume, LV_STATE_DEFAULT, TENCELL_RM_COLOR_VOLUME_BLUE);//TENCELL_RM_COLOR_VOLUME_BLUE
	//lv_style_set_pad_right(&style_bar_indic_volume, LV_STATE_DEFAULT, 50);

	/**/
	lv_style_init(&style_dark_bg_keep_pad);
	lv_style_set_bg_opa(&style_dark_bg_keep_pad, LV_STATE_DEFAULT, LV_OPA_60);
	lv_style_set_pad_top(&style_dark_bg_keep_pad, LV_STATE_DEFAULT, 10);
	lv_style_set_pad_bottom(&style_dark_bg_keep_pad, LV_STATE_DEFAULT, 10);
	lv_style_set_border_side(&style_dark_bg_keep_pad, LV_STATE_DEFAULT, LV_BORDER_SIDE_BOTTOM);
	lv_style_set_border_width(&style_dark_bg_keep_pad, LV_STATE_DEFAULT, 1);
	lv_style_set_pad_left(&style_dark_bg_keep_pad, LV_STATE_DEFAULT, 10);

	//tencell
	/*Bg*/
	lv_style_init(&style_bg_transp);
	lv_style_set_bg_opa(&style_bg_transp, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_style_set_pad_all(&style_bg_transp, LV_STATE_DEFAULT, 0);
	lv_style_set_text_color(&style_bg_transp, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	
	lv_style_init(&style_bg_anti_transp);
	lv_style_copy(&style_bg_anti_transp, &style_bg_transp);
	lv_style_set_bg_opa(&style_bg_anti_transp, LV_STATE_DEFAULT, LV_OPA_COVER);

	lv_style_init(&style_bg_temp_visual);
	lv_style_copy(&style_bg_temp_visual, &style_bg_transp);
	lv_style_set_bg_opa(&style_bg_temp_visual, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_bg_temp_visual, LV_STATE_DEFAULT, LV_COLOR_TEAL);
	//basic style, opa/radius/text_color/text_font
	lv_style_init(&style_bg_gray);
	lv_style_set_bg_opa(&style_bg_gray, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_bg_gray, LV_STATE_DEFAULT, TENCELL_COLOR_DARK_BLUE);
	lv_style_set_radius(&style_bg_gray, LV_STATE_DEFAULT, 10);
	lv_style_set_text_color(&style_bg_gray, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_bg_gray, LV_STATE_DEFAULT, &THEME_CHINESE_FONT_NORMAL);

	lv_style_init(&style_bg_gray_kb_btn);
	lv_style_set_bg_opa(&style_bg_gray_kb_btn, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_bg_gray_kb_btn, LV_STATE_DEFAULT, TENCELL_RM_COLOR_GRAY_PWD_BTN_BG);
	lv_style_set_text_color(&style_bg_gray_kb_btn, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_radius(&style_bg_gray_kb_btn, LV_STATE_DEFAULT, 10);

	lv_style_init(&style_bg_gray_kb_btn_pre);
	//lv_style_copy(&style_bg_gray_kb_btn_pre, &style_bg_gray_kb_btn);
	lv_style_set_bg_opa(&style_bg_gray_kb_btn_pre, LV_STATE_PRESSED, LV_OPA_COVER);
	lv_style_set_bg_color(&style_bg_gray_kb_btn_pre, LV_STATE_PRESSED, LV_COLOR_MAKE(0x8e, 0x8e, 0x8e));

	lv_style_init(&style_bg_water_blue);
	lv_style_set_bg_opa(&style_bg_water_blue, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_bg_color(&style_bg_water_blue, LV_STATE_DEFAULT, TENCELL_RM_COLOR_WATER_BLUE);
	lv_style_set_text_color(&style_bg_water_blue, LV_STATE_DEFAULT, LV_COLOR_WHITE);

	lv_style_init(&style_bg_dart_blue_pre);
	lv_style_set_bg_opa(&style_bg_dart_blue_pre, LV_STATE_PRESSED, LV_OPA_COVER);
	lv_style_set_bg_color(&style_bg_dart_blue_pre, LV_STATE_PRESSED, TENCELL_RM_COLOR_BLACK_BLUE);

	lv_style_init(&style_bg_black_gray);
	lv_style_copy(&style_bg_black_gray ,&style_bg_gray);
	lv_style_set_bg_color(&style_bg_black_gray, LV_STATE_DEFAULT, TENCELL_RM_COLOR_GRAY_0x223337);
	//lv_style_set_radius(&style_bg_black_gray, LV_STATE_DEFAULT, 10);

	/*Pad*/
	lv_style_init(&style_pad_ta_pwd);
	lv_style_set_pad_top(&style_pad_ta_pwd, LV_STATE_DEFAULT, screen_adaption_lvgl_ver_calculate(12));
	lv_style_set_pad_bottom(&style_pad_ta_pwd, LV_STATE_DEFAULT, screen_adaption_lvgl_ver_calculate(12));
	lv_style_set_pad_left(&style_pad_ta_pwd, LV_STATE_DEFAULT, screen_adaption_lvgl_hor_calculate(20));

	lv_style_init(&style_pad_kb_pwd);
	lv_style_set_pad_inner(&style_pad_kb_pwd, LV_STATE_DEFAULT, 10);

	lv_style_init(&style_pad_btn_sound_list);
	lv_style_set_pad_all(&style_pad_btn_sound_list, LV_STATE_DEFAULT, 10);

	/*Text*/
	lv_style_init(&style_text_font_normal);
	lv_style_set_text_color(&style_text_font_normal, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&style_text_font_normal, LV_STATE_DEFAULT, &THEME_CHINESE_FONT_NORMAL);

	lv_style_init(&style_text_font_gray);
	lv_style_set_text_color(&style_text_font_gray, LV_STATE_DEFAULT, TENCELL_RM_COLOR_GRAY_PWD_WORD);

	/*Radius*/
	lv_style_init(&style_radius_circle);
	lv_style_set_radius(&style_radius_circle, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
	
}

static void arc_init(void)
{
#if LV_USE_ARC != 0

#endif
}

static void bar_init(void)
{
#if LV_USE_BAR

#endif
}

static void btn_init(void)
{
#if LV_USE_BTN != 0

#endif
}


static void btnmatrix_init(void)
{
#if LV_USE_BTNMATRIX

#endif
}


static void calendar_init(void)
{
#if LV_USE_CALENDAR

#endif
}

static void chart_init(void)
{
#if LV_USE_CHART

#endif
}


static void cpicker_init(void)
{
#if LV_USE_CPICKER

#endif
}

static void checkbox_init(void)
{
#if LV_USE_CHECKBOX != 0

#endif
}


static void cont_init(void)
{
#if LV_USE_CONT != 0

#endif
}


static void gauge_init(void)
{
#if LV_USE_GAUGE != 0

#endif
}

static void img_init(void)
{
#if LV_USE_IMG != 0

#endif
}

static void label_init(void)
{
#if LV_USE_LABEL != 0

#endif
}


static void linemeter_init(void)
{
#if LV_USE_LINEMETER != 0

#endif
}

static void line_init(void)
{
#if LV_USE_LINE != 0

#endif
}

static void led_init(void)
{
#if LV_USE_LED != 0

#endif
}

static void page_init(void)
{
#if LV_USE_PAGE

#endif
}

static void slider_init(void)
{
#if LV_USE_SLIDER != 0

#endif
}

static void switch_init(void)
{
#if LV_USE_SWITCH != 0

#endif
}


static void spinbox_init(void)
{
#if LV_USE_SPINBOX

#endif
}


static void spinner_init(void)
{
#if LV_USE_SPINNER != 0

#endif
}

static void keyboard_init(void)
{
#if LV_USE_KEYBOARD

#endif
}

static void msgbox_init(void)
{
#if LV_USE_MSGBOX

#endif
}

static void textarea_init(void)
{
#if LV_USE_TEXTAREA

#endif
}

static void list_init(void)
{
#if LV_USE_LIST != 0

#endif
}

static void ddlist_init(void)
{
#if LV_USE_DROPDOWN != 0

#endif
}

static void roller_init(void)
{
#if LV_USE_ROLLER != 0

#endif
}

static void tabview_init(void)
{
#if LV_USE_TABVIEW != 0

#endif
}

static void tileview_init(void)
{
#if LV_USE_TILEVIEW != 0
#endif
}

static void table_init(void)
{
#if LV_USE_TABLE != 0

#endif
}

static void win_init(void)
{
#if LV_USE_WIN != 0

#endif
}


/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the default
 * @param color_primary the primary color of the theme
 * @param color_secondary the secondary color for the theme
 * @param flags ORed flags starting with `LV_THEME_DEF_FLAG_...`
 * @param font_small pointer to a small font
 * @param font_normal pointer to a normal font
 * @param font_subtitle pointer to a large font
 * @param font_title pointer to a extra large font
 * @return a pointer to reference this theme later
 */
lv_theme_t * sigma_star_theme_init(lv_color_t color_primary, lv_color_t color_secondary, uint32_t flags,
                        const lv_font_t * font_small, const lv_font_t * font_normal, const lv_font_t * font_subtitle,
                        const lv_font_t * font_title)
{

    /* This trick is required only to avoid the garbage collection of
     * styles' data if LVGL is used in a binding (e.g. Micropython)
     * In a general case styles could be simple `static lv_style_t my style` variables or allocated directly into `styles`*/
    if(!inited) {
#if defined(LV_GC_INCLUDE)
        LV_GC_ROOT(_lv_theme_template_styles) = lv_mem_alloc(sizeof(theme_styles_t));
        styles = (theme_styles_t *)LV_GC_ROOT(_lv_theme_template_styles);
#else
        styles = lv_mem_alloc(sizeof(theme_styles_t));
#endif

    }

    theme.color_primary = color_primary;
    theme.color_secondary = color_secondary;
    theme.font_small = font_small;
    theme.font_normal = font_normal;
    theme.font_subtitle = font_subtitle;
    theme.font_title = font_title;
    theme.flags = flags;

    basic_init();
    cont_init();
    btn_init();
    label_init();
    bar_init();
    img_init();
    line_init();
    led_init();
    slider_init();
    switch_init();
    linemeter_init();
    gauge_init();
    arc_init();
    spinner_init();
    chart_init();
    calendar_init();
    cpicker_init();
    checkbox_init();
    btnmatrix_init();
    keyboard_init();
    msgbox_init();
    page_init();
    textarea_init();
    spinbox_init();
    list_init();
    ddlist_init();
    roller_init();
    tabview_init();
    tileview_init();
    table_init();
    win_init();

    theme.apply_xcb = NULL;
    theme.apply_cb = theme_apply;

    return &theme;
}


void theme_apply(lv_theme_t * th, lv_obj_t * obj, lv_theme_style_t name)
{
    LV_UNUSED(th);

    lv_style_list_t * list;
	uint8_t suit_value = (uint8_t)name;

    switch(suit_value) {
        case LV_THEME_NONE:
            break;

        case LV_THEME_SCR:
            list = lv_obj_get_style_list(obj, LV_OBJ_PART_MAIN);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->tight);
            break;
        case LV_THEME_OBJ:
            list = lv_obj_get_style_list(obj, LV_OBJ_PART_MAIN);
            _lv_style_list_add_style(list, &styles->bg);
            break;
#if LV_USE_CONT
        case LV_THEME_CONT:
            list = lv_obj_get_style_list(obj, LV_CONT_PART_MAIN);
            _lv_style_list_add_style(list, &styles->bg);
            break;
#endif

#if LV_USE_BTN
        case LV_THEME_BTN:
            list = lv_obj_get_style_list(obj, LV_BTN_PART_MAIN);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->btn);
            break;
#endif

#if LV_USE_BTNMATRIX
        case LV_THEME_BTNMATRIX:
            list = lv_obj_get_style_list(obj, LV_BTNMATRIX_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_BTNMATRIX_PART_BTN);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->btn);
            break;
#endif

#if LV_USE_KEYBOARD
        case LV_THEME_KEYBOARD:
            list = lv_obj_get_style_list(obj, LV_KEYBOARD_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_KEYBOARD_PART_BTN);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->btn);
            break;
#endif

#if LV_USE_BAR
        case LV_THEME_BAR:
            list = lv_obj_get_style_list(obj, LV_BAR_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->tight);

            list = lv_obj_get_style_list(obj, LV_BAR_PART_INDIC);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->color);
            break;
#endif

#if LV_USE_SWITCH
        case LV_THEME_SWITCH:
            list = lv_obj_get_style_list(obj, LV_SWITCH_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->tight);
            _lv_style_list_add_style(list, &styles->round);

            list = lv_obj_get_style_list(obj, LV_SWITCH_PART_INDIC);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->color);

            list = lv_obj_get_style_list(obj, LV_SWITCH_PART_KNOB);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->tight);
            _lv_style_list_add_style(list, &styles->round);
            break;
#endif

#if LV_USE_CANVAS
        case LV_THEME_CANVAS:
            list = lv_obj_get_style_list(obj, LV_CANVAS_PART_MAIN);
            break;
#endif

#if LV_USE_IMG
        case LV_THEME_IMAGE:
            list = lv_obj_get_style_list(obj, LV_IMG_PART_MAIN);
            break;
#endif

#if LV_USE_IMGBTN
        case LV_THEME_IMGBTN:
            list = lv_obj_get_style_list(obj, LV_IMG_PART_MAIN);
            break;
#endif

#if LV_USE_LABEL
        case LV_THEME_LABEL:
            list = lv_obj_get_style_list(obj, LV_LABEL_PART_MAIN);
            break;
#endif

#if LV_USE_LINE
        case LV_THEME_LINE:
            list = lv_obj_get_style_list(obj, LV_LABEL_PART_MAIN);
            break;
#endif

#if LV_USE_ARC
        case LV_THEME_ARC:
            list = lv_obj_get_style_list(obj, LV_ARC_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->tick_line);
            _lv_style_list_add_style(list, &styles->round);

            list = lv_obj_get_style_list(obj, LV_ARC_PART_INDIC);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->color);
            _lv_style_list_add_style(list, &styles->tick_line);
            break;
#endif

#if LV_USE_SPINNER
        case LV_THEME_SPINNER:
            list = lv_obj_get_style_list(obj, LV_SPINNER_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->tick_line);

            list = lv_obj_get_style_list(obj, LV_SPINNER_PART_INDIC);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->color);
            _lv_style_list_add_style(list, &styles->tick_line);
            break;
#endif

#if LV_USE_SLIDER
        case LV_THEME_SLIDER:
            list = lv_obj_get_style_list(obj, LV_SLIDER_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_SLIDER_PART_INDIC);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->color);

            list = lv_obj_get_style_list(obj, LV_SLIDER_PART_KNOB);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->round);
            break;
#endif

#if LV_USE_CHECKBOX
        case LV_THEME_CHECKBOX:
            list = lv_obj_get_style_list(obj, LV_CHECKBOX_PART_BG);

            list = lv_obj_get_style_list(obj, LV_CHECKBOX_PART_BULLET);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->btn);
            break;
#endif

#if LV_USE_MSGBOX
        case LV_THEME_MSGBOX:
            list = lv_obj_get_style_list(obj, LV_MSGBOX_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);
            break;

        case LV_THEME_MSGBOX_BTNS:
            list = lv_obj_get_style_list(obj, LV_MSGBOX_PART_BTN_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_MSGBOX_PART_BTN);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->btn);
            break;

#endif
#if LV_USE_LED
        case LV_THEME_LED:
            list = lv_obj_get_style_list(obj, LV_LED_PART_MAIN);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->color);
            _lv_style_list_add_style(list, &styles->round);
            break;
#endif
#if LV_USE_PAGE
        case LV_THEME_PAGE:
            list = lv_obj_get_style_list(obj, LV_PAGE_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->gray);

            list = lv_obj_get_style_list(obj, LV_PAGE_PART_SCROLLABLE);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_PAGE_PART_SCROLLBAR);
            _lv_style_list_add_style(list, &styles->bg);
            break;
#endif
#if LV_USE_TABVIEW
        case LV_THEME_TABVIEW:
            list = lv_obj_get_style_list(obj, LV_TABVIEW_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_TABVIEW_PART_BG_SCRLLABLE);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->color);

            list = lv_obj_get_style_list(obj, LV_TABVIEW_PART_TAB_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_TABVIEW_PART_INDIC);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->color);

            list = lv_obj_get_style_list(obj, LV_TABVIEW_PART_TAB_BTN);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->btn);
            break;

        case LV_THEME_TABVIEW_PAGE:
            list = lv_obj_get_style_list(obj, LV_PAGE_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->gray);

            list = lv_obj_get_style_list(obj, LV_PAGE_PART_SCROLLABLE);
            _lv_style_list_add_style(list, &styles->bg);

            break;
#endif

#if LV_USE_TILEVIEW
        case LV_THEME_TILEVIEW:
            list = lv_obj_get_style_list(obj, LV_TILEVIEW_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_TILEVIEW_PART_SCROLLBAR);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_TILEVIEW_PART_EDGE_FLASH);
            _lv_style_list_add_style(list, &styles->bg);
            break;
#endif


#if LV_USE_ROLLER
        case LV_THEME_ROLLER:
            list = lv_obj_get_style_list(obj, LV_ROLLER_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_ROLLER_PART_SELECTED);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->color);
            break;
#endif


#if LV_USE_OBJMASK
        case LV_THEME_OBJMASK:
            list = lv_obj_get_style_list(obj, LV_OBJMASK_PART_MAIN);
            break;
#endif

#if LV_USE_LIST
        case LV_THEME_LIST:
            list = lv_obj_get_style_list(obj, LV_LIST_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_LIST_PART_SCROLLABLE);

            list = lv_obj_get_style_list(obj, LV_LIST_PART_SCROLLBAR);
            _lv_style_list_add_style(list, &styles->bg);
            break;

        case LV_THEME_LIST_BTN:
            list = lv_obj_get_style_list(obj, LV_BTN_PART_MAIN);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->btn);
            break;
#endif

#if LV_USE_DROPDOWN
        case LV_THEME_DROPDOWN:
            list = lv_obj_get_style_list(obj, LV_DROPDOWN_PART_MAIN);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->btn);

            list = lv_obj_get_style_list(obj, LV_DROPDOWN_PART_LIST);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_DROPDOWN_PART_SCROLLBAR);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_DROPDOWN_PART_SELECTED);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->color);
            break;
#endif

#if LV_USE_CHART
        case LV_THEME_CHART:
            list = lv_obj_get_style_list(obj, LV_CHART_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_CHART_PART_SERIES_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_CHART_PART_SERIES);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->tight);
            break;
#endif
#if LV_USE_TABLE
        case LV_THEME_TABLE:
            list = lv_obj_get_style_list(obj, LV_TABLE_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_TABLE_PART_CELL1);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_TABLE_PART_CELL2);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_TABLE_PART_CELL3);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_TABLE_PART_CELL4);
            _lv_style_list_add_style(list, &styles->bg);
            break;
#endif

#if LV_USE_WIN
        case LV_THEME_WIN:
            list = lv_obj_get_style_list(obj, LV_WIN_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_WIN_PART_SCROLLBAR);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_WIN_PART_CONTENT_SCROLLABLE);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_WIN_PART_HEADER);
            _lv_style_list_add_style(list, &styles->bg);
            break;

        case LV_THEME_WIN_BTN:
            list = lv_obj_get_style_list(obj, LV_BTN_PART_MAIN);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->btn);
            break;
#endif

#if LV_USE_TEXTAREA
        case LV_THEME_TEXTAREA:
            list = lv_obj_get_style_list(obj, LV_TEXTAREA_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_TEXTAREA_PART_PLACEHOLDER);
            _lv_style_list_add_style(list, &styles->gray);

            list = lv_obj_get_style_list(obj, LV_TEXTAREA_PART_CURSOR);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->tight);

            list = lv_obj_get_style_list(obj, LV_TEXTAREA_PART_SCROLLBAR);
            _lv_style_list_add_style(list, &styles->bg);
            break;
#endif


#if LV_USE_SPINBOX
        case LV_THEME_SPINBOX:
            list = lv_obj_get_style_list(obj, LV_SPINBOX_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_SPINBOX_PART_CURSOR);
            _lv_style_list_add_style(list, &styles->bg);
            break;

        case LV_THEME_SPINBOX_BTN:
            list = lv_obj_get_style_list(obj, LV_BTN_PART_MAIN);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->btn);
            break;
#endif

#if LV_USE_CALENDAR
        case LV_THEME_CALENDAR:
            list = lv_obj_get_style_list(obj, LV_CALENDAR_PART_BG);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_CALENDAR_PART_DATE);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->btn);
            _lv_style_list_add_style(list, &styles->tight);

            list = lv_obj_get_style_list(obj, LV_CALENDAR_PART_HEADER);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_CALENDAR_PART_DAY_NAMES);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->tight);
            break;
#endif
#if LV_USE_CPICKER
        case LV_THEME_CPICKER:
            list = lv_obj_get_style_list(obj, LV_CPICKER_PART_MAIN);
            _lv_style_list_add_style(list, &styles->bg);

            list = lv_obj_get_style_list(obj, LV_CPICKER_PART_KNOB);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->round);
            break;
#endif

#if LV_USE_LINEMETER
        case LV_THEME_LINEMETER:
            list = lv_obj_get_style_list(obj, LV_LINEMETER_PART_MAIN);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->round);
            break;
#endif
#if LV_USE_GAUGE
        case LV_THEME_GAUGE:
            list = lv_obj_get_style_list(obj, LV_GAUGE_PART_MAIN);
            _lv_style_list_add_style(list, &styles->bg);
            _lv_style_list_add_style(list, &styles->round);

            list = lv_obj_get_style_list(obj, LV_GAUGE_PART_MAJOR);
            _lv_style_list_add_style(list, &styles->tick_line);

            list = lv_obj_get_style_list(obj, LV_GAUGE_PART_NEEDLE);
            _lv_style_list_add_style(list, &styles->bg);
            break;
#endif
		case sigma_star_theme_test:
			ui_printf("apply sigma_star_theme_test\r\n");
			//lv_obj_clean_style_list(obj,LV_CONT_PART_MAIN);
			list = lv_obj_get_style_list(obj,LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list,&style_test);
			break;

		case sigma_star_theme_home_internet:
			ui_printf("apply sigma_star_theme_home_internet\r\n");
			list = lv_obj_get_style_list(obj,LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list,&style_cont_internet);
			break;

		case sigma_star_theme_home_message:
			list = lv_obj_get_style_list(obj, LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list, &style_cont_message);
			break;

		case sigma_star_theme_home_date:
			list = lv_obj_get_style_list(obj, LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list, &style_cont_date);
			break;

		case sigma_star_theme_home_blockinfo:
			list = lv_obj_get_style_list(obj, LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list, &style_cont_blockinfo);
			break;

		case sigma_star_theme_home_doorCCTV:
			list = lv_obj_get_style_list(obj, LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list, &style_cont_doorCCTV);
			break;

		case sigma_star_theme_home_VisitorMessage:
			list = lv_obj_get_style_list(obj, LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list, &style_cont_VisitorMessage);
			break;

		case sigma_star_theme_home_VoiceSetting:
			list = lv_obj_get_style_list(obj, LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list, &style_cont_VoiceSetting);
			break;

		case sigma_star_theme_VoiceSetting_part:
			list = lv_obj_get_style_list(obj, LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list,&style_cont_VoiceSetting_part);
			break;

		case sigma_star_theme_VoiceSetting_ring:
			list = lv_obj_get_style_list(obj,LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list,&style_cont_VoiceSetting_ring);
			break;

		case sigma_star_theme_Btn_opa:
			lv_obj_clean_style_list(obj, LV_BTN_PART_MAIN);
			list = lv_obj_get_style_list(obj,LV_BTN_PART_MAIN);
			_lv_style_list_add_style(list,&style_btn_bg_transp);			
			break;

		case sigma_star_theme_Btn_home_return:
			lv_obj_clean_style_list(obj, LV_BTN_PART_MAIN);
			list = lv_obj_get_style_list(obj, LV_BTN_PART_MAIN);
			_lv_style_list_add_style(list,&style_btn_border_thin);
			break;

		case sigma_star_theme_home_middle_cont:
			//lv_obj_clean_style_list(obj,LV_CONT_PART_MAIN);
			list = lv_obj_get_style_list(obj,LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list,&style_cont_home_middle);
			break;

		case sigma_star_theme_title:
			lv_obj_clean_style_list(obj, LV_CONT_PART_MAIN);
			list = lv_obj_get_style_list(obj, LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list,&style_cont_head_title);
			break;

		case sigma_star_theme_keyboard:
			//lv_obj_clean_style_list(obj,LV_BTNMATRIX_PART_BG);
			list = lv_obj_get_style_list(obj, LV_BTNMATRIX_PART_BG);
			_lv_style_list_add_style(list,&style_keyboard_background);

			//lv_obj_clean_style_list(obj,LV_BTNMATRIX_PART_BTN);
			list = lv_obj_get_style_list(obj,LV_BTNMATRIX_PART_BTN);
			_lv_style_list_add_style(list,&style_keyboard_btn);
			break;

		case sigma_star_theme_dial_ta:
			lv_obj_clean_style_list(obj, LV_TEXTAREA_PART_BG);
			list = lv_obj_get_style_list(obj, LV_TEXTAREA_PART_BG);
			_lv_style_list_add_style(list, &style_ta_dial);

			list = lv_obj_get_style_list(obj, LV_TEXTAREA_PART_PLACEHOLDER);
			_lv_style_list_add_style(list, &style_label_cn_normal_gray);
			break;

		case sigma_star_theme_ta:
			lv_obj_clean_style_list(obj,LV_TEXTAREA_PART_BG);
			list = lv_obj_get_style_list(obj,LV_TEXTAREA_PART_BG);
			_lv_style_list_add_style(list,&style_ta_bg_blue);

			list = lv_obj_get_style_list(obj,LV_TEXTAREA_PART_PLACEHOLDER);
			_lv_style_list_add_style(list,&style_label_chinese);
			break;

		case sigma_star_theme_cont_keyboard:
			lv_obj_clean_style_list(obj,LV_CONT_PART_MAIN);
			list = lv_obj_get_style_list(obj,LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list,&style_cont_keyboard);
			break;

		case sigma_star_theme_label_chinese:
			lv_obj_clean_style_list(obj,LV_LABEL_PART_MAIN);
			list = lv_obj_get_style_list(obj,LV_LABEL_PART_MAIN);
			_lv_style_list_add_style(list,&style_label_chinese);
			break;

		case sigma_star_theme_unit_door_list:
			lv_obj_clean_style_list(obj,LV_CONT_PART_MAIN);
			list = lv_obj_get_style_list(obj,LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list,&style_cont_bg_transp);

			_lv_style_list_add_style(list,&style_label_chinese);
			break;

		case sigma_star_theme_unit_door_list_img:
			lv_obj_clean_style_list(obj, LV_CONT_PART_MAIN);
			list = lv_obj_get_style_list(obj, LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list, &style_cont_bg_blueish);
			break;

		case sigma_star_theme_square_transparent_bg:
			lv_obj_clean_style_list(obj,LV_OBJ_PART_MAIN);
			list = lv_obj_get_style_list(obj, LV_OBJ_PART_MAIN);
			_lv_style_list_add_style(list,&style_obj_bg_transp);
			break;

		case sigma_star_theme_hand_up:
			lv_obj_clean_style_list(obj,LV_BTN_PART_MAIN);
			list = lv_obj_get_style_list(obj,LV_BTN_PART_MAIN);
			_lv_style_list_add_style(list,&style_btn_bg_red);

			_lv_style_list_add_style(list, &style_label_chinese);
			break;

		case sigma_star_theme_default_bg:
			lv_obj_clean_style_list(obj,LV_OBJ_PART_MAIN);
			list = lv_obj_get_style_list(obj,LV_OBJ_PART_MAIN);
			_lv_style_list_add_style(list,&style_obj_bg);
			break;

		case sigma_star_theme_btnmatrix_pwd:
			lv_obj_clean_style_list(obj,LV_BTNMATRIX_PART_BG);
			lv_obj_clean_style_list(obj,LV_BTNMATRIX_PART_BTN);
			list = lv_obj_get_style_list(obj,LV_BTNMATRIX_PART_BG);
			_lv_style_list_add_style(list,&style_keyboard_background);

			list = lv_obj_get_style_list(obj, LV_BTNMATRIX_PART_BTN);
			_lv_style_list_add_style(list, &style_btnmatrix_btn_pwd);
			break;

		case sigma_star_theme_cont_bg_transparent:
			lv_obj_clean_style_list(obj,LV_CONT_PART_MAIN);
			
			list = lv_obj_get_style_list(obj,LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list,&style_cont_bg_transp);
			_lv_style_list_add_style(list,&style_cont_with_inner);
			break;

		case sigma_star_theme_sysset_widget_cont:
			lv_obj_clean_style_list(obj,LV_CONT_PART_MAIN);

			list = lv_obj_get_style_list(obj,LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list,&style_cont_grid_set_widget);
			break;

		case sigma_star_theme_obj_bg_cover_black:
			lv_obj_clean_style_list(obj, LV_OBJ_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_OBJ_PART_MAIN);
			_lv_style_list_add_style(list, &style_obj_bg_cover);
			break;

		case sigma_star_theme_syset_cont_trans_border:
			lv_obj_clean_style_list(obj, LV_CONT_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list, &style_cont_bg_transp);
			_lv_style_list_add_style(list, &style_cont_with_border);
			break;

		case sigma_star_theme_label_dynamic_chinese_mid:
			lv_obj_clean_style_list(obj, LV_LABEL_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_LABEL_PART_MAIN);
			_lv_style_list_add_style(list, &style_label_dynamic_chinese_16);			
			break;

		case sigma_star_theme_label_chinese_normal:
			lv_obj_clean_style_list(obj, LV_LABEL_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_LABEL_PART_MAIN);
			_lv_style_list_add_style(list, &style_label_cn_normal);
			break;

		case sigma_star_theme_label_chinese_small:
			lv_obj_clean_style_list(obj, LV_LABEL_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_LABEL_PART_MAIN);
			_lv_style_list_add_style(list, &style_label_cn_small);
			break;

		case tencell_theme_btn_chinese_normal:
			lv_obj_clean_style_list(obj, LV_BTN_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_BTN_PART_MAIN);
			_lv_style_list_add_style(list, &style_tencell_btn_common);

			_lv_style_list_add_style(list, &style_label_cn_normal);
			break;

		case tencell_theme_obj_bg:
			lv_obj_clean_style_list(obj, LV_OBJ_PART_MAIN);
			
			list = lv_obj_get_style_list(obj, LV_OBJ_PART_MAIN);
			_lv_style_list_add_style(list, &style_obj_bg_cover_dark_blue);
			
			break;

		case sigma_star_theme_dial_kb_cont_bg:
			lv_obj_clean_style_list(obj, LV_CONT_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_CONT_PART_MAIN);
			_lv_style_list_add_style(list, &style_cont_kb_bg);
			break;

		case tencell_theme_bar_volume:
			lv_obj_clean_style_list(obj, LV_BAR_PART_BG);
			lv_obj_clean_style_list(obj, LV_BAR_PART_INDIC);

			list = lv_obj_get_style_list(obj, LV_BAR_PART_BG);
			_lv_style_list_add_style(list, &style_bar_bg_transp);

			list = lv_obj_get_style_list(obj, LV_BAR_PART_INDIC);
			_lv_style_list_add_style(list, &style_bar_indic_volume);
			
			break;

		case tencell_theme_obj_transp:
			lv_obj_clean_style_list(obj, LV_OBJ_PART_MAIN);
			
			list = lv_obj_get_style_list(obj, LV_OBJ_PART_MAIN);
			_lv_style_list_add_style(list, &style_obj_bg_just_transp);
			break;

		case tencell_theme_obj_bg_cont_kb:
			lv_obj_clean_style_list(obj, LV_OBJ_PART_MAIN);
			
			list = lv_obj_get_style_list(obj, LV_OBJ_PART_MAIN);
			_lv_style_list_add_style(list, &style_obj_bg_kb_cont);
			break;

		case tencell_theme_btn_dark_bg:
			lv_obj_clean_style_list(obj, LV_BTN_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_BTN_PART_MAIN);
			_lv_style_list_add_style(list, &style_obj_bg_kb_cont);
			break;

		case tencell_theme_btn_list_screen_time:
			lv_obj_clean_style_list(obj, LV_BTN_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_BTN_PART_MAIN);
			_lv_style_list_add_style(list, &style_obj_bg_kb_cont);
			_lv_style_list_add_style(list, &style_dark_bg_keep_pad);
			_lv_style_list_add_style(list, &style_label_cn_normal);
			break;

		case tencell_theme_pwd_ta:
			lv_obj_clean_style_list(obj, LV_TEXTAREA_PART_BG);

			list = lv_obj_get_style_list(obj, LV_TEXTAREA_PART_BG);
			//_lv_style_list_add_style(list, &style_bg_gray);
			//_lv_style_list_add_style(list, &style_pad_ta_pwd);
			_lv_style_list_add_style(list, &style_ta_dial);
			

			list = lv_obj_get_style_list(obj, LV_TEXTAREA_PART_PLACEHOLDER);
			_lv_style_list_add_style(list, &style_text_font_normal);
			_lv_style_list_add_style(list, &style_text_font_gray);
			break;

		case tencell_theme_pwd_obj_bg:
			lv_obj_clean_style_list(obj, LV_OBJ_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_OBJ_PART_MAIN);
			_lv_style_list_add_style(list, &style_bg_transp);
			break;

		case tencell_theme_temp_obj:
			lv_obj_clean_style_list(obj, LV_OBJ_PART_MAIN);
			
			list = lv_obj_get_style_list(obj, LV_OBJ_PART_MAIN);
			_lv_style_list_add_style(list, &style_bg_temp_visual);
			break;

		case tencell_theme_pwd_kb:
			lv_obj_clean_style_list(obj, LV_BTNMATRIX_PART_BTN);
			lv_obj_clean_style_list(obj, LV_BTNMATRIX_PART_BG);
			
			list = lv_obj_get_style_list(obj, LV_BTNMATRIX_PART_BTN);
			//_lv_style_list_add_style(list, &)
			_lv_style_list_add_style(list, &style_bg_gray_kb_btn);
			_lv_style_list_add_style(list, &style_bg_gray_kb_btn_pre);

			list = lv_obj_get_style_list(obj, LV_BTNMATRIX_PART_BG);
			_lv_style_list_add_style(list, &style_bg_transp);//style_bg_transp
			_lv_style_list_add_style(list, &style_pad_kb_pwd);
			break;

		case tencell_theme_pwd_kb_obj_bg:
			lv_obj_clean_style_list(obj, LV_OBJ_PART_MAIN);
			
			list = lv_obj_get_style_list(obj, LV_OBJ_PART_MAIN);
			_lv_style_list_add_style(list, &style_bg_gray);
			break;

		case tencell_theme_pwd_btn_below_kb:
			lv_obj_clean_style_list(obj, LV_BTN_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_BTN_PART_MAIN);
			_lv_style_list_add_style(list, &style_bg_transp);
			_lv_style_list_add_style(list, &style_bg_gray_kb_btn_pre);
			break;

		case tencell_theme_sound_volume_btn:
			lv_obj_clean_style_list(obj, LV_BTN_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_BTN_PART_MAIN);
			_lv_style_list_add_style(list, &style_bg_water_blue);
			_lv_style_list_add_style(list, &style_bg_dart_blue_pre);
			_lv_style_list_add_style(list, &style_radius_circle);
			break;

		case tencell_theme_common_obj_dark_gray_bg:
			lv_obj_clean_style_list(obj, LV_OBJ_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_OBJ_PART_MAIN);
			_lv_style_list_add_style(list, &style_bg_black_gray);
			break;

		case tencell_theme_sound_arrow_btn:
			lv_obj_clean_style_list(obj, LV_BTN_PART_MAIN);
			
			list = lv_obj_get_style_list(obj, LV_BTN_PART_MAIN);
			_lv_style_list_add_style(list, &style_bg_black_gray);
			break;

		case tencell_theme_sound_list_btn:
			lv_obj_clean_style_list(obj, LV_BTN_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_BTN_PART_MAIN);
			_lv_style_list_add_style(list, &style_bg_transp);
			_lv_style_list_add_style(list, &style_pad_btn_sound_list);
			break;

		case tencell_theme_date_time_btn_below_roller:
			lv_obj_clean_style_list(obj, LV_BTN_PART_MAIN);

			list = lv_obj_get_style_list(obj, LV_BTN_PART_MAIN);
			_lv_style_list_add_style(list, &style_bg_transp);
			_lv_style_list_add_style(list, &style_pad_btn_sound_list);
			_lv_style_list_add_style(list, &style_text_font_normal);
			break;

        default:
            break;
    }

    lv_obj_refresh_style(obj,LV_OBJ_PART_ALL,LV_STYLE_PROP_ALL);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void style_init_reset(lv_style_t * style)
{
    if(inited) lv_style_reset(style);
    else lv_style_init(style);
}

#endif
