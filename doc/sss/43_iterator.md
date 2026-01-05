# 一、迭代器面试必考总览

## ① STL 迭代器是什么？

**标准回答（背）：**

> 迭代器是对容器元素的抽象访问接口，
> 提供统一方式遍历不同底层结构的容器，
> 本质是对指针语义的封装。

关键词：

* 抽象
* 泛型算法
* 指针语义

---

## 二、迭代器 5 大分类（必背）

### 面试 100% 会问：**有哪些迭代器？**

| 类型                     | 能做什么  | 常见容器           |
| ---------------------- | ----- | -------------- |
| Input Iterator         | 只读、单向 | istream        |
| Output Iterator        | 只写、单向 | ostream        |
| Forward Iterator       | 读写、单向 | forward_list   |
| Bidirectional Iterator | 双向    | list / map     |
| Random Access Iterator | 随机访问  | vector / deque |

📌 **口诀**：

> 单向 → 双向 → 随机

---

## 三、容器 & 迭代器类型对照（高频）

| 容器           | 迭代器类型         |
| ------------ | ------------- |
| vector       | Random Access |
| deque        | Random Access |
| list         | Bidirectional |
| forward_list | Forward       |
| map / set    | Bidirectional |
| unordered_*  | Forward       |

❗ 很多人会错：
**unordered_map 不是随机访问**

---

## 四、迭代器失效（重点追问）

### Q：什么是迭代器失效？

> 容器结构变化后，原有迭代器指向非法位置。

---

### vector 失效规则（必考）

| 操作             | 是否失效       |
| -------------- | ---------- |
| push_back（未扩容） | 尾后迭代器失效    |
| push_back（扩容）  | **全部失效**   |
| erase          | erase 之后失效 |

📌 面试常补一句：

> vector 扩容会整体搬家

---

### deque 失效规则

* 头尾插入：可能失效
* 中间插入：一定失效
* 比 vector 稍复杂

---

### list 失效规则（面试加分）

> **list 插入不会使已有迭代器失效**
> 只有被 erase 的那个失效

📌 因为节点独立

---

### unordered_map 失效规则（很爱考）

* rehash：**全部失效**
* erase：被删的失效

---

## 五、erase 的正确写法（经典坑）

❌ 错误写法：

```cpp
for (auto it = v.begin(); it != v.end(); ++it) {
    if (*it == 3)
        v.erase(it); // UB
}
```

✅ 正确写法：

```cpp
for (auto it = v.begin(); it != v.end();) {
    if (*it == 3)
        it = v.erase(it);
    else
        ++it;
}
```

📌 面试必会写

---

## 六、迭代器 vs 指针（常问）

| 对比  | 指针 | 迭代器 |
| --- | -- | --- |
| 抽象  | 无  | 有   |
| 安全  | 低  | 相对高 |
| 泛型  | ❌  | ✅   |
| 运算符 | 原生 | 重载  |

---

## 七、const_iterator（进阶）

### Q：iterator 和 const_iterator 区别？

> const_iterator 不能修改元素，
> 用于只读遍历，保证接口安全。

```cpp
vector<int>::const_iterator it;
```

---

## 八、为什么 map 的迭代器不是随机访问？

**标准回答：**

> map 基于红黑树，
> 节点非连续，无法 O(1) 定位第 i 个元素，
> 所以只能双向遍历。

---

## 九、算法对迭代器的要求（加分）

| 算法       | 要求            |
| -------- | ------------- |
| sort     | Random Access |
| find     | Input         |
| advance  | Forward       |
| distance | Input         |

📌 面试问：

> 为什么 sort 不能用在 list？

---

## 十、++it vs it++（细节加分）

**标准回答：**

> ++it 返回引用，it++ 产生临时对象，
> 对迭代器推荐使用前置 ++。

---

## 十一、面试「一句话模板」合集

* **迭代器是 STL 泛型算法的核心**
* **容器不同，迭代器能力不同**
* **结构变化会导致迭代器失效**
* **erase 要用返回值接**

---

## 十二、面试官最爱连环问（你已经在这条线上了）

1. vector 扩容为什么会失效？
2. list 为什么不会？
3. unordered_map 什么时候 rehash？
4. sort 为什么不能用于 map？
5. iterator 还能做哪些设计？
