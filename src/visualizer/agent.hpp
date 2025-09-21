#pragma once

#include <SFML/Graphics.hpp>
#include <data/point.hpp>

namespace scs::vis {

class Agent
    : public sf::Drawable
    , public sf::Transformable {
public:
    explicit Agent() { m_circle.setFillColor(sf::Color::White); }

    auto setSize(float size) { m_circle.setRadius(size / 2); }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        states.texture = nullptr;

        target.draw(m_circle, states);
    }

    sf::CircleShape m_circle;
};

}    // namespace scs::vis
