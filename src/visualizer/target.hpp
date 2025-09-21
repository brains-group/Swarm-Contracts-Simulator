#pragma once

#include <SFML/Graphics.hpp>
#include <data/point.hpp>

namespace scs::vis {

class Target
    : public sf::Drawable
    , public sf::Transformable {
public:
    explicit Target(const std::vector<data::Point>& points)
        : m_vertices(sf::PrimitiveType::LineStrip, points.size()) {
        for (unsigned int i = 0; i < points.size(); i++) {
            m_vertices[i].position = sf::Vector2f(points[i].x, points[i].y);
            m_vertices[i].color    = sf::Color::Green;
        }
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();

        states.texture = nullptr;

        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
};

}    // namespace scs::vis
