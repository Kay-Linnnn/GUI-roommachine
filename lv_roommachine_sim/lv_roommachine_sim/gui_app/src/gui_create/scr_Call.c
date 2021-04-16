#include "scr_Call.h"

#include "data_tencell.h"
#include "scr_system_setting.h"



#ifndef _VS_SIMULATOR
#include "ss20x_audio_module.h"
#endif

#ifdef _VS_SIMULATOR
#pragma execution_character_set("utf-8")
#endif

static lv_obj_t *pre_screen = NULL;

typedef struct{
	bool b_busy;
	int16_t voice_volume;
}_Call_Status;
_Call_Status call_status;

static void call_invite_act_send(SysCommu_message_type act);


LV_FONT_DECLARE(SC_MicroSoft_YaHei_R16);

/*===========================
 ******DATA LAYER***********
============================*/
void scr_Call_data_init()
{
	memset(&call_status,0,sizeof(_Call_Status));
	
	int16_t vol_voice = (int16_t)SysSet_get_memory_sound_volume(item_sound_voice);

	if (vol_voice != -1)
		call_status.voice_volume = vol_voice;
	else
		call_status.voice_volume = 5;

	ui_printf_plus("call data init:%d",call_status.voice_volume);
}

/*============================
  *****UI LAYER***************
===============================*/
static int calling_voice_change_signal(int16_t voice_value)
{
	return datajs_set_sound_item_value_separately(item_sound_voice, (uint8_t)voice_value);
}

lv_obj_t *temp_voice_bar = NULL;
LV_EVENT_CB_DECLARE(calling_voice_plus_callback)
{
	if(e == LV_EVENT_CLICKED){
		int16_t init_volume = call_status.voice_volume;
		if(init_volume < 30){
			ui_printf("->>>>>VOICE UP:%d\n",init_volume);
			init_volume++;
			lv_bar_set_value(temp_voice_bar,init_volume,LV_ANIM_ON);
			ss20x_aodio_ao_setVolume(init_volume);
			call_status.voice_volume = init_volume;
			calling_voice_change_signal(init_volume); //send voice data out
			}
		}
}

LV_EVENT_CB_DECLARE(calling_voice_minus_callback)
{
	if(e == LV_EVENT_CLICKED){
		int16_t init_volume = call_status.voice_volume;
		if(init_volume > 0){
			ui_printf("->>>>VOICE DOWN%d\n",init_volume);
			init_volume--;
			lv_bar_set_value(temp_voice_bar,init_volume,LV_ANIM_ON);
			ss20x_aodio_ao_setVolume(init_volume);
			call_status.voice_volume = init_volume;
			calling_voice_change_signal(init_volume); //send voice data out
			}
		}
}

LV_EVENT_CB_DECLARE(open_door_btn_callback)
{
	if(e == LV_EVENT_CLICKED){
		ui_printf("open the door\n");
		call_invite_act_send(message_sip_local_opendoor);
		}
}


