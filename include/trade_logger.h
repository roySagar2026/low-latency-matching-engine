#pragma once
#include <vector>
#include <fstream>
#include <mutex>
#include <string>
#include "trade.h"

class TradeLogger {
public:
    explicit TradeLogger(const std::string& file = "../data/trades.csv",
                         bool enable_file_logging = false);

    ~TradeLogger();

    void log_trade(const Trade& t);
    const std::vector<Trade>& get_trades() const;

private:
    std::mutex mtx;
    std::vector<Trade> trades;
    std::ofstream csv;
    bool enable_file_logging;   // 🔥 YOU FORGOT THIS
};
