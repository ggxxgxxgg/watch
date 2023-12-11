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
    // �����л�����
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
    // ҳ�汻�϶�����
    typedef enum
    {
        ROOT_DRAG_DIR_NONE,
        ROOT_DRAG_DIR_HOR,
        ROOT_DRAG_DIR_VER,
        //_ROOT_DRAG_DIR_LAST = ROOT_DRAG_DIR_VER
    } RootDragDir_t;

    /* Animated setter */
    // ��������
    typedef void(*lv_anim_setter_t)(void*, int32_t);

    /* Animated getter */
    typedef int32_t(*lv_anim_getter_t)(void*);

    /* Animation switching record  */
    //�����л�ʱ��¼
    typedef struct
    {
        /* As the entered party */
        //��Ϊ���벿��
        struct
        {
            int32_t start;
            int32_t end;
        } enter;

        /* As the exited party */
        //��Ϊ�������
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
    //bool Install(const char* Name);//��Ϥ�ܹ���
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
    //����ȫ�ֵĶ�������
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
    /// �ж��Ƿ���Over���͵Ķ���
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
    /// �õ���ǰ�ļ��ض���������
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
    //ҳ�湤�� ÿ��Manager����һ��
    PageFactory* _Factory;

    /* Page pool */
    //����ע���ʱ�����е�page���������
    //�ܵ�ҳ���
    std::vector<PageBase*> _PagePool;

    /* Page stack */
    //pageջ�����������ʱ��Ҫ
    //Ϊ����ִ��ҳ����ת��ʱ�����õ�
    std::stack<PageBase*> _PageStack;

    /* Previous page */
    //ǰһ��page
    PageBase* _PagePrev;

    /* The current page */
    //�����page
    PageBase* _PageCurrent;

    /* Page animation status */
    //ҳ�涯��״̬
    struct
    {
        //�п�������
        bool IsSwitchReq;              // Has switch request
        //�Ƿ�æ
        bool IsBusy;                   // Is switching
        //���ڸ����ж�
        bool IsEntering;               // Is in entering action
        //��ǰ��������
        PageBase::AnimAttr_t Current;  // Current animation properties
        //ȫ�ֶ�������
        PageBase::AnimAttr_t Global;   // Global animation properties
    } _AnimState;

    /* Root style */
    //������ʽ
    lv_style_t* _RootDefaultStyle;
};

#endif
