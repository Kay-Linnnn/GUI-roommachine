#ifndef _SCR_SECURITY_H
#define _SCR_SECURITY_H


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

	void  scr_security_data_init(void);
	bool scr_security_interface_register(void);

#ifdef _cplusplus
}
#endif // _cplusplus

#endif
