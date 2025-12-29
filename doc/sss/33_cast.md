C++ 提供了一系列 **类型转换运算符**，用于不同场景下的安全或低层次类型转换，分别为 `static_cast`、`dynamic_cast`、`const_cast`、`reinterpret_cast`，以及在运行时类型检查失败时可能抛出的异常 `std::bad_cast`。

---

### 1. `static_cast`

* **编译时类型检查**，用于已知安全的转换
* 可用于：

    * 基类 ↔ 派生类指针或引用（非多态情况下安全）
    * 内置类型之间转换（如 `int` ↔ `double`）
    * 指针和 `void*` 之间转换

```cpp
double d = 3.14;
int i = static_cast<int>(d); // double → int

class Base {};
class Derived : public Base {};
Base* b = new Derived;
Derived* dptr = static_cast<Derived*>(b);
```

---

### 2. `dynamic_cast`

* **运行时类型检查**，用于多态类型
* 只能用于有虚函数的类（多态类）
* 安全向下转换（基类 → 派生类），失败返回 `nullptr`（指针）或抛出 `std::bad_cast`（引用）

```cpp
class Base { virtual void foo() {} };
class Derived : public Base {};
Base* b = new Derived;
Derived* d = dynamic_cast<Derived*>(b); // 成功返回指针
```

* 引用示例：

```cpp
Base b;
try {
    Derived& dref = dynamic_cast<Derived&>(b); // 若类型不匹配抛 std::bad_cast
} catch (const std::bad_cast& e) {
    std::cout << e.what();
}
```

---

### 3. `const_cast`

* 用于 **去掉或添加 const/volatile 修饰符**
* 不改变对象类型本身
* 常用于调用需要非常量参数的函数

```cpp
const int x = 10;
int& y = const_cast<int&>(x); // 去掉 const（慎用）
y = 20; // 未定义行为，如果原对象本身是 const
```

---

### 4. `reinterpret_cast`

* **低级类型转换**，直接解释内存表示
* 可用于指针类型之间转换、整数 ↔ 指针等
* 使用不当可能导致未定义行为

```cpp
long p = 0x12345678;
int* ip = reinterpret_cast<int*>(p); // 内存重新解释
```

---

### 5. `std::bad_cast`

* 当 `dynamic_cast` 对 **引用类型** 转换失败时抛出
* 可通过捕获异常处理类型不匹配情况

```cpp
try {
    Derived& dref = dynamic_cast<Derived&>(b);
} catch (const std::bad_cast& e) {
    std::cout << "类型转换失败: " << e.what();
}
```

---

### 6. 总结对比

| 转换类型               | 用途                | 检查方式 | 风险                |
| ------------------ | ----------------- | ---- | ----------------- |
| `static_cast`      | 编译时安全转换           | 编译期  | 基类/派生类多态不安全       |
| `dynamic_cast`     | 多态类型安全向下转换        | 运行时  | 失败返回 nullptr 或抛异常 |
| `const_cast`       | 修改 const/volatile | 编译期  | 对原本 const 对象修改未定义 |
| `reinterpret_cast` | 低级指针/整数转换         | 编译期  | 高风险，易未定义行为        |

---

### 一句话总结

> C++ 提供多种类型转换运算符用于不同场景：
> `static_cast` 编译时安全，`dynamic_cast` 多态运行时安全，`const_cast` 修改 const 修饰，`reinterpret_cast` 低级强制转换，而 `std::bad_cast` 用于捕获引用类型转换失败。

可结合 **嵌入式硬件寄存器操作、机器人多态模块管理、低层数据处理** 等场景进一步说明。
