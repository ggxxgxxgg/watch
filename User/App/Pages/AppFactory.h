#include "Page.h"
class AppFactory : public PageFactory
{
public:
    /// <summary>
    /// �̳���PageFactory��CreatePage
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    PageBase* CreatePage(const char* name);
private:
};

