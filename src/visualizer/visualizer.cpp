#include "visualizer.hpp"

#include <SFML/Graphics.hpp>
#include <common/logger.hpp>
#include <config/config.hpp>
#include <simulator/materialstore.hpp>
#include <simulator/simulator.hpp>

namespace {

class VisualizerImpl : public scs::vis::Visualizer {
public:
    DELETE_COPY_MOVE(VisualizerImpl);
    DEFAULT_DTOR(VisualizerImpl);
    explicit VisualizerImpl(const scs::config::VisualizerConfig& config)
        : m_config(config)
        , m_window(sf::VideoMode({m_config.windowWidth(), m_config.windowHeight()}),
                   m_config.windowName())
        , m_simulator(scs::sim::Simulator::create()) {
        m_window.setFramerateLimit(m_config.framerateLimit());
        LOG(INFO) << "Constructed new visualizer";
    }

    auto run() -> Result<void> override {
        LOG(INFO) << "Beginning running of visualizer";
        sf::RectangleShape target({50, 50});
        target.setFillColor(sf::Color::Green);
        target.setPosition(
            {m_simulator->getTargetPosition().x, m_simulator->getTargetPosition().y});

        sf::RectangleShape materialStore({50, 50});
        materialStore.setOutlineThickness(5);
        while (m_window.isOpen()) {
            while (const std::optional<sf::Event> event = m_window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) { m_window.close(); }
            }

            sf::CircleShape entity(10);
            entity.setFillColor(sf::Color::Black);

            m_window.clear(sf::Color::White);

            m_window.draw(target);

            for (const scs::sim::MaterialStore& mat : m_simulator->getMaterialStores()) {
                materialStore.setPosition({mat.getLoc().start.x, mat.getLoc().start.y});
                materialStore.setOutlineColor(
                    {mat.getMaterial().red, mat.getMaterial().green, mat.getMaterial().blue});
                m_window.draw(materialStore);
            }

            m_simulator->runFrame();
            for (const scs::sim::Agent& agent : m_simulator->getAgents()) {
                entity.setPosition({agent.getX(), agent.getY()});
                m_window.draw(entity);
            }

            m_window.display();
        }
        return {};
    }

private:
    const scs::config::VisualizerConfig& m_config;
    sf::RenderWindow                     m_window;
    std::unique_ptr<scs::sim::Simulator> m_simulator;
};

}    // namespace

auto scs::vis::Visualizer::create() -> std::unique_ptr<Visualizer> {
    return std::make_unique<VisualizerImpl>(scs::config::Config::instance().getVisualizerConfig());
}
