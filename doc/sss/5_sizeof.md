在 C / C++ 中，`sizeof` 是**编译期运算符**，用于获取**对象或类型所占的字节数**，结果类型为 `size_t`。

---

### 1. 基本用法

```cpp
sizeof(int)
sizeof x
sizeof(MyStruct)
```

* 可作用于 **类型** 或 **表达式**
* 返回值单位始终是 **字节**

---

### 2. 常见结果（与平台相关）

```cpp
sizeof(char)      == 1
sizeof(short)     == 2
sizeof(int)       == 4
sizeof(long)      == 4 或 8
sizeof(long long) == 8
sizeof(void*)     == 4 或 8
```

与 **编译器、架构、ABI** 有关。

---

### 3. 对数组与指针的区别

```cpp
int a[10];
int* p = a;

sizeof(a); // 10 * sizeof(int)
sizeof(p); // sizeof(int*)
```

`sizeof` **不会退化为指针**，但函数参数中的数组会。

---

### 4. 结构体大小与内存对齐

```cpp
struct A {
    char c;
    int  i;
};
```

```cpp
sizeof(A); // 通常为 8，而不是 5
```

原因：**内存对齐 / padding**

* 成员按对齐规则排列
* 结构体整体大小为最大对齐数的整数倍

---

### 5. 空结构体与类

```cpp
struct Empty {};
sizeof(Empty); // >= 1
```

保证对象具有唯一地址。

---

### 6. `sizeof` 的编译期特性

```cpp
int i = 0;
sizeof(i++); // i 不会自增
```

表达式不会被求值。

例外：

* C99 的 VLA（变长数组）
* 某些编译器扩展

---

### 7. 与模板、泛型常用搭配

```cpp
template <typename T>
void foo() {
    std::cout << sizeof(T) << std::endl;
}
```

---

### 8. 常见陷阱

* `sizeof(pointer)` ≠ 指向内容大小
* `sizeof` 不能获取动态内存大小
* 不等同于对象“占用的实际内存”（缓存、虚函数表等不在内）

---

### 一句话总结

> `sizeof` 计算的是**类型在内存中的静态布局大小**，
> 与运行期数据量无关。

如需结合 **内存对齐、ARM / x86 差异、嵌入式内存优化** 等场景，可进一步展开。
