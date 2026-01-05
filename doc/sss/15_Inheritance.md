**继承（Inheritance）** 是面向对象编程的核心机制之一，指一个类（子类）可以**复用另一个类（基类）的属性和行为**
，并可在此基础上扩展或修改功能，从而实现代码重用和层次化设计。

---

### 1. 基本语法

```cpp
class Base {
public:
    void show() { std::cout << "Base" << std::endl; }
};

class Derived : public Base { // 公有继承
public:
    void display() { std::cout << "Derived" << std::endl; }
};

Derived d;
d.show();    // 调用基类方法
d.display(); // 调用子类方法
```

---

### 2. 继承类型

| 类型          | 描述                                               | 子类访问权限             |
|-------------|--------------------------------------------------|--------------------|
| `public`    | 公有继承，基类 public → public，protected → protected    | 子类和外部可访问 public 成员 |
| `protected` | 保护继承，基类 public → protected，protected → protected | 子类可访问，外部不可访问       |
| `private`   | 私有继承，基类 public/protected → private               | 子类内部可访问，外部不可访问     |

---

### 3. 构造与析构顺序

* **构造函数**：先基类 → 再子类
* **析构函数**：先子类 → 再基类

```cpp
class Base {
public:
    Base() { std::cout << "Base ctor\n"; }
    ~Base() { std::cout << "Base dtor\n"; }
};

class Derived : public Base {
public:
    Derived() { std::cout << "Derived ctor\n"; }
    ~Derived() { std::cout << "Derived dtor\n"; }
};
```

输出：

```
Base ctor
Derived ctor
Derived dtor
Base dtor
```

---

### 4. 方法重写（覆盖）

```cpp
class Base {
public:
    virtual void speak() { std::cout << "Base" << std::endl; }
};

class Derived : public Base {
public:
    void speak() override { std::cout << "Derived" << std::endl; }
};

Base* b = new Derived();
b->speak(); // 调用 Derived 的 speak()
```

* 使用 `virtual` 实现**运行时多态**
* 子类可以覆盖基类方法

---

### 5. 继承的优势

1. **代码复用**：无需重复定义相同属性和方法
2. **层次化设计**：清晰表示“是一个”的关系
3. **多态基础**：支持接口抽象与运行时行为选择

---

### 6. 注意事项

* 构造函数不会被继承，需显式调用
* 多重继承可能导致二义性（菱形继承需虚继承）
* 访问权限决定子类可见性

---

### 一句话总结

> 继承是**子类复用和扩展基类功能的机制**，
> 支持代码复用、层次化设计，并为多态提供基础。
