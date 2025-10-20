#pragma once

#include <SFML/Graphics.hpp>
#include <data/point.hpp>

namespace scs::vis::drawables {

class MaterialStore
    : public sf::Drawable
    , public sf::Transformable {
public:
    MaterialStore()
        : m_containerRect({1, 1})
        , m_materialRect({0.8, 0.8}) {}
    void setColor(sf::Color color) {
        m_containerRect.setFillColor(sf::Color(183, 183, 183));
        m_materialRect.setFillColor(color);
        m_materialRect.setPosition({0.1, 0.1});
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform = getTransform();

        target.draw(m_containerRect, states);
        target.draw(m_materialRect, states);
    }

    sf::RectangleShape m_containerRect;
    sf::RectangleShape m_materialRect;
};

}    // namespace scs::vis::drawables
