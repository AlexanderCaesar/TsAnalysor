//作者  蔡砚刚  20160622  xiaoc@pku.edu.cn
#include <stdlib.h>
#include <string.h>
#include "analysis.h"

#define AV_BSWAP16C(x) (((x) << 8 & 0xff00)  | ((x) >> 8 & 0x00ff))
#define AV_BSWAP32C(x) (AV_BSWAP16C(x) << 16 | AV_BSWAP16C((x) >> 16))

static const unsigned int av_crc_table[257] = 
{
	0x00000000, 0xB71DC104, 0x6E3B8209, 0xD926430D, 0xDC760413, 0x6B6BC517,
	0xB24D861A, 0x0550471E, 0xB8ED0826, 0x0FF0C922, 0xD6D68A2F, 0x61CB4B2B,
	0x649B0C35, 0xD386CD31, 0x0AA08E3C, 0xBDBD4F38, 0x70DB114C, 0xC7C6D048,
	0x1EE09345, 0xA9FD5241, 0xACAD155F, 0x1BB0D45B, 0xC2969756, 0x758B5652,
	0xC836196A, 0x7F2BD86E, 0xA60D9B63, 0x11105A67, 0x14401D79, 0xA35DDC7D,
	0x7A7B9F70, 0xCD665E74, 0xE0B62398, 0x57ABE29C, 0x8E8DA191, 0x39906095,
	0x3CC0278B, 0x8BDDE68F, 0x52FBA582, 0xE5E66486, 0x585B2BBE, 0xEF46EABA,
	0x3660A9B7, 0x817D68B3, 0x842D2FAD, 0x3330EEA9, 0xEA16ADA4, 0x5D0B6CA0,
	0x906D32D4, 0x2770F3D0, 0xFE56B0DD, 0x494B71D9, 0x4C1B36C7, 0xFB06F7C3,
	0x2220B4CE, 0x953D75CA, 0x28803AF2, 0x9F9DFBF6, 0x46BBB8FB, 0xF1A679FF,
	0xF4F63EE1, 0x43EBFFE5, 0x9ACDBCE8, 0x2DD07DEC, 0x77708634, 0xC06D4730,
	0x194B043D, 0xAE56C539, 0xAB068227, 0x1C1B4323, 0xC53D002E, 0x7220C12A,
	0xCF9D8E12, 0x78804F16, 0xA1A60C1B, 0x16BBCD1F, 0x13EB8A01, 0xA4F64B05,
	0x7DD00808, 0xCACDC90C, 0x07AB9778, 0xB0B6567C, 0x69901571, 0xDE8DD475,
	0xDBDD936B, 0x6CC0526F, 0xB5E61162, 0x02FBD066, 0xBF469F5E, 0x085B5E5A,
	0xD17D1D57, 0x6660DC53, 0x63309B4D, 0xD42D5A49, 0x0D0B1944, 0xBA16D840,
	0x97C6A5AC, 0x20DB64A8, 0xF9FD27A5, 0x4EE0E6A1, 0x4BB0A1BF, 0xFCAD60BB,
	0x258B23B6, 0x9296E2B2, 0x2F2BAD8A, 0x98366C8E, 0x41102F83, 0xF60DEE87,
	0xF35DA999, 0x4440689D, 0x9D662B90, 0x2A7BEA94, 0xE71DB4E0, 0x500075E4,
	0x892636E9, 0x3E3BF7ED, 0x3B6BB0F3, 0x8C7671F7, 0x555032FA, 0xE24DF3FE,
	0x5FF0BCC6, 0xE8ED7DC2, 0x31CB3ECF, 0x86D6FFCB, 0x8386B8D5, 0x349B79D1,
	0xEDBD3ADC, 0x5AA0FBD8, 0xEEE00C69, 0x59FDCD6D, 0x80DB8E60, 0x37C64F64,
	0x3296087A, 0x858BC97E, 0x5CAD8A73, 0xEBB04B77, 0x560D044F, 0xE110C54B,
	0x38368646, 0x8F2B4742, 0x8A7B005C, 0x3D66C158, 0xE4408255, 0x535D4351,
	0x9E3B1D25, 0x2926DC21, 0xF0009F2C, 0x471D5E28, 0x424D1936, 0xF550D832,
	0x2C769B3F, 0x9B6B5A3B, 0x26D61503, 0x91CBD407, 0x48ED970A, 0xFFF0560E,
	0xFAA01110, 0x4DBDD014, 0x949B9319, 0x2386521D, 0x0E562FF1, 0xB94BEEF5,
	0x606DADF8, 0xD7706CFC, 0xD2202BE2, 0x653DEAE6, 0xBC1BA9EB, 0x0B0668EF,
	0xB6BB27D7, 0x01A6E6D3, 0xD880A5DE, 0x6F9D64DA, 0x6ACD23C4, 0xDDD0E2C0,
	0x04F6A1CD, 0xB3EB60C9, 0x7E8D3EBD, 0xC990FFB9, 0x10B6BCB4, 0xA7AB7DB0,
	0xA2FB3AAE, 0x15E6FBAA, 0xCCC0B8A7, 0x7BDD79A3, 0xC660369B, 0x717DF79F,
	0xA85BB492, 0x1F467596, 0x1A163288, 0xAD0BF38C, 0x742DB081, 0xC3307185,
	0x99908A5D, 0x2E8D4B59, 0xF7AB0854, 0x40B6C950, 0x45E68E4E, 0xF2FB4F4A,
	0x2BDD0C47, 0x9CC0CD43, 0x217D827B, 0x9660437F, 0x4F460072, 0xF85BC176,
	0xFD0B8668, 0x4A16476C, 0x93300461, 0x242DC565, 0xE94B9B11, 0x5E565A15,
	0x87701918, 0x306DD81C, 0x353D9F02, 0x82205E06, 0x5B061D0B, 0xEC1BDC0F,
	0x51A69337, 0xE6BB5233, 0x3F9D113E, 0x8880D03A, 0x8DD09724, 0x3ACD5620,
	0xE3EB152D, 0x54F6D429, 0x7926A9C5, 0xCE3B68C1, 0x171D2BCC, 0xA000EAC8,
	0xA550ADD6, 0x124D6CD2, 0xCB6B2FDF, 0x7C76EEDB, 0xC1CBA1E3, 0x76D660E7,
	0xAFF023EA, 0x18EDE2EE, 0x1DBDA5F0, 0xAAA064F4, 0x738627F9, 0xC49BE6FD,
	0x09FDB889, 0xBEE0798D, 0x67C63A80, 0xD0DBFB84, 0xD58BBC9A, 0x62967D9E,
	0xBBB03E93, 0x0CADFF97, 0xB110B0AF, 0x060D71AB, 0xDF2B32A6, 0x6836F3A2,
	0x6D66B4BC, 0xDA7B75B8, 0x035D36B5, 0xB440F7B1, 0x00000001
};

