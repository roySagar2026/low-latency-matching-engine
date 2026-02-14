#include "market_simulator.h"
#include <iostream>
#include <vector>

MarketSimulator::MarketSimulator(OrderBook& book, RiskEngine& risk, LatencyTracker& lt, PositionManager& pm)
    : order_book(book),
      risk_engine(risk),
      latency_tracker(lt),
      position_manager(pm),
      rng(42),
      price_dist(95, 105),
      qty_dist(1, 10),
      side_dist(0, 1),
      market_order_dist(0.1) // 10% market orders
{}

void MarketSimulator::run(int num_orders){
    int num_threads = std::thread::hardware_concurrency();
    if(num_threads == 0) num_threads = 4; // fallback

    std::vector<std::thread> threads;
    int orders_per_thread = num_orders / num_threads;

    for(int t = 0; t < num_threads; ++t){
        threads.emplace_back([&, t](){
            for(int i = t * orders_per_thread; i < (t+1) * orders_per_thread; ++i){
                Order* o = order_book.allocate_order();
                o->order_id = i + 1000;
                o->price = price_dist(rng);
                o->quantity = qty_dist(rng);
                o->side = side_dist(rng) == 0 ? Side::BUY : Side::SELL;
                o->type = market_order_dist(rng) ? OrderType::MARKET : OrderType::LIMIT;
                o->timestamp = now_ts();

                uint64_t start = now_ts();

                if(risk_engine.check_order(*o)){
                    std::lock_guard<std::mutex> lock(orderbook_mutex);
                    auto trades = order_book.process_order(o);

                    // **Update PositionManager**
                    for(auto& tr : trades)
                        position_manager.on_trade(tr);
                }

                uint64_t end = now_ts();
                latency_tracker.record(end - start);

                // Print top 5 bids/asks every 10k orders
                if ((i+1) % 10000 == 0 && t == 0){
                    std::lock_guard<std::mutex> lock(orderbook_mutex);
                    auto top_bids = order_book.top_n_bids(5);
                    auto top_asks = order_book.top_n_asks(5);

                    std::cout << "\n--- OrderBook Snapshot @ Order " << (i+1) << " ---\n";
                    std::cout << "Bids:\n";
                    for(auto& [px, qty] : top_bids)
                        std::cout << px << " x " << qty << "  ";
                    std::cout << "\nAsks:\n";
                    for(auto& [px, qty] : top_asks)
                        std::cout << px << " x " << qty << "  ";
                    std::cout << "\n--------------------------------\n";
                }
            }
        });
    }

    for(auto& th : threads) th.join();
}