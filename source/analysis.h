#ifndef XIAOC_ANALYSIS_H
#define XIAOC_ANALYSIS_H
//作者  蔡砚刚  20160622  xiaoc@pku.edu.cn
#include "ts.h"
#include "tsfile.h"

#define  PROGRAM_STREAM_MAP       0xBC
#define  PRIVATE_STREAM1          0xBD
#define  PADDING_STREAM           0xBE
#define  PRIVATE_STREAM2          0xBF

#define  ECM                      0xF0
#define  EMM                      0xF1
#define  DSMCC_STREAM             0xF2

#define  H222_1_E                 0xF8

#define  PROGRAM_STREAM_DIRECTORY 0xFF

struct TS
{
	unsigned char data[188];//TS 数据包
	int count;              //指示当前TS读取到的位置
	int left;               //指示指示当前位置还剩余多少位
};

//TS的包头占用四个字节,含有同步字节、PID、起始位等信息
struct TsHeader
{
	unsigned char  sync_byte;                   //为固定的8 比特字段，其值为'0100 0111' (0x47)。
	/*为1 比特标志。置于1 时，它指示在相关传输流包中至少存在1 
	个不可校正比特错。此比特可以由传输层以外的实体设置为‘1’。
	设置为‘1’时，此比特应不重新设置为‘0’，除非误差比特值
	已经校正。	**/
	unsigned char  transport_error_indicator;   //错误标识
	unsigned char  payload_unit_start_indicator;//有效载荷单元起始符:1 表示新的一帧的开始
	/*
	为1 比特指示符。设置为‘1’时，它指示该相关包比具有相同PID
	但不具有该比特设置为1 的其他包有更大的优先级。传输机制可以
	使用该字段优先考虑基本流内的该包数据。取决于应用，transport
	_priority 字段可以不管PID或者此字段仅在一个PID 范围内编码。
	此字段可以由信道特定编码器或解码器来改变。
	**/
	unsigned char  transport_priority;         //优先级   

	/*PID 值0x0000 为节目相关表所保留  PAT
	  PID 值0x0001 为有条件访问表所保留
	  PID 值0x0002 为传输流描述表所保留
	  PID 值0x0003 为IPMP 控制信息表（见ISO/IEC 13818-11），
	  PID 值0x0004-0x000F 为保留的值。
	  PID 值0x0010-0x1FFE 可指派为 network_PID, Program_map_PID, elementary_PID, 或为了其他目的
	  PID 值0x1FFF 为空包所保留。	**/
	unsigned int   PID;                        //PID 为13 比特字段，指示包有效载荷中存储的数据类型。
	/*传输流包头以及自适应字段若存在，应不加扰。
	  在空包的情况中，transport_scrambling_control 字段的值应设置为“00”
	  00 不加扰
	  01 用户定义
	  10 用户定义
	  11 用户定义
	**/
	unsigned char  transport_scrambling_control;//此2 比特字段指示传输流包有效载荷的加扰方式
	/* ITU-T H.222.0 建议书|ISO/IEC 13818-1 解码器应丢弃具有adaptation_field_control 字段设置为
	  ‘00’值的传输流包。在空包的情况中，adaptation_field_control 的赋值应为‘01’。
	    00 供未来使用，由ISO/IEC 所保留
	    01 无 adaptation_field，仅有效载荷
	    10 仅有Adaptation_field，无有效载荷
	    11 Adaptation_field 后随有效载荷
	**/
	unsigned char  adaptation_field_control;   //此2 比特字段指示此传输流包头是否后随自适应字段和/或有效载荷
	/*continuity_counter 为4 比特字段，随着具有相同PID 的每个传输流包而增加。
	  continuity_counter 在取其最大值之后循环返回到0 值。当包的adaptation_field_control 为‘00’或‘10’时，
	  continuity_counter 不增加。
	  传输流中，复制包可以作为2 个并且仅只作为2 个具有相同PID 的连续传输流包。该复制包必须有
	  与原始包相同的 continuity_counter 赋值并且 adaptation_field_control 字段必须等于‘01’或‘11’。复制
	  包中，原始包的每个字节都必须复制，有一种情况除外，即节目时钟参考字段中的内容若存在，该有效
	  值必须编码。
	  特定传输流包中continuity_counter 是连续的，只要它与具有相同PID 的先前的传输流包中的
	  continuity_counter 赋值的差为正值1，或者任意一个非增量条件（adaptation_field_control 设置为‘00’或
	  ‘10’，或如上所述的复制包）被满足时。discontinuity_indicator 设置为‘1’时，连续性计数器可以中断（参
	  阅2.4.3.4）。在空包的情况中，continuity_counter 值未确定。
	**/
	unsigned char  continuity_counter;         //随着具有相同PID 的每个传输流包而增加。
};

