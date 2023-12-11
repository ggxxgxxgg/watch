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
/// �����Զ�����ض�������
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
/// ���������ݴ�������
/// </summary>
/// <param name="ptr">�������ݵĵ�ַ</param>
/// <param name="size">�������ݵĴ�С</param>
/// <returns></returns>
bool PageBase::StashPop(void* ptr, uint32_t size)
{
    //�������ݵĵ�ַΪ��
    if (ptr == nullptr){
        PM_LOG_WARN("ptr is nullptr");
        return false;
    }

    //û�����ݵ�ַ
    if (priv.Stash.ptr == nullptr)
    {
        PM_LOG_WARN("No Stash found");
        return false;
    }

    //���ݴ�С��ƥ��
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
    
    //��������
    memcpy(ptr, priv.Stash.ptr, priv.Stash.size);
    
    //ɾ������
    lv_mem_free(priv.Stash.ptr);
    
    //����ָ��Ϊ��
    priv.Stash.ptr = nullptr;
    return true;
}

