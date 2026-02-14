#include "position_manager.h"

void PositionManager::on_trade(const Trade& t){
    //BUY Order
    {
        auto& pos = positions[t.buy_order_id];
        int qty = t.quantity;
        double price = t.price;

        if(pos.net_qty < 0){
            int close_qty = std::min(-pos.net_qty, qty);
            pos.realized_pnl += (pos.avg_price - price) * close_qty;
            pos.net_qty += close_qty;
            qty -= close_qty;
        }

        if (qty > 0) {
            double cost = pos.avg_price * pos.net_qty + price * qty;
            pos.net_qty += qty;
            pos.avg_price = cost / pos.net_qty;
        }
    }

    //SELL Order
    {
        auto& pos = positions[t.sell_order_id];
        int qty = t.quantity;
        double price = t.price;

        if (pos.net_qty > 0) {
            int close_qty = std::min(pos.net_qty, qty);
            pos.realized_pnl += (price - pos.avg_price) * close_qty;
            pos.net_qty -= close_qty;
            qty -= close_qty;
        }

        if (qty > 0) {
            double cost = pos.avg_price * (-pos.net_qty) + price * qty;
            pos.net_qty -= qty;
            pos.avg_price = cost / (-pos.net_qty);
        }
    }
}

void PositionManager::print_positions() const{
    std::cout<<"\n---  Positions ---\n";
    for(const auto& [id, pos] : positions){
        std::cout << "Trader " << id
                  << " | Qty " << pos.net_qty
                  << " | AvgPx " << pos.avg_price
                  << " | RealizedPnL " << pos.realized_pnl
                  << "\n";
    }
}

double PositionManager::tot_realized_pnl() const {
    double total = 0.0;
    for (const auto& [_, pos] : positions){
        total += pos.realized_pnl;
    }
    return total;
}