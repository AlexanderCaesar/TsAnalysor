//作者  蔡砚刚  20160622  xiaoc@pku.edu.cn
#include <stdlib.h>
#include "tsfile.h"

#pragma warning(disable: 4996) // POSIX setmode and fileno deprecated

FILE *g_ts_file  = NULL;    //ts 原始文件
FILE *g_ts_ts    = NULL;    //打印每个ts的详细信息
FILE *g_ts_video = NULL;    //输出视频流信息
FILE *g_ts_audio = NULL;    //输出音频流信息
FILE *g_ts_out   = NULL;    //输出修改后的ts流

char *ts_file_name  = NULL; //ts 原始文件            -i 配置 如：-i brazil-bq.ts
char *ts_ts_name   = NULL;  //打印每个ts的详细信息   -t 配置 如：-t brazil-bq.log
char *ts_video_name = NULL; //输出视频流信息         -v 配置 如：-v brazil-bq.264 
char *ts_audio_name = NULL; //输出音频流信息         -a 配置 如：-a brazil-bq.aac 
char *ts_out_name   = NULL; //输出修改后的ts流       -o 配置 如：-o brazil-new.ts

int  g_errors        = 0;    //统计错误个数

//打开ts相关文件
void openFlvFiles()    
{
	if(ts_file_name)
	{
		g_ts_file = fopen(ts_file_name,"rb");
		if(!g_ts_file)
		{
			printf("打开文件失败: %s\n",ts_file_name);
			system("pause");
			exit(0);
		}
	}
	if(ts_ts_name)
	{
		g_ts_ts = fopen(ts_ts_name,"w");
		if(!g_ts_ts)
		{
			printf("打开文件失败: %s\n",ts_ts_name);
			system("pause");
			exit(0);
		}
	}

	if(ts_video_name)
	{
		g_ts_video = fopen(ts_video_name,"wb");
		if(!g_ts_video)
		{
			printf("打开文件失败: %s\n",ts_video_name);
			system("pause");
			exit(0);
		}
	}

	if(ts_audio_name)
	{
		g_ts_audio = fopen(ts_audio_name,"wb");
		if(!g_ts_audio)
		{
			printf("打开文件失败: %s\n",ts_audio_name);
			system("pause");
			exit(0);
		}
	}

	if(ts_out_name)
	{
		g_ts_out = fopen(ts_out_name,"wb");
		if(!g_ts_out)
		{
			printf("打开文件失败: %s\n",ts_out_name);
			system("pause");
			exit(0);
		}
	}
}

//关闭ts相关文件
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