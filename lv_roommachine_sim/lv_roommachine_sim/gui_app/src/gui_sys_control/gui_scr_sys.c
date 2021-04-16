#include "gui_scr_sys.h"
#include "../../gui_app.h"
#include "../gui_create/scr_home.h"
#include "../gui_create/scr_VoiceSetting.h"
#include "../gui_create/scr_Dial.h"
#include "../gui_create/scr_doorCCTV.h"
#include "../gui_create/scr_system_setting.h"
#include "gui_commu_center.h"
#include "scr_Call.h"
#include "scr_security.h"
#include "scr_visitor_left.h"
#include "scr_block.h"

#include "data_tencell.h"

bool screen_busy;
bool new_register;
uint8_t NewScreen_id;
uint8_t OldScreen_id;
#define IS_SCREEN(scr_id)  ((scr_id)<MaxScreen)
#define NEED_POP(scr_id)    ((scr_id)<OldScreen_id)


_scr_running_signal sys_running_signal;

Screen_Manage scr_manager;

static void gui_sys_screen_running(void);
static Landing_scr* screen_memoryapply(void);
static bool screen_StackPush(Landing_scr *new_scr);
static void screen_StackPop(void);
static bool screen_display(void);
static bool screen_return(void);
static bool screen_exit(void);
static void screen_change(uint8_t scr_id);

static void systask_data_in_queue(Screen_Manage *man,_task_data *node_new);


void gui_sys_handle() //control power_machine, include screen, data, communication, event-callback
{
  //ui running
  gui_sys_screen_running();  
}

/*
 * Keep UI running
*/
static void gui_sys_screen_running()
{
  if(NewScreen_id != OldScreen_id)
  {
	screen_busy = true;

	if(scr_manager.screen_pre != NULL)
	{
		if(scr_manager.screen_pre->ScreenExit != NULL && IS_SCREEN(NewScreen_id))
		screen_exit();
		ui_printf("[gui_sys_screen_running]scr_exit\r\n");
	}

	//display new screen
	if(scr_manager.screen_now->ScreenCreate != NULL && IS_SCREEN(NewScreen_id))
	{
		screen_display();
		new_register = false;
		ui_printf("[gui_sys_screen_running]scr_display\r\n");
		//scr_manager.test_event = true;
	}

	//check whether to pop out the old screen
	if(NEED_POP(NewScreen_id))
	{
		screen_StackPop();
	}
	//to-do after new screen display
	//screen_clear();    
	OldScreen_id = NewScreen_id;
	}
	else
	{
		screen_busy = false;
	}
  
}

static Landing_scr* screen_memoryapply()
{
  Landing_scr *p_apply = (Landing_scr *)LV_MEM_CUSTOM_ALLOC(sizeof(Landing_scr));
  return p_apply;
}

static bool screen_StackPush(Landing_scr *new_scr)
{
  bool bool_stack = ui_stackPush(new_scr);
  return bool_stack;
}

static void screen_StackPop()
{
  ui_stackPop();
}

static bool screen_display()
{
  //just show current screen?
  //if(scr_manager.running_signal.register_succe){
  	//maybe here we can do some checking thing before we show screen
  	//for example, got change screen, but somthing wrong so screen doesn't show, then no need to display
  	//cause alreay displaying now
  	//bool is_display_ok = scr_manager.sys_scr_act->ScreenBefore();
	//if(is_display_ok){
		scr_manager.screen_now->ScreenCreate();
	    //scr_manager.running_signal.scr_is_running = true;
	    return true;
		//}  	
  	//}
  
}

static bool screen_return()  //return to previous screen
{
  
  Landing_scr *pre_scr = ui_Get_StackPre();
  if(pre_scr == NULL){
  	return false;  //got previous screen failed
  	}
  
  NewScreen_id = pre_scr->screen_id;
  scr_manager.screen_pre = scr_manager.screen_now;
  scr_manager.screen_now = pre_scr;
  ui_printf("screen_return,NewScreen:%d,OldScreen:%d\r\n",NewScreen_id,OldScreen_id);
  return true;
}