struct TSPAT
{
	/*table_id 字段标识传输流PSI 分段的内容
	0x00 program_association_section（PAT）
	0x01 conditional_access_section (CA_section)
	0x02 TS_program_map_section PMT
	0x03 TS_description_section
	0x04 ISO_IEC_14496_scene_description_section
	0x05 ISO_IEC_14496_object_descriptor_section
	0x06 Metadata_section
	0x07 IPMP_Control_Information_section (defined in ISO/IEC 13818-11)
	0x08-0x3F ITU-T H.222.0 建议书 | ISO/IEC 13818-1 保留
	0x40-0xFE 用户专用
	0xFF 禁用 **/
	unsigned char  table_id;                //此为8 比特字段，应设置为0x00
	unsigned char  section_syntax_indicator;//section_syntax_indicator 为1 比特字段，应设置为‘1’。
	unsigned char  zero0;                   //固定值0
	unsigned char  reserved0;               //保留字段
	/*此为12 比特字段，该字段的头两比特必为‘00’，剩余10 比特指定该分段的字节数，
	  紧随分段长度字段开始，并包括CRC。此字段中，该值应不超过1021（0x3FD）。**/
	unsigned int   section_length;          //长度
	unsigned int   transport_stream_id;     //此为16 比特字段，该字段充当标签，标识网络内此传输流有别于任何其他多路复用流。其值由用户规定。
	unsigned char  reserved1;               //保留字段
	/*此5 比特字段为整个节目相关表的版本号。当节目相关表的定义改变时，版本号
	应增1 模32。current_next_indicator 设置为‘1’时，version_number 必须为当前有效的节目相关表的版本号。
	current_next_indicator 设置为‘0’时，version_number 必须为下一个有效的节目相关表的版本号。	**/
	unsigned char  version_number;          //版本号
	/*1 比特指示符，置于‘1’时指示发送的节目相关表为当前有效的。该比特
	设置为‘0’时，它指示发送的该表尚未有效并且下一个表将生效	**/
	unsigned char  current_next_indicator;  //生效位
	/*此8 比特字段给出此分段的编号。节目相关表中首分段的section_number 
	必须为	0x00。随着节目相关表中每个增加的分段它应增1。**/
	unsigned char  section_number;          //分段号
	/*此8比特字段指定完整节目相关表的最后分段编号（即具有最高section_number
	的分段）。	**/
	unsigned char  last_section_number;     //最后分段号
	unsigned char  N;                       //计数节目个数
	/* Program_number 为16 比特字段。它指定program_map_PID 所适用的节目。置于
	0x0000 时，后随的PID 参考必为网络PID。对于所有其他情况，此字段的值由用户规定。
	在节目相关表的	一个版本内，此字段应不只一次地取任何非单一值。
	注 — 例如，可以把 program_number 用做为广播信道的标识。**/
	unsigned int  program_number[12];      //节目号
	unsigned char reserved[12];            //保留字段
	/*network_PID 为13 比特字段，仅同设置为0x0000 值的 program_number 
	一起使用，	指定应包含网络信息表的传输流包的PID。network_PID 字段的值由用户自定义，
	但仅限于取表中所指定的值。network_PID 的存在是任选的。	**/
	unsigned int  network_PID[12];             //包含网络信息表的传输流包的PID
	/*program_map_PID 为13 比特字段，对于由program_number 所指定的节目而言，
	指定应包含program_map_section 应用的传输流包的PID 。任何program_number 都应没有多个
	program_map_PID 赋值。program_map_PID 的值由用户定义，但仅限于取表2-3 中所指定的值。
	**/
	unsigned int  program_map_PID[12];         //PMT
	/*此为32 比特字段，包含处理全部节目相关分段后**/
	unsigned int  CRC_32;                      //CRC
};

