在 C++ 中，**信号量（Semaphore）** 是一种多线程同步机制，用于控制对共享资源的访问数量，适合 **限制同时访问资源的线程数**。C++20 引入了标准信号量 `std::counting_semaphore` 和 `std::binary_semaphore`。

---

### 1. 基本概念

* **计数信号量（Counting Semaphore）**：维护一个计数值，表示可用资源数量

    * `acquire()`：尝试获取资源，计数减 1，如果计数为 0，线程阻塞
    * `release()`：释放资源，计数加 1，可能唤醒等待线程
* **二值信号量（Binary Semaphore）**：类似互斥锁，计数仅为 0 或 1

---

### 2. C++20 示例

```cpp
#include <iostream>
#include <thread>
#include <semaphore>

std::counting_semaphore<3> sem(3); // 初始计数为 3，可同时允许 3 个线程

void worker(int id) {
    sem.acquire();  // 获取信号量
    std::cout << "Worker " << id << " is running\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    sem.release();  // 释放信号量
}

int main() {
    std::thread threads[6];
    for (int i = 0; i < 6; ++i)
        threads[i] = std::thread(worker, i);
    for (auto& t : threads) t.join();
}
```

* 同一时间最多 3 个线程进入临界区
* 多余线程会阻塞等待

---

### 3. 特点

1. **控制并发数量**：比互斥锁更灵活
2. **可用于生产者-消费者**：管理缓冲区容量
3. **支持多线程阻塞和唤醒**

---

### 4. 注意事项

* 释放次数不能超过信号量最大值，否则行为未定义
* 信号量适合控制资源池、线程数量等
* 二值信号量可替代互斥锁，但 RAII 封装更安全

---

### 5. 使用场景

* **线程池**：限制同时运行的线程数量
* **资源池管理**：数据库连接、硬件接口等
* **生产者-消费者**：控制缓冲区容量

---

### 一句话总结

> 信号量通过 **计数控制资源访问**，允许有限数量线程并发执行，
> 是多线程资源管理和生产者-消费者模式的常用同步工具。
