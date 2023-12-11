#ifndef __STORAGE_SERVICE_H
#define __STORAGE_SERVICE_H

#include <stdint.h>
#include <vector>
#include "lvgl/lvgl.h"

/// <summary>
/// 存储服务
/// </summary>
class StorageService
{
public:
    /// <summary>
    /// 数据类型
    /// </summary>
    typedef enum
    {
        TYPE_UNKNOW,
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_DOUBLE,
        TYPE_STRING
    } DataType_t;
public:
    StorageService(const char* filePath, uint32_t bufferSize = 1024);
    ~StorageService();
    bool Add(const char* key, void* value, uint16_t size, DataType_t type);
    bool Remove(const char* key);
    bool SaveFile(const char* backupPath = nullptr);//保存为json文件
    bool LoadFile(lv_fs_mode_t mode = LV_FS_MODE_RD);//
private:
    /// <summary>
    /// 键值对节点
    /// </summary>
    typedef struct
    {
        const char* key;//名字
        void* value;//数据
        uint16_t size;//大小
        DataType_t type;//数据类型
    } Node_t;
private:
    const char* FilePath;//文件路径
    std::vector<Node_t*> NodePool;//节点池
    uint32_t BufferSize;//缓存大小
private:
    Node_t* SearchNode(const char* key);    
};

#endif
