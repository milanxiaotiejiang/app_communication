在 C++ 中，`noexcept` 是用于函数声明的 **异常规范关键字**，用于**告知编译器该函数不会抛出异常**，从而可以优化性能并辅助异常安全设计。

---

### 1. 基本语法

```cpp
void foo() noexcept {
    // 不会抛出异常
}
```

* 如果函数在执行过程中抛出了异常，程序会调用 `std::terminate()` 终止运行
* 可以作为编译优化依据（比如移动操作、容器重排）

---

### 2. 条件 `noexcept`

```cpp
template<typename T>
void bar(T t) noexcept(noexcept(T())) {
    // 如果 T() 构造函数不抛异常，则 bar 也是 noexcept
}
```

* 条件表达式返回 `true` → 函数 noexcept
* 条件表达式返回 `false` → 函数可能抛异常

---

### 3. 与移动构造/移动赋值结合

STL 容器优化：

```cpp
struct Buffer {
    Buffer(Buffer&&) noexcept; // 移动时不抛异常，容器可直接移动
};
```

* 如果移动构造函数标记 `noexcept`，容器在扩容时可直接移动元素而非拷贝
* 提升性能，保证异常安全

---

### 4. 与 try-catch 的关系

```cpp
void foo() noexcept {
    throw std::runtime_error("error"); // 调用 std::terminate
}
```

* `noexcept` 函数内部抛异常 → 程序立即终止
* 所以只能在**保证不会抛异常的函数**中使用

---

### 5. 与 noexcept 运算符

```cpp
noexcept(expr) // 编译期常量
```

* 用于检查表达式是否可能抛出异常
* 常用于模板元编程或条件 noexcept

```cpp
static_assert(noexcept(std::vector<int>().push_back(1)), "may throw");
```

---

### 6. 常见用途

* 移动构造和移动赋值
* STL 容器优化（vector、string 扩容）
* 函数模板条件 noexcept
* 异常安全接口设计

---

### 一句话总结

> `noexcept` 告诉编译器**函数不会抛异常**，
> 提供性能优化机会，并确保异常安全策略可行。

