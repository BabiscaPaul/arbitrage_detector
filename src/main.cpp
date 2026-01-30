#include <iostream>
#include <chrono>
#include <thread>
#include "storage/price_storage.h"
#include "fetcher/fetcher.h"
#include "shared/types.h"

int main() {
    std::cout << "Starting Arbitrage Detector..." << std::endl;
    
    PriceStorage storage;
    BinanceFetcher binance_fetcher(storage, Symbol::BTCUSDT);
    
    // Start the fetcher
    binance_fetcher.start();
    
    // Let it run for 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds(10));
    
    // Read the last price from storage
    double price = storage.getPrice(Exchange::Binance, Symbol::BTCUSDT);
    std::cout << "\nLast price from storage: $" << price << std::endl;
    
    // Stop the fetcher
    binance_fetcher.stop();
    
    std::cout << "Arbitrage Detector stopped" << std::endl;
    return 0;
}