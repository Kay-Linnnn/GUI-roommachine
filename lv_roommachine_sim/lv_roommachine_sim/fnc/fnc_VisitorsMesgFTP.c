
#if 0
#include<sys/types.h> 
#include<sys/socket.h> 
#include<unistd.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<errno.h> 
#include<netdb.h> 
#include<stdarg.h> 
#include<string.h> 
#include <pthread.h>
#include <sys/time.h> 
#include <signal.h>
#include <stdbool.h>
#include <fcntl.h>

#include <sys/stat.h>



#include "fnc_VisitorsMesgFTP.h"
#include "mtype.h"
#include "Communication_ErrorCode.h"
#include "datatype_fnc.h"

#pragma GCC diagnostic ignored "-Wstack-usage="


static char visitorMsgpath[100]={'\0'};

#define DIR_VisitorsMesgDIR				"VisitorsMes"

#define fnc_dev2devInteProtocol_MAX_UDP_SOCKETNUM	8
// =========================================================================================
//           �ļ����� �ְ��������ݽṹ
// =========================================================================================
#define udp_recevMaxDataLen				1024*61     

typedef struct
{
	unsigned int HeadID;
	unsigned short cmd;
	unsigned short datalen;
	// ���ݸ�ʽ
	unsigned int ident;  			// ָ���ʾ
	unsigned char deviceId[16]; 	// �豸ID
	unsigned char id[24]; 			//�ļ�ID(Ψһ��־)�����ļ������ã�������չ����û��\0��β
	int totalAmount;				// �ܰ���
	int whichPackage;				// ��ǰ��
}fnc_VistorsMesg_PackTpye;

// =========================================================================================
// �ļ����� �ְ����ճɹ�Ӧ��ṹ��
// =========================================================================================
typedef struct
{
	unsigned int ident;  		// ָ���ʾ
	unsigned char deviceId[16]; // �豸ID
	unsigned short code;  		// �ɹ�����0��ʧ�ܷ��ؾ��������
}fnc_VistorsMesg_ACK_DataTpye;

typedef struct
{
	unsigned int HeadID;
	unsigned short cmd;
	unsigned short datalen;
	fnc_VistorsMesg_ACK_DataTpye dataType;
	unsigned short checksum;
}fnc_VistorsMesg_ACK_PackTpye;

//========================================================================================= 
typedef struct 
{
	struct sockaddr_in addr; 
	socklen_t addr_length;
	struct sockaddr_in RemoteAddr; 
	socklen_t RemoteAddr_length;
	int fd;
}fnc_UdpRecv_CH_TYPE;
fnc_UdpRecv_CH_TYPE fnc_udpT[fnc_dev2devInteProtocol_MAX_UDP_SOCKETNUM];

static void temp_printf_recv_data(unsigned char *data, int data_len)
{
	printf("[temp_printf_recv_data]data_len:%d\n", data_len);
	int i = 0, j = 0;
	for(;data_len > 0; --data_len)
	{
		printf("0x%02x ", *(data+j));
		j++;
		i++;
		if( i == 16){
			printf("\n");
			i = 0;
			}
	}
	printf("\n");
}

//  UDP �����߳�
void *fnc_VisitorMesgFTP_UDPRecv_Thread(int ch)
{
	/* ����һ����ַ�����ڲ���ͻ��˵�ַ */
	int i,senddatalen;
	unsigned char *pdataAddr;
	unsigned short checksum;
	socklen_t addr_length = sizeof(fnc_udpT[ch].addr); 
	unsigned char buffer[udp_recevMaxDataLen]={0};
	int recev_len;
	struct sockaddr_in server_addr;
	int server_len;
	while(1) 
	{   
		/* �������� */
		printf("waiting data....\n");
		bzero(buffer, udp_recevMaxDataLen); 
		recev_len = recvfrom(fnc_udpT[ch].fd,buffer, udp_recevMaxDataLen,0,(struct sockaddr*)&fnc_udpT[ch].RemoteAddr, &fnc_udpT[ch].RemoteAddr_length);
		if(recev_len == -1) 
		{ 
			VisitorsMesgFTP_ERROR("Receive Data Failed:"); 
		} 
		else
		{
			temp_printf_recv_data(buffer, recev_len);
			VisitorsMesgFTP_DEBUGE("Receive Data len = %d",recev_len); 
			/*
			for(i=0;i<recev_len;i++)
			{
				printf("%x ",buffer[i]);	
			}
			printf("\n");
			*/
			fnc_VisitorMesgFTP_FTP_Analysis(&buffer,recev_len,ch);
		}
		usleep(1000);
	} 
}

