#ifndef _SCR_CALL_H
#define _SCR_CALL_H

#ifdef __cplusplus
extern "C"{
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

#include "gui_app.h"

void scr_Call_data_init();

Ex_Communication_t scr_Call_register(void);

#ifdef __cplusplus
}
#endif

#endif
