#include "matching_engine.h"

std::vector<Trade> MatchingEngine::process_order(Order* order){
    return book.process_order(order);
}