//  ��������Ӧ��
int fnc_VisitorMesgFTP_FTP_ACK(unsigned char cmdtype,unsigned char ch)
{
	fnc_VistorsMesg_ACK_PackTpye ACKtype;
	int i,DataType_len;
	unsigned char *pdataAddr;
	unsigned short checksum;
	memset(&ACKtype,0,sizeof(fnc_VistorsMesg_ACK_PackTpye));
	ACKtype.HeadID = 0xBBAA5AA5;
	ACKtype.cmd = htons(16);
	ACKtype.datalen = htons(22);
	ACKtype.dataType.ident =0;
	for(i=0;i<16;i++)
	{
		ACKtype.dataType.deviceId[i] = 0;	
	}
	ACKtype.dataType.code = htons(cmdtype);  // �ɹ�����0��ʧ�ܷ��ؾ��������
	// ����У���
	DataType_len = sizeof(fnc_VistorsMesg_ACK_PackTpye)-4;
	ACKtype.checksum  = htons(datypefnc_x16_checksum_BigData((unsigned char*)&ACKtype,DataType_len));
	
	VisitorsMesgFTP_DEBUGE("ACKtype->checksum = %d",ACKtype.checksum);
	
	if(sendto(fnc_udpT[ch].fd,(unsigned char*)&ACKtype,DataType_len+2,0,(struct sockaddr*)&fnc_udpT[ch].RemoteAddr,fnc_udpT[ch].RemoteAddr_length) < 0) 
	{ 
		VisitorsMesgFTP_ERROR("Send File Name Failed:");  
		return -1;
	} 	
	return 0;
}

int fnc_VisitorMesgFTP_SaveData(fnc_VistorsMesg_PackTpye *dyp,unsigned char *databuf)
{	
#if 1
	unsigned char namestr[25]={0};
	int i =0;
	unsigned int datalen;
	FILE *fd;
	datalen = htons(dyp->datalen);
	VisitorsMesgFTP_DEBUGE("datalen:%d",datalen);
	if(dyp->whichPackage == 0)
	{
		VisitorsMesgFTP_DEBUGE("First Package");
		// ��ǰ������Ϊ��һ������ȡ�ļ��������ļ�
		for(i = 0;i<24;i++)
		{
			namestr[i] = dyp->id[i];
		}
		// �жϷÿ�������Ϣ�洢Ŀ¼�Ƿ���ڣ�û�д��ھʹ���
		if(access(DIR_VisitorsMesgDIR, NULL)!=0) 
		{
			VisitorsMesgFTP_DEBUGE("mkdir VisitorsMes\n");
			if(mkdir(DIR_VisitorsMesgDIR, 0755)==-1)  
			{
				VisitorsMesgFTP_ERROR("mkdir VisitorsMes error!\n");
				return -1;
			}
		}
		memset(visitorMsgpath,0,sizeof(visitorMsgpath));
		sprintf(visitorMsgpath,"%s/%s",DIR_VisitorsMesgDIR,namestr);
		VisitorsMesgFTP_DEBUGE("file path = %s",visitorMsgpath);
		fd=open(visitorMsgpath,O_RDWR|O_CREAT,0777);  		// �򿪲�����һ���ļ�
		if(fd == NULL)
		{
			VisitorsMesgFTP_ERROR("open file error!\n");
			return -1;	
		}
		lseek(fd,0,SEEK_SET);
		int w_len = write(fd,databuf+sizeof(fnc_VistorsMesg_PackTpye), datalen);//(dyp->datalen-52)
		VisitorsMesgFTP_DEBUGE("write len:%d", w_len);
		close(fd);
	}
	else
	{	
		VisitorsMesgFTP_DEBUGE("Package:%d",htons(dyp->whichPackage));
		if(htons(dyp->whichPackage) == htons(dyp->totalAmount))
		{	
			// ��������յ���Ϣ����
			
		}
		
		fd=open(visitorMsgpath,O_WRONLY);
		if(fd == NULL)
		{
			VisitorsMesgFTP_ERROR("open file error!\n");
			return -1;	
		}
		lseek(fd,0,SEEK_END);
		write(fd,databuf+sizeof(fnc_VistorsMesg_PackTpye),datalen);
		close(fd);		
	}
#endif
	return 0;
}


