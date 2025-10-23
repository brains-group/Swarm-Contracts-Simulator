#pragma once

#include <SFML/Graphics.hpp>
#include <data/station.hpp>

namespace scs::vis::drawables {

class Station
    : public sf::Drawable
    , public sf::Transformable {
public:
    explicit Station(const sf::Texture& texture)
        : m_sprite(texture) {
        auto size = texture.getSize();
        m_sprite.setScale({2.0F / size.x, 2.0F / size.y});
        m_sprite.setOrigin({size.x / 2.0F, size.y / 2.0F});
    }

    void setType(data::Station::Type type) {
        switch (type) {
            case data::Station::Type::ADD: m_sprite.setRotation(sf::degrees(45)); return;
            case data::Station::Type::MIX: m_sprite.setRotation(sf::Angle::Zero); return;
        }
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform = getTransform();
        target.draw(m_sprite, states);
    }

    sf::Sprite m_sprite;
};

}    // namespace scs::vis::drawables
