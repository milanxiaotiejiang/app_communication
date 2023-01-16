//
// Created by Looper on 2022/11/14.
//

#ifndef APP_COMMUNICATION_ASYNC_DEFER_H
#define APP_COMMUNICATION_ASYNC_DEFER_H

#include "functional"

/**
 * 我们都希望在退出前执行某些资源等回收
 * 我们只需要在 ASYNC_DEFER 后使用大括号将要执行的语句包围起来，以分号结束即可。
 * RAII机制确保语句块将在作用域失效前执行。
 *
 * 参考 golang 中 defer
 * golang 中更常用的是关闭句柄、释放资源等操作，但C++一般将资源管理交给了RAII，所以DEFER中一般不需要这些操作。
 */
namespace async {
    namespace {
        class ExecuteOnScopeExit {
        public:
            ExecuteOnScopeExit() = default;

            // movable
            ExecuteOnScopeExit(ExecuteOnScopeExit &&) = default;

            ExecuteOnScopeExit &operator=(ExecuteOnScopeExit &&) = default;

            // non copyable
            ExecuteOnScopeExit(const ExecuteOnScopeExit &e) = delete;

            void operator=(const ExecuteOnScopeExit &f) = delete;

            template<typename F, typename... Args>
            ExecuteOnScopeExit(F &&f, Args &&... args) {
                func_ = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            }

            ~ExecuteOnScopeExit() noexcept {
                if (func_) func_();
            }

        private:
            std::function<void()> func_;
        };
    }

#define _CONCAT(a, b) a##b
#define _MAKE_DEFER_(line) async::ExecuteOnScopeExit _CONCAT(defer, line) = [&]()

#undef ASYNC_DEFER
#define ASYNC_DEFER _MAKE_DEFER_(__LINE__)

}

#endif //APP_COMMUNICATION_ASYNC_DEFER_H
