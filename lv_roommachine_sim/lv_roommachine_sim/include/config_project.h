
#ifndef __CONFIG_PROJECT_H
#define __CONFIG_PROJECT_H


#define CFG_P_software_version_num	"1.0.9"

#define CFG_P_sipLocalPort		5060    //  本机SIP 端口
#define CFG_P_videolocalPort 	2020   //  本机ORTP 视频接收端口
#define CFG_P_audiolocalPort	3030   // 本机ORTP 音频接收端口
#define CFG_P_videolocalTYPE 	96 	  // 视频类型 96 H264
#define CFG_P_audiolocalTYPE	100	  // 音频类型


#define CFG_P_sipRemoteIp		"192.168.1.170"

#define CFG_P_sipRemotePort		5060	// 默认远端SIP UDP 端口

//comment this is run on board
#define PC_SIMULATOR

#ifndef PC_SIMULATOR
#define CFG_P_DEBUG_ON		1

#define SYS_DEBUGE(fmt,arg...) 	do{\
                                         if(CFG_P_DEBUG_ON)\
                                         printf("<<-SYS-DEBUG->> "fmt"\n", ##arg);\
                                      }while(0)

#define SYS_P_ERROR(fmt,arg...) 		printf("<<-SYS-ERROR->> "fmt"\n",##arg)			

#define SYS_INFO(fmt,arg...) 			printf("<<-SYS-INFO->> "fmt"\n",##arg)

#else
#define CFG_P_DEBUG_ON		1
#define SYS_DEBUGE(fmt,...)			do{\
										if(CFG_P_DEBUG_ON)\
											printf("<<-SYS-DEBUG->> "fmt"\n",__VA_ARGS__);\
										}while(0)

#define SYS_P_ERROR(fmt,...)		printf("<<_SYS_ERROR->> "fmt"\n",__VA_ARGS__)

#define SYS_INFO(fmt,...)			printf("<<-SYS-INFO->> "fmt"\n",__VA_ARGS__)
#endif

#endif


