#include "StorageService.h"
#include "lvgl/lvgl.h"
#include <algorithm>

#define USE_STATIC_JSON_DOC        0
#if USE_STATIC_JSON_DOC
#  define STATIC_JSON_DOC_BUF_SIZE 2048
#endif

#define VALUE_TO_DOC(type)\
do{\
    type value = 0;\
    size_t size = iter->size > sizeof(value) ? sizeof(value) : iter->size;\
    memcpy(&value, iter->value, size);\
    doc[iter->key] = value;\
}while(0)

#define DOC_TO_VALUE(type)\
do{\
    type value = doc[iter->key];\
    size_t size = iter->size < sizeof(value) ? iter->size : sizeof(value);\
    memcpy(iter->value, &value, size);\
}while(0)

/// <summary>
/// 文件操作的封装类
/// </summary>
class FileWrapper
{
public:
    /// <summary>
    /// 构造函数
    /// </summary>
    /// <param name="path">传入地址参数</param>
    /// <param name="mode">文件打开模式</param>
    FileWrapper(const char* path, lv_fs_mode_t mode)
    {
        memset(&file, 0, sizeof(file));
        fs_res = lv_fs_open(&file, path, mode);
    }
    
    ~FileWrapper()
    {
        lv_fs_close(&file);
    }
    /// <summary>
    /// 读取一个字节
    /// </summary>
    /// <returns>读取到的数据</returns>
    uint8_t read()
    {
        uint8_t data = 0;
        readBytes(&data, 1);
        return data;
    }
    /// <summary>
    /// 读取指定的Byte
    /// </summary>
    /// <param name="buffer">存储buffer</param>
    /// <param name="length">读取的字节数</param>
    /// <returns>读取到的字节数</returns>
    size_t readBytes(void* buffer, size_t length)
    {
        uint32_t br = 0;
        lv_fs_read(&file, buffer, (uint32_t)length, &br);
        return br;
    }
    /// <summary>
    /// 写一个字节的数据
    /// </summary>
    /// <param name="c">要写的数据</param>
    /// <returns></returns>
    size_t write(uint8_t c)
    {
        return write(&c, 1);
    }
    /// <summary>
    /// 写指定长度的数据
    /// </summary>
    /// <param name="s">数据地址</param>
    /// <param name="n">长度</param>
    /// <returns>成功写入的字节数</returns>
    size_t write(const uint8_t* s, size_t n)
    {
        uint32_t bw = 0;
        lv_fs_write(&file, s, (uint32_t)n, &bw);
        return bw;
    }
    operator bool()
    {
        return fs_res == LV_FS_RES_OK;
    };
private:
    lv_fs_res_t fs_res;//文件操作结果
    lv_fs_file_t file;//文件
};

/// <summary>
/// 存储服务的构造函数
/// </summary>
/// <param name="filePath"></param>
/// <param name="bufferSize"></param>
StorageService::StorageService(const char* filePath, uint32_t bufferSize)
{
    FilePath = filePath;
    BufferSize = bufferSize;
}

StorageService::~StorageService()
{

}

/// <summary>
/// 增加数据
/// </summary>
/// <param name="key">数据节点名字</param>
/// <param name="value">数据指针</param>
/// <param name="size">数据大小</param>
/// <param name="type">数据类型</param>
/// <returns></returns>
bool StorageService::Add(const char* key, void* value, uint16_t size, DataType_t type)
{
    Node_t* findNode = SearchNode(key);

    //不为空 表示已经有节点了
    if (findNode != nullptr)
    {
        return false;
    }

    Node_t* node = new Node_t;//构造数据节点
    node->key = key;
    node->value = value;
    node->size = size;
    node->type = type;

    NodePool.push_back(node);

    return true;
}

