//作者  蔡砚刚  20160622  xiaoc@pku.edu.cn

#include <stdio.h>
#include <stdlib.h>
#include "tsfile.h"
#include "ts.h"

/* 打印help信息 */
static void showHelp()
{
#define H0 printf
	H0("\n-h 打印参数配置信息并退出\n");
	H0("\n-i 配置输入文件 如：-i brazil-bq.ts\n");
	H0("\n-t 配置输出日志 如：-t brazil-bq.log\n");
	H0("\n-v 配置输出视频 如：-v brazil-bq.264\n");
	H0("\n-a 配置输出音频 如：-a brazil-bq.aac\n");
	H0("\n-o 配置输出TS   如：-o brazil-new.ts\n");
}
/* 设置ts的默认参数 */
void ts_param_default(ts_param *param)
{
	param->b_ts    = 1;
	param->b_video = 0;
	param->b_audio = 0;    
	param->b_nts   = 0;
}

/* 解析配置参数 */
bool parse(ts_param *param,int argc, char **argv)
{
	if (argc <= 1)
	{
		printf("没有任何配置参数\n");
		return false;
	}

	for(int i = 1; i < argc; i += 2)
	{
		if(argv[i][0] != '-')
		{
			printf("参数配置有误:%s\n",argv[i]);
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
		default:printf("参数配置有误:%s\n",argv[i]); return false;
		}
	}
	return true;
}
