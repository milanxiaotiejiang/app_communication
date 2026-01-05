**重载运算符（Operator Overloading）** 是 C++ 的一种特性，允许**为自定义类型定义或修改运算符的行为**，使对象可以像内置类型一样使用运算符。

---

### 1. 基本语法

```cpp
class Complex {
public:
    double real, imag;
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // 重载 + 运算符
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }
};
```

使用示例：

```cpp
Complex a(1, 2), b(3, 4);
Complex c = a + b; // 调用 operator+
```

---

### 2. 可以重载的运算符

* **算术运算符**：`+ - * / % ++ --`
* **比较运算符**：`== != < > <= >=`
* **逻辑运算符**：`! && ||`（注意逻辑短路限制）
* **赋值运算符**：`= += -= *= /=`
* **下标和函数调用**：`[] ()`
* **流操作符**：`<< >>`

---

### 3. 重载方式

#### 3.1 成员函数

* 左操作数为类对象

```cpp
class A {
public:
    A operator+(const A& other) const;
};
```

#### 3.2 非成员函数（通常与友元一起）

* 支持左操作数为非类对象

```cpp
class A {
    int x;
public:
    A(int val) : x(val) {}
    friend A operator+(const A& lhs, const A& rhs);
};

A operator+(const A& lhs, const A& rhs) {
    return A(lhs.x + rhs.x);
}
```

---

### 4. 注意事项

1. **不可改变运算符优先级和结合性**
2. **某些运算符不能重载**：`:: . .* sizeof typeid` 等
3. **尽量保持语义一致**，不要违反运算符原本意义
4. 对复杂类型最好返回引用或新对象，避免不必要拷贝

---

### 5. 典型应用场景

* **数学类**：复数、矩阵、向量运算
* **智能指针**：重载 `*` 和 `->`
* **容器类**：重载下标 `[]` 或流操作符 `<< >>`
* **自定义数据类型**：提供自然语义的操作

---

### 一句话总结

> 重载运算符允许**自定义类型像内置类型一样操作**，
> 提高代码可读性和可维护性，同时保留语义一致性。
