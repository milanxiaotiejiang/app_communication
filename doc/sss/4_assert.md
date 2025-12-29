在 C / C++ 中，`assert` 用于**在调试阶段验证“必须成立的条件”**，当条件为假时立即终止程序，用于尽早暴露逻辑错误。

---

### 1. 基本用法

```cpp
#include <cassert>

assert(ptr != nullptr);
assert(x > 0);
```

* 条件为真：无任何影响
* 条件为假：输出错误信息并终止程序

---

### 2. 工作机制

* 发生位置、表达式、源文件、行号会被打印
* 通常调用 `abort()` 直接结束进程
* 不进行异常处理，也不会被 `try-catch` 捕获

---

### 3. 与 `NDEBUG`

```cpp
#define NDEBUG
#include <cassert>
```

* 定义 `NDEBUG` 后，所有 `assert` 会被**完全移除**
* 常用于 Release 构建

因此：

* `assert` 中**不能写有副作用的代码**

---

### 4. 适合断言的内容

✔ 内部逻辑不变量
✔ 前置条件 / 后置条件
✔ 不可能发生的分支
✔ 调试阶段的安全检查

```cpp
assert(index < size);
assert(state == INIT || state == RUNNING);
```

---

### 5. 不适合用 `assert` 的场景

✘ 用户输入校验
✘ 运行期可恢复错误
✘ 必须在 Release 中生效的检查

这些应使用：

* `if + 错误处理`
* `throw exception`
* 返回错误码

---

### 6. `assert` 与异常的对比

| 项目    | assert  | exception |
|-------|---------|-----------|
| 目的    | 找程序 bug | 处理可预期错误   |
| 是否可关闭 | 可以      | 不可以       |
| 是否可恢复 | 不可      | 可以        |
| 运行阶段  | 调试期     | 运行期       |

---

### 7. 扩展用法：自定义断言

```cpp
#ifdef DEBUG
#define MY_ASSERT(x) assert(x)
#else
#define MY_ASSERT(x) ((void)0)
#endif
```

---

### 一句话总结

> `assert` 用于**证明程序写对了**，
> 而不是用于**应对错误发生**。