static void dial_ui_calling_begin()
{
	lv_obj_set_style_local_bg_opa(lv_scr_act(), LV_OBJMASK_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	call_status.b_busy = true;
	int16_t init_value = call_status.voice_volume;

	lv_obj_t *temp_cont = lv_cont_create(lv_scr_act(),NULL);
	lv_cont_set_fit(temp_cont,LV_FIT_TIGHT);
	lv_cont_set_layout(temp_cont,LV_LAYOUT_ROW_MID);
	lv_obj_set_pos(temp_cont,230,420);
	lv_obj_set_style_local_bg_opa(temp_cont,LV_OBJMASK_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_20);

	lv_obj_t *btn_m = lv_btn_create(temp_cont,NULL);
	lv_obj_set_size(btn_m,60,60);
	lv_obj_set_pos(btn_m,10,400);
	lv_obj_set_event_cb(btn_m,calling_voice_minus_callback);

	lv_obj_t *label = lv_label_create(btn_m,NULL);
	lv_label_set_text(label,LV_SYMBOL_MINUS);

	lv_obj_t *obj_bar = lv_bar_create(temp_cont,NULL);	
	lv_obj_set_size(obj_bar,200,30);
	//lv_obj_align(obj_bar, NULL, LV_ALIGN_CENTER, 40, 0);
	lv_bar_set_range(obj_bar,1,30);
	lv_bar_set_start_value(obj_bar, 0, LV_ANIM_OFF);
	lv_bar_set_value(obj_bar,init_value, LV_ANIM_OFF);
	ss20x_aodio_ao_setVolume(init_value);

	temp_voice_bar = obj_bar;


	lv_obj_t *btn_p = lv_btn_create(temp_cont,NULL);
	lv_obj_set_size(btn_p,60,60);
	lv_obj_align(btn_p,btn_m,LV_ALIGN_OUT_RIGHT_TOP,50,0);
	lv_obj_set_event_cb(btn_p, calling_voice_plus_callback);

	label = lv_label_create(btn_p,NULL);
	lv_label_set_text(label,LV_SYMBOL_PLUS);
}

static void dial_ui_quit()
{
	lv_obj_t *now_scr = lv_scr_act();
	lv_scr_load(pre_screen);
	lv_obj_del(now_scr);
	pre_screen = NULL;
	//call_invite_act_send(message_sip_local_holdup);
	call_status.b_busy = false;
}
lv_obj_t *temp_callhand_image = NULL;
LV_EVENT_CB_DECLARE(invite_answer_event_handle)
{
	if (e == LV_EVENT_CLICKED)
	{	
		lv_obj_del(obj);
		lv_obj_del(temp_callhand_image);
		dial_ui_calling_begin();
		call_invite_act_send(message_sip_local_connecet);
	}
}

LV_EVENT_CB_DECLARE(invite_cancel_event_handle)
{
	if (e == LV_EVENT_CLICKED)
	{
		dial_ui_quit();
		call_invite_act_send(message_sip_local_holdup);
	}
}


LV_IMG_DECLARE(dial_hand_green_200_200);
static void call_ui_call_waitting(lv_obj_t *parent,bool b_answer, bool b_opendoor)
{
	lv_obj_t *call_img = lv_img_create(parent,NULL);
	lv_img_set_src(call_img, &dial_hand_green_200_200);//LV_SYMBOL_CALL
	lv_obj_align(call_img,NULL,LV_ALIGN_IN_LEFT_MID,50,0);
	temp_callhand_image = call_img;

	lv_obj_t *obj_cont = lv_cont_create(parent,NULL);
	uint16_t c_height = 200;
	uint16_t c_y = LV_VER_RES / 2 - c_height / 2;
	lv_obj_set_pos(obj_cont,640, c_y);
	lv_obj_set_size(obj_cont,160, c_height);
	lv_cont_set_fit(obj_cont,LV_FIT_TIGHT);
	lv_cont_set_layout(obj_cont,LV_LAYOUT_COLUMN_MID);
	lv_theme_apply(obj_cont,(lv_theme_style_t)sigma_star_theme_cont_bg_transparent);

	static lv_style_t btn_style;
	lv_style_init(&btn_style);
	lv_style_set_bg_color(&btn_style,LV_STATE_DEFAULT, LV_COLOR_RED);
	//lv_style_set_bg_grad_color(&btn_style,LV_STATE_DEFAULT,LV_COLOR_YELLOW);
	lv_style_set_bg_opa(&btn_style,LV_STATE_DEFAULT,LV_OPA_90);
	lv_style_set_text_font(&btn_style,LV_STATE_DEFAULT,&SC_MicroSoft_YaHei_R16);

	static lv_style_t ack_btn_style;
	lv_style_init(&ack_btn_style);
	lv_style_copy(&ack_btn_style,&btn_style);
	lv_style_set_bg_color(&ack_btn_style,LV_STATE_DEFAULT,LV_COLOR_GREEN);

	lv_obj_t *btn,*label;

	if(b_answer)
	{
		btn = lv_btn_create(obj_cont,NULL);
		lv_obj_set_size(btn,120,80);
		lv_obj_set_event_cb(btn, invite_answer_event_handle);
		lv_obj_add_style(btn,LV_BTN_PART_MAIN,&ack_btn_style);
 	
		label = lv_label_create(btn,NULL);
		//lv_label_set_text(label,"接听");
		lv_label_set_text(label,"接听");
		lv_theme_apply(label, (lv_theme_style_t)sigma_star_theme_label_chinese);
	}

	btn = lv_btn_create(obj_cont,NULL);
	lv_obj_set_size(btn,120,80);
	lv_obj_set_event_cb(btn, invite_cancel_event_handle);
	lv_obj_add_style(btn,LV_BTN_PART_MAIN,&btn_style);

	label = lv_label_create(btn,NULL);	
	lv_theme_apply(label, (lv_theme_style_t)sigma_star_theme_label_chinese);
	lv_label_set_text(label,"挂断");

	if (b_opendoor)
	{
		static lv_style_t open_btn_style;
		lv_style_init(&open_btn_style);
		//lv_style_copy(&ack_btn_style,&btn_style);
		lv_style_set_bg_opa(&open_btn_style, LV_STATE_DEFAULT, LV_OPA_90);
		lv_style_set_text_font(&open_btn_style, LV_STATE_DEFAULT, &SC_MicroSoft_YaHei_R16);
		lv_style_set_bg_color(&open_btn_style, LV_STATE_DEFAULT, LV_COLOR_GREEN);

		lv_obj_t *btn_open_door = lv_btn_create(obj_cont, NULL);
		screen_adaption_lvgl_obj_set_size(btn_open_door, 120, 80);
		//lv_obj_set_pos(btn_open_door,650,220);
		screen_adaption_lvgl_obj_align(btn_open_door, btn, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
		lv_obj_set_event_cb(btn_open_door, open_door_btn_callback);
		lv_obj_add_style(btn_open_door, LV_BTN_PART_MAIN, &open_btn_style);

		lv_obj_t *open_door_label = lv_label_create(btn_open_door, NULL);
		lv_label_set_text(open_door_label, "开门");
		//lv_label_set_text(open_door_label,"open the door");
		lv_theme_apply(open_door_label, (lv_theme_style_t)sigma_star_theme_label_chinese);
	}
	
}

static void call_ui_dial_in_win()
{
	pre_screen = lv_scr_act();

	lv_obj_t *obj = lv_obj_create(NULL,NULL);
	lv_scr_load(obj);

	call_ui_call_waitting(obj, false, true);
}

static void call_ui_invite_in_win()
{
	pre_screen = lv_scr_act();

	lv_obj_t *obj = lv_obj_create(NULL,NULL);
	lv_scr_load(obj);

	call_ui_call_waitting(obj, true, true);

	lv_obj_set_style_local_bg_opa(lv_scr_act(), LV_OBJMASK_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	
}

static void call_ui_cctv_in_win()
{
	pre_screen = lv_scr_act();

	lv_obj_t *obj = lv_obj_create(NULL, NULL);
	lv_scr_load(obj);

	call_ui_call_waitting(obj, false, false);

	lv_obj_set_style_local_bg_opa(lv_scr_act(), LV_OBJMASK_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
}


/*============================
  *** Communication LAYER*****
============================*/
static void call_communication_in(int message,void *in_data)
{
	ui_printf("[call_communication_in]message:%d\n",message);
	switch(message)
	{
		case message_sip_ex_call:
			if(!call_status.b_busy){
				call_ui_invite_in_win();
				}
			break;

		case message_sip_ex_recv:
			dial_ui_calling_begin();
			break;

		case message_sip_ex_cancel:
			dial_ui_quit();
			break;

		case message_call_dial:
			if(!call_status.b_busy){
				call_ui_dial_in_win();
				}
			break;

		case message_call_cctv:
			if (!call_status.b_busy) {
				call_ui_cctv_in_win();
			}
			break;

		default:
		break;
	}
}

static void call_communication_out(Sys_Commu_type comm_type,SysCommu_message_type message,void *data)
{
	gui_sys_commu_event_send(SysCommuDir_scr2ex,comm_type,message,data);
}

static void call_invite_act_send(SysCommu_message_type act)
{
	call_communication_out(SysCommu_type_sipmessage,act,NULL);
}

Ex_Communication_t scr_Call_register()
{
	Ex_Communication_t call_commu = (Ex_Communication_t)general_malloc(sizeof(Ex_Communication_t));

	call_commu = call_communication_in;
	return call_commu;	
}

