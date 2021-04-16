#ifndef _GUI_SCR_SYS_H
#define _GUI_SCR_SYS_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "../../gui_app.h"
//#include "../gui_create/gui_test.h"
//#include "../gui_create/scr_menu_test.h"
#include "../gui_logic/stack_lvgl_sys.h"
//#include "../gui_logic/GUI_self_define.h"

typedef bool(*BeforeScreen_t)(void);
typedef void(*ScreenCreate_t)(void);
typedef void(*ScreenExit_t)(void);
typedef void(*EventFunction_t)(void*,int);
typedef void(*DataFunction_t)(void*);
typedef void(*Ex_Communication_t)(int , void*);

typedef bool(*FunRegister_t)(void);

typedef void(*BackStage_t)(int, int, void*); //for Ex_Communication_t, the first int decides which screen to call

typedef struct screen_class{
	BeforeScreen_t   ScreenBefore;
	ScreenCreate_t   ScreenCreate;
	ScreenExit_t     ScreenExit;
	EventFunction_t  EventCallback;
	DataFunction_t   DataFunction;
	Ex_Communication_t Ex_Communication;

	uint8_t screen_id;
}Landing_scr;

typedef struct scr_running_signal{
    bool register_succe;  //register successfully, which means about to do change screen
	//bool scr_is_running;
	//bool change_screen;  //signal for changing creen
	//uint8_t change_type;  //1:return 2:enter next
	//uint8_t change_scr_id; //scr_id about to change
	//uint8_t NewScreen;
	//uint8_t OldScreen;	
}_scr_running_signal;

typedef struct task_data{
	//bool    triggle;
	uint8_t task_signal;
	void*   event_data;
	//struct task_data *next;
}_task_data;

#define TASK_DATA_QUEUE_LENGTH_NAX 10
typedef struct ScrManage{
	Landing_scr *screen_now;  //all screen api,always register the current screen here

	Landing_scr *screen_pre;

	Ex_Communication_t screen_call_commu; //special for "Call" sceen
	BackStage_t	backstage_call;

	_scr_running_signal *running_signal;

	_task_data *task_data_queue[TASK_DATA_QUEUE_LENGTH_NAX];
	uint8_t queue_in_cnt;
	uint8_t queue_out_cnt;

	uint8_t act_scr_id;
	//bool test_event;

}Screen_Manage;

//task event struct
typedef struct task_screen{
	uint8_t change_type;
	uint8_t scr_id;
}_task_screen;

typedef uint8_t task_triggle_type;
enum {
	task_triggle_invalid = 0,
	task_triggle_screen   = 1,
	task_triggle_Commu_ex2scr,
	task_triggle_Commu_scr2ex,
	task_triggle_inner_commu,
	//task_triggle_SOON,
};

enum {
	scr_chtype_change = 1,
	scr_chtype_return,
};


//screen system communication part

typedef int8_t Sys_Commu_Direction;
typedef enum Sys_CommuEve_direction{
	SysCommuDir_ex2scr = 0,
	SysCommuDir_scr2ex,
	SysCommuDir_toCall,//to screen_call
}_Sys_CommuEve_direction;

typedef int8_t Sys_Commu_type;
typedef enum Sys_CommuEve_type{
	SysCommu_type_sipmessage = 0,
	SysCommu_type_callmessage,
	SysCommu_type_visitor,
	#if 0
	CommuEve_type_sip_invalid = 0,
	CommuEve_type_sip_call = 1,
	CommuEve_type_sip_recv,
	#endif
}_Sys_CommuEve_type;

#if 0
typedef enum CommuEve_sip_message{
	sipmessage_invalid = 0,
	ex_sipmessage_call = 1,
	ex_sipmessage_recv,
	scr_sipmessage_call = 10,
	scr_sipmessage_recv,
}_CommuEve_sip_message;
#endif
typedef int SysCommu_message_type;
enum {
	message_invalid = 0,
	//sip
	message_sip_ex_call = 1,
	message_sip_ex_recv,
	message_sip_ex_cancel,
	message_sip_local_call = 10, //commnication center要映射osip的message，这里顺序要与osip保持一致
	message_sip_local_connecet,
	message_sip_local_holdup,
	message_sip_local_exit,
	message_sip_local_opendoor,
	//Visitor
	message_visitor_left,
	///inner message
	message_call_dial,
	message_call_cctv,
};

typedef struct{
	Sys_Commu_type SC_type;
	int		message;
	void *data;
}_CommunEve_data;

	
typedef uint8_t scr_change_type;

/*Security*/
enum {
	security_smoke,
	security_balcony_infrared,
	security_door_mag,
	security_thermal,
	security_emergency_btn,
	security_gas,
	security_window_mag,
	security_infrared,
	security_mode_max,
};
typedef uint8_t security_mode_t;


void gui_sys_init(void);

bool gui_sys_screen_register(ScreenCreate_t scr_create,
                                    ScreenExit_t scr_exit,
                                    EventFunction_t event_fun,
                                    DataFunction_t  data_fun,
                                    Ex_Communication_t ex_communication,
                                    uint8_t scr_id);
void gui_sys_handle(void);

//void gui_sys_return(void);
//void gui_sys_screen_change(FunRegister_t fun_register);
void gui_sys_screen_event_send(scr_change_type change_type, uint8_t scr_id);

void gui_sys_commu_event_send(Sys_Commu_Direction CE_dir,Sys_Commu_type CE_type,SysCommu_message_type message,void *data);




#ifdef __cplusplus
 }
#endif
 
#endif


