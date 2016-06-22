//����  �����  20160622  xiaoc@pku.edu.cn

#include <stdio.h>
#include <stdlib.h>
#include "tsfile.h"
#include "ts.h"

/* ��ӡhelp��Ϣ */
static void showHelp()
{
#define H0 printf
	H0("\n-h ��ӡ����������Ϣ���˳�\n");
	H0("\n-i ���������ļ� �磺-i brazil-bq.ts\n");
	H0("\n-t ���������־ �磺-t brazil-bq.log\n");
	H0("\n-v ���������Ƶ �磺-v brazil-bq.264\n");
	H0("\n-a ���������Ƶ �磺-a brazil-bq.aac\n");
	H0("\n-o �������TS   �磺-o brazil-new.ts\n");
}
/* ����ts��Ĭ�ϲ��� */
void ts_param_default(ts_param *param)
{
	param->b_ts    = 1;
	param->b_video = 0;
	param->b_audio = 0;    
	param->b_nts   = 0;
}

/* �������ò��� */
bool parse(ts_param *param,int argc, char **argv)
{
	if (argc <= 1)
	{
		printf("û���κ����ò���\n");
		return false;
	}

	for(int i = 1; i < argc; i += 2)
	{
		if(argv[i][0] != '-')
		{
			printf("������������:%s\n",argv[i]);
			return false;
		}
		switch(argv[i][1])
		{
		case 'h':showHelp();system("pause\n");exit(0);          break;
		case 'i':ts_file_name = argv[i+1];                     break;
		case 't':ts_ts_name = argv[i+1];  param->b_ts     = 1; break;
		case 'v':ts_video_name = argv[i+1];param->b_video = 1; break;
		case 'a':ts_audio_name = argv[i+1];param->b_audio = 1; break;
		case 'o':ts_out_name = argv[i+1];  param->b_nts   = 1; break;
		default:printf("������������:%s\n",argv[i]); return false;
		}
	}
	return true;
}
