#pragma once
#include "order_book.h"
#include <vector>
#include <stdexcept>

struct OrderNode {
    Order* order;
    OrderNode* next;
    OrderNode* prev;
};

class OrderNodePool {
private:
    std::vector<OrderNode> pool;
    size_t next_free = 0;

public:
    explicit OrderNodePool(size_t capacity = 1'500'000) {
        pool.resize(capacity);
    }

    OrderNode* allocate() {
        if (next_free >= pool.size())
            throw std::runtime_error("OrderNodePool exhausted");

        OrderNode* node = &pool[next_free++];
        node->next = nullptr;
        node->prev = nullptr;
        return node;
    }

    void reset() {
        next_free = 0;
    }
};
