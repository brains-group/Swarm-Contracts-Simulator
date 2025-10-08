#include <data/material.hpp>

namespace scs::data {

const Material Material::Red   = {.r = 255, .g = 0, .b = 0};
const Material Material::Green = {.r = 0, .g = 255, .b = 0};
const Material Material::Blue  = {.r = 0, .g = 0, .b = 255};

auto mix(const Material& m1, const Material& m2) -> Material {
    return {.r = static_cast<uint8_t>((m1.r + m2.r) / 2),
            .g = static_cast<uint8_t>((m1.g + m2.b) / 2),
            .b = static_cast<uint8_t>((m1.g + m2.b) / 2)};
}

}    // namespace scs::data
