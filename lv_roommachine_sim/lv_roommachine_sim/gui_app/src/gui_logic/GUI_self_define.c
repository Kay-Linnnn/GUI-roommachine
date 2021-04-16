#include "GUI_self_define.h"

/*Image Bin Path
*/
#ifdef _VS_SIMULATOR
const char *bin_path_img_del_icon = "W:gui_app/src/pic_font_file/photo/del_icon_21x21.bin";
//guest
const char *bin_path_img_message_box_icon = "W:gui_app/src/pic_font_file/photo/message_box_icon.bin";
const char *bin_path_img_message_play_icon_default = "W:gui_app/src/pic_font_file/photo/message_play_rel.bin";
const char *bin_path_img_message_play_icon_pressed = "W:gui_app/src/pic_font_file/photo/message_play_pre.bin";
const char *bin_path_img_message_del_icon_default = "W:gui_app/src/pic_font_file/photo/message_del_rel.bin";
const char *bin_path_img_message_del_icon_pressed = "W:gui_app/src/pic_font_file/photo/message_del_pre.bin";

const char *bin_path_img_guest_win_test = "W:gui_app/src/pic_font_file/photo/nana_478x353.bin";

//setting
const char *bin_path_img_setting_info = "W:gui_app/src/pic_font_file/photo/setting/system_info_icon.bin";
const char *bin_path_img_setting_screen_protect = "W:gui_app/src/pic_font_file/photo/setting/system_scr_protect_icon.bin";
const char *bin_path_img_setting_upgrade = "W:gui_app/src/pic_font_file/photo/setting/system_upgrade_icon.bin";
const char *bin_path_img_setting_address = "W:gui_app/src/pic_font_file/photo/setting/system_address_icon.bin";
const char *bin_path_img_setting_date_time = "W:gui_app/src/pic_font_file/photo/setting/system_date_time_icon.bin";
const char *bin_path_img_setting_password = "W:gui_app/src/pic_font_file/photo/setting/system_password_icon.bin";
const char *bin_path_img_setting_ring = "W:gui_app/src/pic_font_file/photo/setting/system_ring_icon.bin";
#else
const char *bin_path_img_del_icon = "W:";
//guest
const char *bin_path_img_message_box_icon = "W:";
const char *bin_path_img_message_play_icon_default = "W:";
const char *bin_path_img_message_play_icon_pressed = "W:";
const char *bin_path_img_message_del_icon_default = "W:";
const char *bin_path_img_message_del_icon_pressed = "W:";

//setting
const char *bin_path_img_setting_info = "W:";
const char *bin_path_img_setting_screen_protect = "W:";
const char *bin_path_img_setting_upgrade = "W:";
const char *bin_path_img_setting_address = "W:";
const char *bin_path_img_setting_date_time = "W:";
const char *bin_path_img_setting_password = "W:";
const char *bin_path_img_setting_ring = "W:";
#endif // _VS_SIMULATOR

/*Font Bin Path*/
#ifdef _VS_SIMULATOR
const char *bin_path_font_normal = "W:gui_app/src/pic_font_file/font/font_test_gb.bin";
#else
const char *bin_path_font_normal = "W:font_dir/font_test_gb.bin";
#endif