unsigned int av_crc(const unsigned int *ctx, unsigned int crc,
	const unsigned char *buffer, size_t length)
{
	const unsigned char *end = buffer + length;

	while (buffer < end)
		crc = ctx[((unsigned char) crc) ^ *buffer++] ^ (crc >> 8);

	return crc;
}

static unsigned int getCRC(unsigned int  x)
{
	return AV_BSWAP32C(x);
}
void author(ts_param *param)
{
	if(!param->b_ts) return;
	fprintf(g_ts_ts,"==================作者 信息=========================\n");
	fprintf(g_ts_ts,"网       名:    xiaoC/cabbage/Alexander/Caesar\n");
	fprintf(g_ts_ts,"邮       箱:    xiaoc@pku.edu.cn\n");
	fprintf(g_ts_ts,"博       客:    http://blog.csdn.net/cabbage2008\n");
}
Anlysis::Anlysis()
{
	m_param          = NULL;
	m_VideoPID       = 0;
	m_AudioPID       = 0;
	m_PATPID         = 0;
	m_PMTPID         = 0;
};
/* 申请内存 */
void Anlysis::create(ts_param *param)
{
	bool ok = true;
	m_param = param;

	if(!ok)
	{
		printf("Anlysis类申请内存失败\n");
		system("pause");
		exit(0);
	}
}
/* 释放内存 */
void Anlysis::destory()
{
}

/* 分析ts Header文件 */
bool Anlysis::anlysisTsHeader()
{
	m_tsHeader.sync_byte = m_ts.data[m_ts.count++];
	m_tsHeader.transport_error_indicator    = (m_ts.data[m_ts.count]&0x80)>>7;
	m_tsHeader.payload_unit_start_indicator = (m_ts.data[m_ts.count]&0x40)>>6;
	m_tsHeader.transport_priority           = (m_ts.data[m_ts.count]&0x20)>>5;
	m_tsHeader.PID                          = ((m_ts.data[m_ts.count]&0x1F)<<8) + m_ts.data[m_ts.count+1];
	m_ts.count += 2;
	m_tsHeader.transport_scrambling_control = (m_ts.data[m_ts.count]&0xC0)>>6;
	m_tsHeader.adaptation_field_control     = (m_ts.data[m_ts.count]&0x30)>>4;
	m_tsHeader.continuity_counter           =  m_ts.data[m_ts.count++]&0x0F;

	if(m_param->b_ts)//输出包头信息
	{
		fprintf(g_ts_ts,"++++++ TS [%d]  ++++++\n",m_tsConter);
		fprintf(g_ts_ts,"同步字节:0x%X\n",m_tsHeader.sync_byte);
		fprintf(g_ts_ts,"错误位  :%X\n",m_tsHeader.transport_error_indicator);
		fprintf(g_ts_ts,"起始位  :%X\n",m_tsHeader.payload_unit_start_indicator);
		fprintf(g_ts_ts,"优先级  :%X\n",m_tsHeader.transport_priority);
		fprintf(g_ts_ts,"PID     :0x%X ",m_tsHeader.PID);
		switch(m_tsHeader.PID)
		{
		case 0x0000:fprintf(g_ts_ts,"节目相关表\n");break;
		case 0x0001:fprintf(g_ts_ts,"有条件访问表\n");break;
		case 0x0002:fprintf(g_ts_ts,"传输流描述表\n");break;
		case 0x0003:fprintf(g_ts_ts,"IPMP 控制信息表\n");break;
		case 0x1FFF:fprintf(g_ts_ts,"空包\n",m_tsHeader.PID);break;
		default:
			{
				if(m_tsHeader.PID >= 0x0004 && m_tsHeader.PID <= 0x000F)
				{
					fprintf(g_ts_ts,"保留的\n");
				}
				else
					fprintf(g_ts_ts,"\n");
			}
		}
		fprintf(g_ts_ts,"加扰方式:0x%X ",m_tsHeader.transport_scrambling_control);
		switch(m_tsHeader.transport_scrambling_control)
		{
			case 0x00:fprintf(g_ts_ts,"不加扰\n");break;
			case 0x01:fprintf(g_ts_ts,"用户定义\n");break;
			case 0x02:fprintf(g_ts_ts,"用户定义\n");break;
			case 0x03:fprintf(g_ts_ts,"用户定义\n");break;
			default:fprintf(g_ts_ts,"加扰方式错误\n");g_errors++;
		}
		fprintf(g_ts_ts,"自适应字:0x%X ",m_tsHeader.adaptation_field_control);
		switch(m_tsHeader.adaptation_field_control)
		{
		case 0x00:fprintf(g_ts_ts,"供未来使用，由ISO/IEC 所保留\n");break;
		case 0x01:fprintf(g_ts_ts,"无 adaptation_field，仅有效载荷\n");break;
		case 0x02:fprintf(g_ts_ts,"仅有Adaptation_field，无有效载荷\n");break;
		case 0x03:fprintf(g_ts_ts,"Adaptation_field 后随有效载荷\n");break;
		default:fprintf(g_ts_ts,"自适应字错误\n");g_errors++;
		}
		fprintf(g_ts_ts,"序列号  :0x%X \n",m_tsHeader.continuity_counter);

	}

	if(m_param->b_nts&&m_tsHeader.PID!=0x11&&m_tsHeader.PID!=0x101)
	{
		memcpy(&m_tsNewHeader,&m_tsHeader,sizeof(TsHeader));
		//待添加 修改
		m_newTs.count = 0;
		m_newTs.left  = 0;

		m_newTs.data[m_newTs.count++] = m_tsNewHeader.sync_byte;
		m_newTs.data[m_newTs.count++] = (m_tsNewHeader.transport_error_indicator<<7) + (m_tsNewHeader.payload_unit_start_indicator<<6) + (m_tsNewHeader.transport_priority<<5) + (m_tsNewHeader.PID>>8);
		m_newTs.data[m_newTs.count++] = m_tsNewHeader.PID&0x000000FF;
		m_newTs.data[m_newTs.count++] = (m_tsNewHeader.transport_scrambling_control<<6) + (m_tsNewHeader.adaptation_field_control<<4) + (m_tsNewHeader.continuity_counter);

		memcpy(m_newTs.data+m_newTs.count,m_ts.data+m_newTs.count,184);
		fwrite(m_newTs.data,1,188,g_ts_out); //写ts信息

	}
	
	if(m_tsHeader.sync_byte != 0x47) //同步字节不正确
	{
		return false;
	}
	else
	{
		return true;
	}	
}

