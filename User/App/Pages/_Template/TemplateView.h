#ifndef __TEMPLATE_VIEW_H
#define __TEMPLATE_VIEW_H

#include "../Page.h"

namespace Page
{

/// <summary>
/// ҳ�����ͼ��
/// </summary>
class TemplateView
{
public:
    /// <summary>
    /// ������ͼ
    /// </summary>
    /// <param name="root">������ͼ��root</param>
    void Create(lv_obj_t* root);
public:
    struct
    {
        lv_obj_t* input;

        lv_obj_t* cont;
        lv_obj_t* btnSend;
        lv_obj_t* btnRecv;
    } ui;
    
    typedef struct
    {
        lv_obj_t* img;
        lv_obj_t* message;
    }chatBubble;
private:

};

}

#endif // !__VIEW_H
