/*
 * MIT License
 * Copyright (c) 2021 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __ACCOUNT_H
#define __ACCOUNT_H

#include <stdint.h>
#include <vector>
#include "PingPongBuffer/PingPongBuffer.h"
#include "lvgl/lvgl.h"

class DataCenter;

class Account
{
public:

    // 事件类型
    typedef enum
    {
        EVENT_NONE,
        EVENT_PUB_PUBLISH, // 发布者发布信息
        EVENT_SUB_PULL,    // 用户数据提取请求
        EVENT_NOTIFY,      // 订阅者向发布者发送通知
        EVENT_TIMER,       // 定时事件
        _EVENT_LAST
    } EventCode_t;

    //错误类型
    typedef enum
    {
        RES_OK                  =  0,
        RES_UNKNOW              = -1,
        RES_SIZE_MISMATCH       = -2,
        RES_UNSUPPORTED_REQUEST = -3,
        RES_NO_CALLBACK         = -4,
        RES_NO_CACHE            = -5,
        RES_NO_COMMITED         = -6,
        RES_NOT_FOUND           = -7,
        RES_PARAM_ERROR         = -8
    } ResCode_t;

    //事件参数结构体
    typedef struct
    {
        EventCode_t event; // 事件类型
        Account* tran;     // 发送方指针
        Account* recv;     // 接受方指针
        void* data_p;      // 数据指针
        uint32_t size;     // 数据的长度
    } EventParam_t;

    //回调函数
    typedef int(*EventCallback_t)(Account* account, EventParam_t* param);

    //用户容器
    typedef std::vector<Account*> AccountVector_t;

public:
    Account(
        const char* id,
        DataCenter* center,
        uint32_t bufSize = 0,
        void* userData = nullptr
    );
    ~Account();

    Account* Subscribe(const char* pubID);
    bool Unsubscribe(const char* pubID);
    bool Commit(const void* data_p, uint32_t size);
    int Publish();
    int Pull(const char* pubID, void* data_p, uint32_t size);
    int Pull(Account* pub, void* data_p, uint32_t size);
    int Notify(const char* pubID, const void* data_p, uint32_t size);
    int Notify(Account* pub, const void* data_p, uint32_t size);
    void SetEventCallback(EventCallback_t callback);
    void SetTimerPeriod(uint32_t period);
    void SetTimerEnable(bool en);
    size_t GetPublishersSize();
    size_t GetSubscribersSize();

public:
    const char* ID;         //唯一帐户ID
    DataCenter* Center;     //指向数据中心的指针
    void* UserData;         //

    AccountVector_t publishers;  //订阅的发布者
    AccountVector_t subscribers; //订阅者

    //账户的结构体
    struct
    {
        EventCallback_t eventCallback;  //回调函数
        lv_timer_t* timer;              //定时器
        PingPongBuffer_t BufferManager; //Buffer管理 格式为uint8_t
        uint32_t BufferSize;            //单个buffer的大小
    } priv;
private:
    //回调函数
    static void TimerCallbackHandler(lv_timer_t* task);
};

#endif
