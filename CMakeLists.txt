cmake_minimum_required(VERSION 3.15)
project(ArbitrageDetector)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files
set(SOURCES
    src/main.cpp
    src/storage/price_storage.cpp
)

# Create executable
add_executable(arbitrage ${SOURCES})

# Link Boost libraries
target_link_libraries(arbitrage ${Boost_LIBRARIES})