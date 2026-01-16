## 一、DDS 是什么

**DDS 是一种去中心化、以“数据”为核心的发布-订阅通信标准**，由 OMG 制定，强调实时性、可靠性与分布式系统稳定性。

> 不关心“谁给谁发消息”，只关心“某类数据在系统中是否存在”。

---

## 二、DDS 的核心思想（与 ROS1 的根本区别）

### ROS1（节点中心）

```
Node A → Topic → Node B
        ↑
     Master
```

* 依赖中心节点
* 面向“连接关系”

---

### DDS（数据中心）

```
Data(类型 + QoS)
   ↑          ↑
Publisher   Subscriber
```

* 无 Master
* 面向“数据对象”
* 节点是否存在并不重要

---

## 三、DDS 的基本实体（必须掌握）

### 1️⃣ Domain（域）

* 一个 **逻辑通信空间**
* 同一 Domain 内才能互相发现
* ROS2 中由 `ROS_DOMAIN_ID` 控制

> 等价于：一张“广播网”

---

### 2️⃣ Participant（参与者）

* 一个进程在 DDS 中的身份
* 一个 ROS2 节点 ≈ 一个或多个 Participant

---

### 3️⃣ Topic（主题）

**= 数据类型 + 名称**

* 类型固定
* 发布者和订阅者必须类型一致

---

### 4️⃣ Publisher / Subscriber

* Publisher：数据生产者
* Subscriber：数据消费者
* 不直接互相连接

---

### 5️⃣ DataWriter / DataReader（底层）

* 真正负责：

    * 序列化
    * 发送
    * 接收
    * 重传

ROS2 层基本不直接接触。

---

## 四、DDS 的自动发现机制（关键特性）

### 工作流程

1. 节点启动
2. 向 Domain 广播自身信息
3. 发现匹配的 Topic + QoS
4. 自动建立通信

➡️ **无需注册、无需 Master、无需配置 IP**

---

### 发现失败的常见原因

* Domain ID 不一致
* QoS 不兼容
* 网络被隔离（子网 / VLAN）

---

## 五、QoS（DDS 的灵魂）

### 为什么 DDS 一定要 QoS

因为它用于：

* 控制系统
* 机器人
* 工业网络

而不是聊天消息。

---

### 核心 QoS 策略（ROS2 常用）

| QoS         | 含义      | 典型用途    |
| ----------- | ------- | ------- |
| Reliability | 可靠 / 尽力 | 控制 / 激光 |
| History     | 缓存策略    | 传感器     |
| Depth       | 队列深度    | 高频数据    |
| Durability  | 是否保留    | 参数 / 地图 |
| Deadline    | 频率约束    | 控制链路    |

---

### QoS 不匹配会发生什么

> **双方都在，Topic 却“完全没数据”**

这是 DDS 新手最常见问题。

---

## 六、DDS 的通信模式

### 1️⃣ Publish-Subscribe（最常用）

* Topic
* 异步
* 多对多

ROS2 的 Topic 即 DDS Pub-Sub。

---

### 2️⃣ Request-Reply（Service）

* 基于 DDS 实现
* 无中心服务器
* 超时与可靠性更可控

---

### 3️⃣ Event / State（Action 的底层思想）

* 状态更新
* 事件通知
* 进度反馈

---

## 七、DDS 如何满足实时性

### 设计层面

* 无 Master
* 本地发现
* UDP 为主
* 可零拷贝（Loaned Message）

---

### 系统层面

* 可绑定 CPU
* 可配置线程优先级
* 支持 RTOS

➡️ **这是 ROS1 无法做到的**

---

## 八、DDS 在 ROS2 中的真实角色

### ROS2 架构位置

```
ROS2 API
   ↓
rcl / rclcpp
   ↓
RMW（中间层）
   ↓
DDS（FastDDS / CycloneDDS）
```

* ROS2 **不是 DDS**
* ROS2 **跑在 DDS 之上**

---

### 为什么 ROS2 选 DDS

* 工业标准
* 多厂商实现
* 跨语言 / 跨平台
* 实时性强

---

## 九、对清洁机器人的现实意义

### DDS 带来的直接好处

* 多进程稳定通信
* 激光、IMU、控制链路可差异化 QoS
* 节点异常不拖垮系统
* 适合长期运行设备

---

### 需要警惕的问题

* 网络环境复杂时发现失败
* QoS 配置不当导致“假死”
* 初期调试成本高于 ROS1

---

## 十、一句话工程总结

> **DDS 不是更复杂的 ROS，而是“把通信当成工程问题来解决”的体系。**

在清洁机器人中：

* 感知 → DDS
* 控制 → DDS + QoS
* 可靠性 → DDS 的强项
