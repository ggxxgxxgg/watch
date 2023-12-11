#include "Account.h"
#include "DataCenter.h"

//������
#define ACCOUNT_DISCARD_READ_DATA   1

/**
  * @brief  �˻��Ĺ��캯��
  * @param  id:       Ψһ������
  * @param  center:   ָ���������ĵ�ָ��
  * @param  bufSize:  Ҫ��������ݵĳ���
  * @param  userData: ָ���û��������ݵĵ�ַ
  * @retval None
  */
Account::Account(
    const char* id,
    DataCenter* center,
    uint32_t bufSize,
    void* userData
)
{
    //��ʼ��priv
    memset(&priv, 0, sizeof(priv));

    ID = id;
    Center = center;
    UserData = userData;

    if (bufSize != 0)
    {
        //����buffer ��СΪ2��bufSize
        uint8_t* buffer = (uint8_t*)lv_mem_alloc(bufSize * sizeof(uint8_t) * 2);
        if (buffer == nullptr)
        {
            DC_LOG_ERROR("Account[%s] buffer malloc failed!", ID);
            return;
        }
        //��ʼ��Ϊ0
        memset(buffer, 0, bufSize * sizeof(uint8_t) * 2);
        //����buffer
        uint8_t* buf0 = buffer;
        uint8_t* buf1 = buffer + bufSize;
        //��ʼ��
        PingPongBuffer_Init(&priv.BufferManager, buf0, buf1);
        DC_LOG_INFO("Account[%s] cached %d x2 bytes", ID, bufSize);
        priv.BufferSize = bufSize;
    }
    //�������������˻�
    Center->AddAccount(this);
    DC_LOG_INFO("Account[%s] created", ID);
}

/**
  * @brief  �˻���������
  * @param  None
  * @retval None
  */
Account::~Account()
{
    DC_LOG_INFO("Account[%s] deleting...", ID);

    //�ͷ�cache
    if(priv.BufferSize != 0)
    {
        lv_mem_free(priv.BufferManager.buffer);
    }

    //ɾ��timer
    if (priv.timer != nullptr)
    {
        lv_timer_del(priv.timer);
        DC_LOG_INFO("Account[%s] task deleted", ID);
    }

    //�ö�����ȡ����ע
    for(auto iter : subscribers)
    {
        iter->Unsubscribe(ID);
        DC_LOG_INFO("sub[%s] unsubscribed pub[%s]", iter->ID, ID);
    }

    //Ҫ�󷢲���ɾ���˶�����
    for (auto iter : publishers)
    {
        Center->Remove(&iter->subscribers, this);
        DC_LOG_INFO("pub[%s] removed sub[%s]", iter->ID, ID);
    }

    //����������ɾ�����ʻ�
    Center->RemoveAccount(this);

    DC_LOG_INFO("Account[%s] deleted", ID);
}

/**
  * @brief  ���ķ�����
  * @param  pubID: ������ID
  * @retval ָ�򷢲��ߵ�ָ��
  */
Account* Account::Subscribe(const char* pubID)
{
    //�����������Լ�
    if (strcmp(pubID, ID) == 0)
    {
        DC_LOG_ERROR("Account[%s] try to subscribe to it itself", ID);
        return nullptr;
    }

    //�Ƿ��ظ�����
    Account* pub = Center->Find(&publishers, pubID);
    if(pub != nullptr)
    {
        DC_LOG_ERROR("Multi subscribe pub[%s]", pubID);
        return nullptr;
    }

    //�Ƿ������������
    pub = Center->SearchAccount(pubID);
    if (pub == nullptr)
    {
        DC_LOG_ERROR("pub[%s] was not found", pubID);
        return nullptr;
    }

    //����������ӵ������б���
    publishers.push_back(pub);

    //�÷�������Ӵ˶�����
    pub->subscribers.push_back(this);

    DC_LOG_INFO("sub[%s] subscribed pub[%s]", ID, pubID);

    return pub;
}

/**
  * @brief  ȡ������
  * @param  pubID: ������ID
  * @retval ����˶��ɹ�����true
  */
