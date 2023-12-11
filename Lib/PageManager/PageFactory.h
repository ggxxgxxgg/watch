#ifndef __PAGE_FACTORY_H
#define __PAGE_FACTORY_H

#include "PageBase.h"
#include "PM_Log.h"

class PageFactory
{
public:
    /// <summary>
    /// ����ҳ��Ļ��࣬AppFactory�̳�  ����Ϊ�˷ֱ�APP��Page
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
