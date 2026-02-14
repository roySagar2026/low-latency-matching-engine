#pragma once
#include <cstdint>

struct Trade {
    uint64_t trade_id;
    uint64_t buy_order_id;
    uint64_t sell_order_id;
    uint64_t price;
    uint64_t quantity;
    uint64_t timestamp;
};