struct TSPMT
{
	/*table_id 字段标识传输流PSI 分段的内容
	0x00 program_association_section（PAT）
	0x01 conditional_access_section (CA_section)
	0x02 TS_program_map_section PMT
	0x03 TS_description_section
	0x04 ISO_IEC_14496_scene_description_section
	0x05 ISO_IEC_14496_object_descriptor_section
	0x06 Metadata_section
	0x07 IPMP_Control_Information_section (defined in ISO/IEC 13818-11)
	0x08-0x3F ITU-T H.222.0 建议书 | ISO/IEC 13818-1 保留
	0x40-0xFE 用户专用
	0xFF 禁用 **/
	unsigned char  table_id;                //此为8 比特字段，应设置为0x02
	unsigned char  section_syntax_indicator;//section_syntax_indicator 为1 比特字段，应设置为‘1’。
	unsigned char  zero0;                   //固定值0
	unsigned char  reserved0;               //保留字段
	/*此为12 比特字段，该字段的头两比特必为‘00’，剩余10 比特指定该分段的字节数，
	  紧随分段长度字段开始，并包括CRC。此字段中，该值应不超过1021（0x3FD）。**/
	unsigned int   section_length;          //长度
    /* Program_number 为16 比特字段。它指定program_map_PID 所适用的节目。置于
	0x0000 时，后随的PID 参考必为网络PID。对于所有其他情况，此字段的值由用户规定。
	在节目相关表的	一个版本内，此字段应不只一次地取任何非单一值。
	注 — 例如，可以把 program_number 用做为广播信道的标识。**/
	unsigned int  program_number;          //节目号
    /*此5 比特字段为整个节目相关表的版本号。当节目相关表的定义改变时，版本号
	应增1 模32。current_next_indicator 设置为‘1’时，version_number 必须为当前有效的节目相关表的版本号。
	current_next_indicator 设置为‘0’时，version_number 必须为下一个有效的节目相关表的版本号。	**/
	unsigned char  reserved1;               //保留字段
	unsigned char  version_number;          //版本号
	/*1 比特指示符，置于‘1’时指示发送的节目相关表为当前有效的。该比特
	设置为‘0’时，它指示发送的该表尚未有效并且下一个表将生效	**/
	unsigned char  current_next_indicator;  //生效位
	unsigned char  section_number;          //此8 比特字段值必为0x00。
	unsigned char  last_section_number;     //此8 比特字段值必为0x00。
	unsigned char  reserved2;               //保留字段
	/*此为13 比特字段，指示传输流包的PID，该传输流包应包含对于program_number 所指
	定节目有效的PCR 字段。若任何PCR 均与专用流的节目定义无关，则此字段应取0x1FFF 值
	**/
	unsigned int   PCR_PID;                 //PCR
	unsigned char  reserved3;               //保留字段
	/*此为12 比特字段，该字段的头两比特必为‘00’，剩余10 比特指定紧随
	program_info_length 字段的描述符的字节数。**/
	unsigned int   program_info_length;     //描述字节数目
	unsigned char  N;                       //描述数据个数
	unsigned char  N1;                      //计数音频或者视频流个数
	/*此为8比特字段，指示具有PID值的包内承载的节目元类型，其PID值由elementary_PID
	所指定。stream_type 值在表中指定。
	0x00 ITU-T | ISO/IEC 保留
	0x01 ISO/IEC 11172-2 视频
	0x02 ITU-T H.262 建议书 | ISO/IEC 13818-2 视频或 ISO/IEC 11172-2 受限参数视频流
	0x03 ISO/IEC 11172-3 音频
	0x04 ISO/IEC 13818-3 音频
	0x05 ITU-T H.222.0 建议书 | ISO/IEC 13818-1 private_sections
	0x06 ITU-T H.222.0 建议书 | ISO/IEC 13818-1 PES 包含专用数据的PES 包
	0x07 ISO/IEC 13522 MHEG
	0x08 ITU-T H.222.0 建议书 | ISO/IEC 13818-1 附件A DSM-CC
	0x09 ITU-T H.222.1 建议书
	0x0A ISO/IEC 13818-6 类型 A
	0x0B ISO/IEC 13818-6 类型 B
	0x0C ISO/IEC 13818-6 类型 C
	0x0D ISO/IEC 13818-6 类型 D
	0x0E ITU-T H.222.0 建议书 | ISO/IEC 13818-1 辅助
	0x0F 具有ADTS 传输句法的ISO/IEC 13818-7 音频
	0x10 ISO/IEC 14496-2 可视
	0x11 具有LATM 传输句法的ISO/IEC 14496-3 音频，如ISO/IEC 14496-3 所规定
	0x12 PES 包中承载的ISO/IEC 14496-1 SL 包式流或FlexMux 流
	0x13 ISO/IEC 14496_ sections 中承载的ISO/IEC 14496-1 SL 包式流或FlexMux 流
	0x14 ISO/IEC 13818-6 同步下载协议
	0x15 PES 包中承载的元数据
	0x16 metadata_sections 中承载的元数据
	0x17 ISO/IEC 13818-6 数据转盘式磁带中承载的元数据
	0x18 ISO/IEC 13818-6 目标转盘式磁带中承载的元数据
	0x19 ISO/IEC 13818-6 同步下载协议中承载的元数据
	0x1A IPMP 流 （ISO/IEC 13818-11, MPEG-2 IPMP 中规定）
	0x1B ITU-T H.264 建议书 | ISO/IEC 14496-10 视频中定义的AVC 视频流
	0x24 ITU-T H.265 建议书
	0x1C-0x7E ITU-T H.222.0 建议书 | ISO/IEC 13818-1 保留
	0x7F IPMP 流
	0x80-0xFF 用户专用
	**/
	unsigned char  stream_type[12];         //流类型
	unsigned char  reservedN1_0[12];        //保留字段
	unsigned int   elementary_PID[12];      //此为13 比特字段，指定承载相关节目元的传输流包的PID。
	unsigned char  reservedN1_1[12];        //保留字段
	/*此为12比特字段，该字段的头两比特必为‘00’，剩余10比特指示紧随ES_info_length
	字段的相关节目元描述符的字节数**/
	unsigned int   ES_info_length[12];      //ES长度
	/*此为32 比特字段，包含处理全部节目相关分段后**/
	unsigned int  CRC_32;                      //CRC
};