/* 分析ts PAT文件 */
void Anlysis::anlysisPAT()
{
	pointer_field                  = m_ts.data[m_ts.count++];//读取指针
	m_ts.count                    += pointer_field;          //偏移到PAT位置

	m_pat.table_id                 = m_ts.data[m_ts.count++];
	m_pat.section_syntax_indicator = (m_ts.data[m_ts.count]&0x80)>>7;
	m_pat.zero0                    = (m_ts.data[m_ts.count]&0x40)>>6;
	m_pat.reserved0                = (m_ts.data[m_ts.count]&0x30)>>4;
	m_pat.section_length           = ((m_ts.data[m_ts.count]&0x0F)<<8) + m_ts.data[m_ts.count+1]; m_ts.count += 2;
	m_pat.transport_stream_id      = (m_ts.data[m_ts.count]<<8) + m_ts.data[m_ts.count+1];m_ts.count += 2;
	m_pat.reserved1                = (m_ts.data[m_ts.count]&0xC0)>>6;
	m_pat.version_number           = (m_ts.data[m_ts.count]&0x1E)>>1;
	m_pat.current_next_indicator   = m_ts.data[m_ts.count++]&0x01;
	m_pat.section_number           = m_ts.data[m_ts.count++];
	m_pat.last_section_number      = m_ts.data[m_ts.count++];
	m_pat.N                        = (m_pat.section_length - 9)/4;//循环内部占4字节  CRC加上number信息等占用9个字节
	for(int i = 0; i < m_pat.N; i++)
	{
		m_pat.program_number[i]    = (m_ts.data[m_ts.count]<<8) + m_ts.data[m_ts.count+1];m_ts.count += 2;
		m_pat.reserved[i]          = (m_ts.data[m_ts.count]&0xE0)>>5;
		if(m_pat.program_number[i]==0)
		{
			m_pat.network_PID[i]   = ((m_ts.data[m_ts.count]&0x1F)<<8) + m_ts.data[m_ts.count+1];
		}
		else
		{
			m_pat.program_map_PID[i] = ((m_ts.data[m_ts.count]&0x1F)<<8) + m_ts.data[m_ts.count+1];
			m_PMTPID                 = m_pat.program_map_PID[i];
		}
		m_ts.count += 2;
	}
	m_pat.CRC_32 = (m_ts.data[m_ts.count]<<24) + (m_ts.data[m_ts.count+1]<<16) + (m_ts.data[m_ts.count+2]<<8) + m_ts.data[m_ts.count+3];
	m_ts.count += 4;

	/*验证CRC*/
	unsigned len = m_ts.count -5;  //PAT长度
	unsigned char *buf = m_ts.data + 5;//PAT位置
	unsigned int crc = getCRC(av_crc(av_crc_table,-1,buf,len-4));
	/*写CRC方法
	buf[len - 4] = (crc >> 24) & 0xff;
	buf[len - 3] = (crc >> 16) & 0xff;
	buf[len - 2] = (crc >>  8) & 0xff;
	buf[len - 1] =  crc        & 0xff;
	*/

	if(m_param->b_ts)
	{
		fprintf(g_ts_ts,"pointer_field:%d\n",pointer_field);
		fprintf(g_ts_ts,"PAT解析\n");
		fprintf(g_ts_ts,"table_id:0x%X\n",m_pat.table_id);
		fprintf(g_ts_ts,"section_syntax_indicator:%d\n",m_pat.section_syntax_indicator);
		fprintf(g_ts_ts,"0:%d\n",m_pat.zero0);
		fprintf(g_ts_ts,"reserved:%d\n",m_pat.reserved0);
		fprintf(g_ts_ts,"ssection_length:%d\n",m_pat.section_length);
		fprintf(g_ts_ts,"transport_stream_id:%d\n",m_pat.transport_stream_id);
		fprintf(g_ts_ts,"reserved:%d\n",m_pat.reserved1);
		fprintf(g_ts_ts,"version_number:%d\n",m_pat.version_number);
		fprintf(g_ts_ts,"current_next_indicator:%d\n",m_pat.current_next_indicator);
		fprintf(g_ts_ts,"section_number:%d\n",m_pat.section_number);
		fprintf(g_ts_ts,"last_section_number:%d\n",m_pat.last_section_number);
		fprintf(g_ts_ts,"N:%d\n",m_pat.N);
		for(int i = 0; i < m_pat.N; i++)
		{
			fprintf(g_ts_ts,"   program_number:0x%X\n",m_pat.program_number[i]);
			fprintf(g_ts_ts,"   reserved:0x%X\n",m_pat.reserved[i]);
			if(m_pat.program_number[i]==0)
			{
				fprintf(g_ts_ts,"   network_PID:0x%X\n",m_pat.network_PID[i]);
			}
			else
			{
				fprintf(g_ts_ts,"   program_map_PID:0x%X\n",m_pat.program_map_PID[i]);
			}
		}
		
		fprintf(g_ts_ts,"CRC_32:0x%X\n",m_pat.CRC_32);
	}
}

