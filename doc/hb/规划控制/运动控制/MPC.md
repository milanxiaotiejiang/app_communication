# 总体架构

清洁机器人路径控制目标往往是：

* **路径平滑**（大回转、弧线、贴边）
* **速度柔和**
* **抗抖动**
* **可用在曲线细长路径**
* **能处理地面摩擦变化**

因此，架构可定为：

```
全局规划（NavFn/A* 或 SLAM 的 coverage path）  
          ↓  
轨迹解算器（Bezier / Cubic Spline / clothoid 曲线）  
          ↓  
MPC 控制器（局部执行器）  
          ↓  
底盘速度接口（差速/全向）
```

特点：

* 全局规划只生成大致路径（稀疏点）
* 本地先用曲线重建路径并采样
* MPC 以**固定采样步长**（如 0.1s）追踪这条本地曲线

---

## **方案 A：使用 acados（强烈推荐）**

acados 是现代 C/优化库，能生成高效 C 代码：

* 支持 NMPC
* 支持硬件实时控制（嵌入式可用）
* 求解速度快（每步 <1ms）
* C 接口灵活

官网: [https://github.com/acados/acados](https://github.com/acados/acados)

👉 必须自己写的部分：

* 机器人运动学模型（差速）
* cost 设计（tracking error & smooth）
* 输入约束（v/w）

---

## **方案 B：使用 MPC 控制库 do-mpc（Python 原型）**

如果你先做 demo 快速验证：

* 简单
* 类似 Matlab MPC Toolbox
* 不能直接用于实时控制，但能帮你确定参数

---

## **方案 C：使用 Eigen + qpOASES（轻量级）**

如果你不想依赖 acados，可以：

* 使用 Eigen 维护状态矩阵
* 使用 qpOASES 作为 QP 解算器

适合需求不高或芯片较弱的机器人。

---

# MPC 模块结构

建议把 MPC 控制器拆成 4 层：

---

### **1）轨迹预处理（Path Smoothing）**

输入：稀疏全局路径点

输出：平滑曲线 + 时间参数化（v, w profile）

你可以直接用：

* **Spline 曲线：Cubic B-Spline**
* **Bezier 曲线**
* **clothoid 曲线（最常用于清洁机器人）**

曲线库推荐：

* `tinyspline` (C)
* `b-spline` (C++)
* or use `Eigen` 自己构 Cubic Spline

---

### **2）状态预测模型（Kinematic Model）**

清洁机器人一般是差速模型：

```
x' = v * cos(θ)
y' = v * sin(θ)
θ' = ω
```

离散化后用于 MPC。

---

### **3）误差计算（Tracking Error）**

MPC 控制的是误差，不是绝对坐标：

```
cte     = y_target - y_robot
etheta  = θ_target - θ_robot
```

误差坐标系要转换到**车体坐标系**，实现更稳定。

---

### **4）求解器调用（QP 求解）**

最终得到：

```
输出：
v（线速度）
w（角速度）
```

---

# 与 Nav2／move_base 的接入方式

你不需要改 Nav2，只需写一个新 **Controller plugin**。

目录结构：

```
nav2_mpc_controller/
  ├── include/nav2_mpc_controller/mpc_controller.hpp
  ├── src/mpc_controller.cpp
  ├── plugin.xml
  ├── CMakeLists.txt
  ├── package.xml
```

在 **plugin.xml** 中注册：

```xml
<class_libraries>
  <library path="libmpc_controller">
    <class type="nav2_mpc_controller::MPCController" 
           base_class_type="nav2_core::Controller">
    </class>
  </library>
</class_libraries>
```

然后在 `mpc_controller.cpp` 的 computeVelocityCommands 中：

* 获取当前 odom
* 从 Nav2 接收局部轨迹
* 转换到车体坐标系
* 入 MPC 求解器
* 返回 geometry_msgs/Twist

---

# 调参指南

MPC 关键参数只有 6 类：

---

### **1）预测步长 N**

清洁机器人速度慢，用：

```
N = 10~15
dt = 0.1
```

---

### **2）权重系数 （Cost Function）**

```
w_cte:      500 ~ 2000     # 横向误差
w_etheta:   100 ~ 500       # 方向误差
w_v:        1 ~ 50          # 速度误差
w_w:        10 ~ 100        # 角速度开销
w_a:        1 ~ 10          # 加速度开销
```

---

### **3）输入限幅**

清洁机器人差速速度较低：

```
v_max = 0.7 m/s
w_max = 1.5 rad/s
```

---

### **4）连续性约束（平滑性）**

```
|Δv/dt| < 限制
|Δw/dt| < 限制
```

让机器人不抖动、不突然急加速。

---

### **5）跟踪的参考速度**

正常走廊清洁：

```
0.4~0.6 m/s
```

贴边模式：

```
0.2~0.3 m/s
```

---

### **6）障碍物处理（optional）**

可以不让 MPC 看障碍物，依旧使用 costmap：

* local costmap handle obstacle inflation
* MPC 只负责 tracking

---

# ✅ 六、后续扩展研发路线

| 阶段  | 目标     | 内容                  |
| --- | ------ | ------------------- |
| MVP | 快速跑起来  | 曲线平滑 + MPC tracking |
| V1  | 商用稳定性  | 增加所有速度限制、平滑约束       |
| V2  | 高性能工业化 | 用 acados 自生成 C 控制器  |
| V3  | 智能化    | 添加动态避障约束（障碍约束进 MPC） |
| V4  | 高端产品   | 采用增强型 NMPC + 滑模观测器  |
