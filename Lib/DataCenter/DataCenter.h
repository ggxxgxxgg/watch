#ifndef __DATA_CENTER_H
#define __DATA_CENTER_H

#include "Account.h"

class DataCenter
{
public:
    /* The name of the data center will be used as the ID of the main account */
    const char* Name;

    /* Main account, will automatically follow all accounts */
    //一个总的账户 用以关注每一个账户
    Account AccountMain;
public:
    DataCenter(const char* name);
    ~DataCenter();
    bool AddAccount(Account* account);
    bool RemoveAccount(Account* account);
    bool Remove(Account::AccountVector_t* vec, Account* account);
    Account* SearchAccount(const char* id);
    Account* Find(Account::AccountVector_t* vec, const char* id);
    size_t GetAccountLen();
private:
    /* Account pool */
    //账户池 用以对每个账户进行遍历
    Account::AccountVector_t AccountPool;
};

#define DATA_CENTER_USE_LOG 0

#if DATA_CENTER_USE_LOG
#include <stdio.h>
#  define _DC_LOG(format, ...)      printf("[DC]" format "\r\n", ##__VA_ARGS__)
#  define DC_LOG_INFO(format, ...)  _DC_LOG("[Info] "format, ##__VA_ARGS__)
#  define DC_LOG_WARN(format, ...)  _DC_LOG("[Warn] " format, ##__VA_ARGS__)
#  define DC_LOG_ERROR(format, ...) _DC_LOG("[Error] " format, ##__VA_ARGS__)
#else
#  define DC_LOG_INFO(...)
#  define DC_LOG_WARN(...)
#  define DC_LOG_ERROR(...)
#endif

#endif