/* 分析ts PMT文件 */
void Anlysis::anlysisPMT()
{
	pointer_field                  = m_ts.data[m_ts.count++];//读取指针
	m_ts.count                    += pointer_field;          //偏移到PAT位置

	m_pmt.table_id                 = m_ts.data[m_ts.count++];
	m_pmt.section_syntax_indicator = (m_ts.data[m_ts.count]&0x80)>>7;
	m_pmt.zero0                    = (m_ts.data[m_ts.count]&0x40)>>6;
	m_pmt.reserved0                = (m_ts.data[m_ts.count]&0x30)>>4;
	m_pmt.section_length           = ((m_ts.data[m_ts.count]&0x0F)<<8) + m_ts.data[m_ts.count+1]; m_ts.count += 2;
	m_pmt.program_number           = (m_ts.data[m_ts.count]<<8) + m_ts.data[m_ts.count+1];m_ts.count += 2;
	m_pmt.reserved1                = (m_ts.data[m_ts.count]&0xC0)>>6;
	m_pmt.version_number           = (m_ts.data[m_ts.count]&0x1E)>>1;
	m_pmt.current_next_indicator   = m_ts.data[m_ts.count++]&0x01;
	m_pmt.section_number           = m_ts.data[m_ts.count++];
	m_pmt.last_section_number      = m_ts.data[m_ts.count++];
	m_pmt.reserved2                = (m_ts.data[m_ts.count]&0xE0)>>5;
	m_pmt.PCR_PID                  = ((m_ts.data[m_ts.count]&0x1F)<<8) + m_ts.data[m_ts.count+1];m_ts.count += 2;
	m_pmt.reserved3                = (m_ts.data[m_ts.count]&0xF0)>>4;
	m_pmt.program_info_length      = ((m_ts.data[m_ts.count]&0x0F)<<8) + m_ts.data[m_ts.count+1];m_ts.count += 2;
	m_pmt.N                        = 0;//descriptor() 待添加
	for(int i = 0; i < m_pat.N; i++)
	{
		//descriptor() 待添加
	}
	m_ts.count += m_pmt.program_info_length;

	int length = m_pmt.section_length - 9 - m_pmt.program_info_length;//9指的是前面占用的字节

	m_pmt.N1 = 0;
	while(length > 4)
	{
		m_pmt.stream_type[m_pmt.N1]      = m_ts.data[m_ts.count++];
		m_pmt.reservedN1_0[m_pmt.N1]     = (m_ts.data[m_ts.count]&0xE0)>>5;
		m_pmt.elementary_PID[m_pmt.N1]   = ((m_ts.data[m_ts.count]&0x1F)<<8) + m_ts.data[m_ts.count+1];m_ts.count += 2;
		m_pmt.reservedN1_1[m_pmt.N1]     = (m_ts.data[m_ts.count]&0xF0)>>4;
		m_pmt.ES_info_length[m_pmt.N1]   = ((m_ts.data[m_ts.count]&0x0F)<<8) + m_ts.data[m_ts.count+1];m_ts.count += 2;

		{
			//descriptor() 待添加
		}
		m_ts.count += m_pmt.ES_info_length[m_pmt.N1];
		switch(m_pmt.stream_type[m_pmt.N1])
		{
			//待添加
		case 0x1B:
		case 0x24:m_VideoPID = m_pmt.elementary_PID[m_pmt.N1];break;
		case 0x03:
		case 0x0F:m_AudioPID = m_pmt.elementary_PID[m_pmt.N1];break;
		}
		length = length -5 - m_pmt.ES_info_length[m_pmt.N1];
		m_pmt.N1++;
	}

	m_pat.CRC_32 = (m_ts.data[m_ts.count]<<24) + (m_ts.data[m_ts.count+1]<<16) + (m_ts.data[m_ts.count+2]<<8) + m_ts.data[m_ts.count+3];
	m_ts.count += 4;
	if(m_param->b_ts)
	{
		fprintf(g_ts_ts,"pointer_field:%d\n",pointer_field);
		fprintf(g_ts_ts,"PMT解析\n");
		fprintf(g_ts_ts,"table_id:0x%X\n",m_pmt.table_id);
		fprintf(g_ts_ts,"section_syntax_indicator:%d\n",m_pmt.section_syntax_indicator);
		fprintf(g_ts_ts,"0:%d\n",m_pmt.zero0);
		fprintf(g_ts_ts,"reserved:%d\n",m_pmt.reserved0);
		fprintf(g_ts_ts,"section_length:%d\n",m_pmt.section_length);
		fprintf(g_ts_ts,"program_number:%d\n",m_pmt.program_number);
		fprintf(g_ts_ts,"reserved:%d\n",m_pmt.reserved1);
		fprintf(g_ts_ts,"version_number:0x%X\n",m_pmt.version_number);
		fprintf(g_ts_ts,"current_next_indicator:%d\n",m_pmt.current_next_indicator);
		fprintf(g_ts_ts,"section_number:0x%X\n",m_pmt.section_number);
		fprintf(g_ts_ts,"last_section_number:0x%X\n",m_pmt.last_section_number);
		fprintf(g_ts_ts,"reserved:%d\n",m_pmt.reserved2);
		fprintf(g_ts_ts,"PCR_PID:0x%X\n",m_pmt.PCR_PID);
		fprintf(g_ts_ts,"reserved:%d\n",m_pmt.reserved3);
		fprintf(g_ts_ts,"program_info_length:%d\n",m_pmt.program_info_length);
		fprintf(g_ts_ts,"m_pmt.N1:%d\n",m_pmt.N1);
		for(int i = 0; i < m_pmt.N1; i++)
		{
			fprintf(g_ts_ts,"     stream_type:0x%X\n",m_pmt.stream_type[i]);
			fprintf(g_ts_ts,"     reserved:%d\n",m_pmt.reservedN1_0[i]);
			fprintf(g_ts_ts,"     elementary_PID:0x%X\n",m_pmt.elementary_PID[i]);
			fprintf(g_ts_ts,"     reserved:%d\n",m_pmt.reservedN1_1[i]);
			fprintf(g_ts_ts,"     ES_info_length:%d\n",m_pmt.ES_info_length[i]);
		}

		fprintf(g_ts_ts,"CRC_32:0x%X\n",m_pmt.CRC_32);
	}
}

