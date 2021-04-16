#include "file_system_test.h"
#include "gui_app.h"
#include "lv_fs_if.h"

int fs_test_open_file()
{
	lv_fs_file_t f;
	lv_fs_res_t res;

	res = lv_fs_open(&f, "W:fs_test/test_fs.txt",LV_FS_MODE_RD);
	//res = lv_fs_open(&f,"W:fs_test/SC_MicroSoft_YaHei_R16.bin",LV_FS_MODE_RD);
	if(res != LV_FS_RES_OK){
		ui_printf_plus("open test_fs.txt fail");
		return -1;
		}

	uint32_t read_num;
	uint8_t buf[8];
	res = lv_fs_read(&f,buf,8,&read_num);
	if(res != LV_FS_RES_OK || read_num != 8){
		ui_printf_plus("read test_fs.txt fail");
		return -1;
		}

	int i;
	for(i = 0; i < 8; i++)
	{
		ui_printf_plus("buf[%d]:0x%x",i,buf[i]);
	}

	lv_fs_close(&f);

	return 0;
}
#if 0
lv_fs_file_t *font_file = NULL;

int fs_test_flash_open()
{
	lv_fs_file_t f;
	lv_fs_res_t res;
	
	res = lv_fs_open(&f, "W:fs_test/font_test_gb.bin", LV_FS_MODE_RD);
	if(res != LV_FS_RES_OK){
		ui_printf_plus("open font_test.bin fail");
		return -1;
		}

	font_file = &f;

}
#endif

extern const char *bin_path_font_normal;
int fs_test_flash_read(uint8_t *font_buf, int offset, int size)
{
	lv_fs_file_t f;
	lv_fs_res_t res;

	ui_printf_plus("font offset:%d,size:%d",offset, size);
	
	//res = lv_fs_open(&f, "W:fs_test/font_test_gb.bin", LV_FS_MODE_RD);
	res = lv_fs_open(&f, bin_path_font_normal, LV_FS_MODE_RD);
	if(res != LV_FS_RES_OK){
		ui_printf_plus("open font_test.bin fail");
		return -1;
		}

	if(lv_fs_seek(&f,offset) != LV_FS_RES_OK)
	{
		ui_printf_plus("seek position fail");
		return -1;
	}

	if(lv_fs_read(&f,font_buf,size,NULL) != LV_FS_RES_OK)
	{
		ui_printf_plus("read buf fail");
		return -1;
	}
	lv_fs_close(&f);
	return 0;
}

int fs_test_run_font()
{
#if 0
	lv_font_t *my_font;
	my_font = lv_font_load("W:fs_test/font_test.bin");//W:fs_test/SC_MicroSoft_YaHei_R16.bin
	if(my_font == NULL){
		ui_printf_plus("load font fail");
		return -1;
		}

	lv_fs_file_t f;
	lv_fs_res_t res;
	uint32_t file_size = 0;
	res = lv_fs_open(&f, "W:fs_test/font_test_16.bin", LV_FS_MODE_RD);
	if(res != LV_FS_RES_OK){
		ui_printf_plus("open font_test.bin fail");
		return -1;
		}
	if(lv_fs_size(&f, &file_size) != LV_FS_RES_OK){
		ui_printf_plus("file_size read fail");
		return -1;
		}
	ui_printf_plus("file size:%d", file_size);
	uint8_t *font_buffer = (uint8_t *)general_malloc(sizeof(uint8_t)*file_size);
	if(font_buffer == NULL){
		ui_printf_plus("malloc size for font_buffer fail");
		return -1;
		}
	lv_fs_read(&f,font_buffer,file_size,NULL);
	lv_fs_close(&f);
	
	ui_printf_plus("font_buffer:%d",(uint32_t)font_buffer);
	LV_USER_FONT_ADDR = (uint32_t)font_buffer;
	ui_printf_plus("fs_test_font, font_addr:%d",LV_USER_FONT_ADDR);
#endif	
	LV_FONT_DECLARE(font_test_gb);
	lv_font_t *my_font = &font_test_gb;
	
	static lv_style_t btn_style;
	lv_style_init(&btn_style);
	lv_style_set_bg_color(&btn_style,LV_STATE_DEFAULT,LV_COLOR_RED);
	lv_style_set_text_color(&btn_style,LV_STATE_DEFAULT,LV_COLOR_BLACK);
	//lv_style_set_text_font(&btn_style,LV_STATE_DEFAULT,my_font);

	static lv_style_t label_style;
	lv_style_init(&label_style);
	lv_style_set_text_color(&label_style,LV_STATE_DEFAULT,LV_COLOR_BLACK);
	lv_style_set_text_font(&label_style,LV_STATE_DEFAULT,my_font);
	
	lv_obj_t *btn = lv_btn_create(lv_scr_act(),NULL);
	lv_obj_set_size(btn,400,60);
	lv_obj_align(btn,NULL,LV_ALIGN_CENTER,0,0);
	lv_obj_add_style(btn,LV_BTN_PART_MAIN,&btn_style);

	lv_obj_t *label = lv_label_create(btn,NULL);
	lv_label_set_text(label,"随便测试一下字库");
	lv_obj_add_style(label,LV_LABEL_PART_MAIN,&label_style);
#if 0
	lv_obj_t *cont = lv_cont_create(lv_scr_act(),NULL);
	lv_cont_set_fit(cont,LV_FIT_TIGHT);
	lv_obj_set_pos(cont,10,100);
	lv_obj_set_style_local_bg_color(cont,LV_CONT_PART_MAIN,LV_STATE_DEFAULT,LV_COLOR_GREEN);
	
	lv_obj_t *label_1 = lv_label_create(cont,NULL);
	lv_label_set_text(label_1,"随便测试一下字库吝拎玲菱零龄铃伶羚凌灵陵岭领");
	lv_obj_add_style(label_1,LV_LABEL_PART_MAIN,&label_style);
	lv_obj_align(label_1,NULL,LV_ALIGN_IN_TOP_LEFT,0,0);
#endif
	//general_free(font_buffer);
	

	return 0;
}

