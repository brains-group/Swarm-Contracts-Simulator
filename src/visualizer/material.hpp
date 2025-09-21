#pragma once

#include <SFML/Graphics.hpp>
#include <data/materialinfo.hpp>

namespace scs::vis {

class Material
    : public sf::Drawable
    , public sf::Transformable {
public:
    explicit Material() {
        m_rect.setOutlineThickness(5);
        m_rect.setOutlineColor(sf::Color::White);
    }

    auto update(const data::MaterialInfo& info) -> void {
        m_rect.setPosition({info.loc.x, info.loc.y});
        m_rect.setSize({info.size, info.size});
        m_rect.setFillColor({info.color.r, info.color.g, info.color.b});
    }

private:
    auto draw(sf::RenderTarget& target, sf::RenderStates states) const -> void override {
        states.transform *= getTransform();
        states.texture = nullptr;

        target.draw(m_rect, states);
    }

    sf::RectangleShape m_rect;
};

}    // namespace scs::vis
