#ifndef _GUI_COMMU_CENTER_H
#define _GUI_COMMU_CENTER_H

#ifdef _cplusplus
extern "C"{
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

#include "gui_app.h"

#ifdef _VS_SIMULATOR
#define comment_commu_centet
#endif

typedef int CC_scr2ex_event_type;
enum CC_scr2ex_event{
	CC_scr2ex_Sip = 0,
};

typedef int CC_ex2scr_event_type;
enum CC_ex2scr_event{
	CC_ex2scr_Sip = 0,
	CC_ex2scr_Visitor,
};

typedef enum exosip_message_type{
	exosip_message_call = 0,
	exosip_message_hangup,
}_exosip_message_type;



void commu_center_ex2scr(CC_ex2scr_event_type type,int message,void *data);
void commu_center_scr2ex(CC_scr2ex_event_type type,int message,void *data);


#ifdef _cplusplus
}
#endif

#endif
