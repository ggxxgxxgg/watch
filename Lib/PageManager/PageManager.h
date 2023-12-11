#ifndef __PAGE_MANAGER_H
#define __PAGE_MANAGER_H

#include "PageBase.h"
#include "PageFactory.h"
#include <vector>
#include <stack>

class PageManager
{
public:
    /* Page switching animation type  */
    // 动画切换类型
    typedef enum
    {
        /* Default (global) animation type  */
        LOAD_ANIM_GLOBAL = 0,

        /* New page overwrites old page  */
        LOAD_ANIM_OVER_LEFT,
        LOAD_ANIM_OVER_RIGHT,
        LOAD_ANIM_OVER_TOP,
        LOAD_ANIM_OVER_BOTTOM,

        /* New page pushes old page  */
        LOAD_ANIM_MOVE_LEFT,
        LOAD_ANIM_MOVE_RIGHT,
        LOAD_ANIM_MOVE_TOP,
        LOAD_ANIM_MOVE_BOTTOM,

        /* The new interface fades in, the old page fades out */
        LOAD_ANIM_FADE_ON,

        /* No animation */
        LOAD_ANIM_NONE,

        _LOAD_ANIM_LAST = LOAD_ANIM_NONE
    } LoadAnim_t;

    /* Page dragging direction */
    // 页面被拖动方向
    typedef enum
    {
        ROOT_DRAG_DIR_NONE,
        ROOT_DRAG_DIR_HOR,
        ROOT_DRAG_DIR_VER,
        //_ROOT_DRAG_DIR_LAST = ROOT_DRAG_DIR_VER
    } RootDragDir_t;

    /* Animated setter */
    // 动画设置
    typedef void(*lv_anim_setter_t)(void*, int32_t);

    /* Animated getter */
    typedef int32_t(*lv_anim_getter_t)(void*);

    /* Animation switching record  */
    //动画切换时记录
    typedef struct
    {
        /* As the entered party */
        //作为输入部分
        struct
        {
            int32_t start;
            int32_t end;
        } enter;

        /* As the exited party */
        //作为输出部分
        struct
        {
            int32_t start;
            int32_t end;
        } exit;
    } AnimValue_t;

    /* Page loading animation properties */
    typedef struct
    {
        lv_anim_setter_t setter;
        lv_anim_getter_t getter;
        RootDragDir_t dragDir;
        AnimValue_t push;
        AnimValue_t pop;
    } LoadAnimAttr_t;

public:
    PageManager(PageFactory* factory = nullptr);
    ~PageManager();

    /* Loader */
    bool Install(const char* className, const char* appName);
    //bool Install(const char* Name);//熟悉架构后
    bool Uninstall(const char* appName);

    bool Register(PageBase* base, const char* name);
    bool Unregister(const char* name);

    /* Router */
    bool Replace(const char* name, const PageBase::Stash_t* stash = nullptr);
    bool Push(const char* name, const PageBase::Stash_t* stash = nullptr);
    bool Pop();
    bool BackHome();
    const char* GetPagePrevName();

    /* Global Animation */
    //设置全局的动画类型
    void SetGlobalLoadAnimType(
        LoadAnim_t anim = LOAD_ANIM_OVER_LEFT,
        uint16_t time = 500,
        lv_anim_path_cb_t path = lv_anim_path_ease_out
    );

    void SetRootDefaultStyle(lv_style_t* style)
    {
        _RootDefaultStyle = style;
    }

private:
    /* Page Pool */
    PageBase* FindPageInPool(const char* name);

    /* Page Stack */
    PageBase* FindPageInStack(const char* name);
    PageBase* GetStackTop();
    PageBase* GetStackTopAfter();
    void SetStackClear(bool keepBottom = false);
    bool FourceUnload(PageBase* base);

    /* Animation */
    bool GetLoadAnimAttr(uint8_t anim, LoadAnimAttr_t* attr);
    /// <summary>
    /// 判断是否是Over类型的动画
    /// </summary>
    /// <param name="anim"></param>
    /// <returns></returns>
    bool GetIsOverAnim(uint8_t anim)
    {
        return (anim >= LOAD_ANIM_OVER_LEFT && anim <= LOAD_ANIM_OVER_BOTTOM);
    }
    bool GetIsMoveAnim(uint8_t anim)
    {
        return (anim >= LOAD_ANIM_MOVE_LEFT && anim <= LOAD_ANIM_MOVE_BOTTOM);
    }
    void AnimDefaultInit(lv_anim_t* a);
    /// <summary>
    /// 
    /// </summary>
    /// <param name="attr"></param>
    /// <returns></returns>
    bool GetCurrentLoadAnimAttr(LoadAnimAttr_t* attr)
    {
        return GetLoadAnimAttr(GetCurrentLoadAnimType(), attr);
    }
    /// <summary>
    /// 得到当前的加载动画的类型
    /// </summary>
    /// <returns></returns>
    LoadAnim_t GetCurrentLoadAnimType()
    {
        return (LoadAnim_t)_AnimState.Current.Type;
    }

    /* Root */
    static void onRootDragEvent(lv_event_t* event);
    static void onRootDragAnimFinish(lv_anim_t* a);
    static void onRootAsyncLeave(void* base);
    void RootEnableDrag(lv_obj_t* root);
    static void RootGetDragPredict(lv_coord_t* x, lv_coord_t* y);

    /* Switch */
    bool SwitchTo(PageBase* base, bool isEnterAct, const PageBase::Stash_t* stash = nullptr);
    static void onSwitchAnimFinish(lv_anim_t* a);
    void SwitchAnimCreate(PageBase* base);
    void SwitchAnimTypeUpdate(PageBase* base);
    bool SwitchReqCheck();
    bool SwitchAnimStateCheck();

    /* State */
    PageBase::State_t StateLoadExecute(PageBase* base);
    PageBase::State_t StateWillAppearExecute(PageBase* base);
    PageBase::State_t StateDidAppearExecute(PageBase* base);
    PageBase::State_t StateWillDisappearExecute(PageBase* base);
    PageBase::State_t StateDidDisappearExecute(PageBase* base);
    PageBase::State_t StateUnloadExecute(PageBase* base);
    void StateUpdate(PageBase* base);
    PageBase::State_t GetState(void)
    {
        return _PageCurrent->priv.State;
    }

private:
    /* Page factory */
    //页面工厂 每个Manager都有一个
    PageFactory* _Factory;

    /* Page pool */
    //是在注册的时候将所有的page都放入池中
    //总的页面池
    std::vector<PageBase*> _PagePool;

    /* Page stack */
    //page栈，在任务调度时需要
    //为了在执行页面跳转的时候设置的
    std::stack<PageBase*> _PageStack;

    /* Previous page */
    //前一个page
    PageBase* _PagePrev;

    /* The current page */
    //最近的page
    PageBase* _PageCurrent;

    /* Page animation status */
    //页面动画状态
    struct
    {
        //有开关请求
        bool IsSwitchReq;              // Has switch request
        //是否忙
        bool IsBusy;                   // Is switching
        //正在付诸行动
        bool IsEntering;               // Is in entering action
        //当前动画属性
        PageBase::AnimAttr_t Current;  // Current animation properties
        //全局动画属性
        PageBase::AnimAttr_t Global;   // Global animation properties
    } _AnimState;

    /* Root style */
    //根的样式
    lv_style_t* _RootDefaultStyle;
};

#endif