/* 分析ts 自适应字段 */
void Anlysis::adaptation_field()
{
	m_adt.adaptation_field_length = m_ts.data[m_ts.count++];
	if(m_adt.adaptation_field_length)
	{
		m_adt.discontinuity_indicator              = (m_ts.data[m_ts.count]&0x80)>>7;//一般为0
		m_adt.random_access_indicator              = (m_ts.data[m_ts.count]&0x40)>>6; 
		m_adt.elementary_stream_priority_indicator = (m_ts.data[m_ts.count]&0x20)>>5;//一般为0 
		m_adt.PCR_flag                             = (m_ts.data[m_ts.count]&0x10)>>4;//一般为0  
		m_adt.OPCR_flag                            = (m_ts.data[m_ts.count]&0x08)>>3;//一般为0 
		m_adt.splicing_point_flag                  = (m_ts.data[m_ts.count]&0x04)>>2;//一般为0 
		m_adt.transport_private_data_flag          = (m_ts.data[m_ts.count]&0x02)>>1;//一般为0 
		m_adt.adaptation_field_extension_flag      = (m_ts.data[m_ts.count]&0x01)>>0;m_ts.count++;//一般为0 
		if(m_adt.PCR_flag)
		{
			m_adt.program_clock_reference_base     = (m_ts.data[m_ts.count]<<25) + (m_ts.data[m_ts.count+1]<<17) + (m_ts.data[m_ts.count+2]<<9) + (m_ts.data[m_ts.count+3]<<1) + ((m_ts.data[m_ts.count+4]&0x80)>>7) ;
			m_adt.reverse0                         = ((m_ts.data[m_ts.count+4]&0x7E)>>1);
			m_adt.program_clock_reference_extension= ((m_ts.data[m_ts.count+4]&0x01)<<8) + m_ts.data[m_ts.count+5];
			m_adt.PCR = m_adt.program_clock_reference_base*300 + m_adt.program_clock_reference_extension;
		}
		if(m_adt.OPCR_flag)
		{
			//待添加
		}
		if(m_adt.splicing_point_flag)
		{
			//待添加
		}
		if(m_adt.transport_private_data_flag)
		{
			//待添加
		}
		if(m_adt.adaptation_field_extension_flag)
		{
			//待添加
		}		
	}

	m_ts.count = 5 + m_adt.adaptation_field_length;//读完TS自适应字段

	if(m_param->b_ts)
	{
		fprintf(g_ts_ts,"自适应字段解析\n");
		fprintf(g_ts_ts,"adaptation_field_length:%d\n",m_adt.adaptation_field_length);
		if(m_adt.adaptation_field_length)
		{
			fprintf(g_ts_ts,"    discontinuity_indicator:%d\n",m_adt.discontinuity_indicator);
			fprintf(g_ts_ts,"    random_access_indicator:%d\n",m_adt.random_access_indicator);
			fprintf(g_ts_ts,"    elementary_stream_priority_indicator:%d\n",m_adt.elementary_stream_priority_indicator);
			fprintf(g_ts_ts,"    PCR_flag:%d\n",m_adt.PCR_flag);
			fprintf(g_ts_ts,"    OPCR_flag:%d\n",m_adt.OPCR_flag);
			fprintf(g_ts_ts,"    splicing_point_flag:%d\n",m_adt.splicing_point_flag);
			fprintf(g_ts_ts,"    transport_private_data_flag:%d\n",m_adt.transport_private_data_flag);
			fprintf(g_ts_ts,"    adaptation_field_extension_flag:%d\n",m_adt.adaptation_field_extension_flag);
			if(m_adt.PCR_flag)
			{
				fprintf(g_ts_ts,"        program_clock_reference_base:0x%X\n",m_adt.program_clock_reference_base);
				fprintf(g_ts_ts,"        reverse:0x%X\n",m_adt.reverse0);
				fprintf(g_ts_ts,"        program_clock_reference_extension:0x%X\n",m_adt.program_clock_reference_extension);
				fprintf(g_ts_ts,"        PCR:%I64d  (",m_adt.PCR);
				{
					__int64 time = m_adt.program_clock_reference_base/90;//m_adt.program_clock_reference_base*300*1000/27000000; //毫秒
					fprintf(g_ts_ts,"%I64d:",time/3600000);time = time%3600000;//时
					fprintf(g_ts_ts,"%I64d:",time/60000);time = time%60000;//分
					fprintf(g_ts_ts,"%I64d.",time/1000);time = time%1000;//秒
					fprintf(g_ts_ts,"%I64d)\n",time);//毫秒
				}
			}
		}
	}
}