/* 自适应字段adaptation_field*/
//功能如果当前NAL单元不够188字节，前面会用自适应字段填充
struct TSADT
{
	/*adaptation_field_length 为指定紧随adaptation_field_length 的adaptation_field
	中的字节数的8 比特字段。0 值表示传输流包中插入单个填充字节。当adaptation_field_
	control 值为‘11’时，adaptation_field_length 值必须在0 到182 的区间内。当adapt
	ation_field_control 值为‘10’时，adaptation_field_length 值必须为183。对于承载
	PES 包的传输流包，只要存在欠充足的PES 包数据就需要	通过填充来完全填满传输流包的
	有效载荷字节。填充通过规定自适应字段长度比自适应字段中数据元的长度总和还要长来实
	现，以致于自适应字段在完全容纳有效的PES 包数据后，有效载荷字节仍有剩余。自适
	应字段中额外空间采用填充字节填满	**/
	unsigned char adaptation_field_length;//自适应字段长度

	/*此为1 比特字段，置于‘1’时，指示当前传输流包的不连续性状态为真。当discontinuity
	_indicator 设置为‘0’或不存在时，不连续性状态为假。不连续性指示符用于指示两种类型
	的不连续性，系统时间基不连续性和continuity_counter 不连续性。
	**/
	unsigned char discontinuity_indicator;//不连续性状态

