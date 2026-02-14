#include "order_book.h"
#include <chrono>

static inline uint64_t now_ns(){
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

static uint64_t trade_id_counter = 1;

OrderBook::OrderBook(TradeLogger& logger)
    : trade_logger(logger),
      order_pool(1'500'000),
      node_pool(1'500'000)
{}

Order* OrderBook::allocate_order(){
    return order_pool.allocate();
}

void OrderBook::add_trade_listener(TradeListener* listener){
    listeners.push_back(listener);
}

void OrderBook::notify_trade(const Trade& trade){
    for(auto* l : listeners)
        l->on_trade(trade);
}

std::vector<Trade> OrderBook::process_order(Order* incoming) {
    std::vector<Trade> trades;
    trades.reserve(4);

    auto match_order = [&](PriceLevel* &level, bool is_buy) {
        std::mutex& mtx = is_buy ? ask_mutex : bid_mutex;
        std::lock_guard<std::mutex> lock(mtx); // lock opposite book

        while (incoming->quantity > 0 && level) {
            if (incoming->type == OrderType::LIMIT) {
                if (is_buy && level->price > incoming->price) break;
                if (!is_buy && level->price < incoming->price) break;
            }

            OrderNode* node = level->head;
            while (node && incoming->quantity > 0) {
                Order* resting = node->order;
                int trade_qty = std::min(incoming->quantity, resting->quantity);

                Trade trade{
                    trade_id_counter++,
                    is_buy ? incoming->order_id : resting->order_id,
                    is_buy ? resting->order_id : incoming->order_id,
                    level->price,
                    trade_qty,
                    now_ns()
                };

                trades.emplace_back(trade);
                trade_logger.log_trade(trade);
                notify_trade(trade);

                incoming->quantity -= trade_qty;
                resting->quantity -= trade_qty;

                OrderNode* next_node = node->next;
                if (resting->quantity == 0) {
                    level->remove_order(node);
                    order_id_map.erase(resting->order_id);
                }
                node = next_node;
            }

            if (level->empty()) {
                if (is_buy) asks.remove(*level);
                else bids.remove(*level);
            }

            level = is_buy ? asks.best() : bids.best();
        }
    };

    if (incoming->side == Side::BUY) {
        PriceLevel* best_level = asks.best();
        match_order(best_level, true);
    } else {
        PriceLevel* best_level = bids.best();
        match_order(best_level, false);
    }

    // Insert remaining LIMIT orders
    if (incoming->quantity > 0 && incoming->type == OrderType::LIMIT) {
        std::mutex& mtx = (incoming->side == Side::BUY) ? bid_mutex : ask_mutex;
        std::lock_guard<std::mutex> lock(mtx);

        PriceLevel key{incoming->price};
        PriceLevel* level = (incoming->side == Side::BUY) ? bids.search(key) : asks.search(key);
        if (!level) {
            if (incoming->side == Side::BUY) bids.insert(key);
            else asks.insert(key);
            level = (incoming->side == Side::BUY) ? bids.search(key) : asks.search(key);
        }

        OrderNode* new_node = node_pool.allocate();
        new_node->order = incoming;
        level->add_order(new_node);
        order_id_map[incoming->order_id] = {incoming, new_node, level};
    }

    return trades;
}

bool OrderBook::cancel_order(uint64_t order_id) {

    auto it = order_id_map.find(order_id);
    if (it == order_id_map.end())
        return false;

    OrderLocator& locator = it->second;
    Order* order = locator.order;
    OrderNode* node = locator.node;
    PriceLevel* level = locator.level;

    // Remove node from price level
    level->remove_order(node);
    order_id_map.erase(it);

    // Remove level if empty
    if (level->empty()) {
        if (order->side == Side::BUY) bids.remove(*level);
        else asks.remove(*level);
    }

    return true;
}

bool OrderBook::modify_order(uint64_t order_id, int new_qty, int new_price) {

    auto it = order_id_map.find(order_id);
    if (it == order_id_map.end())
        return false;

    OrderLocator locator = it->second;
    Order* order = locator.order;

    // Cancel current order first
    if (!cancel_order(order_id))
        return false;

    // Update order
    order->quantity = new_qty;
    order->price = new_price;
    order->timestamp = now_ns();
    order->type = OrderType::LIMIT;  // force LIMIT on modify

    // Reprocess order
    process_order(order);
    return true;
}

int OrderBook::best_bid() const {

    auto& mutable_bids =
        const_cast<SkipList<PriceLevel, Descending>&>(bids);

    PriceLevel* level = mutable_bids.best();
    return level ? level->price : 0;
}

int OrderBook::best_ask() const {

    auto& mutable_asks =
        const_cast<SkipList<PriceLevel, Ascending>&>(asks);

    PriceLevel* level = mutable_asks.best();
    return level ? level->price : 0;
}

std::vector<std::pair<int,int>> OrderBook::top_n_bids(int n) {
    std::vector<std::pair<int,int>> result;
    result.reserve(n);

    std::lock_guard<std::mutex> lock(bid_mutex);
    PriceLevel* level = bids.best();
    while(level && result.size() < (size_t)n) {
        int total_qty = 0;
        for(OrderNode* node = level->head; node; node = node->next)
            total_qty += node->order->quantity;

        result.emplace_back(level->price, total_qty);

        level = level->next_level; // Traverse descending skiplist
    }
    return result;
}

std::vector<std::pair<int,int>> OrderBook::top_n_asks(int n) {
    std::vector<std::pair<int,int>> result;
    result.reserve(n);

    std::lock_guard<std::mutex> lock(ask_mutex);
    PriceLevel* level = asks.best();
    while(level && result.size() < (size_t)n) {
        int total_qty = 0;
        for(OrderNode* node = level->head; node; node = node->next)
            total_qty += node->order->quantity;

        result.emplace_back(level->price, total_qty);

        level = level->next_level; // Traverse ascending skiplist
    }
    return result;
}
