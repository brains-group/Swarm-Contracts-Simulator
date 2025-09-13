#pragma once

namespace sim {

class Agent {
public:
    Agent(float x, float y)
        : m_x(x)
        , m_y(y) {}

    [[nodiscard]] auto getX() const -> float { return m_x; }
    [[nodiscard]] auto getY() const -> float { return m_y; }

private:
    float m_x;
    float m_y;
};

}    // namespace sim