	/*random_access_indicator 为1 比特字段，指示当前的传输流包以及可能的
	具有相同PID 的后续传输流包，在此点包含有助于随机接入的某些信息。
	特别的，该比特置于‘1’时，在具有当前PID 的传输流包的有效载荷中起始的下一个PES 包必须包含
	一个discontinuity_indicator 字段中规定的基本流接入点。此外，在视频情况中，显示时间标记必
	须在跟随	基本流接入点的第一图像中存在。
	音频帧的首字节，只要PES 流类型为3 或4。此外，在视频情况中，显示时间标记必须在包含序列头
	的第一图像的PES 包中存在。在PCR_PID 中random_access_indicator 在包含PCR 字段的传输流包中仅可设
	置为‘1’**/
	unsigned char random_access_indicator;

	/*elementary_stream_priority_indicator 为1 比特字段。在具有	相同PID 的包之间，它指示此传输流
	包有效载荷内承载的基本流数据的优先级。‘1’指示该有效载荷具有	比其他传输流包有效载荷更高的
	优先级。
	在ISO/IEC 11172-2 或 ITU-T H.262 建议书 | ISO/IEC 13818-2 或 ISO/IEC 14496-2 视频情况中，该字段
	可设置为‘1’，仅当该有效载荷包含一个或多个来自内编码截面的字节。
	在ITU-T H.264 建议书 | ISO/IEC 14496-10 视频情况中，该字段可设置为‘1’，仅当该有效载荷包含来
	自slice_type 置为2, 4, 7 或 9 的截面的一个或多个字节。
	‘0’值指示该有效载荷具有同所有不具备此比特置于‘1’的其他包一样的优先级。
	**/
	unsigned char elementary_stream_priority_indicator;

	/*PCR_flag 为1 比特标志。‘1’值指示adaptation_field 包含以两部分编码的PCR 字段。
	‘0’值指示自适应字段不包含任何PCR 字段。
	**/
	unsigned char PCR_flag;

	/*OPCR_flag 标志为1 比特标志。‘1’值指示adaptation_field 包含以两部分编码的OPCR
	字段。‘0’值指示自适应字段不包含任何OPCR 字段。**/
	unsigned char OPCR_flag;

	/*splicing_point_flag 为1 比特标志。置于‘1’时，它指示splice_countdown 字段
	必须在相关自适应字段中存在，指定拼接点的出现。‘0’值指示自适应字段中splice_countdown 
	字段不存在。	**/
	unsigned char splicing_point_flag;

	/*transport_private_data_flag 为1 比特标志。‘1’值指示自适应字段包
	含一个或多个private_data 字节。‘0’值指示自适应字段不包含任何private_data 字节。
	**/
	unsigned char transport_private_data_flag;

	/*adaptation_field_extension_flag 为1 比特字段，置于‘1’时，指示
	自适应字段扩展的存在。‘0’值指示自适应字段中自适应字段扩展不存在。
	**/
	unsigned char adaptation_field_extension_flag;

	/*program_clock_reference
	（PCR） 为以两部分编码的42 比特字段。第一部分，program_clock_reference_base 为33 比特字段，其值由
	PCR_ base (i)给出，如公式2-2 中给出的。第二部分，program_clock_reference_extension 为9 比特字段，其
	值由PCR_ext(i) 给出，如公式2-3 中给出的。PCR 指示包含program_clock_reference_base 最后比特的字节
	到达系统目标解码器输入端的预期时间。
	PCR(i) = PCR _ base(i)× 300 + PCR _ ext(i)
	PCR_base(i) = ((system_clock_ fr equency × t(i))DIV 300) %233
	PCR_ext(i) = ((system_clock_ frequency × t(i)) DIV 1)%300
	**/
	unsigned __int64  program_clock_reference_base;
	unsigned char     reverse0;//保留字段
	unsigned __int64  program_clock_reference_extension;
	unsigned __int64  PCR;

	unsigned int  original_program_clock_reference_base;
	unsigned char reverse1;//保留字段
	unsigned int  riginal_program_clock_reference_extension;