static void screen_change(uint8_t scr_id)
{
  //maybe we should do checking things before we change the screen
  //first hide the current screen and display the next screen
  //Then do screen exit
  //Then do some clean work
  switch(scr_id){
  	#if 0
  	case 1:
		gui_test_register();///register
		break;

	case 2:
		menu_test_register();
		break;
	#endif
	case scr_Home:
		scr_home_register();
		break;

	case scr_VoiceSetting:
		scr_VoiceSetting_register();
		//menu_test_register();
		break;

	case scr_Dial:
		scr_dial_register();
		break;

	case scr_doorCCTV:
		scr_doorCCTV_register();
		break;

	case scr_system_setting:
		scr_sysytem_setting_register();
		break;

	case scr_security:
		scr_security_interface_register();
		break;

	case scr_visitor:
		visitor_ex_interface_register();
		break;

	case scr_block:
		block_ex_interface_register();
		break;

	default:  break;
  	}
  
}

static bool screen_exit()
{
  scr_manager.screen_pre->ScreenExit();
  return true;
}


static void screen_backstage_communication(int scr, int mesg, void *data)
{
	switch(scr)
	{
		case scr_visitor:
			visitor_ex_interface_commu(mesg, data);
			break;

		default:
		break;
	}
}

static void screen_communication(uint8_t def_scr,int message,void *comm_data)
{
	ui_printf("[screen_communication] def_scr:%d\n",def_scr);
	if(def_scr != scr_Null)
	{
		switch(def_scr)
		{
			case scr_call:
				scr_manager.screen_call_commu(message,comm_data);
				break;

			case scr_visitor:
				//scr_manager.screen_call_commu();
				scr_manager.backstage_call(scr_visitor, message, comm_data);
				break;

			default:
			break;
		}
	}
	else //if no appointed scr,then use screen_now as default
	{
		if(scr_manager.screen_now->Ex_Communication != NULL)
		scr_manager.screen_now->Ex_Communication(message,comm_data);
	}
	
}

/*===================
***Interior utility***
=====================*/
static bool IU_send_event_task_to_SysMan(task_triggle_type task_type,void *data)
{
	_task_data *task_data = (_task_data *)general_malloc(sizeof(_task_data));
	if(is_null_ptr(task_data)){
		ui_printf("[IU_send_event_task_to_SysMan] task_data malloc fail\n");
		return false;
		}
	task_data->task_signal = task_type;
	task_data->event_data = data;
	//task_data->next = NULL;

	//scr_manager.task_data = task_data;
	systask_data_in_queue(&scr_manager,task_data);
	return true;
}

static void *IU_commu_event_data_handle(Sys_Commu_type SC_type,SysCommu_message_type message,void *data)
{
	_CommunEve_data *ce_data = (_CommunEve_data *)general_malloc(sizeof(_CommunEve_data));
	if(is_null_ptr(ce_data)) return NULL;

	ce_data->SC_type = SC_type;
	
	switch(SC_type)
	{
		//better to class this type,like CommuEve_type_sip:message indicate recv/call/or others
		#if 0
		case CommuEve_type_sip_recv:
			{
			}
		break;

		case CommuEve_type_sip_call:
			{
			int *sip_message = (int *)general_malloc(sizeof(CommuEve_message_type));
			if(is_null_ptr(sip_message)) return NULL;

			*sip_message = message;
			return sip_message;
			}
		break;
		#endif
		case SysCommu_type_sipmessage:
			{
			#if 0
			SysCommu_message_type *sip_message = (SysCommu_message_type *)general_malloc(sizeof(SysCommu_message_type));
			if(is_null_ptr(sip_message)) {
				general_free(ce_data);
				return NULL;
			}

			*sip_message = message;
			ce_data->data = sip_message;
			#endif
			ce_data->message = message;
			ce_data->data = data;
			//return sip_message;
			}
		break;

		case SysCommu_type_callmessage:
			{
			ce_data->message = message;
			ce_data->data = data;
			}
		break;

		case SysCommu_type_visitor:
			{
				ce_data->message = message;
				ce_data->data = data;
			}
		break;

		default:
			ce_data->message = message_invalid;
			ce_data->data = NULL;
			//return NULL;
		break;
	}
	return ce_data;
}

/*===============================
***GUI system API for exterior***
=================================*/
bool gui_sys_screen_register(ScreenCreate_t scr_create,
                                    ScreenExit_t scr_exit,
                                    EventFunction_t event_fun,
                                    DataFunction_t  data_fun,
                                    Ex_Communication_t ex_communication,
                                    uint8_t scr_id)
{
	Landing_scr *active_scr = screen_memoryapply();
	if(NULL == active_scr)
	{
		//scr_manager.running_signal.register_succe = false;
		new_register = false;
		ui_printf("gui_sys_screen_register failed\r\n");
		return false;
	}
	screen_StackPush(active_scr);

	active_scr->ScreenCreate = scr_create;
	active_scr->ScreenExit = scr_exit;
	active_scr->screen_id = scr_id;
	active_scr->Ex_Communication = ex_communication;
	///and so on
	scr_manager.screen_pre = scr_manager.screen_now;
	scr_manager.screen_now = active_scr;
	//scr_manager->screen_list = active_scr;

	//scr_manager.running_signal.register_succe = true;
	new_register = true;

	NewScreen_id = scr_id;
	ui_printf("gui_sys_screen_register successfully\r\n");

	return true;
}

