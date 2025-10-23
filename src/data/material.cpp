#include <cmath>

#include <data/material.hpp>

namespace scs::data {

const Material Material::Red   = {.r = 255, .g = 0, .b = 0};
const Material Material::Green = {.r = 0, .g = 255, .b = 0};
const Material Material::Blue  = {.r = 0, .g = 0, .b = 255};

const Material Material::Cyan    = {.r = 0, .g = 255, .b = 255};
const Material Material::Yellow  = {.r = 255, .g = 255, .b = 0};
const Material Material::Magenta = {.r = 255, .g = 0, .b = 255};

const Material Material::White = {.r = 255, .g = 255, .b = 255};
const Material Material::Black = {.r = 0, .g = 0, .b = 0};

auto mix(const Material& m1, const Material& m2) -> Material {
    return {.r = static_cast<uint8_t>(std::min(m1.r + m2.r, 255)),
            .g = static_cast<uint8_t>(std::min(m1.g + m2.g, 255)),
            .b = static_cast<uint8_t>(std::min(m1.b + m2.b, 255))};
}

}    // namespace scs::data