/* 分析ts PES包文件 */
void Anlysis::anlysisPES()
{
	m_pes.packet_start_code_prefix = (m_ts.data[m_ts.count]<<16) + (m_ts.data[m_ts.count+1]<<8) + m_ts.data[m_ts.count+2]; m_ts.count += 3;
	m_pes.stream_id                = m_ts.data[m_ts.count++];
	m_pes.PES_packet_length        =(m_ts.data[m_ts.count]<<8) + m_ts.data[m_ts.count+1];m_ts.count += 2;
	if(m_pes.stream_id != PROGRAM_STREAM_MAP        //stream_id != program_stream_map
		&&m_pes.stream_id != PADDING_STREAM          //&& stream_id != padding_stream
		&&m_pes.stream_id != PRIVATE_STREAM2         //&& stream_id != private_stream_2
		&&m_pes.stream_id != ECM                     //&& stream_id != ECM
		&&m_pes.stream_id != EMM                     //&& stream_id != EMM
		&&m_pes.stream_id != PROGRAM_STREAM_DIRECTORY//&& stream_id != program_stream_directory
		&&m_pes.stream_id != DSMCC_STREAM            //&& stream_id != DSMCC_stream
		&&m_pes.stream_id != H222_1_E)               //&& stream_id != ITU-T H.222.1 建议书类型E stream)
	{
		m_pes.onezero                  = (m_ts.data[m_ts.count]&0xC0)>>6;//为10
		m_pes.PES_scrambling_control   = (m_ts.data[m_ts.count]&0x30)>>4;//一般为0
		m_pes.PES_priority             = (m_ts.data[m_ts.count]&0x08)>>3;//一般为0
		m_pes.data_alignment_indicator = (m_ts.data[m_ts.count]&0x04)>>2;//一般为0
		m_pes.copyright                = (m_ts.data[m_ts.count]&0x02)>>1;//一般为0
		m_pes.original_or_copy         = (m_ts.data[m_ts.count]&0x01)>>0;m_ts.count++;//一般为0
		m_pes.PTS_DTS_flags            = (m_ts.data[m_ts.count]&0xC0)>>6;//一般为3 I帧P帧一般为3 需要DTS B部分不作参考的可以不打DTS
		m_pes.ESCR_flag                = (m_ts.data[m_ts.count]&0x20)>>5;//一般为0
		m_pes.ES_rate_flag             = (m_ts.data[m_ts.count]&0x10)>>4;//一般为0
		m_pes.DSM_trick_mode_flag      = (m_ts.data[m_ts.count]&0x08)>>3;//一般为0
		m_pes.additional_copy_info_flag= (m_ts.data[m_ts.count]&0x04)>>2;//一般为0
		m_pes.PES_CRC_flag             = (m_ts.data[m_ts.count]&0x02)>>1;//一般为0
		m_pes.PES_extension_flag       = (m_ts.data[m_ts.count]&0x01)>>0;m_ts.count++;//一般为0
		m_pes.PES_header_data_length   = m_ts.data[m_ts.count++];
		int pre_count = m_ts.count;//记录包头前位置
		if(m_pes.PTS_DTS_flags==2)//只有PTS
		{
			m_pes.zero0010    = (m_ts.data[m_ts.count]&0xF0)>>4;//为0010
			m_pes.PTS3230     = (m_ts.data[m_ts.count]&0x07)>>1;
			m_pes.marker_bit0 = (m_ts.data[m_ts.count]&0x01)>>0;m_ts.count++;//1
			m_pes.PTS2915     = (m_ts.data[m_ts.count]<<7) + ((m_ts.data[m_ts.count+1]&0xFE)>>1); m_ts.count ++;
			m_pes.marker_bit1 = (m_ts.data[m_ts.count]&0x01)>>0;m_ts.count++;//1
			m_pes.PTS1400     = (m_ts.data[m_ts.count]<<7) + ((m_ts.data[m_ts.count+1]&0xFE)>>1); m_ts.count ++;
			m_pes.marker_bit2 = (m_ts.data[m_ts.count]&0x01)>>0;m_ts.count++;//1
			m_pes.PTS         = (m_pes.PTS3230<<30) + (m_pes.PTS2915<<15) + m_pes.PTS1400;
		}
		if(m_pes.PTS_DTS_flags==3)// PTS DTS
		{
			m_pes.zero0011    = (m_ts.data[m_ts.count]&0xF0)>>4;//为0011
			m_pes.PTS3230     = (m_ts.data[m_ts.count]&0x07)>>1;
			m_pes.marker_bit3 = (m_ts.data[m_ts.count]&0x01)>>0;m_ts.count++;//1
			m_pes.PTS2915     = (m_ts.data[m_ts.count]<<7) + ((m_ts.data[m_ts.count+1]&0xFE)>>1); m_ts.count ++;
			m_pes.marker_bit4 = (m_ts.data[m_ts.count]&0x01)>>0;m_ts.count++;//1
			m_pes.PTS1400     = (m_ts.data[m_ts.count]<<7) + ((m_ts.data[m_ts.count+1]&0xFE)>>1); m_ts.count ++;
			m_pes.marker_bit5 = (m_ts.data[m_ts.count]&0x01)>>0;m_ts.count++;//1
			m_pes.PTS         = (m_pes.PTS3230<<30) + (m_pes.PTS2915<<15) + m_pes.PTS1400;

			m_pes.zero0001    = (m_ts.data[m_ts.count]&0xF0)>>4;//为0001
			m_pes.DTS3230     = (m_ts.data[m_ts.count]&0x07)>>1;
			m_pes.marker_bit6 = (m_ts.data[m_ts.count]&0x01)>>0;m_ts.count++;//1
			m_pes.DTS2915     = (m_ts.data[m_ts.count]<<7) + ((m_ts.data[m_ts.count+1]&0xFE)>>1); m_ts.count ++;
			m_pes.marker_bit7 = (m_ts.data[m_ts.count]&0x01)>>0;m_ts.count++;//1
			m_pes.DTS1400     = (m_ts.data[m_ts.count]<<7) + ((m_ts.data[m_ts.count+1]&0xFE)>>1); m_ts.count ++;
			m_pes.marker_bit8 = (m_ts.data[m_ts.count]&0x01)>>0;m_ts.count++;//1
			m_pes.DTS         = (m_pes.DTS3230<<30) + (m_pes.DTS2915<<15) + m_pes.DTS1400;
		}
		if (m_pes.ESCR_flag)
		{
			//待添加
		}
		if (m_pes.ES_rate_flag)
		{
			//待添加
		}
		if (m_pes.DSM_trick_mode_flag)
		{
			//待添加
		}
		if (m_pes.additional_copy_info_flag)
		{
			//待添加
		}
		if (m_pes.PES_CRC_flag)
		{
			//待添加
		}
		if (m_pes.PES_extension_flag)
		{
			//待添加
		}
		m_ts.count = pre_count + m_pes.PES_header_data_length;
	}
	else if(m_pes.stream_id == PROGRAM_STREAM_MAP     //stream_id == program_stream_map
		||m_pes.stream_id == PRIVATE_STREAM2          //|| stream_id == private_stream_2
		||m_pes.stream_id == ECM                      //|| stream_id == ECM
		||m_pes.stream_id == EMM                      //|| stream_id == EMM
		||m_pes.stream_id == PROGRAM_STREAM_DIRECTORY //|| stream_id == program_stream_directory
		||m_pes.stream_id == DSMCC_STREAM             //|| stream_id == DSMCC_stream
		||m_pes.stream_id == H222_1_E)                //|| stream_id == ITU-T H.222.1 建议书类型E stream )
	{
		m_ts.count += m_pes.PES_packet_length;
		//待添加
	}
	else if (m_pes.stream_id == PADDING_STREAM)
	{
		m_ts.count += m_pes.PES_packet_length;
		//待添加
	}
	if(m_param->b_ts)
	{
		fprintf(g_ts_ts,"PES包头解析\n");
		fprintf(g_ts_ts,"packet_start_code_prefix:%d\n",m_pes.packet_start_code_prefix);
		fprintf(g_ts_ts,"stream_id:0x%X\n",m_pes.stream_id);
		fprintf(g_ts_ts,"PES_packet_length:%d\n",m_pes.PES_packet_length);
		if(m_pes.stream_id != PROGRAM_STREAM_MAP        //stream_id != program_stream_map
			&&m_pes.stream_id != PADDING_STREAM          //&& stream_id != padding_stream
			&&m_pes.stream_id != PRIVATE_STREAM2         //&& stream_id != private_stream_2
			&&m_pes.stream_id != ECM                     //&& stream_id != ECM
			&&m_pes.stream_id != EMM                     //&& stream_id != EMM
			&&m_pes.stream_id != PROGRAM_STREAM_DIRECTORY//&& stream_id != program_stream_directory
			&&m_pes.stream_id != DSMCC_STREAM            //&& stream_id != DSMCC_stream
			&&m_pes.stream_id != H222_1_E)               //&& stream_id != ITU-T H.222.1 建议书类型E stream)
		{
			fprintf(g_ts_ts,"    10:0x%X\n",m_pes.onezero);
			fprintf(g_ts_ts,"    PES_scrambling_control:%d\n",m_pes.PES_scrambling_control);
			fprintf(g_ts_ts,"    PES_priority :%d\n",m_pes.PES_priority );
			fprintf(g_ts_ts,"    data_alignment_indicator:%d\n",m_pes.data_alignment_indicator);
			fprintf(g_ts_ts,"    copyright:%d\n",m_pes.copyright);
			fprintf(g_ts_ts,"    original_or_copy:%d\n",m_pes.original_or_copy);
			fprintf(g_ts_ts,"    PTS_DTS_flags:%d\n",m_pes.PTS_DTS_flags);
			fprintf(g_ts_ts,"    ESCR_flag:%d\n",m_pes.ESCR_flag);
			fprintf(g_ts_ts,"    ES_rate_flag:%d\n",m_pes.ES_rate_flag);
			fprintf(g_ts_ts,"    DSM_trick_mode_flag:%d\n",m_pes.DSM_trick_mode_flag);
			fprintf(g_ts_ts,"    additional_copy_info_flag:%d\n",m_pes.additional_copy_info_flag);
			fprintf(g_ts_ts,"    PES_CRC_flag:%d\n",m_pes.PES_CRC_flag);
			fprintf(g_ts_ts,"    PES_extension_flag:%d\n",m_pes.PES_extension_flag);
			fprintf(g_ts_ts,"    PES_header_data_length:%d\n",m_pes.PES_header_data_length);

			if(m_pes.PTS_DTS_flags==2)//只有PTS
			{
				fprintf(g_ts_ts,"    0010:0x%X\n",m_pes.zero0010);
				fprintf(g_ts_ts,"    PTS[32...30]:%I64d\n",m_pes.PTS3230);
				fprintf(g_ts_ts,"    marker_bit:%d\n",m_pes.marker_bit0);
				fprintf(g_ts_ts,"    PTS[29...15]:%I64d\n",m_pes.PTS2915);
				fprintf(g_ts_ts,"    marker_bit:%d\n",m_pes.marker_bit1);
				fprintf(g_ts_ts,"    PTS[14...00]:%I64d\n",m_pes.PTS1400);
				fprintf(g_ts_ts,"    marker_bit:%d\n",m_pes.marker_bit2);
				fprintf(g_ts_ts,"    PTS:%I64d (",m_pes.PTS);
				{
					__int64 time = m_pes.PTS/90;//m_pes.PTS*300*1000/27000000; //毫秒
					fprintf(g_ts_ts,"%I64d:",time/3600000);time = time%3600000;//时
					fprintf(g_ts_ts,"%I64d:",time/60000);time = time%60000;//分
					fprintf(g_ts_ts,"%I64d.",time/1000);time = time%1000;//秒
					fprintf(g_ts_ts,"%I64d)\n",time);//毫秒
				}
			}
			if(m_pes.PTS_DTS_flags==3)// PTS DTS
			{
				fprintf(g_ts_ts,"    0011:0x%X\n",m_pes.zero0011);
				fprintf(g_ts_ts,"    PTS[32...30]:%I64d\n",m_pes.PTS3230);
				fprintf(g_ts_ts,"    marker_bit:%d\n",m_pes.marker_bit0);
				fprintf(g_ts_ts,"    PTS[29...15]:%I64d\n",m_pes.PTS2915);
				fprintf(g_ts_ts,"    marker_bit:%d\n",m_pes.marker_bit1);
				fprintf(g_ts_ts,"    PTS[14...00]:%I64d\n",m_pes.PTS1400);
				fprintf(g_ts_ts,"    marker_bit:%d\n",m_pes.marker_bit2);
				fprintf(g_ts_ts,"    PTS:%I64d\n",m_pes.PTS);

				fprintf(g_ts_ts,"    0001:0x%X\n",m_pes.zero0001);
				fprintf(g_ts_ts,"    DTS[32...30]:%I64d\n",m_pes.DTS3230);
				fprintf(g_ts_ts,"    marker_bit:%d\n",m_pes.marker_bit0);
				fprintf(g_ts_ts,"    DTS[29...15]:%I64d\n",m_pes.DTS2915);
				fprintf(g_ts_ts,"    marker_bit:%d\n",m_pes.marker_bit1);
				fprintf(g_ts_ts,"    DTS[14...00]:%I64d\n",m_pes.DTS1400);
				fprintf(g_ts_ts,"    marker_bit:%d\n",m_pes.marker_bit2);

				fprintf(g_ts_ts,"    PTS:%I64d (",m_pes.PTS);
				{
					__int64 time = m_pes.PTS/90;//m_pes.PTS*300*1000/27000000; //毫秒
					fprintf(g_ts_ts,"%I64d:",time/3600000);time = time%3600000;//时
					fprintf(g_ts_ts,"%I64d:",time/60000);time = time%60000;//分
					fprintf(g_ts_ts,"%I64d.",time/1000);time = time%1000;//秒
					fprintf(g_ts_ts,"%I64d)\n",time);//毫秒
				}
				fprintf(g_ts_ts,"    DTS:%I64d (",m_pes.DTS);
				{
					__int64 time = m_pes.DTS/90;//m_pes.PTS*300*1000/27000000; //毫秒
					fprintf(g_ts_ts,"%I64d:",time/3600000);time = time%3600000;//时
					fprintf(g_ts_ts,"%I64d:",time/60000);time = time%60000;//分
					fprintf(g_ts_ts,"%I64d.",time/1000);time = time%1000;//秒
					fprintf(g_ts_ts,"%I64d)\n",time);//毫秒
				}

			}
			if (m_pes.ESCR_flag)
			{
				//待添加
			}
			if (m_pes.ES_rate_flag)
			{
				//待添加
			}
			if (m_pes.DSM_trick_mode_flag)
			{
				//待添加
			}
			if (m_pes.additional_copy_info_flag)
			{
				//待添加
			}
			if (m_pes.PES_CRC_flag)
			{
				//待添加
			}
			if (m_pes.PES_extension_flag)
			{
				//待添加
			}
		}
		else if(m_pes.stream_id == PROGRAM_STREAM_MAP     //stream_id == program_stream_map
			||m_pes.stream_id == PRIVATE_STREAM2          //|| stream_id == private_stream_2
			||m_pes.stream_id == ECM                      //|| stream_id == ECM
			||m_pes.stream_id == EMM                      //|| stream_id == EMM
			||m_pes.stream_id == PROGRAM_STREAM_DIRECTORY //|| stream_id == program_stream_directory
			||m_pes.stream_id == DSMCC_STREAM             //|| stream_id == DSMCC_stream
			||m_pes.stream_id == H222_1_E)                //|| stream_id == ITU-T H.222.1 建议书类型E stream )
		{
			//待添加
		}
		else if (m_pes.stream_id == PADDING_STREAM)
		{
			//待添加
		}
		
	}
}

