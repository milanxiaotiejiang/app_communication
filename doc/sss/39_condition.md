在 C++ 中，**条件变量（`std::condition_variable`）** 是一种线程同步机制，用于线程间的**等待与通知**，通常与互斥锁 (`std::mutex`) 配合使用，实现线程间的协调。

---

### 1. 基本概念

* 线程可以 **等待某个条件成立**
* 当条件满足时，另一个线程 **通知等待线程继续执行**
* 避免忙等待，提高效率

---

### 2. 基本用法

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return ready; }); // 等待条件成立
    std::cout << "Worker is running\n";
}

void notifier() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true; // 修改条件
    }
    cv.notify_one(); // 唤醒一个等待线程
}

int main() {
    std::thread t1(worker);
    std::thread t2(notifier);
    t1.join();
    t2.join();
}
```

* `wait()` 会自动释放互斥锁，线程阻塞等待
* 条件满足后，`wait` 返回并重新获取锁
* `notify_one()` 唤醒一个线程，`notify_all()` 唤醒所有等待线程

---

### 3. 特点

1. **线程阻塞等待**，效率高
2. **必须与互斥锁配合**，保证条件检查和修改的原子性
3. **避免忙等待**，适合生产者-消费者模式

---

### 4. 注意事项

* 条件变量 **不存储条件**，必须结合标志变量使用
* 唤醒后应重新检查条件（防止虚假唤醒）
* 尽量缩小锁作用域，减少阻塞

---

### 5. 使用场景

* **生产者-消费者队列**
* **多线程事件等待**
* **机器人传感器数据同步**
* **异步任务协调**

---

### 一句话总结

> 条件变量配合互斥锁，实现线程间 **高效等待与通知机制**，
> 避免忙等待，广泛用于生产者-消费者和事件驱动的多线程同步场景。
