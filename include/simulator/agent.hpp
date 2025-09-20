#pragma once

#include <simulator/physics.hpp>

namespace scs::sim {

class Agent {
public:
    explicit Agent(const Point& loc)
        : m_loc(loc) {}

    [[nodiscard]] auto getX() const -> float { return m_loc.x; }
    [[nodiscard]] auto getY() const -> float { return m_loc.y; }

    void runFrame() {
        m_loc.x += 1;
        m_loc.y += 1;
    }

private:
    Point m_loc;
};

}    // namespace scs::sim
