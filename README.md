# Low-Latency Matching Engine (Under DEvelopment)

A high-performance **order matching engine** designed for low-latency financial trading simulations.  
Optimized for **millions of orders per second** using memory pools, lock-free structures, and efficient queues.

---

## 📂 Project Folder Structure
low_latency_matching_engine/
├── build/                  # Compiled binaries
├── data/                   # Input and output files (trades.csv, order streams)
├── include/                # Header files
│   ├── order.h
│   ├── order_book.h
│   ├── order_pool.h
│   ├── order_queue.h
│   ├── order_node_pool.h
│   ├── skiplist.h
│   ├── trade.h
│   ├── trade_logger.h
│   ├── trade_listener.h
│   └── replay_engine.h
│   └── risk_engine.h
│   └── position_manager.h
│   └── market_simulator.h
│   └── matching_engine.h
│   └── latency_tracker.h
├── src/                    # Source files
│   ├── main.cpp
│   ├── order_book.cpp
│   ├── order.cpp
│   ├── trade_logger.cpp
│   └── replay_engine.cpp
│   └── risk_engine.cpp
│   └── position_manager.cpp
│   └── market_simulator.cpp
│   └── matching_engine.cpp
│   └── latency_tracker.cpp
├── benchmarks/
|      └── latency_test.cpp   # Test cases & simulation scripts
├── tests/
|      └── basic_test.cpp   # Unit Testing (under development)
├── CMakeLists.txt           # Build configuration
└── README.md

⚡ Features
Order Management: Supports buy/sell orders with price-time priority.
Order Book: Efficient book keeping using OrderNodePool and OrderQueue.
Trade Logging: Thread-safe trade logger with CSV output.
Replay Engine: Re-run historical trades from CSV for analysis and backtesting.
Latency Measurement: Tracks P50, P90, P99, and P99.9 latency in nanoseconds.
High-Performance: Memory pools and optimized data structures for low-latency trading simulation.

--- Latency Statistics (ns) ---
P50  : 6100
P90  : 218200
P99  : 808800
P99.9: 1651800
--------------------------------

📊 Latency Measurement
P50 (Median) — Typical latency
P90 — 90th percentile latency
P99 / P99.9 — Worst-case latency for HFT-critical paths

🧰 Tech Stack
C++17 / C++20 — Core engine
STL Containers — Optimized memory and access
Multithreading & Mutex — Thread-safe logging
CSV I/O — Trade logging and replay
CMake — Build system

🔮 Future Enhancements
Real-time order feed integration
Multi-asset support
Advanced risk management
GUI / dashboard for trade visualization
Networked matching engine with multiple clients

Matching Engine Architecture
Incoming Order
      ↓
Check opposite book
      ↓
While (prices cross AND qty > 0)
      ↓
Generate trades
      ↓
Update quantities
      ↓
Remove filled orders
      ↓

Store remainder (if any)
