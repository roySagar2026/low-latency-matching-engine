#include "trade_logger.h"

TradeLogger::TradeLogger(const std::string& filename, bool enable_file)
    : enable_file_logging(enable_file)
{
    if(enable_file_logging){
        csv.open(filename, std::ios::out);
        if(csv.is_open()){
            csv << "trade_id,buy_id,sell_id,price,qty,timestamp\n";
        }
    }
}

TradeLogger::~TradeLogger() {
    if (enable_file_logging && csv.is_open()) {
        csv.close();
    }
}

void TradeLogger::log_trade(const Trade& trade) {
    std::lock_guard<std::mutex> lock(mtx);

    trades.push_back(trade);

    if(enable_file_logging && csv.is_open()){
        csv << trade.trade_id << ","
            << trade.buy_order_id << ","
            << trade.sell_order_id << ","
            << trade.price << ","
            << trade.quantity << ","
            << trade.timestamp << "\n";

        csv.flush();  // ensure write
    }
}

const std::vector<Trade>& TradeLogger::get_trades() const {
    return trades;
}