	/*splice_countdown 为8 比特字段，表示可以为正或负的值。正值指示相同PID 的
	跟随相关传输流包直至到达拼接点的传输流包的剩余数。复制的传输流包和仅包含自适应字段的传输流包
	被排除。相关splice_countdown 字段达到零的传输流包中的最后字节之后的位置即为拼接点的定位。在
	splice_countdown 达到零的传输流包中，传输流包有效载荷的最后数据字节必须是编码音频帧或编码图像的
	最后字节。在视频情况中，相应的存取单元可能由 sequence_end_code 终止或者不由 sequence_end_code 终
	止。跟随其后的、具有相同PID 的传输流包可以包含来自同一类型的不同基本流的数据。
	相同PID 的下一个传输流包的有效载荷（复制包以及无有效载荷的包除外）应随着PES 包的首字节开
	始启动。在音频情况中，PES 包有效载荷应从接入点开始启动。在视频情况中，PES 包有效载荷应从接入
	点开始启动，或者伴随后随接入点的 sequence_end_code 开始启动。这样，先前编码的音频帧或编码的图像
	伴随包边界定位，或者通过填整实现。拼接点之后，倒数字段也可以存在。当splice_countdown 字段为负数
	即其值为负n(－n)时，它指示相关的传输流包是跟随拼接点的第n 个包（复制包和无有效载荷的包除外）。
	**/
	unsigned char splice_countdown;

	//待添加	
};

