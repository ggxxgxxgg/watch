#ifndef __PAGE_FACTORY_H
#define __PAGE_FACTORY_H

#include "PageBase.h"
#include "PM_Log.h"

class PageFactory
{
public:
    /// <summary>
    /// 创建页面的基类，AppFactory继承  可能为了分辨APP和Page
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    virtual PageBase* CreatePage(const char* name)
    {
        PM_LOG_INFO("This is virtual PageBase* CreatePage");
        return nullptr;
    };
};


#endif // !__PAGE_FACTORY_H
