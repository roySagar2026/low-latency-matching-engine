#include "latency_tracker.h"
#include <algorithm>
#include <iostream>
#include <cmath>

LatencyTracker::LatencyTracker() {
    samples.reserve(1'500'000); // pre-allocate to avoid reallocations
}

void LatencyTracker::record(uint64_t latency_ns){
    std::lock_guard<std::mutex> lock(mutex);
    samples.push_back(latency_ns);
}

void LatencyTracker::report() const{
    std::vector<uint64_t> sorted;
    {
        std::lock_guard<std::mutex> lock(mutex);
        if(samples.empty()) return;
        sorted = samples; // copy for sorting
    }

    std::sort(sorted.begin(), sorted.end());

    auto percentile = [&](double p) {
        size_t idx = std::min(static_cast<size_t>(p * sorted.size()), sorted.size() - 1);
        return sorted[idx];
    };

    std::cout << "\n--- Latency Statistics (ns) ---\n";
    std::cout << "P50  : " << percentile(0.50)  << "\n";
    std::cout << "P90  : " << percentile(0.90)  << "\n";
    std::cout << "P99  : " << percentile(0.99)  << "\n";
    std::cout << "P99.9: " << percentile(0.999) << "\n";
    std::cout << "--------------------------------\n";
}
