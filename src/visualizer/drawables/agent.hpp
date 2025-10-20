#pragma once

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <common/logger.hpp>
#include <data/part.hpp>

namespace scs::vis::drawables {

class Agent
    : public sf::Drawable
    , public sf::Transformable {
public:
    explicit Agent(const sf::Texture& texture)
        : m_sprite(texture)
        , m_materialRect({1, 1}) {
        auto size = texture.getSize();
        m_sprite.setScale({2.0F / size.x, 2.0F / size.y});
        m_sprite.setOrigin({size.x / 2.0F, size.y / 2.0F});
        m_sprite.rotate(sf::degrees(45));
    }

    void setPart(const data::Part& part) {
        unsigned int last_filled = part.size();
        for (unsigned int i = part.size(); i-- > 0;) {
            if (part[i]) {
                last_filled = i;
                break;
            }
        }
        if (last_filled == part.size()) {
            m_partColors.resize(0);
            return;
        }
        m_partColors.resize(last_filled + 1);
        for (unsigned int i = 0; i < m_partColors.size(); i++) {
            const std::optional<data::Material>& mat = part[i];
            m_partColors[i] = mat ? sf::Color(mat->r, mat->g, mat->b) : sf::Color::Transparent;
        }
    };

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform = getTransform();
        target.draw(m_sprite, states);

        unsigned int partDims = std::ceil(std::sqrt(m_partColors.size()));
        float        matScale = 1.0F / static_cast<float>(partDims);
        m_materialRect.setScale({matScale, matScale});
        for (unsigned int x = 0; x < partDims; x++) {
            for (unsigned int y = 0; y < partDims; y++) {
                unsigned int index = (y * partDims) + x;
                if (index >= m_partColors.size()) { break; }
                m_materialRect.setFillColor(m_partColors[index]);
                m_materialRect.setPosition({(static_cast<float>(x) * matScale) - 0.5F,
                                            (static_cast<float>(y) * matScale) - 0.5F});
                target.draw(m_materialRect, states);
            }
        }
    }

    sf::Sprite m_sprite;

    std::vector<sf::Color>     m_partColors;
    mutable sf::RectangleShape m_materialRect;
};

}    // namespace scs::vis::drawables

