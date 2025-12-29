在 C++ 中，`typeid` 和 `type_info` 用于 **运行时类型识别（RTTI, Run-Time Type Information）**，可以获取对象或类型的实际类型信息，常用于多态场景下的类型判断。

---

### 1. `typeid`

* 用于获取对象或类型的类型信息
* 返回一个 `const std::type_info&`
* 可用于 **多态类型**，配合虚函数获取实际派生类型

```cpp
#include <iostream>
#include <typeinfo>

class Base { virtual void foo() {} };
class Derived : public Base {};

Base b;
Derived d;
Base* pb = &d;

std::cout << typeid(b).name() << std::endl;   // 输出 Base
std::cout << typeid(d).name() << std::endl;   // 输出 Derived
std::cout << typeid(*pb).name() << std::endl; // 输出 Derived（多态）
```

* 注意：若对象是非多态类型（没有虚函数），`typeid(*ptr)` 返回 **指针静态类型**，不会识别派生类

---

### 2. `type_info`

* `typeid` 返回的对象类型
* 提供以下主要接口：

    * `name()`：返回类型名（编译器相关）
    * `operator==` / `operator!=`：比较类型是否相同
    * `before()`：用于类型排序（少用）

```cpp
const std::type_info& ti1 = typeid(d);
const std::type_info& ti2 = typeid(*pb);

if (ti1 == ti2) {
    std::cout << "同类型" << std::endl;
} else {
    std::cout << "不同类型" << std::endl;
}
```

---

### 3. 特点与注意事项

1. **依赖 RTTI**：多态类型需要至少一个虚函数
2. `name()` 返回值是编译器特定的，可能需要额外 demangle 才能得到可读类型名
3. 可以和 `dynamic_cast` 配合使用实现类型检查与安全转换

---

### 4. 使用场景

* **多态对象类型判断**：运行时判断派生类类型
* **调试与日志**：输出对象实际类型信息
* **通用容器**：存储不同类型对象时判断类型

---

### 一句话总结

> `typeid` 获取对象或类型的运行时类型信息，返回 `type_info` 对象，
> 可用于多态类型判断、类型比较和调试，是 C++ RTTI 的核心工具。
                 