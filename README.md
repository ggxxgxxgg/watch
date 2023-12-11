`PageBase`每一个页面都有  
* `State_t`页面状态
* `Stash_t`存储器区数据结构体，每一个页面都有
* `AnimAttr_t`页面切换动画属性，规定页面切换的动画

指针
`_root`是页面的根，以此创建页面
`_Manager`每一个页面都有，可能是为了调用。
`_UserData`