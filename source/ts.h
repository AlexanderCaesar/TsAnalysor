#ifndef XIAOC_TS_H
#define XIAOC_TS_H
//作者  蔡砚刚  20160622  xiaoc@pku.edu.cn

typedef struct ts_param
{
    /* 配置信息
    -i 配置 如：-i brazil-bq.ts
    -t 配置 如：-t brazil-bq.log
    -v 配置 如：-v brazil-bq.264
    -a 配置 如：-a brazil-bq.aac
    **/
    int  b_ts;    //是否打印每个ts的详细信息   -t 配置 如：-t brazil-bq.log
    int  b_video; //是否输出视频流信息         -v 配置 如：-v brazil-bq.264 
    int  b_audio; //是否输出音频流信息         -a 配置 如：-a brazil-bq.aac 
    int  b_nts;   //是否输出新的ts文件         -o 配置 如：-o brazil-new.ts

}ts_param;

/* 设置ts的默认参数 */
void ts_param_default(ts_param *param);

/* 解析配置参数 */
bool parse(ts_param *param,int argc, char **argv);

#endif
