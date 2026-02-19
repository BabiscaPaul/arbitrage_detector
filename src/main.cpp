#include <chrono>
#include <thread>
#include <functional>
#include "storage/price_storage.h"
#include "fetcher/fetcher.h"
#include "shared/types.h"
#include "detector/arbitrage_detector.h"

int main() {
    PriceStorage storage;
    ArbitrageDetector arbitrageDetector(storage);
    
    storage.subscribe(
        std::bind(&ArbitrageDetector::onPriceUpdate, &arbitrageDetector,
                            std::placeholders::_1,
                            std::placeholders::_2,
                            std::placeholders::_3)
    );
    
    BinanceFetcher binance_fetcher(storage, Symbol::BTCUSDT);
    CoinbaseFetcher coinbase_fetcher(storage, Symbol::BTCUSDT);  
    
    binance_fetcher.start();
    coinbase_fetcher.start(); 
    
    std::this_thread::sleep_for(std::chrono::seconds(60));
    
    binance_fetcher.stop();
    coinbase_fetcher.stop();  
    
    return 0;
}