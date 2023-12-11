#ifndef __PAGE_BASE_H
#define __PAGE_BASE_H

#include "lvgl/lvgl.h"
// 生成存储区数据
#define PAGE_STASH_MAKE(data) {&(data), sizeof(data)}

/*获取存储区的数据*/
#define PAGE_STASH_POP(data)  this->StashPop(&(data), sizeof(data))

//默认动画时间
#define PAGE_ANIM_TIME_DEFAULT 500 //[ms]

//默认动画PATH
#define PAGE_ANIM_PATH_DEFAULT lv_anim_path_overshoot

class PageManager;

/// <summary>
/// 页面的基础
/// </summary>
class PageBase
{
public:
    // 页面状态
    typedef enum
    {
        PAGE_STATE_IDLE,
        PAGE_STATE_LOAD,
        PAGE_STATE_WILL_APPEAR,
        PAGE_STATE_DID_APPEAR,
        PAGE_STATE_ACTIVITY,
        PAGE_STATE_WILL_DISAPPEAR,
        PAGE_STATE_DID_DISAPPEAR,
        PAGE_STATE_UNLOAD,
        _PAGE_STATE_LAST,
    } State_t;

    /* Stash data area */
    // 存储器区数据结构体
    typedef struct
    {
        void* ptr;//存储区的地址
        uint32_t size;//存储区的大小
    } Stash_t;

    /* Page switching animation properties */
    /* 页面切换动画属性 */
    typedef struct
    {
        uint8_t Type;//类型
        uint16_t Time;//时间
        lv_anim_path_cb_t Path;//路径
    } AnimAttr_t;

public:
    lv_obj_t* _root;       // UI root node
    PageManager* _Manager; // Page manager pointer
    const char* _Name;     // Page name
    uint16_t _ID;          // Page ID
    void* _UserData;       // User data pointer

    /* Private data, Only page manager access */
    struct
    {
        bool ReqEnableCache;        // 缓存启用请求
        bool ReqDisableAutoCache;   // 自动缓存管理启用请求

        bool IsDisableAutoCache;    // 是否为自动缓存管理
        bool IsCached;              // 是否缓存

        Stash_t Stash;              // Stash area
        State_t State;              // Page state

        /* Animation state  */
        struct
        {
            bool IsEnter;           // Whether it is the entering party
            bool IsBusy;            // Whether the animation is playing
            AnimAttr_t Attr;        // Animation properties
        } Anim;
    } priv;
public:
    virtual ~PageBase() {}

    /* 同步用户自定义属性配置 */
    virtual void onCustomAttrConfig() {}

    /* 页面加载开始 */
    virtual void onViewLoad() {}

    /* 页面加载结束 */
    virtual void onViewDidLoad() {}

    /* 页面显示动画开始 */
    virtual void onViewWillAppear() {}

    /* 页面显示动画结束 */
    virtual void onViewDidAppear() {}

    /* 页面消失动画开始 */
    virtual void onViewWillDisappear() {}

    /* 页面消失动画结束 */
    virtual void onViewDidDisappear() {}

    /* 页面卸载启动 */
    virtual void onViewUnload() {}

    /* 页面卸载结束 */
    virtual void onViewDidUnload() {}

    /* 设置是否手动管理缓存 */
    void SetCustomCacheEnable(bool en);

    /* 设置是否启用自动缓存 */
    void SetCustomAutoCacheEnable(bool en);

    /* 设置自定义动画属性 */
    void SetCustomLoadAnimType(
        uint8_t animType,
        uint16_t time = PAGE_ANIM_TIME_DEFAULT,
        lv_anim_path_cb_t path = PAGE_ANIM_PATH_DEFAULT
    );

    bool StashPop(void* ptr, uint32_t size);
};

#endif // ! __PAGE_BASE_H
