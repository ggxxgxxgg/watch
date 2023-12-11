#ifndef __STORAGE_SERVICE_H
#define __STORAGE_SERVICE_H

#include <stdint.h>
#include <vector>
#include "lvgl/lvgl.h"

/// <summary>
/// �洢����
/// </summary>
class StorageService
{
public:
    /// <summary>
    /// ��������
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
    bool SaveFile(const char* backupPath = nullptr);//����Ϊjson�ļ�
    bool LoadFile(lv_fs_mode_t mode = LV_FS_MODE_RD);//
private:
    /// <summary>
    /// ��ֵ�Խڵ�
    /// </summary>
    typedef struct
    {
        const char* key;//����
        void* value;//����
        uint16_t size;//��С
        DataType_t type;//��������
    } Node_t;
private:
    const char* FilePath;//�ļ�·��
    std::vector<Node_t*> NodePool;//�ڵ��
    uint32_t BufferSize;//�����С
private:
    Node_t* SearchNode(const char* key);    
};

#endif
