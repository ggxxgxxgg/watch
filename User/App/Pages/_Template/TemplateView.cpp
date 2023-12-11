#include "TemplateView.h"
#include <stdarg.h>
#include <stdio.h>

using namespace Page;

void TemplateView::Create(lv_obj_t* root)
{
    lv_obj_t* ta = lv_textarea_create(root);
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_set_width(ta, 150);
    lv_textarea_set_one_line(ta, true);
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 10);
    
    lv_obj_add_state(ta, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
    ui.input = ta;

    lv_obj_t* cont = lv_obj_create(root);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 15);
    lv_obj_set_width(cont, 230);
    lv_obj_set_height(cont, 150);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    ui.cont = cont;

    lv_obj_t* btn = lv_btn_create(root);
    lv_obj_align_to(btn, cont, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
    ui.btnRecv = btn;

    btn = lv_btn_create(root);
    lv_obj_align_to(btn, cont, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 5);
    ui.btnSend = btn;

    //img = lv_img_create(root);
    //lv_obj_align(img, LV_ALIGN_BOTTOM_MID, 0, 0);
    //lv_img_set_src(img, ResourcePool::GetImage("user"));
}
