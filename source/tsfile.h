#ifndef XIAOC_TS_FILE_H
#define XIAOC_TS_FILE_H
//����  �����  20160622  xiaoc@pku.edu.cn

#include <stdio.h>
#include "ts.h"


extern FILE *g_ts_file;  //TS ԭʼ�ļ�
extern FILE *g_ts_ts;    //��ӡÿ��ts����ϸ��Ϣ
extern FILE *g_ts_video; //�����Ƶ����Ϣ
extern FILE *g_ts_audio; //�����Ƶ����Ϣ
extern FILE *g_ts_out;   //����޸ĺ��ts��

extern char *ts_file_name;  //FLV ԭʼ�ļ�           -i ���� �磺-i brazil-bq.ts
extern char *ts_ts_name;    //��ӡÿ��ts����ϸ��Ϣ  -t ���� �磺-t brazil-bq.log
extern char *ts_video_name; //�����Ƶ����Ϣ         -v ���� �磺-v brazil-bq.264 
extern char *ts_audio_name; //�����Ƶ����Ϣ         -a ���� �磺-a brazil-bq.aac 
extern char *ts_out_name;   //����޸ĺ��ts��       -o ���� �磺-o brazil-new.ts 

extern int  g_errors;        //ͳ�ƴ������

void openFlvFiles();    //��ts����ļ�
void closeFlvFiles();   //�ر�ts����ļ�
#endif