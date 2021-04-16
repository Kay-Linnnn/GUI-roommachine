#ifndef _SCR_HOME_H
#define _SCR_HOME_H


#ifdef __cplusplus
extern "C" {
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

#include "lvgl/src/lv_core/lv_obj_style_dec.h"

//#define ui_printf(...)  printf(__VA_ARGS__) 
	void scr_home_init(void);

	bool scr_home_register(void);

	void home_theme_init(void);




#ifdef __cplusplus
}
#endif

#endif