int fnc_VisitorMesgFTP_FTP_Analysis(unsigned char *databuf,int datelen,int ch)
{
	fnc_VistorsMesg_PackTpye datatype;
	int i,DataType_len;
	unsigned short checksum,data_checksum;
	memset(&datatype,0,sizeof(fnc_VistorsMesg_PackTpye));
	
	memcpy((unsigned char*)&datatype,databuf,sizeof(fnc_VistorsMesg_PackTpye));
	
	// 1 �ж�֡ͷ���ݸ�ʽ�Ƿ���ȷ
	if(datatype.HeadID == 0xBBAA5AA5)
	{
		//2 �ж��������Ƿ���ȷ
		if(datatype.cmd == htons(16))
		{
			//3 ������յ����ݵ�У���
			checksum = datypefnc_x16_checksum_BigData(databuf,datelen-2);  
			VisitorsMesgFTP_DEBUGE("js checksum = %x",checksum);
			// 4 �ж�У����Ƿ���ȷ
			// ��ȡ��ԭ���ݵ�У����
			APP_UINT16 u16_u8;
			u16_u8.uic[0] = databuf[datelen-1];
			u16_u8.uic[1] = databuf[datelen-2];	
			data_checksum = u16_u8.uivar;
			VisitorsMesgFTP_DEBUGE("data_checksum = %x",data_checksum);
			
			if(data_checksum == checksum)   // �ж�У����Ƿ���ȷ
			{
				//  5 ���ݴ洢
				if(fnc_VisitorMesgFTP_FTP_ACK(0,ch) == 0)
				{
					if(fnc_VisitorMesgFTP_SaveData(&datatype,databuf) <0)
					{
						VisitorsMesgFTP_ERROR("data checksum error!\n");
						fnc_VisitorMesgFTP_FTP_ACK(CERCD_INVALID_INSTRUCTION_TYPE,ch);
					}
				}
			}
			else //checksum error
			{
				VisitorsMesgFTP_ERROR("data checksum error!\n");
				fnc_VisitorMesgFTP_FTP_ACK(CERCD_INVALID_INSTRUCTION_TYPE,ch);
			}
		}
		else //cmd error
		{
			VisitorsMesgFTP_ERROR("data CMD error!\n");
			fnc_VisitorMesgFTP_FTP_ACK(CERCD_CHECK_SUM_ERR,ch);
		}
	}	
	else //head error
	{
		VisitorsMesgFTP_ERROR("data HEAD error!\n");
		fnc_VisitorMesgFTP_FTP_ACK(CERCD_MISS_OR_ABNORMAL_INSTRUCTION_PARAM,ch);
	}
	return 0;
}


