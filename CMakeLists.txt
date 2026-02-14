cmake_minimum_required(VERSION 3.16)
project(low_latency_matching_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------
# Core engine sources
# -----------------------------
set(ENGINE_SOURCES
    src/order.cpp
    src/order_book.cpp
    src/matching_engine.cpp
    src/trade_logger.cpp
    src/position_manager.cpp
    src/replay_engine.cpp
    src/risk_engine.cpp
    src/market_simulator.cpp
    src/latency_tracker.cpp
)

# -----------------------------
# Main engine executable
# -----------------------------
add_executable(engine
    src/main.cpp
    ${ENGINE_SOURCES}
)

# -----------------------------
# Latency benchmark executable
# -----------------------------
add_executable(latency_test
    benchmarks/latency_test.cpp
    ${ENGINE_SOURCES}
)

# 🔑 INCLUDE PATH (THIS IS THE FIX)
target_include_directories(engine PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(latency_test PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