bool Account::Unsubscribe(const char* pubID)
{
    //�Ƿ��ķ�����
    Account* pub = Center->Find(&publishers, pubID);
    if (pub == nullptr)
    {
        DC_LOG_WARN("sub[%s] was not subscribe pub[%s]", ID, pubID);
        return false;
    }

    //�Ӷ����б���ɾ��������
    Center->Remove(&publishers, pub);

    //������ɾ���˶�����
    Center->Remove(&pub->subscribers, this);

    return true;
}

/**
  * @brief  �򻺴��ύ����
  * @param  data_p: Pointer to data
  * @param  size:   The size of the data
  * @retval Return true if the submission is successful
  */
bool Account::Commit(const void* data_p, uint32_t size)
{
    if (!size || size != priv.BufferSize)
    {
        DC_LOG_ERROR("pub[%s] has no cache", ID);
        return false;
    }

    void* wBuf;
    PingPongBuffer_GetWriteBuf(&priv.BufferManager, &wBuf);

    memcpy(wBuf, data_p, size);

    PingPongBuffer_SetWriteDone(&priv.BufferManager);

    DC_LOG_INFO("pub[%s] commit data(0x%p)[%d] >> data(0x%p)[%d] done",
                ID, data_p, size, wBuf, size);

    return true;
}

/**
  * @brief  �����߷�������
  * @param  None
  * @retval error code
  */
int Account::Publish()
{
    int retval = RES_UNKNOW;

    //���ݻ����СΪ0
    if (priv.BufferSize == 0)
    {
        DC_LOG_ERROR("pub[%s] has not cache", ID);
        return RES_NO_CACHE;
    }

    void* rBuf;
    //���ͷ�û���ύ����
    if (!PingPongBuffer_GetReadBuf(&priv.BufferManager, &rBuf))
    {
        DC_LOG_WARN("pub[%s] data was not commit", ID);
        return RES_NO_COMMITED;
    }

    EventParam_t param;
    param.event = EVENT_PUB_PUBLISH;
    param.tran = this;
    param.recv = nullptr;
    param.data_p = rBuf;
    param.size = priv.BufferSize;

    //����ÿһ��������
    for(auto iter : subscribers)
    {
        Account* sub = iter;
        EventCallback_t callback = sub->priv.eventCallback;

        DC_LOG_INFO("pub[%s] publish >> data(0x%p)[%d] >> sub[%s]...",
                    ID, param.data_p, param.size, sub->ID);

        //�����˻ص�����
        if (callback != nullptr)
        {
            param.recv = sub;
            int ret = callback(sub, &param);
            DC_LOG_INFO("publish done: %d", ret);
            retval = ret;
        }
        else
        {
            DC_LOG_INFO("sub[%s] not register callback", sub->ID);
        }
    }
#if ACCOUNT_DISCARD_READ_DATA
    PingPongBuffer_SetReadDone(&priv.BufferManager);
#endif
    return retval;
}

/**
  * @brief  �ӷ����ߴ���ȡ����
  * @param  pubID:  Publisher ID
  * @param  data_p: Pointer to data
  * @param  size:   The size of the data
  * @retval error code
  */
int Account::Pull(const char* pubID, void* data_p, uint32_t size)
{
    Account* pub = Center->Find(&publishers, pubID);
    //û�ж������������
    if (pub == nullptr)
    {
        DC_LOG_ERROR("sub[%s] was not subscribe pub[%s]", ID, pubID);
        return RES_NOT_FOUND;
    }
    return Pull(pub, data_p, size);
}

int Account::Pull(Account* pub, void* data_p, uint32_t size)
{
    int retval = RES_UNKNOW;

    if (pub == nullptr)
    {
        return RES_NOT_FOUND;
    }

    DC_LOG_INFO("sub[%s] pull << data(0x%p)[%d] << pub[%s] ...",
                ID, data_p, size, pub->ID);

    EventCallback_t callback = pub->priv.eventCallback;
    if (callback != nullptr)
    {
        EventParam_t param;
        param.event = EVENT_SUB_PULL;
        param.tran = this;
        param.recv = pub;
        param.data_p = data_p;
        param.size = size;

        int ret = callback(pub, &param);

        DC_LOG_INFO("pull done: %d", ret);
        retval = ret;
    }
    else
    {
        DC_LOG_INFO("pub[%s] not registed pull callback, read commit cache...", pub->ID);

        if (pub->priv.BufferSize == size)
        {
            void* rBuf;
            if (PingPongBuffer_GetReadBuf(&pub->priv.BufferManager, &rBuf))
            {
                memcpy(data_p, rBuf, size);
#if ACCOUNT_DISCARD_READ_DATA
                PingPongBuffer_SetReadDone(&pub->priv.BufferManager);
#endif
                DC_LOG_INFO("read done");
                retval = 0;
            }
            else
            {
                DC_LOG_WARN("pub[%s] data was not commit!", pub->ID);
            }
        }
        else
        {
            DC_LOG_ERROR(
                "Data size pub[%s]:%d != sub[%s]:%d",
                pub->ID,
                pub->priv.BufferSize,
                this->ID,
                size
            );
        }
    }

    return retval;
}

