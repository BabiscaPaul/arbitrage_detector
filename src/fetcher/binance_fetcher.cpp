#include "fetcher.h"
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


BinanceFetcher::BinanceFetcher(PriceStorage& storage, Symbol symbol)
    : storage_(storage), symbol_(symbol), running_(false) {
}

BinanceFetcher::~BinanceFetcher() {
    BinanceFetcher::stop();
}

void BinanceFetcher::start() {
    running_ = true;
    thread_ = std::thread(&BinanceFetcher::run, this);
}

void BinanceFetcher::stop() {
    running_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
}

void BinanceFetcher::run() {
    try {
        std::cout << "Connecting to Binance..." << std::endl;
        
        net::io_context ioc;
        ssl::context ctx(ssl::context::tlsv12_client);
        
        ctx.set_default_verify_paths();
        
        tcp::resolver resolver(ioc);
        websocket::stream<ssl::stream<tcp::socket>> ws(ioc, ctx);
        
        auto const results = resolver.resolve("stream.binance.com", "9443");
        
        // setp 1: connect tcp
        auto ep = net::connect(beast::get_lowest_layer(ws), results);
        
        if (!SSL_set_tlsext_host_name(ws.next_layer().native_handle(), "stream.binance.com")) {
            throw beast::system_error(
                beast::error_code(static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()),
                "Failed to set SNI hostname");
        }
        
        // step 2: ssl handshake 
        ws.next_layer().handshake(ssl::stream_base::client);
        
        std::string host = "stream.binance.com:9443";
        
        // step 3: websocket handshake 
        ws.handshake(host, "/ws/btcusdt@trade");
        
        std::cout << "Connected to Binance!" << std::endl;
        
        beast::flat_buffer buffer;
        
        while (running_) {
            ws.read(buffer);
            
            std::string message = beast::buffers_to_string(buffer.data());
            json j = json::parse(message);
            
            if (j.contains("p")) {
                double price = std::stod(j["p"].get<std::string>());
                Price int_price = Price::fromDouble(price);
                storage_.updatePrice(Exchange::Binance, Symbol::BTCUSDT, int_price);
                std::cout << "Binance: BTC = $" << price << std::endl;
            }
            
            buffer.consume(buffer.size());
        }
        
        ws.close(websocket::close_code::normal);
        
    } catch (boost::system::system_error const& e) {
        if (e.code() != net::ssl::error::stream_truncated) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    std::cout << "Binance Fetcher stopped" << std::endl;
}