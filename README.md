<div align="center">

# ⚡ Low-Latency Matching Engine

### High-Performance Order Matching Engine for HFT & Trading Simulations

<img src="https://img.shields.io/badge/C++-17%20%2F%2020-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" />
<img src="https://img.shields.io/badge/Latency-Nanoseconds-success?style=for-the-badge" />
<img src="https://img.shields.io/badge/HFT-Low%20Latency-orange?style=for-the-badge" />
<img src="https://img.shields.io/badge/Status-Under%20Development-yellow?style=for-the-badge" />

<br/>

A high-performance **order matching engine** designed for  
low-latency financial trading simulations and HFT-style system design.

Optimized for **millions of orders per second** using:
memory pools, efficient queues, low-overhead data structures, and latency-focused architecture.

</div>

---

# ✨ Features

<table>
<tr>
<td width="50%">

### 📈 Trading Engine
- Buy/Sell order management
- Price-time priority matching
- Order book simulation
- Trade generation engine
- Historical trade replay
- Position management

</td>

<td width="50%">

### ⚡ Performance Engineering
- Low-latency architecture
- Memory pool optimization
- Efficient queue structures
- Nanosecond latency tracking
- Thread-safe trade logging
- HFT-inspired system design

</td>
</tr>
</table>

---

# 🏗️ Matching Engine Architecture

<div align="center">

```text
Incoming Order
      │
      ▼
Check Opposite Book
      │
      ▼
While (prices cross AND qty > 0)
      │
      ▼
Generate Trades
      │
      ▼
Update Quantities
      │
      ▼
Remove Filled Orders
      │
      ▼
Store Remaining Quantity
```

</div>

---

# 📂 Project Structure

```bash
low_latency_matching_engine/
│
├── build/
├── data/
│
├── include/
│   ├── order.h
│   ├── order_book.h
│   ├── order_pool.h
│   ├── order_queue.h
│   ├── order_node_pool.h
│   ├── skiplist.h
│   ├── trade.h
│   ├── trade_logger.h
│   ├── trade_listener.h
│   ├── replay_engine.h
│   ├── risk_engine.h
│   ├── position_manager.h
│   ├── market_simulator.h
│   ├── matching_engine.h
│   └── latency_tracker.h
│
├── src/
│   ├── main.cpp
│   ├── order_book.cpp
│   ├── order.cpp
│   ├── trade_logger.cpp
│   ├── replay_engine.cpp
│   ├── risk_engine.cpp
│   ├── position_manager.cpp
│   ├── market_simulator.cpp
│   ├── matching_engine.cpp
│   └── latency_tracker.cpp
│
├── benchmarks/
│   └── latency_test.cpp
│
├── tests/
│   └── basic_test.cpp
│
├── CMakeLists.txt
└── README.md
```

---

# 🚀 Build & Run

## 1️⃣ Create Build Directory

```bash
mkdir build
cd build
```

---

## 2️⃣ Configure Project

```bash
cmake ..
```

---

## 3️⃣ Build Engine

```bash
cmake --build .
```

---

## 4️⃣ Run Application

```bash
./matching_engine
```

---

# ⚡ Core Components

## 📘 Order Management
- Buy/Sell order handling
- FIFO price-time priority
- Partial fill support
- Remaining quantity management

---

## 📚 Order Book
- Optimized order storage
- Efficient lookup and insertion
- Queue-based matching logic
- Memory-efficient node handling

---

## 📄 Trade Logger
- Thread-safe trade logging
- CSV export support
- Historical trade persistence
- Replay-ready trade storage

---

## 🔁 Replay Engine
- Replay historical market activity
- Backtesting support
- Trade stream simulation
- Deterministic execution testing

---

## 📊 Latency Tracker
- Nanosecond-level measurements
- P50 / P90 / P99 statistics
- Critical path latency analysis
- HFT performance benchmarking

---

# 📊 Latency Statistics

<div align="center">

```text
--- Latency Statistics (ns) ---

P50   : 6100
P90   : 218200
P99   : 808800
P99.9 : 1651800

--------------------------------
```

</div>

---

# 📈 Latency Metrics

| Metric | Description |
|--------|-------------|
| **P50** | Median latency |
| **P90** | 90th percentile latency |
| **P99** | Tail latency measurement |
| **P99.9** | Worst-case HFT critical path latency |

---

# 🧰 Tech Stack

<div align="center">

| Technology | Purpose |
|------------|----------|
| C++17 / C++20 | Core Matching Engine |
| STL Containers | Optimized Data Structures |
| Multithreading | Concurrent Processing |
| Mutex & Locks | Thread-Safe Logging |
| CSV I/O | Trade Storage & Replay |
| CMake | Build System |

</div>

---

# ⚙️ Performance Optimizations

- Memory Pool Allocation
- Cache-Friendly Data Structures
- Reduced Dynamic Allocations
- Efficient Queue Traversal
- Low-Overhead Trade Processing
- Latency-Aware System Design

---

# 🛡️ Engine Capabilities

| Capability | Status |
|------------|--------|
| Order Matching | ✅ |
| Trade Logging | ✅ |
| Replay Engine | ✅ |
| Risk Engine | 🚧 Under Development |
| Market Simulator | 🚧 Under Development |
| Multi-Asset Support | 🚧 Planned |
| Networked Engine | 🚧 Planned |

---

# 🔮 Future Enhancements

- 🌐 Real-Time Market Feed Integration
- 📊 GUI Trading Dashboard
- ⚡ Lock-Free Order Queues
- 🧠 Smart Risk Management
- 🏦 Multi-Exchange Simulation
- 📡 Networked Matching Engine
- 📈 Advanced Market Simulation
- 🔥 Kernel-Level Optimizations

---

# 🎯 Project Goals

This project is designed to explore:
- HFT system architecture
- Low-latency programming
- Exchange matching systems
- Performance optimization in C++
- Trading infrastructure engineering
- Real-world financial systems design

---

# ⚠️ Disclaimer

This project is intended for:
- Educational purposes
- Trading system research
- Performance engineering practice
- HFT simulation learning

It is **not intended for real financial trading or production exchange deployment**.

---

<div align="center">

### ⭐ If you like this project, consider giving it a star!

Built with ❤️ using C++ for Low-Latency Systems & HFT Engineering

</div>
