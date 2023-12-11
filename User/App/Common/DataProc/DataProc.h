#ifndef __DATA_PROC_H
#define __DATA_PROC_H

#include "DataCenter/DataCenter.h"
#include "../HAL/HAL_Def.h"
#include "DataProc_Def.h"
#include "Config/Config.h"

//�������ݴ��� Ҫ�󶨵������Լ��ص�����
#define DATA_PROC_INIT_DEF(name)   void _DP_##name##_Init(Account* account)
//���ݴ���ṹ��ĳ�ʼ��
#define DATA_PROC_INIT_STRUCT(sct) memset(&sct, 0, sizeof(sct))

void DataProc_Init();

//���ݴ�������
namespace DataProc
{
	DataCenter* Center();
	uint32_t    GetTick();
	uint32_t    GetTickElaps(uint32_t prevTick);
	const char* MakeTimeString(uint64_t ms, char* buf, uint16_t len);
}

#endif
