**多态（Polymorphism）** 是面向对象编程的核心特性之一，指**同一操作在不同对象上表现出不同的行为**。C++ 中多态主要分为 **编译时多态（静态多态）** 和 **运行时多态（动态多态）**。

---

### 1. 编译时多态（静态多态）

通过 **函数重载** 和 **运算符重载** 实现，调用在编译期就确定。

```cpp
class Print {
public:
    void show(int x) { std::cout << "int: " << x << std::endl; }
    void show(double x) { std::cout << "double: " << x << std::endl; }
};

Print p;
p.show(10);    // 调用 int 版本
p.show(3.14);  // 调用 double 版本
```

---

### 2. 运行时多态（动态多态）

通过 **继承 + 虚函数（virtual）** 实现，调用在运行时根据对象实际类型决定。

```cpp
class Shape {
public:
    virtual void draw() { std::cout << "Shape" << std::endl; }
    virtual ~Shape() = default;
};

class Circle : public Shape {
public:
    void draw() override { std::cout << "Circle" << std::endl; }
};

Shape* s = new Circle();
s->draw(); // 调用 Circle 的 draw()
delete s;
```

* `virtual` 声明允许子类覆盖
* 使用基类指针或引用实现多态调用
* 运行时通过虚表（vtable）实现动态绑定

---

### 3. 纯虚函数与抽象类

* **纯虚函数**：必须在派生类中实现

```cpp
class Shape {
public:
    virtual void draw() = 0; // 纯虚函数
};
```

* **抽象类**：包含至少一个纯虚函数的类，不能实例化
* 为多态提供接口基础

---

### 4. 多态的优势

1. **接口统一**：不同对象可通过同一接口调用
2. **可扩展性强**：新增子类无需修改调用代码
3. **降低耦合**：调用者只依赖基类接口，不关心具体实现

---

### 5. 注意事项

* 析构函数应为虚函数，避免资源泄漏
* 非虚函数无法实现运行时多态
* 多态增加了一定的性能开销（虚表查找）

---

### 一句话总结

> 多态是**同一接口、不同实现的机制**，
> 支持统一操作、灵活扩展，是面向对象设计的重要支柱。
