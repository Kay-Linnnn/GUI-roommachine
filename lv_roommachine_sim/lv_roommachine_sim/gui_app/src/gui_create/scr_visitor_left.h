#ifndef _SCR_VISITOR_LEFT_H
#define _SCR_VISITOR_LEFT_H

#ifdef __cplusplus
extern "C" {
#endif // _cplusplus

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif


	void visitor_data_init(void);
	bool visitor_ex_interface_register(void);
	void visitor_ex_interface_commu(int mesg, void *data);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !_SCR_VISITOR_LEFT_H
