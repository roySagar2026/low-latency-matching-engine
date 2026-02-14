#pragma once
#include <cstdint>

enum class Side : uint8_t {
    BUY,
    SELL
};

// Added OrderType
enum class OrderType : uint8_t {
    LIMIT,
    MARKET
};

// Use small, packed struct for HFT
struct Order {
    uint64_t order_id;
    int trader_id;
    int price;              // Price in integer units
    int quantity;
    Side side;
    OrderType type;         // LIMIT or MARKET
    uint64_t timestamp;
};
