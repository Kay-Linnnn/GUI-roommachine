#include "scr_block.h"

#include "gui_app.h"

#include "message_mechanism.h"


#include "universal_api_v7.h"


#ifdef _VS_SIMULATOR
#pragma execution_character_set("utf-8")

#else
#include "image_module.h"
#endif

typedef struct {
	uint8_t temp;
}_block_data_dynamic;

typedef struct {
	lv_obj_t *this_scr;
}_block_data_memory;
_block_data_memory block_data_memory;

typedef struct {
	_block_data_dynamic *dynamic;
	_block_data_memory *memory;
}_block_data;
_block_data block_local;

/*===========================
***********DATA LAYER********
============================*/

static bool dynamic_is_null(_block_data_dynamic *p)
{
	if (p == NULL) return true;
	else return false;
}

static uint32_t* get_dynamic_pointer_32bits(_block_data *local_data)
{
	bool null_true = dynamic_is_null(local_data->dynamic);
	if (null_true) {
		local_data->dynamic = (_block_data_dynamic *)general_malloc(sizeof(_block_data_dynamic));
	}
	return (uint32_t *)local_data->dynamic;
}

static uint32_t *get_local_pointer(_data_pointer_type p_type)
{
	if (p_type == p_data_dynamic) {
		return get_dynamic_pointer_32bits(&block_local);
	}
	else if (p_type == p_data_memory) {
		return (uint32_t *)block_local.memory;
	}
	return NULL;
}

static void block_data_free()
{
	if (is_null_ptr(block_local.dynamic)) return;

	general_free(block_local.dynamic);
	block_local.dynamic = NULL;
}

void block_data_init()
{
	memset(&block_local, 0, sizeof(_block_data));

	memset(&block_data_memory, 0, sizeof(_block_data_memory));

	block_local.dynamic = NULL;
	block_local.memory = &block_data_memory;
}


/*===============================
***** Interior Interface API*****
================================*/
/*Format:
***prefix: block_interior_
*/
static void block_interior_change_scr(scr_change_type type, _ui_scr_id scr_id)
{
	gui_sys_screen_event_send(type, scr_id);
}

/*===================================
************UI Layer*****************
===================================*/
static lv_obj_t *block_ui_head_title(lv_obj_t *parent,const char *head_str, lv_event_cb_t event)
{
	return universal_misc_rm_head_title(parent, head_str, event);
}

static lv_obj_t *block_ui_return_create(lv_obj_t *parent, lv_event_cb_t event_cb)
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


static void block_event_return(lv_obj_t *obj, lv_event_t e)
{
	if (e == LV_EVENT_CLICKED)
	{
		block_interior_change_scr(scr_chtype_return, scr_Null);
	}
}

static void block_temp_textarea(lv_obj_t *parent)
{
	lv_obj_t *label_title = lv_label_create(parent, NULL);
	lv_theme_apply(label_title, (lv_theme_style_t)sigma_star_theme_label_dynamic_chinese_mid);
	lv_label_set_text(label_title, "做好小区防疫\"守门人\",筑牢小区防疫墙");
	screen_adaption_lvgl_obj_align(label_title, NULL, LV_ALIGN_IN_TOP_MID, 0, 100);

	lv_obj_t *label_content = lv_label_create(parent, label_title);
	lv_label_set_long_mode(label_content, LV_LABEL_LONG_BREAK);
	lv_obj_set_width(label_content, SCREEN_BASE_RESOLUTION_HOR / 4 * 3);
	screen_adaption_lvgl_obj_set_pos(label_content, 30, 150);
	lv_label_set_text(label_content, "逐户排查人员,上门测量体温,宣传防疫知识。防疫时刻不能放松,如无必要,请尽量避免人员聚集");
}


static void block_ui_create(void)
{
	_block_data_memory *p_memory = (_block_data_memory *)get_local_pointer(p_data_memory);

	if (p_memory->this_scr == NULL) {
		lv_obj_t *obj_scr = lv_obj_create(NULL, NULL);
		p_memory->this_scr = obj_scr;
	}
	lv_scr_load(p_memory->this_scr);

	const char *head_title = "小区信息";
	//lv_obj_t *bg = universal_misc_rm_bg_default_create();
	lv_obj_t *bg = universal_misc_rm_pic_bg_create();

	block_ui_head_title(bg, head_title, block_event_return);
//return
	//block_ui_return_create(bg, block_event_return);

	block_temp_textarea(bg);
}

/*=======================================
*** Exterior Interface with screen system
========================================*/
/*Format:
***@ prefix: block_ex_interface_
*/
static void block_quit(void)
{
	block_data_free();
	lv_obj_clean(block_local.memory->this_scr);
}

bool block_ex_interface_register()
{
	bool is_true = gui_sys_screen_register(block_ui_create,
											block_quit,
											NULL,
											NULL,
											NULL,
											scr_block);

	if (is_true) return true;
	else return false;
}