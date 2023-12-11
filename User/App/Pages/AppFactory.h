#include "Page.h"
class AppFactory : public PageFactory
{
public:
    /// <summary>
    /// ¼Ì³ÐÓÚPageFactoryµÄCreatePage
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    PageBase* CreatePage(const char* name);
private:
};

