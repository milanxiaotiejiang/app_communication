在 C++ 中，`decltype` 是一个 **编译期类型推导运算符**，用于获取表达式的类型，而无需显式指定类型。它与 `auto` 配合常用于泛型编程和模板元编程。

---

### 1. 基本用法

```cpp
int a = 10;
decltype(a) b; // b 的类型为 int
b = 20;
```

---

### 2. 推导表达式类型

```cpp
int x = 1;
const int& y = x;

decltype(x)   a; // int
decltype(y)   b; // const int&
```

* **保留表达式的 const、引用等修饰**
* 区别于 `auto`：`auto` 会去掉引用和 const 修饰（除非加 `&` 或 `const`）

---

### 3. 与函数返回类型结合

```cpp
int add(int a, int b) { return a + b; }

decltype(add(1,2)) sum; // sum 的类型为 int
```

* 适合复杂表达式或模板函数返回类型推导

#### 3.1 C++11 后尾返回类型

```cpp
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) {
    return t + u;
}
```

---

### 4. 与模板配合

```cpp
template<typename T1, typename T2>
void foo(T1 a, T2 b) {
    decltype(a + b) c; // 推导 a+b 的类型
}
```

* 泛型编程中可用于 **类型安全的自动推导**

---

### 5. 与 `auto` 对比

| 特性           | auto     | decltype         |
| ------------ | -------- | ---------------- |
| 推导方式         | 根据初始化值   | 根据表达式类型          |
| 保留 const / & | 去掉（需修饰符） | 保留表达式修饰          |
| 用途           | 声明变量     | 声明变量 / 函数返回 / 模板 |

---

### 6. 常见使用场景

* 函数返回类型依赖参数表达式
* 模板中复合类型推导
* 保留 const / 引用语义
* 泛型容器类型获取

```cpp
std::vector<int> v;
decltype(v.begin()) it; // 推导为 std::vector<int>::iterator
```

---

### 一句话总结

> `decltype` 用于**获取表达式的完整类型信息**，
> 保留 const、引用等修饰，适合泛型和模板场景。
