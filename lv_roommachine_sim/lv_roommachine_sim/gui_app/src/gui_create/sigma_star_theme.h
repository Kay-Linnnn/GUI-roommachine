/**
 * @file lv_theme_template.h
 *
 */

#ifndef _SIAMA_STAR_THEME_H
#define _SIAMA_STAR_THEME_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
//#include "../lv_conf_internal.h"
#include "scr_home.h"

#define LV_USE_THEME_SIAMA_STAR 1
#if LV_USE_THEME_SIAMA_STAR

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
#ifdef _VS_SIMULATOR
#define general_malloc(size_t)  LV_MEM_CUSTOM_ALLOC(size_t)
#define general_free(p)         LV_MEM_CUSTOM_FREE(p)
#endif

#define SIGMA_STAR_HOME_COLOE_PINK   lv_color_hex(0xff99cc)
#define SIGMA_STAR_HOME_SHITYELLOW   lv_color_hex(0xffcc66)
#define SIGMA_STAR_HOME_LIGHTBULE    lv_color_hex(0x66cccc)
#define SIGMA_STAR_HOME_LITGHPURPLE  lv_color_hex(0xcc6699)
#define SIGMA_STAR_HOME_GREENBLUE    lv_color_hex(0x66ccff)
#define SIGMA_STAT_HOME_DIRTYORANGE  lv_color_hex(0xff9933)
#define SIGMA_STAR_HOEM_BLUE1        lv_color_hex(0x3399ff)
#define SIGMA_STAR_HOME_PURPLEISSH   lv_color_hex(0x9933cc)

/*tencell ui decalre*/
//bg
#define	TENCELL_COLOR_DARK_BLUE			LV_COLOR_MAKE(0x22, 0x33, 0x37) //usually used as bg, like bg of dial->ta or dial->kb
#define TENCELL_COLOR_LIGHTER_DARK_BLUE	LV_COLOR_MAKE(0x2a, 0x40, 0x45)	//could be used as border color of dark blue, which more lighter visually
//first with guest
#define	TENCELL_RM_COLOR_WATER_BLUE		LV_COLOR_MAKE(0x00, 0xaa, 0x88) //usually used as btn color
#define TENCELL_RM_COLOR_HIGHLIGHT_BLUE	LV_COLOR_MAKE(0x00, 0x4e, 0x46) //usuall used as highlight, like visiotr message,
#define	TENCELL_RM_COLOR_WORD_BLUE		LV_COLOR_MAKE(0x00, 0xb7, 0x92)  //usually used as visitor message "read/not" word
#define TENCELL_RM_COLOR_BLACK_BLUE		LV_COLOR_MAKE(0x2c, 0x42, 0x47) //used as pressed state color of btn

#define	TENCELL_RM_COLOR_KB_BTN_GRAY	LV_COLOR_MAKE(0xe9, 0xe9, 0xe9)
#define TENCELL_RM_COLOR_VOLUME_BLUE	LV_COLOR_MAKE(0x14, 0xad, 0x8f)
#define TENCELL_RM_COLOR_VOLUME_GRAY	LV_COLOR_MAKE(0x22, 0x33, 0x37)

//first with setting
/*setting->menu*/
#define TENCELL_RM_COLOR_NAVY_BLUE_SET_WIDGET	LV_COLOR_MAKE(0x09, 0x35, 0x38)  //usually used as widget cont of seetting
#define TENCELL_RM_COLOR_SET_WIDGET_BORDER		LV_COLOR_MAKE(0x00, 0x50, 0x40)  //used as border color of widget cont of setting

/*setting->info*/
#define TENCELL_RM_COLOR_BLACK_BLUE_SET_INFO_WIDGET		LV_COLOR_MAKE(0x22, 0x33, 0x37)  //used as data obj bg color of setting->info
#define TENCELL_RM_COLOR_GARY_WHITE_WORD		LV_COLOR_MAKE(0xad, 0xae, 0xae)  //used as color of label text in settng->info,data block

