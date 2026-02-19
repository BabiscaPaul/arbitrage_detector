#pragma once

#include <array>
#include <cstddef>
#include "types.h"

namespace config {

    // Minimum spread (%) required to consider an opportunity profitable
    // Set this higher than total fees to ensure actual profit
    constexpr double MIN_SPREAD_THRESHOLD = 0.1;  // 0.1%

    inline constexpr std::array<double, static_cast<size_t>(Exchange::COUNT)> exchangeFees = {0.1, 0.1};

    inline double getFee(Exchange exchange) {
        return exchangeFees[static_cast<size_t>(exchange)];
    }

}