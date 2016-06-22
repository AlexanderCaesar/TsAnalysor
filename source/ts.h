#ifndef XIAOC_TS_H
#define XIAOC_TS_H
//����  �����  20160622  xiaoc@pku.edu.cn

typedef struct ts_param
{
    /* ������Ϣ
    -i ���� �磺-i brazil-bq.ts
    -t ���� �磺-t brazil-bq.log
    -v ���� �磺-v brazil-bq.264
    -a ���� �磺-a brazil-bq.aac
    **/
    int  b_ts;    //�Ƿ��ӡÿ��ts����ϸ��Ϣ   -t ���� �磺-t brazil-bq.log
    int  b_video; //�Ƿ������Ƶ����Ϣ         -v ���� �磺-v brazil-bq.264 
    int  b_audio; //�Ƿ������Ƶ����Ϣ         -a ���� �磺-a brazil-bq.aac 
    int  b_nts;   //�Ƿ�����µ�ts�ļ�         -o ���� �磺-o brazil-new.ts

}ts_param;

/* ����ts��Ĭ�ϲ��� */
void ts_param_default(ts_param *param);

/* �������ò��� */
bool parse(ts_param *param,int argc, char **argv);

#endif
