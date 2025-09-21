#pragma once

#include <SFML/Graphics.hpp>
#include <data/agentinfo.hpp>

namespace scs::vis {

class Agent
    : public sf::Drawable
    , public sf::Transformable {
public:
    Agent() { m_circle.setFillColor(sf::Color::White); }

    auto update(const data::AgentInfo& info) {
        m_circle.setPosition({info.loc.x, info.loc.y});
        m_circle.setRadius(info.size / 2);
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        states.texture = nullptr;

        target.draw(m_circle, states);
    }

    sf::CircleShape m_circle;
};

}    // namespace scs::vis