/*setting->pwd*/
#define TENCELL_RM_COLOR_GRAY_PWD_WORD		LV_COLOR_MAKE(0x6b, 0x6b, 0x6b) //used as color of txt color of placeholder of ta in setting->pwd
#define TENCELL_RM_COLOR_GRAY_PWD_BTN_BG	LV_COLOR_MAKE(0xa8, 0xa8, 0xa8)

#define	TENCELL_RM_COLOR_GRAY_0x223337		LV_COLOR_MAKE(0x22, 0x33, 0x37)
#define	TENCELL_RM_COLOR_CYAN_0x00604d		LV_COLOR_MAKE(0x00, 0x60, 0x4d)

#define tencell_theme_NULL	-1
	typedef enum {
		sigma_star_theme_test = 100,
		sigma_star_theme_home_internet,
		sigma_star_theme_home_message,
		sigma_star_theme_home_date,
		sigma_star_theme_home_blockinfo,
		sigma_star_theme_home_doorCCTV,
		sigma_star_theme_home_VisitorMessage,
		sigma_star_theme_home_VoiceSetting,
		sigma_star_theme_VoiceSetting_part, //voice setting->cont:voice,bell,microphone
		sigma_star_theme_VoiceSetting_ring,//voice setting->cont:ring
		sigma_star_theme_Btn_opa,  //switch between opa_transp and opa_cover for voice setting->ring
		sigma_star_theme_Btn_home_return,
		sigma_star_theme_home_middle_cont,
		sigma_star_theme_title,
		sigma_star_theme_keyboard,
		sigma_star_theme_ta,
		sigma_star_theme_cont_keyboard,
		sigma_star_theme_label_chinese,
		sigma_star_theme_unit_door_list,
		sigma_star_theme_unit_door_list_img,
		sigma_star_theme_square_transparent_bg,
		sigma_star_theme_hand_up,
		sigma_star_theme_default_bg,
		//dial
		sigma_star_theme_dial_ta,
		//system setting
		sigma_star_theme_btnmatrix_pwd,
		sigma_star_theme_sysset_widget_cont,
		sigma_star_theme_syset_cont_trans_border,
		//call cont
		sigma_star_theme_cont_bg_transparent,
		//background
		sigma_star_theme_obj_bg_cover_black,
		sigma_star_theme_dial_kb_cont_bg,
		//label font
		sigma_star_theme_label_dynamic_chinese_mid,
		sigma_star_theme_label_chinese_normal,
		sigma_star_theme_label_chinese_small,
		//btn
		tencell_theme_btn_chinese_normal,
		tencell_theme_btn_dark_bg,
		tencell_theme_btn_list_screen_time,
		//obj
		tencell_theme_obj_bg,
		tencell_theme_obj_transp,
		tencell_theme_obj_bg_cont_kb,//like kb bg or info->data->block bg
		//bar
		tencell_theme_bar_volume,
		/*>>>>>TENCELL NEW STYLE<<<<<<<<<*/
		//temp
		tencell_theme_temp_obj,
		//common
		tencell_theme_common_obj_dark_gray_bg,
		//pwd
		tencell_theme_pwd_ta,
		tencell_theme_pwd_obj_bg,//obj holds widgets
		tencell_theme_pwd_kb_obj_bg, //obj holds all kb widgets 
		//tencell_theme_pwd_kb_bg,
		//tencell_theme_pwd_kb_btn,
		tencell_theme_pwd_kb,
		tencell_theme_pwd_btn_below_kb,
		/*System->Sound*/
		tencell_theme_sound_volume_btn,
		tencell_theme_sound_arrow_btn,
		tencell_theme_sound_list_btn,
		/*System->date_time*/
		tencell_theme_date_time_btn_below_roller,
}_sigma_star_theme_t;


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
                                    const lv_font_t * font_title);
/**********************
 *      MACROS
 **********************/

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_THEME_TEMPLATE_H*/
