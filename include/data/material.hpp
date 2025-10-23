#pragma once

#include <cstdint>
#include <ostream>

namespace scs::data {

struct Material {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    // PRIMARY COLORS
    static const Material Red;
    static const Material Green;
    static const Material Blue;

    // SECONDARY COLORS
    static const Material Cyan;
    static const Material Yellow;
    static const Material Magenta;

    // WHITE & BLACK
    static const Material White;
    static const Material Black;
};

auto mix(const Material& m1, const Material& m2) -> Material;

inline auto operator==(const Material& m1, const Material& m2) {
    return m1.r == m2.r && m1.g == m2.g && m1.b == m2.b;
}

inline auto operator<<(std::ostream& ostr, const Material& m) -> std::ostream& {
    return ostr << "(" << static_cast<uint32_t>(m.r) << ", " << static_cast<uint32_t>(m.g) << ", "
                << static_cast<uint32_t>(m.b) << ")";
}

}    // namespace scs::data