/// <summary>
/// 依据名字删除数据池中的节点
/// </summary>
/// <param name="key">名字</param>
/// <returns></returns>
bool StorageService::Remove(const char* key)
{
    Node_t* node = SearchNode(key);//查找数据节点

    if (node == nullptr)
    {
        return false;
    }
    //迭代器遍历
    auto iter = std::find(NodePool.begin(), NodePool.end(), node);

    if (iter == NodePool.end())
    {
        return false;
    }

    NodePool.erase(iter);//擦除数据节点
    delete node;//释放内存

    return true;
}

/// <summary>
/// 加载文件
/// </summary>
/// <param name="mode">加载文件的模式</param>
/// <returns></returns>
bool StorageService::LoadFile(lv_fs_mode_t mode)
{
    FileWrapper file(FilePath, mode);
    bool retval = true;

    if (!file)///打开文件失败
    {
        LV_LOG_ERROR("Failed to open file: %s", FilePath);
        return false;
    }

#if USE_STATIC_JSON_DOC
    StaticJsonDocument<STATIC_JSON_DOC_BUF_SIZE> doc;
#else
    //DynamicJsonDocument doc(BufferSize);
#endif

    //DeserializationError error = deserializeJson(doc, file);
    //if (error)
    //{
    //    LV_LOG_ERROR("Failed to read file: %s", FilePath);
    //    return false;
    //}

    // Copy values from the JsonDocument to the Config
    /*for (auto iter : NodePool)
    {
        if (!doc.containsKey(iter->key))
        {
            LV_LOG_WARN("NOT contains key: %s, use default value", iter->key);
            retval = false;
            continue;
        }

        switch (iter->type)
        {
        case TYPE_INT:
        {
            DOC_TO_VALUE(int);
            break;
        }
        case TYPE_FLOAT:
        {
            DOC_TO_VALUE(float);
            break;
        }
        case TYPE_DOUBLE:
        {
            DOC_TO_VALUE(double);
            break;
        }
        case TYPE_STRING:
        {
            const char* str = doc[iter->key];
            if (str)
            {
                char* value = (char*)iter->value;
                strncpy(value, str, iter->size);
                value[iter->size - 1] = '\0';
            }
            break;
        }
        default:
            LV_LOG_ERROR("Unknow type: %d", iter->type);
            break;
        }
    }*/

    return retval;
}

bool StorageService::SaveFile(const char* backupPath)
{
    const char* path = backupPath ? backupPath : FilePath;

    // Open file for writing
    FileWrapper file(path, LV_FS_MODE_WR | LV_FS_MODE_RD);
    if (!file)
    {
        LV_LOG_ERROR("Failed to open file");
        return false;
    }

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use https://arduinojson.org/assistant to compute the capacity.

#if USE_STATIC_JSON_DOC
    StaticJsonDocument<STATIC_JSON_DOC_BUF_SIZE> doc;
#else
    //DynamicJsonDocument doc(BufferSize);
#endif

    // Set the values in the document
    /*for (auto iter : NodePool)
    {
        switch (iter->type)
        {
        case TYPE_INT:
        {
            VALUE_TO_DOC(int);
            break;
        }
        case TYPE_FLOAT:
        {
            VALUE_TO_DOC(float);
            break;
        }
        case TYPE_DOUBLE:
        {
            VALUE_TO_DOC(double);
            break;
        }
        case TYPE_STRING:
        {
            doc[iter->key] = (const char*)iter->value;
            break;
        }
        default:
            LV_LOG_ERROR("Unknow type: %d", iter->type);
            break;
        }
    }*/

    // Serialize JSON to file
    //if (!serializeJsonPretty(doc, file))
    //{
    //    LV_LOG_ERROR("Failed to write to file");
    //    return false;
    //}
    return true;
}

/// <summary>
/// 在节点池中查询节点
/// </summary>
/// <param name="key">查询的节点名字</param>
/// <returns>查询成功返回节点指针 失败返回空指针</returns>
StorageService::Node_t* StorageService::SearchNode(const char* key)
{
    for (auto iter : NodePool)
    {
        if (strcmp(key, iter->key) == 0)
        {
            return iter;
        }
    }
    return nullptr;
}