static void systask_data_in_queue(Screen_Manage *man,_task_data *node_new)
{
	if(is_null_ptr(node_new)) return;

	man->task_data_queue[man->queue_in_cnt] = node_new;
	man->queue_in_cnt++;
	if(man->queue_in_cnt >= TASK_DATA_QUEUE_LENGTH_NAX)
		man->queue_in_cnt = 0;
}

static _task_data *systask_data_out_queue(Screen_Manage *man)
{

	_task_data *out_data = NULL;
	if(man->queue_out_cnt != man->queue_in_cnt)
	{
		out_data = man->task_data_queue[man->queue_out_cnt];
		man->queue_out_cnt++;
		if(man->queue_out_cnt >= TASK_DATA_QUEUE_LENGTH_NAX)
			man->queue_out_cnt = 0;
	}
	return out_data;
}

void gui_sys_task_callback(lv_task_t * task)
{
  Screen_Manage *user_task = (Screen_Manage *)task->user_data;
  //ui_printf("[gui_sys_task]\r\n");
  _task_data *task_data = systask_data_out_queue(user_task);
  if(task_data == NULL){//user_task->task_data
  	return;
  	}
  
  	ui_printf("system task be called\r\n");
	
    switch(task_data->task_signal)
      {
    	case task_triggle_screen:
    		{
			//to-do
			_task_screen *task_scr = (_task_screen *)task_data->event_data;
			if(task_scr->change_type == scr_chtype_change) //change screen
			{
			   screen_change(task_scr->scr_id);
			}
			else if(task_scr->change_type == scr_chtype_return) //return
			{  
			   screen_return();
			}
			general_free(task_scr);			
    		}
		break;

		case task_triggle_Commu_ex2scr:
			{
				ui_printf("triggle commu,ex to scr\n");
				_CommunEve_data *ce_data = (_CommunEve_data *)task_data->event_data;
				switch(ce_data->SC_type)
				{
					case SysCommu_type_sipmessage:
						{
							uint8_t scr = scr_Null;
							//if(ce_data->message == message_sip_ex_call)
							if(message_sip_ex_call <= ce_data->message && ce_data->message <= message_sip_ex_cancel)
								scr = scr_call;
							
							screen_communication(scr,ce_data->message,ce_data->data);
						}						
						break;

					case SysCommu_type_visitor:
						{
							screen_communication(scr_visitor, ce_data->message, ce_data->data);
						}
					default:
					break;
				}
				general_free(ce_data);
			}
		break;

		case task_triggle_Commu_scr2ex:
			{
				_CommunEve_data *ce_data = (_CommunEve_data *)task_data->event_data;
				ui_printf("commu type:%d\n",(int)ce_data->SC_type);
				switch(ce_data->SC_type)
				{
					case SysCommu_type_sipmessage:
						commu_center_scr2ex(CC_scr2ex_Sip,ce_data->message,ce_data->data);
						break;

					default:
					break;
				}
				if (ce_data->data != NULL) {
					general_free(ce_data->data);
					ui_printf_plus("free sipmessage, ce_data->data");
				}				

				general_free(ce_data);
			}
		break;

		case task_triggle_inner_commu:
			{
				ui_printf("triggle inner commu\n");
				_CommunEve_data *ce_data = (_CommunEve_data *)task_data->event_data;
				switch(ce_data->SC_type)
				{
					case SysCommu_type_callmessage:
						screen_communication(scr_call,ce_data->message,ce_data->data);
						break;

					default:
					break;
				}
				general_free(ce_data);
			}
		break;
		
		default:
		  break;
      }
	general_free(task_data);
	//user_task->task_data = NULL;
}

static int sys_security_warning(security_mode_t which)
{
	ui_printf_plus("sys_security_warning, mode:%d", which);
	//to do, send out the warning information
	
	return 0;//to do, the return value could be useful for periority
}

