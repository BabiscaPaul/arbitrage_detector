cmake_minimum_required(VERSION 3.15)
project(ArbitrageDetector)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)

# Help CMake find Homebrew's OpenSSL on macOS
set(OPENSSL_ROOT_DIR /opt/homebrew/opt/openssl@3)
find_package(OpenSSL REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${Boost_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/main.cpp
    src/storage/price_storage.cpp
    src/fetcher/binance_fetcher.cpp
)

# Create executable
add_executable(arbitrage ${SOURCES})

# Link Boost and OpenSSL libraries
target_link_libraries(arbitrage 
    Boost::boost
    nlohmann_json::nlohmann_json
    OpenSSL::SSL
    OpenSSL::Crypto
)
