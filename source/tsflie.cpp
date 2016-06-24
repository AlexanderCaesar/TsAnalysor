//����  �����  20160622  xiaoc@pku.edu.cn
#include <stdlib.h>
#include "tsfile.h"

#pragma warning(disable: 4996) // POSIX setmode and fileno deprecated

FILE *g_ts_file  = NULL;    //ts ԭʼ�ļ�
FILE *g_ts_ts    = NULL;    //��ӡÿ��ts����ϸ��Ϣ
FILE *g_ts_video = NULL;    //�����Ƶ����Ϣ
FILE *g_ts_audio = NULL;    //�����Ƶ����Ϣ
FILE *g_ts_out   = NULL;    //����޸ĺ��ts��

char *ts_file_name  = NULL; //ts ԭʼ�ļ�            -i ���� �磺-i brazil-bq.ts
char *ts_ts_name   = NULL;  //��ӡÿ��ts����ϸ��Ϣ   -t ���� �磺-t brazil-bq.log
char *ts_video_name = NULL; //�����Ƶ����Ϣ         -v ���� �磺-v brazil-bq.264 
char *ts_audio_name = NULL; //�����Ƶ����Ϣ         -a ���� �磺-a brazil-bq.aac 
char *ts_out_name   = NULL; //����޸ĺ��ts��       -o ���� �磺-o brazil-new.ts

int  g_errors        = 0;    //ͳ�ƴ������

//��ts����ļ�
void openFlvFiles()    
{
	if(ts_file_name)
	{
		g_ts_file = fopen(ts_file_name,"rb");
		if(!g_ts_file)
		{
			printf("���ļ�ʧ��: %s\n",ts_file_name);
			system("pause");
			exit(0);
		}
	}
	if(ts_ts_name)
	{
		g_ts_ts = fopen(ts_ts_name,"w");
		if(!g_ts_ts)
		{
			printf("���ļ�ʧ��: %s\n",ts_ts_name);
			system("pause");
			exit(0);
		}
	}

	if(ts_video_name)
	{
		g_ts_video = fopen(ts_video_name,"wb");
		if(!g_ts_video)
		{
			printf("���ļ�ʧ��: %s\n",ts_video_name);
			system("pause");
			exit(0);
		}
	}

	if(ts_audio_name)
	{
		g_ts_audio = fopen(ts_audio_name,"wb");
		if(!g_ts_audio)
		{
			printf("���ļ�ʧ��: %s\n",ts_audio_name);
			system("pause");
			exit(0);
		}
	}

	if(ts_out_name)
	{
		g_ts_out = fopen(ts_out_name,"wb");
		if(!g_ts_out)
		{
			printf("���ļ�ʧ��: %s\n",ts_out_name);
			system("pause");
			exit(0);
		}
	}
}

//�ر�ts����ļ�
void closeFlvFiles()   
{
	if(g_ts_file)
		fclose(g_ts_file);
	if(g_ts_ts)
		fclose(g_ts_ts);
	if(g_ts_video)
		fclose(g_ts_video);
	if(g_ts_audio)
		fclose(g_ts_audio);
	if(g_ts_out)
		fclose(g_ts_out);
}