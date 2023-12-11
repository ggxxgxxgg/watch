#include "PageBase.h"
#include "PM_Log.h"
/// <summary>
/// 
/// </summary>
/// <param name="en"></param>
void PageBase::SetCustomCacheEnable(bool en)
{
    PM_LOG_INFO("Page(%s) %s = %d", _Name, __func__, en);
    SetCustomAutoCacheEnable(false);
    priv.ReqEnableCache = en;
}
/// <summary>
/// 
/// </summary>
/// <param name="en"></param>
void PageBase::SetCustomAutoCacheEnable(bool en)
{
    PM_LOG_INFO("Page(%s) %s = %d", _Name, __func__, en);
    priv.ReqDisableAutoCache = !en;
}
/// <summary>
/// 设置自定义加载动画类型
/// </summary>
/// <param name="animType">PageManager::LoadAnim_t Type</param>
/// <param name="time"></param>
/// <param name="path"></param>
void PageBase::SetCustomLoadAnimType(
    uint8_t animType,
    uint16_t time,
    lv_anim_path_cb_t path
)
{
    priv.Anim.Attr.Type = animType;
    priv.Anim.Attr.Time = time;
    priv.Anim.Attr.Path = path;
}
/// <summary>
/// 弹出数据暂存区数据
/// </summary>
/// <param name="ptr">接收数据的地址</param>
/// <param name="size">接收数据的大小</param>
/// <returns></returns>
bool PageBase::StashPop(void* ptr, uint32_t size)
{
    //接收数据的地址为空
    if (ptr == nullptr){
        PM_LOG_WARN("ptr is nullptr");
        return false;
    }

    //没有数据地址
    if (priv.Stash.ptr == nullptr)
    {
        PM_LOG_WARN("No Stash found");
        return false;
    }

    //数据大小不匹配
    if (priv.Stash.size != size)
    {
        PM_LOG_WARN(
            "Stash[0x%p](%d) does not match the size(%d)",
            priv.Stash.ptr,
            priv.Stash.size,
            size
        );
        return false;
    }
    
    //复制数据
    memcpy(ptr, priv.Stash.ptr, priv.Stash.size);
    
    //删除数据
    lv_mem_free(priv.Stash.ptr);
    
    //数据指针为空
    priv.Stash.ptr = nullptr;
    return true;
}

