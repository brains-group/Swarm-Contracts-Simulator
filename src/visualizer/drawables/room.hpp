#pragma once

#include <SFML/Graphics.hpp>
#include <data/point.hpp>

namespace scs::vis::drawables {

class Room : public sf::Drawable {
public:
    explicit Room(const std::vector<data::Point>& corners)
        : m_corners(sf::PrimitiveType::LineStrip, corners.size()) {
        for (unsigned int i = 0; i < corners.size(); i++) {
            m_corners[i].position = sf::Vector2f(corners[i].x, corners[i].y);
        };
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates /* states */) const override {
        target.draw(m_corners);
    }
    sf::VertexArray m_corners;
};

}    // namespace scs::vis::drawables
