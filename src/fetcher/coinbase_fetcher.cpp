#include "fetcher.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

CoinbaseFetcher::CoinbaseFetcher(PriceStorage& storage, Symbol symbol)  
    : storage_(storage), symbol_(symbol), running_(false) {
    }

CoinbaseFetcher::~CoinbaseFetcher() {
    stop();
}

void CoinbaseFetcher::start() {
    running_ = true;
    thread_ = std::thread(&CoinbaseFetcher::run, this);
}

void CoinbaseFetcher::stop() {
    running_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
}

void CoinbaseFetcher::run() {
    try {
        std::cout << "Connecting to Coinbase..." << std::endl;
        
        net::io_context ioc;
        ssl::context ctx(ssl::context::tlsv12_client);
        ctx.set_default_verify_paths();
        
        tcp::resolver resolver(ioc);
        websocket::stream<ssl::stream<tcp::socket>> ws(ioc, ctx);
        
        // resolve host
        auto const results = resolver.resolve("ws-feed.exchange.coinbase.com", "443");
        
        // TCP connect
        auto ep = net::connect(beast::get_lowest_layer(ws), results);
        
        if (!SSL_set_tlsext_host_name(ws.next_layer().native_handle(), 
                                    "ws-feed.exchange.coinbase.com")) {
            throw beast::system_error(
                beast::error_code(static_cast<int>(::ERR_get_error()), 
                                net::error::get_ssl_category()),
                "Failed to set SNI hostname");
        }
        
        // SSL handshake
        ws.next_layer().handshake(ssl::stream_base::client);
        
        // WS handshake
        std::string host = "ws-feed.exchange.coinbase.com";
        ws.handshake(host, "/");
        
        std::cout << "Connected to Coinbase!" << std::endl;
        
        // send subscription message (key difference between binance and coinbase)
        json subscribe_msg = {
            {"type", "subscribe"},
            {"product_ids", {"BTC-USD"}},
            {"channels", {"matches"}}
        };
        ws.write(net::buffer(subscribe_msg.dump()));
        
        beast::flat_buffer buffer;
        
        while (running_) {
            ws.read(buffer);
            
            std::string message = beast::buffers_to_string(buffer.data());
            json j = json::parse(message);
            
            if (j.contains("type") && j["type"] == "match" && j.contains("price")) {
                double price = std::stod(j["price"].get<std::string>());
                Price int_price = Price::fromDouble(price);
                storage_.updatePrice(Exchange::Coinbase, symbol_, int_price);
                std::cout << "Coinbase: BTC = $" << price << std::endl;
            }
            
            buffer.consume(buffer.size());
        }
        
        ws.close(websocket::close_code::normal);
        
    } catch (std::exception const& e) {
        std::cerr << "Coinbase Error: " << e.what() << std::endl;
    }
    
    std::cout << "Coinbase Fetcher stopped" << std::endl;
}