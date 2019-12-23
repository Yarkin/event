# **事件驱动框架**

## **概述**

> 该模块主要为了简化基于事件驱动的开发而设计, 整体参考业界消息中间件设计实现, 实现为订阅/发布模式, 主要支持如下三个类:

1. subscriber: 订阅者, 对关心业务事件进行订阅, 如此相应的订阅者只要关注自身相关业务实现即可
2. publisher: 发布者, 将事件发送到broker, 再由broker将消息派发给对应的订阅者
3. broker: 消息分发中心, 完成事件分发功能, 只需要订阅者订阅即可由该类实现相应事件分发

> 为了方便开发者使用, 提供部分辅助类和宏来实现各个发布订阅之间的粘合, 也让开发者能够更加关注自身业务

1. TO_EV_TYPE: 将主事件类型和次事件类型构建为具体事件, 主要在用户构建事件时使用
2. PARSE_MAJOR: 解析事件主事件类型
3. PARSE_MINOR: 解析事件次事件类型
4. DECL_EVENT_BEGIN: 声明具体事件类型, 用户粘合用户自定义数据与具体实现, 自动根据用户数据构建出相应事件类
5. SUBSCRIBE_BEGIN: 订阅声明
6. SUBSCRIBE: 事件订阅实现

## **使用**

1. 定义用户数据
```c++
// 定义具体数据类
class xx1_data {
 public:
    int val; //具体数据
};
```

2. 声明事件

```c++
// 声明事件, 自动生成xx1_data_event_t, 对应事件为TO_EV_TYPE(e_xx, e_xx1)
DECL_EVENT_BEGIN(xx1_data, e_xx, e_xx1)
DECL_EVENT_END()
```

3. 创建订阅者

```c++
// 构建事件处理类
class xmock_xx_subscriber : public xsubscriber {
 public:
    virtual int32_t on_xx1_event(xevent_t * ev); //exx-e_xx1事件响应函数
    virtual int32_t on_xx2_event(xevent_t * ev); //exx-e_xx2事件响应函数

    SUBSCRIBE_DECL() // 声明订阅函数
};


int32_t xmock_xx_subscriber::on_xx1_event(xevent_t * ev) {
    EXPECT_NE(ev->type(), TO_EV_TYPE(e_xx, e_xx2));
    EXPECT_EQ(ev->type(), TO_EV_TYPE(e_xx, e_xx1));
    return 1;
}

int32_t xmock_xx_subscriber::on_xx2_event(xevent_t * ev) {
    EXPECT_NE(ev->type(), TO_EV_TYPE(e_xx, e_xx1));
    EXPECT_EQ(ev->type(), TO_EV_TYPE(e_xx, e_xx2));
    return 1;
}

// 完成事件订阅
SUBSCRIBE_BEGIN(xmock_xx_subscriber)
    SUBSCRIBE(xmock_xx_subscriber, TO_EV_TYPE(e_xx, e_xx1), on_xx1_event)
    SUBSCRIBE(xmock_xx_subscriber, TO_EV_TYPE(e_xx, e_xx2), on_xx2_event)
SUBSCRIBE_END()
```

4. 订阅

```c++
// 构建订阅者
xmock_xx_subscriber *mock_sub = new xmock_xx_subscriber();
// 初始化
mock_sub->init();

// 构建broker
xbroker broker;
// 订阅e_xx事件
auto ret = broker.subscribe(top::event::e_xx, mock_sub);
```

5. 发布事件

```c++
// 构建发布者
xpublisher publisher;

// 发布事件
auto ret1 = publisher.pub(&broker, new xx1_data_event_t(new xx1_data()));
```

6. 订阅者将自动收到相应事件并调用到对应的响应函数

## **TODO**

>现在支持同步调用, 后续将支持异步调用, 远程调用