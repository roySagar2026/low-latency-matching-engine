#pragma once
#include <vector>
#include <mutex>
#include <cstdint>

class LatencyTracker {
public:
    LatencyTracker();

    // Thread-safe record
    void record(uint64_t latency_ns);

    void report() const;

private:
    std::vector<uint64_t> samples;
    mutable std::mutex mutex;  // protects samples for multi-threaded writes
};
