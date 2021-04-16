#include "gui_commu_center.h"

#include "../../gui_app.h"

#ifndef comment_commu_centet
#include "../../voip/sip/lth_exosip.h"
#include "../../voip/sip/lth_osip_cfg.h"
#include "../../voip/rtp/lth_exortp.h"
#include "../../voip/rtp/lth_rtp_cfg.h"

#include "lth_voip_media.h"
#endif

static void ortp_delete()
{
	//temp_aaa();
}

/*=============================================
  Commnication Center Handle->screen to exterior
===============================================*/
//这里将message_type做一个映射，sys_commu里面定义的message里，local call是10，映射为osip_send的message为0
static void CC_send_message_to_sip(int sipch,int message_type, char *des_ip)
{
	ui_printf_plus("[CC_send_message_to_sip] des_ip:%s", des_ip);
#ifndef comment_commu_centet
	int message_send[] = {exOSIP_SEND_C2CINVITE,
							exOSIP_SEND_200_CONNET,
							exOSIP_SEND_HOLDED,
							exOSIP_SEND_EXIT,
							exOSIP_SEND_OpenDoor,
							exOSIP_SEND_APPAnswer};
	ui_printf("[CC_send_message_to_sip] message:%d\n",message_type);
	if (message_type != message_sip_local_holdup)
		lth_exosip_send(sipch, message_send[message_type - 10], des_ip);//uncommonet this
		//lth_exosip_send(sipch, message_send[message_type - 10]);

	if(message_type == message_sip_local_holdup){
		ui_printf("ortp_delete\n");
		//ortp_delete();
	 	//lth_exosip_send(sipch,exOSIP_SEND_EXIT);
	 	lthmedia_voip_HOLDED();
		}
#endif
}



/*==============================================
  Commnication Center Handle->exterior to screen
================================================*/
static void CC_send_exosip_message_to_screen(int message,void *data)
{
	//最好是这个message和CommuEve_type之间有一定规律，直接转换过去而不用判断message类型
	Sys_Commu_type CE_type = SysCommu_type_sipmessage;
	#if 0
	switch(message):
	{
		case sip_client_MEG_INVITE:
			CE_type = CommuEve_type_sip_call;
			break;

		default:
			CE_type = CommuEve_type_sip_invalid;
			break;
	}
	#endif
	int sys_message[] = {message_sip_ex_call,
							message_invalid,
							message_sip_ex_recv,
							message_sip_ex_cancel};
	gui_sys_commu_event_send(SysCommuDir_ex2scr,CE_type,sys_message[message-1],NULL);
}

static void CC_send_visitor_message_to_screen(int message, char *path)
{
	Sys_Commu_type CE_type = SysCommu_type_visitor;

	gui_sys_commu_event_send(SysCommuDir_ex2scr, CE_type, 0, path);
}
/*=========================
  Communication Center API
===========================*/
void commu_center_ex2scr(CC_ex2scr_event_type type,int message,void *data)
{
	switch(type)
	{
		case CC_ex2scr_Sip:
			CC_send_exosip_message_to_screen(message,data);
			break;

		case CC_ex2scr_Visitor:
			{
				char *path = (char *)general_malloc(sizeof(char) * (strlen((char*)data) + 1));
				strcpy(path,(char *)data);
				CC_send_visitor_message_to_screen(message, path);
			}			
			break;

		default:
		break;
	}
}

void commu_center_scr2ex(CC_scr2ex_event_type type,int message,void *data)
{
	ui_printf("[commu_center_scr2ex] type:%d\n",type);
	switch(type)
	{
		case CC_scr2ex_Sip:
				ui_printf("case CC_scr2ex_Sip\n");
				CC_send_message_to_sip(0,message,(char *)data);
			break;

		default:
		break;
	}
}