/*
localIP   : ����IP    
LocalPort  :�̶���6070


*/
int fnc_VisitorMesgFTP_CreateUDPRECV(char *localIP, int LocalPort,int udp_ch)
{
	int ret;
	int reuse = 1;
	pthread_t Thread_ID = 0;
	/* ����UDP�׽ӿ� */
	bzero(&fnc_udpT[udp_ch].addr, sizeof(fnc_udpT[udp_ch].addr)); 
	fnc_udpT[udp_ch].addr.sin_family = AF_INET; 
	fnc_udpT[udp_ch].addr.sin_addr.s_addr = inet_addr(localIP);; 
	fnc_udpT[udp_ch].addr.sin_port = htons(LocalPort); 
	/* ����socket */
	fnc_udpT[udp_ch].fd = socket(AF_INET, SOCK_DGRAM, 0); 
	if(fnc_udpT[udp_ch].fd  < 0) 
	{ 
		VisitorsMesgFTP_ERROR("Create Socket Failed!"); 
		return -1;
	} 
	setsockopt(fnc_udpT[udp_ch].fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	/* ���׽ӿ� */
	if(-1 == (bind(fnc_udpT[udp_ch].fd,(struct sockaddr*)&fnc_udpT[udp_ch].addr,sizeof(fnc_udpT[udp_ch].addr)))) 
	{ 
		VisitorsMesgFTP_ERROR("sockit Bind Failed:"); 
		return -1;
	}
	//����һ��UDP socket �����߳�
	ret = pthread_create(&Thread_ID,NULL,(void *)fnc_VisitorMesgFTP_UDPRecv_Thread,udp_ch);
	return 0;
}




//=================================================================================================

//
//											������
//
//=================================================================================================

//  UDP �������ݳ�ʼ���� ����������ʼ����ʱ�����
int fnc_VisitorMesgFTP_INIT()
{
	bzero(&fnc_udpT, sizeof(fnc_udpT)); 
	fnc_VisitorMesgFTP_CreateUDPRECV("192.168.24.19",6070,0);;
	return 0;
}

char toAscII(int i)
{
	return i+'0';
}

int SaveData_test()
{	
	unsigned char databuf[24]="1234567 1234567 1234567 ";
	int i =0;
	FILE *fd;
	sprintf(visitorMsgpath,"%s/%s",DIR_VisitorsMesgDIR,"a.txt");
	// �ж��ļ�Ŀ¼�Ƿ����
	if(access(DIR_VisitorsMesgDIR, NULL)!=0) 
	{
		printf("mkdir \n");
		if(mkdir(DIR_VisitorsMesgDIR, 0755)==-1)  
		{
			printf("mkdir error\n");	
		}
	}
	fd=open(visitorMsgpath,O_RDWR|O_CREAT,0777);  // �򿪲�����һ���ļ�
	if(fd == NULL)
	{
		printf("open file error 1\n");
		return -1;	
	}
	lseek(fd,0,SEEK_SET);
	write(fd,databuf,sizeof(databuf));
	close(fd);
	usleep(1000*500);
	fd=open(visitorMsgpath,O_RDWR);
	if(fd == NULL)
	{
		printf("open file error 2\n");
		return -1;	
	}
	lseek(fd,0,SEEK_END);	
	for(i =0;i<24;i++)
	{
		write(fd,databuf,sizeof(databuf));	
	}
	close(fd);
	usleep(1000*500);
	fd=open(visitorMsgpath,O_RDWR);
	lseek(fd,0,SEEK_END);	
	write(fd,"abcdefg",7);	
	close(fd);
	return 0;
}


int fnc_udptestmain() 
{	
	int i;
	char c;
	unsigned short xx;
	unsigned int data_int = 0xBBAA5AA5;
	unsigned char *u8addr;
	/*
	unsigned char name[11]={48,49,50,51,52,53,54,55,56,57,0};
	char strtab[10]={"\0"};
	char ascii_str[2];
	char namestr[100];
	printf("%c ",48);
	for(i =0;i<10;i++)
	{
		// c = toAscII(i);	
		// printf("%c ",c);
		printf("%c ",name[i]); 
		//sprintf(strtab[i],"%c",name[i]);
	}
	sprintf(namestr,"%s/%s",DIR_VisitorsMesgDIR,name);
	printf("str = %s\n",namestr);
	//SaveData_test();
	*/
	fnc_VisitorMesgFTP_INIT();
	return 0;
}


#endif
