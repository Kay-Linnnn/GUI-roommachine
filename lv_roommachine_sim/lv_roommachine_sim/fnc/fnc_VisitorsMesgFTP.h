


#ifndef __FNC_VISITORSMESGFTP_H
#define __FNC_VISITORSMESGFTP_H


#if 0
#define VisitorsMesgFTP_P_DEBUG_ON		1

#define VisitorsMesgFTP_DEBUGE(fmt,arg...) 	do{\
                                         				if(VisitorsMesgFTP_P_DEBUG_ON)\
                                         					printf("<<-INTER>> "fmt"\n", ##arg);\
                                      				  }while(0)

#define VisitorsMesgFTP_ERROR(fmt,arg...) 			printf("<<-INTER->> "fmt"\n",##arg)			
#define VisitorsMesgFTP_INFO(fmt,arg...) 			printf("<<-INTER->> "fmt"\n",##arg)	



//  接收数据应答
int fnc_VisitorMesgFTP_FTP_ACK(unsigned char cmdtype,unsigned char ch);
int fnc_VisitorMesgFTP_FTP_Analysis(unsigned char *databuf,int datelen,int ch);
int fnc_VisitorMesgFTP_CreateUDPRECV(char *localIP, int LocalPort,int udp_ch);
int fnc_VisitorMesgFTP_INIT();
int fnc_udptestmain(); 


#endif




#endif
