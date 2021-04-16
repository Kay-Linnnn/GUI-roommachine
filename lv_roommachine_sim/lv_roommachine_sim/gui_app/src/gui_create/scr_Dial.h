#ifndef _SCR_DIAL_H
#define _SCR_DIAL_H

#ifdef __cplusplus
  extern "C"{
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

//#include "scr_home.h"
//#include "sigma_star_theme.h"

void scr_dial_init(void);

bool scr_dial_register(void);



#ifdef __cplusplus
  	}
#endif

#endif