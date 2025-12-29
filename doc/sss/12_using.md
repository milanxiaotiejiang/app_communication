在 C++ 中，`using` 是一个多用途关键字，用于**类型别名、命名空间引入以及基类成员引入**，功能灵活，目的是**简化代码书写和提高可读性**。

---

### 1. 类型别名

#### 1.1 基本写法（C++11）

```cpp
using IntPtr = int*;
IntPtr p;  // 等价于 int* p;
```

* 功能等同于 `typedef`，语法更直观
* 支持模板别名（C++11 新增）

#### 1.2 模板别名

```cpp
template <typename T>
using Vec = std::vector<T>;

Vec<int> v; // std::vector<int>
```

`typedef` 不支持模板别名。

---

### 2. 命名空间引入

```cpp
using namespace std;
cout << "Hello" << endl;
```

* 将整个命名空间内的名字引入当前作用域
* 小心全局作用域使用，容易命名冲突

#### 2.1 单个名字引入

```cpp
using std::cout;
using std::endl;
```

* 更安全，避免污染命名空间

---

### 3. 基类成员引入（解决隐藏问题）

```cpp
class Base {
public:
    void foo(int);
};

class Derived : public Base {
public:
    using Base::foo;  // 将 Base::foo 引入 Derived
    void foo(double);
};
```

* 防止函数重载被隐藏
* 常用于多继承 / 重载场景

---

### 4. 与 `typedef` 对比

| 特性     | typedef | using |
| ------ | ------- | ----- |
| 简单类型别名 | ✔       | ✔     |
| 模板别名   | ✘       | ✔     |
| 语法直观性  | 较复杂     | 简洁    |
| 可读性    | 一般      | 优秀    |

---

### 5. 小提示

* `using` 可以在函数内部作用域使用
* 结合 `namespace` 可局部引入，提高安全性
* 模板别名是现代 C++ 类型简化的重要手段

---

### 一句话总结

> `using` 是**类型、命名空间、基类成员的快捷桥梁**，
> 让代码更清晰、可维护，避免复杂嵌套与隐藏问题。
