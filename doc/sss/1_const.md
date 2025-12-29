在 C++ 中，`const` 用于**限定对象不可被修改**，是类型系统的重要组成部分。合理使用 `const` 能提升代码的安全性、可读性与可维护性。

---

### 1. 修饰变量

```cpp
const int a = 10;
```

`a` 为只读变量，初始化后不可再赋值。

---

### 2. 修饰指针

核心在于区分：**指针本身是否可变**，以及**指向的内容是否可变**。

```cpp
const int* p1;      // 指向 const int，*p1 不可改，p1 可改
int const* p2;      // 同上
int* const p3 = &a; // 指针常量，p3 不可改，*p3 可改
const int* const p4 = &a; // 指针和内容都不可改
```

记忆方式：`const` 修饰**左边最近的类型**。

---

### 3. 修饰引用

```cpp
int a = 10;
const int& r = a;
```

通过 `r` 不能修改 `a`，常用于函数参数，避免拷贝且保证只读。

---

### 4. 修饰函数参数

```cpp
void foo(const std::string& s);
```

表示函数内部不会修改 `s`，同时避免对象拷贝，属于推荐写法。

---

### 5. 修饰返回值

```cpp
const int getValue();
```

对内置类型意义不大；对返回引用或指针时，可防止调用方修改返回对象。

---

### 6. 修饰成员函数

```cpp
class A {
public:
    int get() const;
};
```

`const` 成员函数承诺：**不修改对象的成员变量**（除 `mutable` 外）。
该函数只能访问其他 `const` 成员函数。

---

### 7. 与 `mutable` 配合

```cpp
class A {
    mutable int cnt;
public:
    void visit() const { cnt++; }
};
```

`mutable` 允许在 `const` 对象或 `const` 成员函数中被修改，常用于缓存、计数。

---

### 8. `const` 的设计意义

* 编译期约束，减少逻辑错误
* 明确接口语义（只读 / 可修改）
* 支持重载区分（`const` / 非 `const` 成员函数）
* 提升代码可维护性

---

### 常见建议

* 能加 `const` 就加
* 函数参数优先使用 `const T&`
* 成员函数不修改状态时加 `const`
* 区分“逻辑常量性”与“物理常量性”