/**
  * @brief  Send a notification to the publisher
  * @param  pubID: Publisher ID
  * @param  data_p: Pointer to data
  * @param  size:   The size of the data
  * @retval error code
  */
int Account::Notify(const char* pubID, const void* data_p, uint32_t size)
{
    Account* pub = Center->Find(&publishers, pubID);
    if (pub == nullptr)
    {
        DC_LOG_ERROR("sub[%s] was not subscribe pub[%s]", ID, pubID);
        return RES_NOT_FOUND;
    }
    return Notify(pub, data_p, size);
}

/**
  * @brief  Send a notification to the publisher
  * @param  pub:    Pointer to publisher
  * @param  data_p: Pointer to data
  * @param  size:   The size of the data
  * @retval error code
  */
int Account::Notify(Account* pub, const void* data_p, uint32_t size)
{
    int retval = RES_UNKNOW;

    if (pub == nullptr)
    {
        return RES_NOT_FOUND;
    }

    DC_LOG_INFO("sub[%s] notify >> data(0x%p)[%d] >> pub[%s] ...",
                ID, data_p, size, pub->ID);

    EventCallback_t callback = pub->priv.eventCallback;
    if (callback != nullptr)
    {
        EventParam_t param;
        param.event = EVENT_NOTIFY;
        param.tran = this;
        param.recv = pub;
        param.data_p = (void*)data_p;
        param.size = size;

        int ret = callback(pub, &param);

        DC_LOG_INFO("send done: %d", ret);
        retval = ret;
    }
    else
    {
        DC_LOG_WARN("pub[%s] not register callback", pub->ID);
        retval = RES_NO_CALLBACK;
    }

    return retval;
}

/**
  * @brief  Set event callback
  * @param  callback: Callback function pointer
  * @retval None
  */
void Account::SetEventCallback(EventCallback_t callback)
{
    priv.eventCallback = callback;
}

/**
  * @brief  Timer callback entry function
  * @param  timer: Pointer to timer
  * @retval None
  */
void Account::TimerCallbackHandler(lv_timer_t* timer)
{
    Account* instance = (Account*)(timer->user_data);
    EventCallback_t callback = instance->priv.eventCallback;
    if(callback)
    {
        EventParam_t param;
        param.event = EVENT_TIMER;
        param.tran = instance;
        param.recv = instance;
        param.data_p = nullptr;
        param.size = 0;

        callback(instance, &param);
    }
}

/**
  * @brief  Set timing period
  * @param  period: Timing period
  * @retval None
  */
void Account::SetTimerPeriod(uint32_t period)
{
    if(priv.timer)
    {
        lv_timer_del(priv.timer);
        priv.timer = nullptr;
    }

    if(period == 0)
    {
        return;
    }

    priv.timer = lv_timer_create(
                     TimerCallbackHandler,
                     period,
                     this
                 );
}

/**
  * @brief  Set timer enable
  * @param  en: Whether to enable
  * @retval None
  */
void Account::SetTimerEnable(bool en)
{
    lv_timer_t* timer = priv.timer;

    if (timer == nullptr)
    {
        return;
    }

    en ? lv_timer_resume(timer) : lv_timer_pause(timer);
}

/**
  * @brief  Get the number of publishers
  * @retval number of publishers
  */
size_t Account::GetPublishersSize()
{
    return publishers.size();
}

/**
  * @brief  Get the number of subscribes
  * @retval number of subscribes
  */
size_t Account::GetSubscribersSize()
{
    return subscribers.size();
}