//PES包，一帧的开始
struct PES
{
	/*packet_start_code_prefix 为24 比特码。同跟随它的stream_id 一起组成标
	识包起始端的包起始码。packet_start_code_prefix 为比特串‘0000 0000 0000 
	0000 0000 0001’（0x000001）	**/
	unsigned int   packet_start_code_prefix;//包起始码
	/*节目流中，stream_id 指示基本流的类型和编号
	Stream_id 注流 编 码
	1011 1100 1 program_stream_map  
	1011 1101 2 private_stream_1
	1011 1110   padding_stream
	1011 1111 3 private_stream_2
	110x xxxx   ISO/IEC 13818-3 或 ISO/IEC 11172-3 或 ISO/IEC 13818-7 或 ISO/IEC 14496-3 音频流编号 x xxxx
	1110 xxxx   ITU-T H.262 建议书 | ISO/IEC 13818-2, ISO/IEC 11172-2, ISO/IEC 14496-2 或 ITU-T H.264 建议书 | ISO/IEC 14496-10 视频流编号 xxxx
	1111 0000 3 ECM_stream
	1111 0001 3 EMM_stream
	1111 0010 5 ITU-T H.222.0 建议书 | ISO/IEC 13818-1 附件A或 ISO/IEC 13818-6_DSMCC_stream
	1111 0011 2 ISO/IEC_13522_stream
	1111 0100 6 ITU-T H.222.1 建议书类型A
	1111 0101 6 ITU-T H.222.1 建议书类型B
	1111 0110 6 ITU-T H.222.1 建议书类型C
	1111 0111 6 ITU-T H.222.1 建议书类型D
	1111 1000 6 ITU-T H.222.1 建议书类型E
	1111 1001 7 ancillary_stream
	1111 1010   ISO/IEC 14496-1_SL-packetized_stream
	1111 1011   ISO/IEC 14496-1_FlexMux_stream
	1111 1100   元数据流
	1111 1101 8 extended_stream_id
	1111 1110   保留数据流
	1111 1111 4 program_stream_directory
	符号x 意味着 ‘0’值或 ‘1’值均可，并产生相同的流类型。流编号通过x 的取值给出。
	注 1 — program_stream_map 类型的PES 包具有2.5.4.1 中指定的唯一句法。
	注 2 — private_stream_1和ISO/IEC_13552_stream 类型的PES包遵从与ITU-T H.262建议书|ISO/IEC 13818-2视频和ISO/IEC
	13818-3 音频流相同的PES 包句法。
	注 3 — private_stream_2, ECM_stream 和EMM_stream 类型的PES 包类似于private_stream_1，除PES_packet_length 字段之
	后未指定任何句法外。
	注 4 — program_stream_directory 类型的PES 包具有2.5.5 中指定的唯一句法。
	注 5 — DSM-CC_stream 类型的PES 包具有ISO/IEC 13818-6 中指定的唯一句法。
	注 6 — 此stream_id 同表2-34 中的stream_type 0x09 有关。
	注 7 — 此stream_type 0x09 仅在PES 包中使用，承载来自节目流或传输流中ISO/IEC 11172-1 系统流的数据（参阅2.4.3.8）。
	注 8 — 采用 stream_id 0xFD (extended_stream_id) 确定此PES 包使用扩展的句法以允许确定附加的流类型。
	**/
	unsigned char  stream_id;//指示当前属于音频还是视频
	/*16 比特字段指示PES 包中跟随该字段最后字节的字节数。0 值指示PES 包长度
	既未指示也未限定并且仅在这样的PES 包中才被允许，该PES 包的有效载荷由来自传输流包中所包含的视
	频基本流的字节组成。	**/
	unsigned int   PES_packet_length;//PES 包长度
	unsigned char  onezero;          //'10'
	/*2 比特PES_scrambling_control 字段指示PES 包有效载荷的加扰方式。当
	加扰在PES 等级上实施时，PES 包头，其中包括任选字段只要存在，应不加扰
	00 不加扰
	01 用户定义
	10 用户定义
	11 用户定义
	**/
	unsigned char  PES_scrambling_control;//加扰方式
	/*此为1 比特字段，指示在此PES 包中该有效载荷的优先级。‘1’指示该PES 包有效
	载荷比具有此字段置于‘0’的其他PES 包有效载荷有更高的有效载荷优先级。多路复用器
	能够使用该	PES_priority 比特最佳化基本流内的数据。此字段不能由传输机制加以改变。
	**/
	unsigned char  PES_priority;          //优先级
	unsigned char  data_alignment_indicator;
	/*此为1 比特字段。置于‘1’时，它指示相关PES 包有效载荷的素材依靠版权所保护。置
	于‘0’时不能确定该素材是否依靠版权所保护。
	**/
	unsigned char  copyright;//版权保护
	/*此为1 比特字段。置于‘1’时，相关PES 包有效载荷的内容是原始的。置于‘0’
	时，它指示相关PES 包有效载荷的内容是复制的。
	**/
	unsigned char  original_or_copy;
	/*
	此为2 比特字段。当PTS_DTS_flags 字段设置为‘10’时，PES 包头中PTS 字段
	存在。当PTS_DTS_flags 字段设置为‘11’时，PES 包头中PTS 字段和DTS 字段均存在。当PTS_DTS_flags
	字段设置为‘00’时，PES 包头中既无任何PTS 字段也无任何DTS 字段存在。值‘01’禁用。
	**/
	unsigned char  PTS_DTS_flags;
	/*1 比特标志，置于‘1’时指示PES 包头中ESCR 基准字段和ESCR 扩展字段均存在。
	置于‘0’时指示无任何ESCR 字段存在。
	**/
	unsigned char  ESCR_flag;
	/*1 比特标志，置于‘1’时指示PES 包头中ES_rate 字段存在。置于‘0’时指示无任
	何 ES_rate 字段存在。
	**/
	unsigned char  ES_rate_flag;
	/*1 比特标志，置于‘1’时指示8 比特特技方式字段存在。置于‘0’时指示
	此字段不存在。
	**/
	unsigned char  DSM_trick_mode_flag;

	/*	1 比特标志，置于‘1’时指示additional_copy_info 存在。置于‘0’时指
	示此字段不存在。
	**/
	unsigned char  additional_copy_info_flag;

	/*1 比特标志，置于‘1’时指示PES 包中CRC 字段存在。置于‘0’时指示此字段不存
	在。**/
	unsigned char  PES_CRC_flag;

