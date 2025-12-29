在 C++ 中，**读写锁（读写互斥锁，shared_mutex / shared_timed_mutex）** 是一种允许**多个线程同时读共享资源，但写操作独占访问**的同步机制。它适合读多写少的场景，提高并发性能。

---

### 1. 类型

* **C++17**：`std::shared_mutex`
* **C++14 / Boost**：`boost::shared_mutex`
* **C++17** 还提供 `std::shared_lock` 用于读锁

---

### 2. 基本用法

```cpp
#include <iostream>
#include <thread>
#include <shared_mutex>
#include <vector>

std::shared_mutex rwlock;
int data = 0;

void reader(int id) {
    std::shared_lock<std::shared_mutex> lock(rwlock); // 读锁
    std::cout << "Reader " << id << ": " << data << std::endl;
}

void writer(int value) {
    std::unique_lock<std::shared_mutex> lock(rwlock); // 写锁
    data = value;
    std::cout << "Writer: " << data << std::endl;
}

int main() {
    std::vector<std::thread> threads;
    threads.emplace_back(writer, 100);
    threads.emplace_back(reader, 1);
    threads.emplace_back(reader, 2);

    for (auto& t : threads) t.join();
}
```

* `std::shared_lock`：共享锁，多个线程可同时持有（读）
* `std::unique_lock`：独占锁，写操作独占

---

### 3. 特点

1. **读操作并行**：多个线程可同时持有读锁
2. **写操作独占**：写锁申请会阻塞其他读写锁
3. **适合读多写少场景**：减少读操作阻塞，提高效率

---

### 4. 注意事项

* 写锁和读锁冲突：写锁需要等待所有读锁释放
* 尽量缩小锁作用域，减少阻塞时间
* 使用 `std::shared_lock` 或 `std::unique_lock` RAII 管理锁更安全

---

### 5. 使用场景

* **共享数据缓存**：多个线程读取数据，少量线程更新
* **机器人传感器数据访问**：读取频繁、写入少
* **数据库或配置管理**：多线程读访问，偶尔更新

---

### 一句话总结

> 读写锁允许 **多个线程同时读取资源而写操作独占**，
> 提高读多写少场景下的并发性能，是多线程同步的重要工具。
