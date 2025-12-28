#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
public:
    void push(T item) {
        std::lock_guard<std::mutex> lock(m);
        q.push(std::move(item));
        cv.notify_one();
    }

    bool try_pop(T& out) {
        std::lock_guard<std::mutex> lock(m);
        if (q.empty()) return false;
        out = std::move(q.front());
        q.pop();
        return true;
    }

    void wait_pop(T& out) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&] { return !q.empty(); });
        out = std::move(q.front());
        q.pop();
    }

private:
    std::queue<T> q;
    std::mutex m;
    std::condition_variable cv;
};