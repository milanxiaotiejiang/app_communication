## 一、Kubernetes 基本概念

**Kubernetes 是什么**
用于容器化应用的自动化部署、扩缩容与运维的容器编排系统。

**解决了什么问题**

* 容器调度与资源管理
* 服务发现与负载均衡
* 自动扩缩容
* 故障自愈
* 配置与密钥管理

---

## 二、整体架构

### 1️⃣ Master / Control Plane 组件

* **kube-apiserver**
  集群唯一入口，所有组件通过 API Server 通信

* **etcd**
  分布式 KV 存储，保存集群状态（高可用核心）

* **kube-scheduler**
  Pod 调度，决定 Pod 跑在哪个 Node

* **kube-controller-manager**
  各类控制器（Deployment / Node / ReplicaSet）

---

### 2️⃣ Node 组件

* **kubelet**
  Node 代理，负责 Pod 生命周期管理

* **kube-proxy**
  实现 Service 的网络转发（iptables / ipvs）

* **Container Runtime**
  containerd / CRI-O

---

## 三、核心资源对象（必考）

### Pod

* 最小调度单元
* 一组共享网络与存储的容器
* 生命周期短暂，不直接部署业务

---

### Deployment

* 管理无状态应用
* 支持滚动更新、回滚
* 底层通过 ReplicaSet 控制 Pod 数量

---

### StatefulSet

* 有状态应用
* 稳定 Pod 名称、顺序启动
* 常用于 MySQL、Redis

---

### DaemonSet

* 每个 Node 运行一个 Pod
* 常用于日志、监控、网络插件

---

### Job / CronJob

* Job：一次性任务
* CronJob：定时任务

---

### Service

* 提供稳定访问入口
* 类型：

    * ClusterIP（默认）
    * NodePort
    * LoadBalancer
    * Headless Service

---

### Ingress

* 七层 HTTP / HTTPS 路由
* 依赖 Ingress Controller（Nginx、Traefik）

---

## 四、调度机制

**调度流程**

1. 过滤（Filter）：排除不满足条件的 Node
2. 打分（Score）：对 Node 进行权重排序
3. 绑定（Bind）：Pod 绑定到最优 Node

**常见调度策略**

* 资源请求（CPU / Memory）
* 节点亲和性 / 反亲和性
* Pod 亲和性
* Taint / Toleration

---

## 五、网络模型（高频）

**K8s 网络三大原则**

* Pod 之间可直接通信
* Pod 与 Node 可直接通信
* 无 NAT

**CNI 插件**

* Calico（BGP / NetworkPolicy）
* Flannel（Overlay）
* Cilium（eBPF）

---

## 六、存储体系

* **Volume**：Pod 级别，生命周期随 Pod
* **PersistentVolume（PV）**：集群级资源
* **PersistentVolumeClaim（PVC）**：存储申请
* **StorageClass**：动态存储供应

---

## 七、配置与密钥

* **ConfigMap**
  存放配置文件、环境变量

* **Secret**
  存放敏感信息（Base64 编码）

---

## 八、扩缩容机制

* **HPA**
  基于 CPU / Memory / 自定义指标

* **VPA**
  自动调整资源请求（生产慎用）

* **Cluster Autoscaler**
  Node 级别扩缩容

---

## 九、健康检查（常考）

* **livenessProbe**
  判断是否需要重启容器

* **readinessProbe**
  判断是否接收流量

* **startupProbe**
  解决启动慢应用误杀问题

---

## 十、发布与回滚

* RollingUpdate
* Recreate
* 蓝绿发布（Blue-Green）
* 金丝雀发布（Canary）

---

## 十一、常见面试追问

* Deployment 和 StatefulSet 区别
* Service 为什么需要 kube-proxy
* Pod 重启 vs 容器重启区别
* etcd 为什么要高可用
* Kubernetes 如何实现自愈
* 为什么不直接操作 Pod
* Ingress 和 Service 的区别

---

## 十二、生产实践加分点

* 资源 requests / limits 合理设置
* 使用 Namespace 隔离环境
* NetworkPolicy 做网络隔离
* RBAC 控制权限
* 优先使用 Declarative YAML
* 控制器而非裸 Pod

这是一个**偏架构 / 偏原理**的问题，回答得好，基本可以把 Kubernetes 从“工具”上升到“系统设计能力”。

下面分两层说明：
**① Kubernetes 本质是什么**
**② 如何从 0 到 1 设计一个“小型 Kubernetes”**

---

## 一、Kubernetes 的本质：不是工具，而是「声明式分布式控制系统」

一句话概括：

> **Kubernetes 本质上是一个以 API 为核心、以控制器为驱动的声明式分布式系统。**

它解决的不是“容器怎么跑”，而是：

> **如何让“期望状态”持续逼近“实际状态”**

---

### 1️⃣ 三个核心思想（极其重要）

