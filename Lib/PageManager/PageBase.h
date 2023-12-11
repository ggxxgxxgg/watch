#ifndef __PAGE_BASE_H
#define __PAGE_BASE_H

#include "lvgl/lvgl.h"
// ���ɴ洢������
#define PAGE_STASH_MAKE(data) {&(data), sizeof(data)}

/*��ȡ�洢��������*/
#define PAGE_STASH_POP(data)  this->StashPop(&(data), sizeof(data))

//Ĭ�϶���ʱ��
#define PAGE_ANIM_TIME_DEFAULT 500 //[ms]

//Ĭ�϶���PATH
#define PAGE_ANIM_PATH_DEFAULT lv_anim_path_overshoot

class PageManager;

/// <summary>
/// ҳ��Ļ���
/// </summary>
class PageBase
{
public:
    // ҳ��״̬
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
    // �洢�������ݽṹ��
    typedef struct
    {
        void* ptr;//�洢���ĵ�ַ
        uint32_t size;//�洢���Ĵ�С
    } Stash_t;

    /* Page switching animation properties */
    /* ҳ���л��������� */
    typedef struct
    {
        uint8_t Type;//����
        uint16_t Time;//ʱ��
        lv_anim_path_cb_t Path;//·��
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
        bool ReqEnableCache;        // ������������
        bool ReqDisableAutoCache;   // �Զ����������������

        bool IsDisableAutoCache;    // �Ƿ�Ϊ�Զ��������
        bool IsCached;              // �Ƿ񻺴�

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

    /* ͬ���û��Զ����������� */
    virtual void onCustomAttrConfig() {}

    /* ҳ����ؿ�ʼ */
    virtual void onViewLoad() {}

    /* ҳ����ؽ��� */
    virtual void onViewDidLoad() {}

    /* ҳ����ʾ������ʼ */
    virtual void onViewWillAppear() {}

    /* ҳ����ʾ�������� */
    virtual void onViewDidAppear() {}

    /* ҳ����ʧ������ʼ */
    virtual void onViewWillDisappear() {}

    /* ҳ����ʧ�������� */
    virtual void onViewDidDisappear() {}

    /* ҳ��ж������ */
    virtual void onViewUnload() {}

    /* ҳ��ж�ؽ��� */
    virtual void onViewDidUnload() {}

    /* �����Ƿ��ֶ������� */
    void SetCustomCacheEnable(bool en);

    /* �����Ƿ������Զ����� */
    void SetCustomAutoCacheEnable(bool en);

    /* �����Զ��嶯������ */
    void SetCustomLoadAnimType(
        uint8_t animType,
        uint16_t time = PAGE_ANIM_TIME_DEFAULT,
        lv_anim_path_cb_t path = PAGE_ANIM_PATH_DEFAULT
    );

    bool StashPop(void* ptr, uint32_t size);
};

#endif // ! __PAGE_BASE_H
