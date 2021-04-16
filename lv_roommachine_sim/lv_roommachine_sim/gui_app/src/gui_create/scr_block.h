#ifndef _SCR_BLOCK_H
#define _SCR_BLOCK_H



#ifdef _cplusplus
extern "C" {
#endif // _cplusplus


#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

	void block_data_init(void);

	bool block_ex_interface_register(void);



#ifdef _cplusplus
}
#endif // _cpluspls


#endif
