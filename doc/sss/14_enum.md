在 C++ 中，`enum`（枚举类型）用于**定义一组具名整型常量**，提高代码可读性与可维护性。C++ 在 C 的基础上进行了扩展，引入了 **强类型枚举（enum class）**。

---

### 1. 基本枚举（传统 C 风格）

```cpp
enum Color {
    Red,
    Green,
    Blue
};

Color c = Red;
```

* 默认底层类型为 `int`
* 枚举值在同一作用域中是全局可见的
* 可隐式转换为整数

```cpp
int x = Red; // 合法
```

---

### 2. 指定枚举值

```cpp
enum Color {
    Red = 1,
    Green = 5,
    Blue = 10
};
```

* 可以手动指定数值
* 未指定的按前一个值 + 1 自动递增

---

### 3. 枚举的底层类型（C++11）

```cpp
enum Color : uint8_t {
    Red, Green, Blue
};
```

* 明确控制存储类型
* 有利于内存优化和二进制兼容

---

### 4. 强类型枚举（enum class / scoped enum）

```cpp
enum class Color {
    Red,
    Green,
    Blue
};

Color c = Color::Red;
int x = c; // 编译错误，需要显式转换
```

* 不会隐式转换为整数
* 枚举值作用域被限定在枚举类型内
* 可以减少命名冲突

---

### 5. 显式类型与强类型结合

```cpp
enum class Color : uint8_t { Red, Green, Blue };
```

* 控制底层类型
* 保留类型安全

---

### 6. 常见应用场景

* 状态机 / 事件类型
* 配置选项
* 标记 / 位域（配合位操作）

```cpp
enum Flags : uint8_t {
    FLAG_A = 1 << 0,
    FLAG_B = 1 << 1,
    FLAG_C = 1 << 2
};
```

---

### 7. 注意事项

* 传统枚举可被隐式转换为整数 → 容易出错
* 强类型枚举需要使用 `static_cast<int>(Color::Red)` 获取数值
* 不同枚举类型之间不可直接比较

---

### 一句话总结

> `enum` 用于**定义一组具名整型常量**，
> `enum class` 提供**强类型与作用域保护**，提升安全性与可读性。
