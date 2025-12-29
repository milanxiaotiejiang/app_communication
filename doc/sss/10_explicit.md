在 C++ 中，`explicit` 用于**禁止构造函数的隐式类型转换**，核心目的在于：**避免“看似合理、实则危险”的自动构造**。

---

### 1. 问题背景：隐式构造

```cpp
struct A {
    A(int x) {}
};

void foo(A a) {}

foo(10);   // 合法：int → A 的隐式转换
```

这类转换往往降低代码可读性，隐藏逻辑错误。

---

### 2. `explicit` 的基本用法

```cpp
struct A {
    explicit A(int x) {}
};

foo(10);        // 编译错误
foo(A(10));     // 合法
```

只能**显式构造**。

---

### 3. 适用范围

* 构造函数
* 转换运算符（C++11 起）

```cpp
struct B {
    explicit operator bool() const;
};
```

防止在算术或比较中被意外使用。

---

### 4. 多参数构造的特殊情况

```cpp
struct A {
    A(int x, int y) {}
};
```

本身就不会发生隐式转换，`explicit` **没有实际意义**。

---

### 5. C++11 的扩展：条件 `explicit`

```cpp
template <typename T>
struct A {
    explicit(sizeof(T) == 4) A(T v) {}
};
```

根据条件决定是否允许隐式构造。

---

### 6. 常见使用建议

* **单参数构造函数默认加 `explicit`**
* 作为“强类型”包装时必须使用
* 数值单位、句柄、ID 类型应禁止隐式转换

```cpp
struct Meter {
    explicit Meter(double v);
};
```

---

### 7. 常见误区

* `explicit` 不影响显式构造
* 不影响拷贝 / 移动构造
* 与性能无关

---

### 一句话总结

> `explicit` 用来阻止**不经意的类型转换**，
> 是构造函数层面的“安全阀”。
