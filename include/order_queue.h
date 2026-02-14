#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "order.h"

class OrderQueue {
public:
    void push(Order&& o) {
        std::lock_guard<std::mutex> lock(mtx);
        orders.push(std::move(o));
        cv.notify_one();
    }

    bool pop(Order& o) {
        std::lock_guard<std::mutex> lock(mtx);
        if (orders.empty()) return false;
        o = std::move(orders.front());
        orders.pop();
        return true;
    }

    void wait_and_pop(Order& o) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{ return !orders.empty(); });
        o = std::move(orders.front());
        orders.pop();
    }

private:
    std::queue<Order> orders;
    std::mutex mtx;
    std::condition_variable cv;
};
