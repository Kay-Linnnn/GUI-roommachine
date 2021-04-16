#ifndef _SCR_VOICESETTING_H
#define _SCR_VOICESETTING_H

#ifdef _cplusplus
  extern "C"{
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

#include "scr_home.h"
#include "sigma_star_theme.h"

	  void scr_VoiceSetting_init(void);

	  bool scr_VoiceSetting_register(void);
	  void scr_VoiceSetting_create(void);


#ifdef _cplusplus
  	}
#endif

#endif
