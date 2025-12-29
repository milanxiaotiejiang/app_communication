在 C++ 中，`friend` 用于**授予非成员函数或其他类访问本类私有成员的权限**，核心目的是**控制访问范围而不破坏封装**。

---

### 1. 基本语法

#### 1.1 友元函数

```cpp
class A {
private:
    int x;
public:
    friend void showX(const A& a);
};

void showX(const A& a) {
    std::cout << a.x << std::endl; // 可访问 private
}
```

---

#### 1.2 友元类

```cpp
class B;

class A {
private:
    int x;
    friend class B; // B 类可访问 A 的私有成员
};

class B {
public:
    void setA(A& a, int val) { a.x = val; }
};
```

---

### 2. 友元函数与成员函数对比

| 特性      | 友元函数    | 成员函数       |
| ------- | ------- | ---------- |
| 可访问私有成员 | ✔       | ✔          |
| 属于类     | ✘       | ✔          |
| 通过对象调用  | ✔（显式对象） | ✔（隐式 this） |
| 破坏封装    | 较小风险    | 无风险        |

---

### 3. 注意事项

* **不是继承**：友元关系不传递给派生类
* **不是成员**：不占类内存
* **声明顺序**：友元函数可以在类外定义
* **作用范围**：只对被声明的类生效

---

### 4. 常见应用场景

* 重载运算符（如 `operator<<` / `operator>>`）
* 两类紧密耦合的数据结构（如 `Matrix` 与 `Vector`）
* 访问私有成员以实现高效接口

```cpp
class Vector;

class Matrix {
    friend Vector operator*(const Matrix&, const Vector&);
};
```

---

### 5. 与封装原则的平衡

* **谨慎使用**：只在确有必要时暴露
* 可替代方案：公共 getter / setter 或接口函数
* 避免滥用导致类设计松散

---

### 一句话总结

> `friend` 是**控制访问权的工具**，
> 它让非成员函数或类可以安全访问私有成员，而不破坏整体封装。
