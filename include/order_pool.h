#pragma once
#include "order.h"
#include <vector>
#include <stdexcept>

class OrderPool {
private:
    std::vector<Order> pool;
    size_t next_free = 0;

public:
    explicit OrderPool(size_t capacity = 1'500'000) {
        pool.resize(capacity);
    }

    Order* allocate() {
        if (next_free >= pool.size())
            throw std::runtime_error("OrderPool exhausted");

        return &pool[next_free++];
    }

    void reset() {
        next_free = 0;
    }
};
