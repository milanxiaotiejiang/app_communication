**面向对象（Object-Oriented Programming, OOP）** 是一种程序设计范式，以 **对象（Object）** 为核心，通过封装、继承、多态等机制来组织代码，提高可重用性、可维护性和可扩展性。

---

### 1. 核心概念

#### 1.1 对象与类

* **类（Class）**：描述对象属性（成员变量）和行为（成员函数）的模板
* **对象（Object）**：类的实例，拥有状态和行为

```cpp
class Car {
    int speed;
public:
    void run() { speed += 10; }
};

Car myCar; // 对象实例
myCar.run();
```

---

#### 1.2 封装（Encapsulation）

* 将数据和操作组合在类内部
* 提供**访问控制**：`private` / `protected` / `public`
* 保护内部状态不被外部随意修改

```cpp
class Account {
private:
    double balance;
public:
    void deposit(double amt) { balance += amt; }
    double getBalance() const { return balance; }
};
```

---

#### 1.3 继承（Inheritance）

* 子类继承父类成员和行为
* 提高代码复用
* 支持**方法重写**

```cpp
class Vehicle {
public:
    void start() { }
};

class Car : public Vehicle {
public:
    void openDoor() { }
};
```

---

#### 1.4 多态（Polymorphism）

* **同一接口，不同实现**
* C++ 通过**虚函数**实现运行时多态（动态绑定）

```cpp
class Shape {
public:
    virtual void draw() = 0; // 纯虚函数
};

class Circle : public Shape {
public:
    void draw() override { /* 画圆 */ }
};

Shape* s = new Circle();
s->draw(); // 调用 Circle 的 draw()
```

---

### 2. 优势

* **模块化**：对象为单位组织代码
* **可扩展**：通过继承和接口扩展功能
* **可维护**：封装减少耦合
* **可复用**：类和对象可在不同项目中复用

---

### 3. 常见设计原则

* **单一职责原则（SRP）**：每个类只做一件事
* **开闭原则（OCP）**：对扩展开放，对修改封闭
* **里氏替换原则（LSP）**：子类可以替代父类
* **接口隔离原则（ISP）**：使用小而专一的接口
* **依赖倒置原则（DIP）**：依赖抽象，不依赖具体实现

---

### 一句话总结

> 面向对象通过**类和对象封装数据与行为，利用继承和多态实现代码复用与灵活扩展**，是现代 C++ 编程的核心范式。