void gui_sys_listen_callback(lv_task_t * task)
{
	//here listen the gpio state
	//ui_printf_plus("Listen the gpio state");
	int i = 0;
	int gpio_state = -1;
	int mode_array[security_mode_max] = {6, 7, 8, 9, 10, 11, 12, 14};//see security_mode_t for more detail
	//int mode_defautl_state[security_mode_max] = {0}; //this colud be useful if not all gpio state by default is high
#if 1
	for(i = 0; i < security_mode_max; ++i)
	{
		if(0 == datasys_security_listen(mode_array[i]))//mode_defautl_state[i] != datasys_security_listen(mode_array[i])
		{
			sys_security_warning(i);
		}
		
	}
#endif
}

static void gui_sys_task()
{
	ui_printf("gui system task init\n");
	lv_task_t *task_t = lv_task_create(gui_sys_task_callback, 200, LV_TASK_PRIO_MID, &scr_manager);

#ifndef _VS_SIMULATOR
	int mode_array[security_mode_max] = { 6, 7, 8, 9, 10, 11, 12, 14 };
	datasys_security_start(mode_array, security_mode_max);//start first and then listen
	lv_task_t *task_listen = lv_task_create(gui_sys_listen_callback, 1000, LV_TASK_PRIO_MID, NULL);
#endif // !_VS_SIMULATOR

	
}

//send event to change screen
//parameter "change_type": 1:change, 2:return
//scr_id: scr_id about to change, return type do nothing,scr_NULL is ok if change_type is scr_chtype_return
void gui_sys_screen_event_send(scr_change_type change_type, uint8_t scr_id)
{
  if(change_type >=1 && change_type <= 2){
  	ui_printf("[gui_sys_screen_event_send],type:%d,scr:%d\r\n",change_type,scr_id);
  	_task_screen *event_scr = (_task_screen *)general_malloc(sizeof(_task_screen));
    if(event_scr == NULL){
		ui_printf("screen event,malloc failed");
		return;
    	}
	event_scr->change_type = change_type;
    event_scr->scr_id = scr_id;
#if 0
	_task_data *task_data = (_task_data *)general_malloc(sizeof(_task_data));
	task_data->task_signal = task_triggle_screen;
	task_data->event_data = event_scr;

	scr_manager.task_data = task_data;
	ui_printf("screen event send end\r\n");
#endif
	if(!IU_send_event_task_to_SysMan(task_triggle_screen,event_scr)){
		general_free(event_scr);
		}
  	}
}

void gui_sys_commu_event_send(Sys_Commu_Direction SC_dir,Sys_Commu_type SC_type,SysCommu_message_type message,void *data)
{
	void *pack_data = IU_commu_event_data_handle(SC_type,message,data);
	if(is_null_ptr(pack_data)){
		ui_printf("[gui_sys_commu_event_send] pack_data is NULL\n");
		return;
		}
	
	task_triggle_type task_type = task_triggle_invalid;
	if(SC_dir == SysCommuDir_ex2scr)
		task_type = task_triggle_Commu_ex2scr;
	else if(SC_dir == SysCommuDir_scr2ex)
		task_type = task_triggle_Commu_scr2ex;
	else if(SC_dir == SysCommuDir_toCall)
		task_type = task_triggle_inner_commu;

	if(!IU_send_event_task_to_SysMan(task_type,pack_data)){
		general_free(pack_data);
		}
}

void gui_screen_data_init(void)
{
	ui_stackInit();

	scr_system_setting_init(); //make sure system setting is the first to init, the others may take value from it when init
	scr_home_init();
	scr_VoiceSetting_init();
	scr_dial_init();
	scr_doorCCTV_init();
  
	scr_security_data_init();
	visitor_data_init();
	block_data_init();

	scr_Call_data_init();
}


void gui_sys_init()
{
  memset(&sys_running_signal,0,sizeof(_scr_running_signal));
  memset(&scr_manager,0,sizeof(scr_manager));
  screen_busy = false;
  new_register = false;
  OldScreen_id = NewScreen_id = 0;

  scr_manager.running_signal = &sys_running_signal;
  scr_manager.screen_pre = scr_manager.screen_now = NULL;

  gui_screen_data_init();
  //sys_running_signal.register_succe = gui_test_register();
  sys_running_signal.register_succe = scr_home_register();

  scr_manager.screen_call_commu = scr_Call_register();

  scr_manager.backstage_call = screen_backstage_communication;
  home_theme_init();
  ui_printf("gui_sys_init done\r\n");
  //if(false == sys_running_signal.register_succe)
  //something we should do if the init register return "false", cause no screen will display further
  gui_sys_task();
}