	/*1 比特标志，置于‘1’时指示PES 包头中扩展字段存在。置于‘0’时指示此
	字段不存在。**/
	unsigned char  PES_extension_flag;
	/*8比特字段指示在此PES包头中包含的由任选字段和任意填充字节所占据
	的字节总数。任选字段的存在由前导 PES_header_data_length 字段的字节来指定。
	**/
	unsigned char  PES_header_data_length;

	unsigned char    zero0010;   //0010
	unsigned __int64 PTS3230;    //PTS [32..30]
	unsigned char    marker_bit0;//marker_bit 为1 比特字段，具有赋值‘1’
	unsigned __int64 PTS2915;    //PTS [29..15]
	unsigned char    marker_bit1;//marker_bit 为1 比特字段，具有赋值‘1’
	unsigned __int64 PTS1400;    //PTS [14..0]
	unsigned char    marker_bit2;//marker_bit 为1 比特字段，具有赋值‘1’
	unsigned char    zero0011;   //0011
	unsigned char    marker_bit3;//marker_bit 为1 比特字段，具有赋值‘1’
	unsigned char    marker_bit4;//marker_bit 为1 比特字段，具有赋值‘1’
	unsigned char    marker_bit5;//marker_bit 为1 比特字段，具有赋值‘1’
	unsigned char    zero0001;   //0001
	unsigned __int64 DTS3230;    //DTS [32..30]
	unsigned char    marker_bit6;//marker_bit 为1 比特字段，具有赋值‘1’
	unsigned __int64 DTS2915;    //DTS [29..15]
	unsigned char    marker_bit7;//marker_bit 为1 比特字段，具有赋值‘1’
	unsigned __int64 DTS1400;    //DTS [14..0]
	unsigned char    marker_bit8;//marker_bit 为1 比特字段，具有赋值‘1’

	unsigned __int64 PTS;       //显示时间
	unsigned __int64 DTS;       //解码时间
	unsigned char    marker_bit9;//marker_bit 为1 比特字段，具有赋值‘1’
	unsigned char    reserved;  //保留字段

};
class Anlysis
{
	ts_param*     m_param;         //配置参数信息

	TS            m_ts;            //TS数据
	TsHeader      m_tsHeader;      //TS数据包头
	TSPAT         m_pat;           //PAT数据
	TSPMT         m_pmt;           //PMT数据
	TSADT         m_adt;           //自适应字段数据
	PES           m_pes;           //PES包头            

	TsHeader      m_tsNewHeader;   //TS数据包头
	TS            m_newTs;         //修正后TS数据

	unsigned int  m_AudioPID;      //指示音频所在的PID 初始化为0
	unsigned int  m_VideoPID;      //指示视频所在的PID 初始化为0
	unsigned int  m_PATPID;        //指示PAT所在的PID 初始化为0
	unsigned int  m_PMTPID;        //指示PMT所在的PID 初始化为0

	/*pointer_field — 此为8 比特字段，其值为字节数，即紧随pointer_field 直至传输流包
	有效载荷中存在的首分段的首字节出现为止的字节数（因此pointer_field 中的0x00 值指示
	pointer_field 后该分段立即起始）。	当至少一个分段在给定的传输流包中开始传输时，
	payload_unit_start_indicator（参阅2.4.3.2）应置于‘1’，	并且那个传输流包的有效
	载荷的首字节必定包含该指针。当给定的传输流包中没有任何分段开始传输时，	payload_unit
	_start_indicator 应置于‘0’，并且那个包的有效载荷中应不发送任何指针。	**/
	unsigned char pointer_field;   //指针

	int           m_tsConter;      //统计ts的个数    

public:
	Anlysis();
	void create(ts_param *param);/* 申请内存 */
	void destory();/* 释放内存 */
	
	/* 分析ts Header文件 */
	bool anlysisTsHeader();

	/* 分析ts PAT文件 */
	void anlysisPAT();

	/* 分析ts PMT文件 */
	void anlysisPMT();

	/* 分析ts 自适应字段 */
	void adaptation_field();

	/* 分析ts PES包文件 */
	void anlysisPES();

	/* 分析ts文件 */
	void anlysis();
};
#endif