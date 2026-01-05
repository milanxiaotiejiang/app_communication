**完美转发（Perfect Forwarding）** 是 C++11 引入的一种技术，主要用于**将函数参数完整地传递给另一个函数而不改变其值类别（左值/右值）和 cv 修饰符**。它常用于模板函数中，保证参数高效传递，同时支持移动语义。

---

### 1. 基本问题

普通模板转发可能改变参数类别：

```cpp
void foo(int& x) { std::cout << "Lvalue\n"; }
void foo(int&& x) { std::cout << "Rvalue\n"; }

template<typename T>
void wrapper(T t) {
    foo(t); // 总是作为左值传递，右值信息丢失
}

int a = 1;
wrapper(a);   // 调用 Lvalue 版本
wrapper(2);   // 也调用 Lvalue 版本，右值被误当作左值
```

---

### 2. 解决方案：右值引用和 `std::forward`

使用 **万能引用（forwarding reference）** 和 `std::forward` 保留值类别：

```cpp
template<typename T>
void wrapper(T&& t) { // T&& 为万能引用
    foo(std::forward<T>(t)); // 完美转发
}

int a = 1;
wrapper(a);   // 调用 Lvalue 版本
wrapper(2);   // 调用 Rvalue 版本
```

* `T&&` 在模板中可绑定左值或右值
* `std::forward<T>(t)` 根据 T 类型决定转发左值或右值

---

### 3. 与移动语义结合

完美转发可支持高效移动：

```cpp
template<typename T>
void wrapper(T&& t) {
    std::vector<int> v;
    v.push_back(std::forward<T>(t)); // 避免不必要拷贝
}
```

* 右值会调用移动构造函数
* 左值会调用拷贝构造函数

---

### 4. 总结特点

1. **保留值类别**：不改变左值/右值属性
2. **高效传递参数**：支持移动语义，避免额外拷贝
3. **模板编程利器**：常用于泛型工厂函数或包装函数

---

### 5. 典型应用场景

* 泛型工厂函数
* 包装函数（forwarding wrapper）
* 容器或智能指针封装
* 支持完美转发的构造函数模板

---

### 一句话总结

> 完美转发是 **在模板中将参数完整转发给另一个函数的机制**，
> 保留左值/右值特性，实现高效、安全的参数传递。
