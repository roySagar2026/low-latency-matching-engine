#pragma once
#include <vector>
#include <mutex>
#include <unordered_map>
#include <functional>
#include "order.h"
#include "trade.h"
#include "trade_logger.h"
#include "trade_listener.h"
#include "skiplist.h"
#include "order_pool.h"
#include "order_node_pool.h"

struct PriceLevel {
    int price;
    OrderNode* head = nullptr;
    OrderNode* tail = nullptr;
    PriceLevel* next_level = nullptr; // points to next price level in skiplist

    void add_order(OrderNode* node) {
        if (!tail) {
            head = tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
    }

    void remove_order(OrderNode* node) {
        if (node->prev) node->prev->next = node->next;
        else head = node->next;

        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;
    }

    bool empty() const {
        return head == nullptr;
    }
};

struct Ascending {
    bool operator()(const PriceLevel& a, const PriceLevel& b) const {
        return a.price < b.price;
    }
};

struct Descending {
    bool operator()(const PriceLevel& a, const PriceLevel& b) const {
        return a.price > b.price;
    }
};

struct OrderLocator {
    Order* order;
    OrderNode* node;
    PriceLevel* level;
};

class OrderBook {
public:
    explicit OrderBook(TradeLogger& logger);

    Order* allocate_order();

    void add_trade_listener(TradeListener* listener);

    std::vector<Trade> process_order(Order* incoming);

    bool cancel_order(uint64_t order_id);
    bool modify_order(uint64_t order_id, int new_qty, int new_price);

    int best_bid() const;
    int best_ask() const;

    std::vector<std::pair<int,int>> top_n_bids(int n);
    std::vector<std::pair<int,int>> top_n_asks(int n);

private:
    std::mutex bid_mutex;
    std::mutex ask_mutex;
    
    OrderPool order_pool;
    OrderNodePool node_pool;
    
    TradeLogger& trade_logger;
    std::vector<TradeListener*> listeners;

    //Skiplist instead of vector
    SkipList<PriceLevel, Descending> bids;
    SkipList<PriceLevel, Ascending> asks;

    std::unordered_map<uint64_t, OrderLocator> order_id_map;
    void notify_trade(const Trade& trade);
};