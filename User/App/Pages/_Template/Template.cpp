#include "Template.h"

using namespace Page;

Template::Template()
    : timer(nullptr)
{

}

Template::~Template()
{

}

/// @brief 同步用户自定义属性配置
void Template::onCustomAttrConfig()
{
    //LV_LOG_USER(__func__);
    LV_LOG_USER("Template::onCustomAttrConfig");

    SetCustomCacheEnable(true);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_FADE_ON, 500, lv_anim_path_bounce);
}

void Template::onViewLoad()
{
    //LV_LOG_USER(__func__);
    LV_LOG("Template::onViewLoad\n");
    View.Create(_root);
    //lv_label_set_text(View.ui.labelTitle, _Name);
    AttachEvent(_root);
    AttachEvent(View.ui.input, LV_EVENT_READY, View.ui.input);
    AttachEvent(View.ui.btnRecv);
    AttachEvent(View.ui.btnSend);

    Model.TickSave = Model.GetData();
}

void Template::onViewDidLoad()
{
    //LV_LOG_USER(__func__);
    LV_LOG("Template::onViewDidLoad\n");
}

void Template::onViewWillAppear()
{
    //LV_LOG_USER(__func__);
    LV_LOG("Template::onViewWillAppear\n");

    Param_t param;
    param.color = lv_color_make(20, 220, 80);
    param.time = 100;

    int myData = 42;
    struct {
        int* ptr;
        size_t size;
    } myStash = PAGE_STASH_MAKE(myData);

    PAGE_STASH_POP(param);
    lv_obj_set_style_bg_color(_root, param.color, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(_root, LV_OPA_TRANSP, LV_PART_MAIN);
    timer = lv_timer_create(onTimerUpdate, param.time, this);
}

void Template::onViewDidAppear()
{
    LV_LOG("Template::onViewDidAppear\n");
}

void Template::onViewWillDisappear()
{
    //LV_LOG_USER(__func__);
    LV_LOG("Template::onViewWillDisappear\n");
}

void Template::onViewDidDisappear()
{
    //LV_LOG_USER(__func__);
    LV_LOG("Template::onViewDidDisappear\n");

    lv_timer_del(timer);
}

void Template::onViewUnload()
{
    //LV_LOG_USER(__func__);
    LV_LOG("Template::onViewUnload\n");
}

void Template::onViewDidUnload()
{
    //LV_LOG_USER(__func__);
    LV_LOG("Template::onViewDidUnload\n");
}

void Template::AttachEvent(lv_obj_t* obj, lv_event_code_t filter, void* user_data)
{
    if (user_data != nullptr)
    {
        lv_obj_set_user_data(obj, user_data);
    }
    else {
        lv_obj_set_user_data(obj, this);
    }
    lv_obj_add_event_cb(obj, onEvent, filter, this);
}

void Template::Update()
{

}

void Template::onTimerUpdate(lv_timer_t* timer)
{
    Template* instance = (Template*)timer->user_data;

    instance->Update();
}

void Template::onEvent(lv_event_t* event)
{
    Template* instance = (Template*)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t* obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if (obj == instance->_root)
    {
        if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LEAVE)
        {
            instance->_Manager->Pop();
        }
    }
    else if (obj == instance->View.ui.btnRecv) {
        if (code == LV_EVENT_RELEASED)
        {
            LV_LOG("btnRecv\n");
            instance->AddBubble("btnRecv");
        }
    }
    else if (obj == instance->View.ui.btnSend) {
        if (code == LV_EVENT_RELEASED)
        {
            LV_LOG("btnSend\n");
        }
    }
    else if (obj == instance->View.ui.input) {
        const char* msg = lv_textarea_get_text(obj);
        LV_LOG("input:%s\n", msg);
        instance->AddBubble(msg);
    }
}

void Template::AddBubble(std::string message) {
    lv_obj_t* cont = lv_obj_create(View.ui.cont);
    lv_obj_t* label = lv_label_create(cont);
    int maxCharNum = 10;
    LV_LOG("message.length:%d\n", message.length());
    lv_obj_set_width(cont, (message.length()<= maxCharNum ? message.length() : maxCharNum) * 10 + 20);
    for (int i = 1; i <= (int)floor(message.length() / (maxCharNum + 1)); i++)
    {
        message.insert(i * maxCharNum , "\n"); 
    }
    LV_LOG("message:%s\n", message.c_str());
    //lv_obj_set_height(cont, (message.length() / maxCharNum + 1) * 10 +30);
    lv_obj_set_height(cont, 50);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_center(label);
    lv_label_set_text_fmt(label, "%s", message.c_str());
    
}