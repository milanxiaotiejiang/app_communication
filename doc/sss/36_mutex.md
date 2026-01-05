在 C++ 中，`mutex`（互斥锁）是 **线程同步的基本工具**，用于保护共享资源，防止多线程同时访问导致的数据竞争和不一致。

---

### 1. 基本用法

```cpp
#include <iostream>
#include <thread>
#include <mutex>

std::mutex mtx;  // 全局互斥锁
int counter = 0;

void increment() {
    for (int i = 0; i < 1000; ++i) {
        mtx.lock();   // 上锁
        ++counter;
        mtx.unlock(); // 解锁
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join();
    t2.join();
    std::cout << "Counter: " << counter << std::endl;
}
```

* `mtx.lock()` 获取锁，其他线程必须等待
* `mtx.unlock()` 释放锁

---

### 2. 推荐方式：`std::lock_guard` 或 `std::unique_lock`

* RAII 风格，异常安全，不必显式 unlock

```cpp
void increment_safe() {
    for (int i = 0; i < 1000; ++i) {
        std::lock_guard<std::mutex> lock(mtx); // 构造上锁，析构解锁
        ++counter;
    }
}
```

* `std::unique_lock` 更灵活，可延迟锁定或手动解锁

```cpp
std::unique_lock<std::mutex> lock(mtx, std::defer_lock); // 延迟锁
lock.lock();   // 手动锁
lock.unlock(); // 手动解锁
```

---

### 3. 特点

1. **互斥访问**：同一时间只有一个线程可以访问被保护资源
2. **阻塞式**：未获得锁的线程会等待
3. **不可复制**：mutex 对象不能被复制或赋值

---

### 4. 注意事项

* **死锁风险**：多个线程互相等待对方锁，需谨慎锁顺序
* 尽量缩小锁的作用范围
* 对于轻量级锁操作可考虑 `std::atomic` 提高效率

---

### 5. 使用场景

* 多线程共享数据（计数器、缓存）
* 设备或硬件资源的独占访问
* 异步任务或机器人控制模块同步

---

### 一句话总结

> `mutex` 是 **线程同步工具**，通过互斥锁保护共享资源，保证多线程安全访问，
> RAII 风格的 `lock_guard` 或 `unique_lock` 提供异常安全和更高可维护性。
