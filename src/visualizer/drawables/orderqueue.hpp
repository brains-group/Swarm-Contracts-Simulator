#pragma once

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <contracts/contract.hpp>
#include <data/rect.hpp>

namespace scs::vis::drawables {

class OrderQueue : public sf::Drawable {
public:
    explicit OrderQueue(const data::Rect&                                        area,
                        const std::vector<std::shared_ptr<contracts::Contract>>& contracts)
        : m_outlineRect({area.size.x, area.size.y})
        , m_contracts(contracts)
        , m_materialRect({1, 1})
        , m_completedCover({1.2, 1.2}) {
        m_outlineRect.setPosition({area.loc.x, area.loc.y});
        m_outlineRect.setFillColor({243, 243, 243});
        m_completedCover.setOrigin({0.1, 0.1});
        m_completedCover.setScale({50.0f, 50.0f});
        m_completedCover.setFillColor({0, 255, 0, 100});
    }
    DELETE_COPY_MOVE(OrderQueue);
    DEFAULT_DTOR(OrderQueue);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates /* states */) const override {
        target.draw(m_outlineRect);

        unsigned int position = 0;
        for (const std::shared_ptr<contracts::Contract>& contract :
             m_contracts | std::views::reverse | std::views::take(10)) {
            setPart(contract->getPart());

            // Draw the part
            unsigned int partDims = std::ceil(std::sqrt(m_partColors.size()));
            float        matScale = 50.0F / static_cast<float>(partDims);
            m_materialRect.setScale({matScale, matScale});
            for (unsigned int x = 0; x < partDims; x++) {
                for (unsigned int y = 0; y < partDims; y++) {
                    unsigned int index = (y * partDims) + x;
                    if (index >= m_partColors.size()) { break; }
                    m_materialRect.setFillColor(m_partColors[index]);
                    m_materialRect.setPosition(
                        m_outlineRect.getPosition()
                        + sf::Vector2f((static_cast<float>(x) * matScale)
                                           + (m_outlineRect.getSize().x - 50.0F) / 2,
                                       50 + 75 * position + (static_cast<float>(y) * matScale)));
                    target.draw(m_materialRect);
                }
            }

            if (contract->isComplete()) {
                m_completedCover.setPosition(
                    m_outlineRect.getPosition()
                    + sf::Vector2f((m_outlineRect.getSize().x - 50.0F) / 2, 50 + 75 * position));
                target.draw(m_completedCover);
            }

            position++;
        }
    }

    void setPart(const data::Part& part) const {
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
    }

    sf::RectangleShape                                       m_outlineRect;
    const std::vector<std::shared_ptr<contracts::Contract>>& m_contracts;

    mutable std::vector<sf::Color> m_partColors;
    mutable sf::RectangleShape     m_materialRect;

    mutable sf::RectangleShape m_completedCover;
};

}    // namespace scs::vis::drawables
