#pragma once

#include <cstdint>

namespace scs::data {

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

inline auto operator==(const Color& c1, const Color& c2) -> bool {
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

}    // namespace scs::data
