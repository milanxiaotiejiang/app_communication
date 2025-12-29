在 C++ 中，`static` 用于**控制作用域与生命周期**，其含义随使用位置不同而变化，但核心始终是：**与对象实例解绑、在程序整个运行期内只存在一份**。

---

### 1. 修饰局部变量

```cpp
void foo() {
    static int cnt = 0;
    cnt++;
}
```

* 只初始化一次
* 生命周期贯穿整个程序
* 作用域仍限于函数内部

常用于**状态记忆、计数、缓存**。

---

### 2. 修饰全局变量 / 函数（文件作用域）

```cpp
static int g_val = 10;

static void helper() {}
```

* 仅在当前源文件可见
* 避免符号外泄与命名冲突

现代 C++ 中更推荐：

```cpp
namespace {
    int g_val = 10;
}
```

---

### 3. 静态成员变量

```cpp
class A {
public:
    static int count;
};
```

* 属于类，而非对象
* 所有实例共享
* 需在类外定义（C++17 前）：

```cpp
int A::count = 0;
```

C++17 起可用：

```cpp
class A {
public:
    inline static int count = 0;
};
```

---

### 4. 静态成员函数

```cpp
class A {
public:
    static void func();
};
```

* 不依赖对象实例
* 不能访问非静态成员
* 常作为**工具函数 / 工厂方法**

---

### 5. `static` 与构造 / 初始化顺序

* 函数内 `static`：首次执行到该语句时初始化
* 全局 / 命名空间 `static`：程序启动阶段初始化
* 不同源文件间存在**静态初始化顺序问题**

推荐方案：**函数内 static（Meyers Singleton）**

---

### 6. 与 `const` 的关系

```cpp
class A {
public:
    static const int N = 10;   // 可作为编译期常量
};
```

C++17 后推荐：

```cpp
inline static constexpr int N = 10;
```

---

### 7. 典型使用场景

* 共享计数器 / 全局状态
* 单例模式
* 类级别配置参数
* 文件内部工具函数
* 性能敏感场景下的缓存

---

### 一句话总结

> `static` 决定**“只有一份”**，
> **作用域看位置，生命周期看程序。**

