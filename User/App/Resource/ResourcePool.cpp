#include "ResourcePool.h"
#include "ResourceManager/ResourceManager.h"

static ResourceManager Font_;
static ResourceManager Image_;

extern "C" {
#define IMPORT_FONT(name) \
do{\
    LV_FONT_DECLARE(font_##name)\
    Font_.AddResource(#name, (void*)&font_##name);\
}while(0)

#define IMPORT_IMG(name) \
do{\
    LV_IMG_DECLARE(img_src_##name)\
    Image_.AddResource(#name, (void*)&img_src_##name);\
}while (0)

    /// <summary>
    /// 声明要导入的资源
    /// </summary>
    static void Resource_Init()
    {
        /* Import Fonts */
        //IMPORT_FONT(bahnschrift_13);
        //IMPORT_FONT(bahnschrift_17);
        //IMPORT_FONT(bahnschrift_32);
        //IMPORT_FONT(bahnschrift_65);
        //IMPORT_FONT(agencyb_36);

        /* Import Images */
        IMPORT_IMG(user);
    }
} /* extern "C" */

/// <summary>
/// 初始化资源池
/// </summary>
void ResourcePool::Init()
{
    Resource_Init();
    Font_.SetDefault((void*)LV_FONT_DEFAULT);
}

lv_font_t* ResourcePool::GetFont(const char* name)
{
    return (lv_font_t*)Font_.GetResource(name);
}

const void* ResourcePool::GetImage(const char* name)
{
    return Image_.GetResource(name);
}
