#include "arbitrage_detector.h"
#include "../shared/config.h"
#include <iostream>

ArbitrageDetector::ArbitrageDetector(PriceStorage& storage) : storage_(storage) {
    storage_.subscribe([this](Exchange exc, Symbol sym, double price) {
        this->onPriceUpdate(exc, sym, price);
    });
}

void ArbitrageDetector::onPriceUpdate(Exchange exc, Symbol sym, double price) {
    /*
        1. Get the price from the OTHER exchange for the same symbol
        - If the other exchange has no price yet (returns -1.0), bail out — can't compare
        2. Figure out which exchange is cheaper (buy there) and which is more expensive (sell there)
        3. Calculate the gross spread percentage between them
        4. Subtract fees for both exchanges (you buy on one, sell on the other — so you pay fees twice)
        5. If net spread > MIN_SPREAD_THRESHOLD → log the opportunity
    */
    
    Exchange other_exc = (exc == Exchange::Binance) ? Exchange::Coinbase : Exchange::Binance;
    double other_price = storage_.getPrice(other_exc, sym);

    if (other_price < 0) return; // other exc has not reported yet

    double buy_price, sell_price;

    Exchange buy_exc, sell_exc;

    if (price < other_price) {
        buy_exc = exc;          
        sell_exc = other_exc;
        buy_price = price;  
        sell_price = other_price;
    } else {
        buy_exc = other_exc;    
        sell_exc = exc;
        buy_price = other_price;  
        sell_price = price;
    }

    double spread = ((sell_price - buy_price) / buy_price) * 100.0;
    double net_spread = spread - config::getFee(buy_exc) - config::getFee(sell_exc);

    if (net_spread > config::MIN_SPREAD_THRESHOLD) {
        std::cout << "ARBITRAGE: Buy " << to_string(sym) << " on " << to_string(buy_exc)
        << " ($" << buy_price << ") → Sell on " << to_string(sell_exc)
        << " ($" << sell_price << ") | Net spread: " << net_spread << "%"
        << std::endl;
    }
}