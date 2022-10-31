#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool
{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop_{false};
public:
    explicit ThreadPool(unsigned int size = std::thread::hardware_concurrency());
    ~ThreadPool();

    template<class F, class... Args>
    auto add(F &&f, Args... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;
};


template<class F, class... Args>
auto ThreadPool::add(F &&f, Args... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );
    std::future<return_type> ret = task->get_future();
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        if (stop_)
            throw new std::runtime_error("equeue on stopped ThreadPool");
        tasks.emplace([task](){ (*task)(); });
    }
    cv.notify_one();
    return ret;
}
