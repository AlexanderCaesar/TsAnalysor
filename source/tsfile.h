#ifndef XIAOC_TS_FILE_H
#define XIAOC_TS_FILE_H
//作者  蔡砚刚  20160622  xiaoc@pku.edu.cn

#include <stdio.h>
#include "ts.h"


extern FILE *g_ts_file;  //TS 原始文件
extern FILE *g_ts_ts;    //打印每个ts的详细信息
extern FILE *g_ts_video; //输出视频流信息
extern FILE *g_ts_audio; //输出音频流信息
extern FILE *g_ts_out;   //输出修改后的ts流

extern char *ts_file_name;  //FLV 原始文件           -i 配置 如：-i brazil-bq.ts
extern char *ts_ts_name;    //打印每个ts的详细信息  -t 配置 如：-t brazil-bq.log
extern char *ts_video_name; //输出视频流信息         -v 配置 如：-v brazil-bq.264 
extern char *ts_audio_name; //输出音频流信息         -a 配置 如：-a brazil-bq.aac 
extern char *ts_out_name;   //输出修改后的ts流       -o 配置 如：-o brazil-new.ts 

extern int  g_errors;        //统计错误个数

void openFlvFiles();    //打开ts相关文件
void closeFlvFiles();   //关闭ts相关文件
#endif