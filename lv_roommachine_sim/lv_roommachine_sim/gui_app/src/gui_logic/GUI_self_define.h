#ifndef _GUI_SELF_DEFINE_H
#define _GUI_SELF_DEFINE_H


#ifdef __cplusplus
  extern "C"{
#endif

#include "../../gui_app.h"


typedef enum ui_scr_id{
    scr_Null = 0,
    scr_Home = 1,
	scr_VoiceSetting,
	scr_Dial,
	scr_doorCCTV,
	scr_system_setting,
	scr_call,
	scr_security,
	scr_visitor,
	scr_block,
	scr_max,  //attention this one is important, see below "MaxScreen" defined legal screen id
}_ui_scr_id;

#define MaxScreen  scr_max

/*This one move to to global define*/
	typedef enum {
		p_data_dynamic = 1,
		p_data_memory,
	}_data_pointer_type;


	extern const char *bin_path_img_del_icon;
//guest
	extern const char *bin_path_img_message_box_icon;
	extern const char *bin_path_img_message_play_icon_default;
	extern const char *bin_path_img_message_play_icon_pressed;
	extern const char *bin_path_img_message_del_icon_default;
	extern const char *bin_path_img_message_del_icon_pressed;

	extern const char *bin_path_img_guest_win_test;

//setting
	extern const char *bin_path_img_setting_info;
	extern const char *bin_path_img_setting_screen_protect;
	extern const char *bin_path_img_setting_upgrade;
	extern const char *bin_path_img_setting_address;
	extern const char *bin_path_img_setting_date_time;
	extern const char *bin_path_img_setting_password;
	extern const char *bin_path_img_setting_ring;

#ifdef __cplusplus
  	}
#endif

#endif
  