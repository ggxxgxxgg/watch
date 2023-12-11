#ifndef __STARTUP_MODEL_H
#define __STARTUP_MODEL_H

#include "Common/DataProc/DataProc.h"
#include "Common/HAL/HAL.h"

namespace Page
{
//视图（View）是显示和编辑数据的界面组件
//模型（Model）是视图与原始数据之间的接口。
/// <summary>
/// 事件
/// </summary>
class StartupModel
{
public:
    void Init();
    void Deinit();
    void PlayMusic(const char* music);
    void SetEncoderEnable(bool en)
    {
        HAL::Encoder_SetEnable(en);
    }
    void SetStatusBarAppear(bool en);
private:
    Account* account;
};

}

#endif
