#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <future>
#include <queue>
#include <condition_variable>
#include <chrono>
#include <type_traits>

int v = 1;

void critical_section(int change_v) {
    static std::mutex mtx;
    std::unique_lock<std::mutex> lock(mtx);
    lock.unlock();

    v = change_v;
    std::cout << v << std::endl;

    lock.lock();
    v += 1;
    std::cout << v << std::endl;
}

void testThread() {
    std::thread t1(critical_section, 4), t2(critical_section, 2);
    t1.join();
    t2.join();
}

void testFuture() {
    std::packaged_task<int()> task([](){return 7;});
    std::future<int> result = task.get_future();
    std::thread(std::move(task)).detach();
    std::cout << "waiting...";
    //result.wait();
    std::cout << "done!" << std::endl << "future result is " << result.get() << std::endl;
}

void produceConsumer() {
    std::queue<int> produced_nums;
    std::mutex mtx;
    std::condition_variable cv;
    bool notified = false;

    auto producer = [&]() {
        for (int i = 0;; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(900));
            std::unique_lock<std::mutex> lock(mtx);
            std::cout << "producing " << i << std::endl;
            produced_nums.push(i);
            notified = true;
            cv.notify_all();
        }
    };

    auto consumer = [&]() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            while (!notified)
                cv.wait(lock);
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            lock.lock();
            while (!produced_nums.empty()) {
                std:: cout << std::this_thread::get_id();
                std::cout << " consuming " << produced_nums.front() << std::endl;
                produced_nums.pop();
            }
            notified = false;
        }
    };

    std::thread p(producer);
    std::thread cs[2];
    for (int i = 0; i < 2; ++i)
        cs[i] = std::thread(consumer);
    p.join();
    for (int i = 0; i < 2; ++i)
        cs[i].join();
}

void decl() {
    int (*(*v)[])();
}

int main (int argc, char *argv[])
{
    //testThread();
    //testFuture();
    produceConsumer();
}
