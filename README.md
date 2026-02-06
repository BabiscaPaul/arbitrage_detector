# Arbitrage Detector

A real-time cryptocurrency arbitrage detector written in C++17. Streams live prices from multiple exchanges via WebSocket and detects price discrepancies that could be exploited for profit.

## How It Works

1. **Fetchers** connect to exchange WebSocket APIs and stream live trade prices
2. **PriceStorage** stores the latest prices and notifies subscribers on updates
3. **ArbitrageDetector** compares prices across exchanges and flags opportunities where the spread exceeds trading fees

Currently supported exchanges: **Binance** and **Coinbase**.

## Dependencies

- [Boost.Beast](https://www.boost.org/doc/libs/release/libs/beast/) (WebSocket + HTTP)
- [OpenSSL](https://www.openssl.org/) (TLS)
- [nlohmann/json](https://github.com/nlohmann/json) (JSON parsing)
- CMake 3.15+

### macOS (Homebrew)

```sh
brew install boost openssl@3 nlohmann-json cmake
```

## Build

```sh
mkdir -p build && cd build
cmake ..
make
```

## Run

```sh
./arbitrage
```

## License

MIT
