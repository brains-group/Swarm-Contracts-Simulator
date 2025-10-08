#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>

namespace scs::vis::drawables {

class Agent
    : public sf::Drawable
    , public sf::Transformable {
public:
    Agent()
        : m_circle(1)
        , m_rect({1, 1}) {
        m_circle.setFillColor(sf::Color::White);
        m_circle.setOrigin({1, 1});
        m_rect.setOrigin({1, 1});
        m_rect.setFillColor(sf::Color::White);
        m_rect.rotate(sf::degrees(45));
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform = getTransform();
        target.draw(m_circle, states);
        target.draw(m_rect, states);
    }

    sf::CircleShape    m_circle;
    sf::RectangleShape m_rect;
};

}    // namespace scs::vis::drawables
