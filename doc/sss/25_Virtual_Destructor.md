**虚析构函数（Virtual Destructor）** 是 C++ 中用于保证**通过基类指针删除派生类对象时能够正确释放资源**的机制。它是多态和资源管理中非常重要的概念。

---

### 1. 为什么需要虚析构函数

如果基类析构函数不是虚函数，通过基类指针删除派生类对象只会调用基类析构函数，**派生类的资源不会释放**，可能导致内存泄漏。

```cpp
class Base {
public:
    ~Base() { std::cout << "Base dtor\n"; }
};

class Derived : public Base {
public:
    ~Derived() { std::cout << "Derived dtor\n"; }
};

Base* b = new Derived();
delete b; // 只调用 Base 析构函数，Derived 析构函数未执行
```

---

### 2. 正确做法：虚析构函数

```cpp
class Base {
public:
    virtual ~Base() { std::cout << "Base dtor\n"; }
};

class Derived : public Base {
public:
    ~Derived() { std::cout << "Derived dtor\n"; }
};

Base* b = new Derived();
delete b; // 先调用 Derived 析构函数，再调用 Base 析构函数
```

* 保证派生类析构函数被执行
* 正确释放资源，避免内存泄漏

---

### 3. 特点

1. 只需在基类声明一次 `virtual`
2. 派生类自动成为虚析构
3. 允许派生类覆盖析构逻辑

---

### 4. 使用场景

* **通过基类指针或引用操作派生类对象**
* **多态类**：所有可能被继承并通过基类指针删除的类

```cpp
class Shape {
public:
    virtual ~Shape() = default; // 虚析构函数
};
```

* 使用 `= default` 可以生成默认析构逻辑

---

### 5. 注意事项

* 非多态类无需虚析构函数
* 虚析构函数略微增加开销（虚表指针）
* 避免在栈上频繁使用虚析构的对象影响性能

---

### 一句话总结

> 虚析构函数保证**通过基类指针删除派生类对象时，派生类和基类的资源都能正确释放**，
> 是实现安全多态和资源管理的重要手段。
