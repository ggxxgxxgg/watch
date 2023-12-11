#ifndef __TEMPLATE_PRESENTER_H
#define __TEMPLATE_PRESENTER_H

#include "TemplateView.h"
#include "TemplateModel.h"

#include<iostream>
namespace Page
{
/// <summary>
/// 用于创建页面时指定的类(名),继承PageBase
/// </summary>
class Template : public PageBase
{
public:
    typedef struct
    {
        uint16_t time;
        lv_color_t color;
    } Param_t;
public:
    Template();
    ~Template();

    virtual void onCustomAttrConfig();
    virtual void onViewLoad();
    virtual void onViewDidLoad();
    virtual void onViewWillAppear();
    virtual void onViewDidAppear();
    virtual void onViewWillDisappear();
    virtual void onViewDidDisappear();
    virtual void onViewUnload();
    virtual void onViewDidUnload();

    void AddBubble(std::string message);
private:
    void Update();
    void AttachEvent(lv_obj_t* obj, lv_event_code_t filter = LV_EVENT_ALL, void* user_data = nullptr);
    static void onTimerUpdate(lv_timer_t* timer);
    static void onEvent(lv_event_t* event);
    
private:
    TemplateView View;
    TemplateModel Model;
    lv_timer_t* timer;
};

}

#endif
