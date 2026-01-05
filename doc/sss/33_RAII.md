**RAII（Resource Acquisition Is Initialization）** 是 C++ 的核心资源管理理念，意指**资源的获取与对象的生命周期绑定**。通过对象构造获取资源、析构释放资源，实现自动管理，避免泄漏和悬空。

---

### 1. 核心思想

* **构造函数获取资源**：内存、文件、锁、网络连接等
* **析构函数释放资源**：保证离开作用域时资源被正确释放
* 自动管理生命周期，无需手动释放

```cpp
#include <fstream>

void example() {
    std::ofstream file("test.txt"); // 构造时打开文件
    file << "Hello RAII";           // 使用文件
} // 离开作用域自动调用析构函数，文件关闭
```

---

### 2. RAII 与智能指针

智能指针是 RAII 的典型应用：

```cpp
#include <memory>

void foo() {
    std::unique_ptr<int> uptr = std::make_unique<int>(10);
} // uptr 析构，自动释放内存
```

---

### 3. RAII 的优势

1. **自动资源管理**：避免内存泄漏和资源泄露
2. **异常安全**：即使函数抛异常，析构函数仍会释放资源
3. **简化代码**：无需手动调用 `delete` 或 `close`
4. **可扩展**：适用于锁、文件、网络连接等各种资源

---

### 4. 典型应用场景

* 内存管理：`unique_ptr`、`shared_ptr`
* 文件操作：`std::ifstream`、`std::ofstream`
* 线程锁管理：`std::lock_guard`、`std::unique_lock`
* 数据库连接、网络套接字等资源管理

---

### 5. 一句话总结

> RAII 是 **将资源生命周期与对象绑定** 的管理策略，
> 利用构造获取资源、析构释放资源，实现自动、异常安全的资源管理。
