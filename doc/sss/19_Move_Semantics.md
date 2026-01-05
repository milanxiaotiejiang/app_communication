在 C++ 中，**移动语义（Move Semantics）** 是为了提高性能而引入的一种机制，用于**资源的所有权转移**，避免不必要的深拷贝，尤其在大对象和容器中非常重要。C++11 开始正式支持。

---

### 1. 核心思想

* **传统拷贝**：`T a = b;` 会创建一个新对象，复制 b 的内容（深拷贝）
* **移动语义**：`T a = std::move(b);` 会**将 b 的资源直接转移到 a**，避免复制

```cpp
std::string str1 = "Hello, World!";
std::string str2 = std::move(str1); // str1 的内容被转移到 str2
```

* `str1` 变为有效但未定义状态（通常为空）
* 避免了大量内存分配与复制

---

### 2. 右值引用（Rvalue Reference）

```cpp
T&& var = ...;  // 右值引用
```

* 移动语义的基础
* 只能绑定到**右值**（临时对象）
* 区别于普通左值引用 `T&`

---

### 3. 移动构造函数

```cpp
class Buffer {
    char* data;
    size_t size;
public:
    Buffer(size_t s) : size(s), data(new char[s]) {}
    
    // 移动构造函数
    Buffer(Buffer&& other) noexcept
        : data(other.data), size(other.size) {
        other.data = nullptr;
        other.size = 0;
    }

    ~Buffer() { delete[] data; }
};
```

* 接受右值引用
* “窃取”资源指针，避免内存分配和拷贝

---

### 4. 移动赋值运算符

```cpp
Buffer& operator=(Buffer&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
    }
    return *this;
}
```

* 类似移动构造函数
* 用于已有对象的右值赋值

---

### 5. 与 `std::move` 的关系

* `std::move` **不移动**，只是将左值转换为右值引用
* 真正的移动操作由移动构造函数或移动赋值运算符完成

```cpp
std::string a = "abc";
std::string b = std::move(a); // 调用 std::string 的移动构造函数
```

---

### 6. 优势

* 避免不必要的深拷贝
* 提高大对象和 STL 容器操作效率
* 支持资源管理类（内存、文件、套接字等）

---

### 7. 注意事项

* 移动后的对象应仍保持可析构、可赋值的有效状态
* 不要使用已被移动的对象的值（可赋新值或销毁）
* 移动语义只适用于支持移动构造或移动赋值的类

---

### 一句话总结

> 移动语义是 **资源所有权的转移机制**，
> 通过右值引用和移动构造/赋值，避免深拷贝，提高性能。
