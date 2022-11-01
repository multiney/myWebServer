#pragma once

#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool {
 private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex tasks_mtx_;
    std::condition_variable cv_;
    bool stop_{false};

 public:
    explicit ThreadPool(unsigned int size = std::thread::hardware_concurrency());
    ~ThreadPool();

    template<class F, class... Args>
    auto Add(F &&f, Args... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;
};


template<class F, class... Args>
auto ThreadPool::Add(F &&f, Args... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );
    std::future<return_type> ret = task->get_future();
    {
        std::unique_lock<std::mutex> lock(tasks_mtx_);
        if (stop_)
            throw new std::runtime_error("equeue on stopped ThreadPool");
        tasks_.emplace([task](){ (*task)(); });
    }
    cv_.notify_one();
    return ret;
}

