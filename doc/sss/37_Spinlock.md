**自旋锁（Spinlock）** 是一种轻量级线程同步机制，用于多线程环境下保护共享资源。与互斥锁不同，自旋锁**不会使线程阻塞，而是在等待锁释放期间持续“旋转”检查锁状态**。

---

### 1. 基本原理

* 线程尝试获取锁，如果锁已被占用，则不断循环检查（自旋）
* 适合 **临界区非常短、上下文切换成本高** 的场景
* 不会触发线程调度，但可能浪费 CPU

---

### 2. 示例（C++11 `atomic_flag` 实现）

```cpp
#include <atomic>
#include <thread>
#include <iostream>

std::atomic_flag lock_flag = ATOMIC_FLAG_INIT;
int counter = 0;

void spinlock_lock() {
    while (lock_flag.test_and_set(std::memory_order_acquire)) {
        // 自旋等待
    }
}

void spinlock_unlock() {
    lock_flag.clear(std::memory_order_release);
}

void worker() {
    for (int i = 0; i < 1000; ++i) {
        spinlock_lock();
        ++counter;
        spinlock_unlock();
    }
}

int main() {
    std::thread t1(worker), t2(worker);
    t1.join();
    t2.join();
    std::cout << "Counter: " << counter << std::endl;
}
```

* `test_and_set` 尝试获取锁
* `clear` 释放锁
* 临界区很短时效率高

---

### 3. 特点

1. **忙等待**：不会阻塞线程，CPU 会被消耗
2. **轻量级**：没有上下文切换开销
3. **适合短临界区**：长时间占用锁会浪费 CPU

---

### 4. 注意事项

* 临界区过长会导致 CPU 空转，效率低下
* 不适合线程数远大于 CPU 核心数的场景
* 可结合 `std::this_thread::yield()` 或退避策略减少 CPU 消耗

---

### 5. 使用场景

* **嵌入式系统**：短时间访问共享寄存器
* **多核机器人控制模块**：保护短期共享数据
* **高性能计算**：短期计数器、缓存访问

---

### 一句话总结

> 自旋锁通过 **忙等待方式保护共享资源**，避免线程阻塞和上下文切换开销，
> 适合短临界区和多核并行场景，但临界区过长会浪费 CPU。
