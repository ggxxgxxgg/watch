#include "Config/Config.h"
#include "Common/DataProc/DataProc.h"
#include "Resource/ResourcePool.h"
#include "Pages/AppFactory.h"
//#include "Pages/StatusBar/StatusBar.h"
#include "PageManager/PageManager.h"


#define ACCOUNT_SEND_CMD(ACT, CMD) \
do{ \
    DataProc::ACT##_Info_t info; \
    DATA_PROC_INIT_STRUCT(info); \
    info.cmd = DataProc::CMD; \
    DataProc::Center()->AccountMain.Notify(#ACT, &info, sizeof(info)); \
}while(0)

void App_Init()
{
    /* 确保默认组存在 */
    if(!lv_group_get_default())
    {
        lv_group_t* group = lv_group_create();
        lv_group_set_default(group);
    }

    /* 初始化数据处理节点 */
    DataProc_Init();
    //ACCOUNT_SEND_CMD(Storage, STORAGE_CMD_LOAD);
    //ACCOUNT_SEND_CMD(SysConfig, SYSCONFIG_CMD_LOAD);

    /* 设置屏幕样式 */
    lv_obj_t* scr = lv_scr_act();
    lv_obj_remove_style_all(scr);
    lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
    lv_disp_set_bg_color(lv_disp_get_default(), lv_color_black());

    /* 设置根默认样式 */
    //每一个App（PageBase）都有一个
    static lv_style_t rootStyle;
    lv_style_init(&rootStyle);
    lv_style_set_width(&rootStyle, LV_HOR_RES);
    lv_style_set_height(&rootStyle, LV_VER_RES);
    //lv_style_set_border_width(&rootStyle, 0);
    lv_style_set_bg_opa(&rootStyle, LV_OPA_COVER);
    lv_style_set_bg_color(&rootStyle, lv_color_black());
    
    static AppFactory factory;//app 工厂
    static PageManager manager(&factory);//页面管理
    manager.SetRootDefaultStyle(&rootStyle);

    /* Initialize resource pool */
    ResourcePool::Init();

    /* Initialize status bar */
    //Page::StatusBar_Create(lv_layer_top());

    /* Initialize pages */
    manager.Install("Template","Template");
    //manager.Install("LiveMap",     "Pages/LiveMap");
    //manager.Install("Dialplate",   "Pages/Dialplate");
    //manager.Install("SystemInfos", "Pages/SystemInfos");
    manager.Install("Startup","Startup");

    //manager.SetGlobalLoadAnimType(PageManager::LOAD_ANIM_OVER_TOP);

    manager.Push("Startup");
}

void App_Uninit()
{
    ACCOUNT_SEND_CMD(SysConfig, SYSCONFIG_CMD_SAVE);
    ACCOUNT_SEND_CMD(Storage,   STORAGE_CMD_SAVE);
    ACCOUNT_SEND_CMD(Recorder,  RECORDER_CMD_STOP);
}
