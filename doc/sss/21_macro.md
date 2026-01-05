在 C/C++ 中，**宏（macro）**是由预处理器（preprocessor）处理的**文本替换工具**，在编译前进行展开，不属于语言本身的语义范畴。

---

### 1. 定义宏

```cpp
#define PI 3.14159
#define MAX(a,b) ((a) > (b) ? (a) : (b))
```

* `#define` 定义常量或函数式宏
* 纯文本替换，不进行类型检查

---

### 2. 常见宏类型

#### 2.1 对象宏

```cpp
#define SIZE 100
int arr[SIZE];
```

* 简单常量替换
* 不占内存

#### 2.2 函数宏

```cpp
#define SQUARE(x) ((x)*(x))
int y = SQUARE(5); // 展开为 (5)*(5)
```

* 参数会被文本替换
* 注意加括号避免优先级问题

#### 2.3 条件宏

```cpp
#ifdef DEBUG
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x)
#endif
```

* 可用于条件编译

#### 2.4 特殊宏

* `__FILE__` → 当前文件名
* `__LINE__` → 当前行号
* `__DATE__` → 编译日期
* `__TIME__` → 编译时间

```cpp
std::cout << "Error in file " << __FILE__ << " at line " << __LINE__;
```

---

### 3. 宏的优缺点

**优点：**

* 编译期常量替换
* 条件编译
* 可生成简单函数式代码

**缺点：**

* 无类型检查
* 调试困难
* 容易出现副作用（如 `SQUARE(x++)`）
* 可替代方案：`const`、`constexpr`、`inline` 函数

---

### 4. 现代替代方案

| 宏用途  | 替代方式                         |
| ---- | ---------------------------- |
| 常量   | `const` / `constexpr`        |
| 函数宏  | `inline` / `template`        |
| 条件编译 | `constexpr if` / `inline` 函数 |

---

### 5. 使用建议

* 尽量避免函数式宏
* 常量用 `constexpr`
* 调试和平台相关代码可适度使用条件宏

---

### 一句话总结

> 宏是**预处理阶段的文本替换工具**，
> 可做条件编译和简单计算，但缺乏类型安全，现代 C++ 多用 `constexpr` 与 `inline` 替代。
