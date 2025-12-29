在 C++ 中，`auto` 是 **类型推导关键字**，用于**让编译器根据初始化表达式自动推导变量类型**，简化代码书写，提高可读性。

---

### 1. 基本用法

```cpp
auto x = 10;       // int
auto y = 3.14;     // double
auto z = "hello";  // const char*
```

* 编译器在编译期确定类型
* 必须在声明时初始化

---

### 2. 与引用和 const 结合

```cpp
int a = 10;
const int& b = a;

auto c = b;      // c 是 int（去掉 const &）
auto& d = b;     // d 是 const int&
const auto& e = b; // e 是 const int&
```

* `auto` 默认去掉引用和 const 修饰
* 可以通过 `auto&` / `const auto&` 保留修饰

---

### 3. 与函数返回类型

```cpp
auto add(int x, int y) -> int {
    return x + y;
}

// C++14 后可直接写
auto add2(int x, int y) {
    return x + y; // 返回类型由表达式推导
}
```

---

### 4. 与模板、泛型编程

```cpp
template<typename T1, typename T2>
auto multiply(T1 a, T2 b) -> decltype(a*b) {
    return a * b;
}
```

* `auto` 可与 `decltype` 配合，推导复杂表达式类型

---

### 5. 与循环结合（C++11 起）

```cpp
std::vector<int> v = {1,2,3};
for (auto it = v.begin(); it != v.end(); ++it) { ... }

// C++11 范围基循环
for (auto& val : v) { ... }
```

* 提高可读性
* 自动处理迭代器类型

---

### 6. 注意事项

* 必须有初始化值
* 自动类型推导可能隐藏类型差异（注意引用、const、浮点精度）
* 不等同于动态类型（静态类型推导，编译期确定）

---

### 一句话总结

> `auto` 是**让编译器推导变量类型的工具**，
> 简化代码书写，同时保持类型安全与编译期确定性。
