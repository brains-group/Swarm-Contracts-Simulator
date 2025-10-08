#pragma once

#include <SFML/Graphics.hpp>
#include <data/point.hpp>

namespace scs::vis::drawables {

class MaterialStore
    : public sf::Drawable
    , public sf::Transformable {
public:
    MaterialStore()
        : m_rect({1, 1}) {}
    void setColor(sf::Color color) { m_rect.setFillColor(color); }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform = getTransform();

        target.draw(m_rect, states);
    }

    sf::RectangleShape m_rect;
};

}    // namespace scs::vis::drawables