#### ① 声明式（Declarative）

* 用户只描述「想要什么」
* 系统负责「如何达到」

```yaml
replicas: 3
```

并不是“启动 3 个 Pod”，而是：

> **无论发生什么，最终都要有 3 个 Pod**

---

#### ② 控制循环（Control Loop）

K8s 的所有能力，本质都是：

```
Observe → Diff → Act → Repeat
```

* 观察当前状态
* 对比期望状态
* 执行动作
* 无限循环

👉 **这是 K8s 的灵魂**

---

#### ③ 状态中心（etcd）

* etcd 是**唯一事实源**
* 不是组件之间互相调用
* 而是**围绕状态协作**

---

## 二、Kubernetes 的真实架构模型

可以抽象成三层：

```
┌─────────────────────┐
│  API & State Layer  │  ← 声明式接口
├─────────────────────┤
│  Control Loop Layer │  ← 核心逻辑
├─────────────────────┤
│  Execution Layer    │  ← 真正干活
└─────────────────────┘
```

---

### 1️⃣ API & 状态层

**核心职责**

* 接收用户声明
* 存储集群期望状态
* 提供 Watch 能力

**关键组件**

* API Server
* etcd

👉 **这一层 = 分布式系统的“数据库 + 接口”**

---

### 2️⃣ 控制器层（最容易被低估）

Kubernetes 不是一个调度器，而是**一堆控制器**。

例如 Deployment Controller：

```
期望 replicas = 3
当前 Pod = 1
→ 创建 2 个 Pod
```

每一个资源背后都有一个控制器：

| 资源         | 控制器                   |
| ---------- | --------------------- |
| Deployment | Deployment Controller |
| ReplicaSet | RS Controller         |
| Node       | Node Controller       |
| Job        | Job Controller        |

👉 **控制器 ≠ RPC 服务，是状态驱动程序**

---

### 3️⃣ 执行层（最“傻”的一层）

* kubelet
* container runtime
* CNI / CSI

特点：

* 不做决策
* 只执行指令
* 定期上报状态

---

## 三、如何实现一个“小型 Kubernetes”

下面给一个**最小可运行架构**（面试非常加分）。

---

### Step 1：定义核心资源模型

最小资源集：

```yaml
Pod:
  spec:
    containers
    resources
  status:
    phase

Node:
  status:
    capacity
    conditions

Deployment:
  spec:
    replicas
    template
```

👉 **重点：Spec / Status 分离**

---

### Step 2：实现一个状态存储 + Watch

* 可以用：

    * etcd（真实）
    * 或简单 KV（学习版）

必须支持：

* CRUD
* Watch（事件驱动）

---

### Step 3：实现 API Server（最小版）

职责：

* 校验请求
* 写入期望状态
* 提供 Watch 接口

关键点：

* API Server 不做业务逻辑
* 不调度、不创建容器

👉 **只做“状态写入”**

---

### Step 4：写第一个控制器（核心）

示例：Replica Controller

伪逻辑：

```
loop:
  watch Deployment
  if replicas != current_pods:
    create/delete Pod
```

要点：

* 永远循环
* 可重入
* 幂等

👉 **控制器是系统的“大脑”**

---

### Step 5：调度器（可选但加分）

最小调度器逻辑：

```
watch Pod (未绑定 Node)
select Node (CPU 足够)
bind Pod → Node
```

---

### Step 6：Node Agent（简化 kubelet）

职责：

* Watch 分配到本 Node 的 Pod
* 调用 container runtime 启动容器
* 上报 Pod 状态

---

### Step 7：自愈能力（关键）

* Node 掉线 → 删除 Pod → 重新调度
* Pod 异常 → 控制器重建

👉 **自愈不是监控，是状态纠偏**

---

## 四、为什么说 Kubernetes 是“架构范式”

K8s 成功的真正原因不是容器，而是：

### 1️⃣ 状态驱动架构

* 没有复杂调用链
* 所有组件围绕状态协作

---

### 2️⃣ 解耦到极致

* API Server 不关心控制器
* 控制器不关心执行细节
* 执行层不关心业务意图

---

### 3️⃣ 可扩展性来自「控制器模型」

CRD + Controller = 无限能力扩展

> Operator 本质上就是“领域控制器”

---

## 五、面试中一句话升维回答（建议背）

> Kubernetes 并不是一个单一系统，而是一个以声明式 API 为入口、以控制器模式为核心、以状态驱动为运行机制的分布式系统架构范式。容器只是其中一个执行载体。

---

## 六、如果继续深挖（可继续展开）

可继续拆解的方向：

* Scheduler 打分算法如何设计
* Watch 机制如何避免惊群
* etcd 如何保证一致性
* 控制器如何避免双写
* Operator 与传统微服务的差异
* Kubernetes 为什么天然适合云原生

需要的话，可直接把其中一个点**展开成系统设计题级别的答案**。
