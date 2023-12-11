#ifndef __DATA_PROC_H
#define __DATA_PROC_H

#include "DataCenter/DataCenter.h"
#include "../HAL/HAL_Def.h"
#include "DataProc_Def.h"
#include "Config/Config.h"

//用以数据处理 要绑定的数据以及回调函数
#define DATA_PROC_INIT_DEF(name)   void _DP_##name##_Init(Account* account)
//数据处理结构体的初始化
#define DATA_PROC_INIT_STRUCT(sct) memset(&sct, 0, sizeof(sct))

void DataProc_Init();

//数据处理中心
namespace DataProc
{
	DataCenter* Center();
	uint32_t    GetTick();
	uint32_t    GetTickElaps(uint32_t prevTick);
	const char* MakeTimeString(uint64_t ms, char* buf, uint16_t len);
}

#endif