/* 分析ts文件 */
void Anlysis::anlysis()
{
	author(m_param);
	m_tsConter = 0;   //初始化为0
	while(1)
	{
		if(!fread(m_ts.data,1,188,g_ts_file)) //读取TS信息
		{
			break;
		}
		m_ts.count = 0; //指示当前TS读取到的位置
		m_ts.left  = 8; //指示指示当前位置还剩余多少位
		if(!anlysisTsHeader())//分析ts Header文件
		{
			printf("sync_byte error");
			break;
		}

		
		if(m_tsHeader.adaptation_field_control == 2 || m_tsHeader.adaptation_field_control == 3)//自适应字段解析
		{
			adaptation_field();
		}

		if(m_tsHeader.PID != m_PATPID && m_tsHeader.PID != m_PMTPID && m_tsHeader.PID != m_AudioPID && m_tsHeader.PID != m_VideoPID )//未知PID待解析
		{
			//待添加
			if(m_param->b_ts)
			{
				fprintf(g_ts_ts,"目前只能解析PAT、PMT、音视频，其它PID暂无法解析\n");
			}
			m_tsConter++;
			continue;
		}

		if(m_tsHeader.adaptation_field_control == 1 || m_tsHeader.adaptation_field_control == 3)//基本数据
		{
			if(m_tsHeader.PID == m_PATPID) //解析PAT
			{
				anlysisPAT();
			}
			else if(m_tsHeader.PID == m_PMTPID)//解析PMT
			{
				anlysisPMT();
			}
			else if(m_tsHeader.PID == m_VideoPID)//解析视频
			{
				unsigned int packet_start_code_prefix = (m_ts.data[m_ts.count]<<16) + (m_ts.data[m_ts.count+1]<<8) + m_ts.data[m_ts.count+2];
				if(packet_start_code_prefix == 0x000001)
					anlysisPES();
				if(m_param->b_video)
				{
					fwrite(m_ts.data+m_ts.count,1,188-m_ts.count,g_ts_video);
				}
			}
			else if(m_tsHeader.PID == m_AudioPID)//解析音频
			{
				unsigned int packet_start_code_prefix = (m_ts.data[m_ts.count]<<16) + (m_ts.data[m_ts.count+1]<<8) + m_ts.data[m_ts.count+2];
				if(packet_start_code_prefix == 0x000001)
					anlysisPES();
			}
			else
			{
				//待添加
				if(m_param->b_ts)
				{
					fprintf(g_ts_ts,"目前只能解析PAT、PMT、音视频，其它PID暂无法解析\n");
				}
			}
		}
		m_tsConter++;
	}

	printf("errors:    %d\n",g_errors);

	//TS概要信息
	if(m_param->b_ts)
	{
		fprintf(g_ts_ts,"\n\n==================TS 概要信息=========================\n");
		fprintf(g_ts_ts,"TS    个数:    %d\n",m_tsConter);
		fprintf(g_ts_ts,"TS错误个数:    %d\n",g_errors);
	}
}