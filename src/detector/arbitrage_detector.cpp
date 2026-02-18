#include "arbitrage_detector.h"
#include "../shared/config.h"
#include <functional>
#include <iostream>

ArbitrageDetector::ArbitrageDetector(PriceStorage& storage) : storage_(storage) {
    storage_.subscribe(
        std::bind(&ArbitrageDetector::onPriceUpdate, this,
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3)
    );
}

void ArbitrageDetector::onPriceUpdate(Exchange exc, Symbol sym, Price price) {
    /*
        1. Get the price from the OTHER exchange for the same symbol
        - If the other exchange has no price yet (returns -1.0), bail out — can't compare
        2. Figure out which exchange is cheaper (buy there) and which is more expensive (sell there)
        3. Calculate the gross spread percentage between them
        4. Subtract fees for both exchanges (you buy on one, sell on the other — so you pay fees twice)
        5. If net spread > MIN_SPREAD_THRESHOLD → log the opportunity
    */
    
    Exchange other_exc = (exc == Exchange::Binance) ? Exchange::Coinbase : Exchange::Binance;
    auto other_price {storage_.getPrice(other_exc, sym) };
    if (!other_price.has_value()) return;

    Price buy_price{0}, sell_price{0};

    Exchange buy_exc, sell_exc;

    if (price < *other_price) {
        buy_exc = exc;          
        sell_exc = other_exc;
        buy_price = price;  
        sell_price = *other_price; // other value is optional => deref it
    } else {
        buy_exc = other_exc;    
        sell_exc = exc;
        buy_price = *other_price;  
        sell_price = price;
    }

    double spread = ((sell_price.toDouble() - buy_price.toDouble()) / buy_price.toDouble()) * 100.0;
    double net_spread = spread - config::getFee(buy_exc) - config::getFee(sell_exc);

    if (net_spread > config::MIN_SPREAD_THRESHOLD) {
        std::cout << "ARBITRAGE: Buy " << to_string(sym) << " on " << to_string(buy_exc)
        << " ($" << buy_price.toDouble() << ") → Sell on " << to_string(sell_exc)
        << " ($" << sell_price.toDouble() << ") | Net spread: " << net_spread << "%"
        << std::endl;
    }
}