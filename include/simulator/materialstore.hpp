#pragma once

#include <cstdint>
#include <simulator/physics.hpp>

namespace sim {

struct Material {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class MaterialStore {
public:
    MaterialStore(const Rect& loc, const Material& material)
        : m_loc(loc)
        , m_material(material) {}

    [[nodiscard]] auto getLoc() const -> const Rect& { return m_loc; }
    [[nodiscard]] auto getMaterial() const -> const Material& { return m_material; }

private:
    Rect     m_loc;
    Material m_material;
};

}    // namespace sim
