#pragma once

#include <cstdint>

namespace scs::data {

struct Material {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    static const Material Red;
    static const Material Green;
    static const Material Blue;
};

auto mix(const Material& m1, const Material& m2) -> Material;

inline auto operator==(const Material& m1, const Material& m2) {
    return m1.r == m2.r && m1.g == m2.g && m1.b == m2.b;
}

}    // namespace scs::data
