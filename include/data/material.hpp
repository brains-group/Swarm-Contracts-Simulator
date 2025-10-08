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

}    // namespace scs::data
