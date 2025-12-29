在 C++ 中，`struct` 与 `class` **本质完全相同**，差异只体现在**默认规则与设计语义**上，而非能力层级。

---

### 1. 唯一的语言层差异

| 对比项      | struct   | class     |
| -------- | -------- | --------- |
| 默认成员访问权限 | `public` | `private` |
| 默认继承权限   | `public` | `private` |

除此之外：

* 都支持构造 / 析构
* 都支持继承、多态、虚函数
* 都可定义模板、静态成员

---

### 2. 成员访问示例

```cpp
struct S {
    int x;        // public
};

class C {
    int x;        // private
};
```

---

### 3. 继承默认差异

```cpp
struct A : B {};   // public 继承
class  C : B {};   // private 继承
```

---

### 4. 设计语义上的约定（重要）

#### 4.1 `struct` 的典型定位

* 数据聚合体
* Plain Old Data / DTO
* 强调“数据结构”
* 成员通常可直接访问

```cpp
struct Point {
    double x;
    double y;
};
```

---

#### 4.2 `class` 的典型定位

* 抽象与封装
* 行为驱动
* 对外隐藏实现细节
* 不变量由接口保证

```cpp
class Buffer {
public:
    void write(int v);
private:
    int size;
};
```

---

### 5. 与 C 的兼容性

* `struct` 可直接用于 C 风格接口
* 含有构造、虚函数等后即不再是 C 结构体
* `class` 不具备 C 兼容性语义

---

### 6. 常见使用建议

* **只承载数据** → `struct`
* **有封装、有不变量** → `class`
* **接口 / 抽象基类** → `class`
* **配置、参数、消息体** → `struct`

---

### 7. 误区澄清

* `struct` 不是“弱化版 class”
* 性能、内存布局无差异
* 选择取决于**语义表达，而非技术能力**

---

### 一句话总结

> `struct` 与 `class` 的差别只在**默认权限**，
> 选择取决于**设计意图是否强调封装**。
