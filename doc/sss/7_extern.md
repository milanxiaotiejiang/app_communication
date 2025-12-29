`extern "C"` 用于**关闭 C++ 的名称修饰（name mangling）**，使函数或变量按 **C 语言的链接规则** 暴露，核心目的只有一个：**实现 C 与 C++ 的二进制级兼容**。

---

### 1. 为什么需要 `extern "C"`

C++ 支持函数重载，编译器会对符号名进行修饰；
C 不支持重载，符号名保持原样。

```cpp
// C++
int add(int, int);
```

编译后符号名 ≠ `add`

```c
// C
int add(int, int);
```

编译后符号名 = `add`

不统一就会 **链接失败**。

---

### 2. 基本用法

#### 2.1 修饰函数声明

```cpp
extern "C" int add(int a, int b);
```

---

#### 2.2 修饰一组声明（常见写法）

```cpp
extern "C" {
    void foo();
    int bar(int);
}
```

---

### 3. 用于头文件（标准写法）

```cpp
#ifdef __cplusplus
extern "C" {
#endif

void c_api_init(void);
int  c_api_run(int);

#ifdef __cplusplus
}
#endif
```

效果：

* C 编译器正常编译
* C++ 编译器关闭名称修饰

这是 **跨语言库头文件的标准模板**。

---

### 4. 可以修饰什么

✔ 函数
✔ 全局变量

不能用于：
✘ 类
✘ 成员函数（非静态）
✘ 函数重载

```cpp
extern "C" void f(int);
extern "C" void f(double); // 非法
```

---

### 5. 与 `static`、`inline` 的关系

* `extern "C"` 控制 **链接约定**
* `static` 控制 **可见性**
* `inline` 控制 **ODR / 多重定义**

```cpp
extern "C" {
    inline int add(int a, int b) { return a + b; }
}
```

合法，但需谨慎用于对外 ABI。

---

### 6. 常见应用场景

* C 库被 C++ 调用
* C++ 库向 C 提供 API
* 插件系统 / 动态库接口
* 操作系统、驱动、嵌入式 SDK

---

### 一句话总结

> `extern "C"` 不是让 C++ 代码“变成 C”，
> 而是**让链接器把它当作 C 来看**。
