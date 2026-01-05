# 一句话面试结论（先背）

> **`constexpr` 用于声明“可在编译期求值的实体”，
> 是否真的在编译期计算，取决于使用场景和上下文。**

---

## 一、`constexpr` 到底解决了什么问题？

在 C++11 之前：

* 编译期常量：`#define`、`enum`
* 没有类型安全
* 不能用于复杂计算

### C++11 引入 `constexpr`：

> 用**类型安全的方式**，
> 把一部分计算提前到**编译期**

---

## 二、`constexpr` 的本质原理

### 关键点 1️⃣

> `constexpr` ≠ 一定编译期执行
> 它的含义是：**“允许在编译期执行”**

### 编译器视角：

* 如果参数是编译期常量
* 且上下文要求编译期常量
* 编译器就在编译期计算

否则：
👉 退化为普通运行期函数 / 变量

---

## 三、`constexpr` 变量

```cpp
constexpr int N = 10;
```

含义：

* `N` 是编译期常量
* 有类型
* 可用于：

    * 数组大小
    * 模板参数
    * case 标签

对比：

```cpp
const int n = foo();   // 不是编译期常量
```

---

## 四、`constexpr` 函数（重点）

### C++11 规则（面试可能问）

* 函数体 **只能一条 return**
* 不能有循环、分支

```cpp
constexpr int square(int x) {
    return x * x;
}
```

---

### C++14 以后（非常重要）

> **constexpr 函数几乎等价于普通函数**

允许：

* if / for / while
* 局部变量
* 复杂逻辑

```cpp
constexpr int sum(int n) {
    int s = 0;
    for (int i = 1; i <= n; ++i)
        s += i;
    return s;
}
```

📌 面试官如果懂，一定会追问 **C++14 的变化**

---

## 五、`constexpr` 函数什么时候在编译期执行？

### 例子：

```cpp
constexpr int f(int x) { return x * 2; }

constexpr int a = f(10); // 编译期
int b = f(10);           // 运行期（也可能被优化）
```

👉 **取决于上下文**

---

## 六、`constexpr` vs `const`（必考）

| 对比     | constexpr | const |
| ------ | --------- | ----- |
| 是否编译期  | 可能        | 不保证   |
| 是否类型安全 | ✅         | ✅     |
| 用于模板参数 | ✅         | ❌     |
| 函数     | ✅         | ❌     |

📌 口诀：

> **constexpr 约束“何时算”，const 约束“能否改”**

---

## 七、`constexpr` 构造函数（进阶）

```cpp
struct Point {
    int x, y;
    constexpr Point(int a, int b) : x(a), y(b) {}
};
```

👉 允许：

* 在编译期构造对象
* 用于 constexpr 上下文

📌 条件：

* 构造函数必须是 constexpr
* 所有成员也必须可 constexpr 构造

---

## 八、`constexpr` 与模板 / metaprogramming

### 经典用途：

* 编译期计算
* 替代部分模板元编程
* 提高可读性

```cpp
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}
```

---

## 九、`constexpr` ≠ inline ≠ 宏（面试爱混）

|       | constexpr | inline | macro |
| ----- | --------- | ------ | ----- |
| 编译期计算 | ✅         | ❌      | ❌     |
| 类型安全  | ✅         | ❌      | ❌     |
| 调试友好  | ✅         | ❌      | ❌     |

---

## 十、常见面试陷阱（一定要知道）

### ❌ 误区 1

> constexpr 一定在编译期执行
> ✅ **错**

---

### ❌ 误区 2

> constexpr 函数不能有循环
> ✅ **C++14 后错**

---

### ❌ 误区 3

> constexpr 就是 const
> ✅ **错**

---

## 十一、面试“标准回答模板”（30 秒版）

> constexpr 用于声明可在编译期求值的变量或函数，
> 是否在编译期执行取决于使用上下文。
> 相比 const，constexpr 提供了更强的编译期语义，
> C++14 之后 constexpr 函数几乎可以写成普通函数，
> 广泛用于编译期计算和模板辅助。
