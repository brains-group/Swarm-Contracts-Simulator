#include "visualizer.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <common/logger.hpp>
#include <config/config.hpp>
#include <simulator/simulator.hpp>

#include "agent.hpp"
#include "material.hpp"
#include "room.hpp"
#include "target.hpp"

namespace scs::vis {

namespace {

class VisualizerImpl : public Visualizer {
public:
    DELETE_COPY_MOVE(VisualizerImpl);
    DEFAULT_DTOR(VisualizerImpl);
    explicit VisualizerImpl(const config::VisualizerConfig& config)
        : m_config(config)
        , m_simulator(sim::Simulator::create())
        , m_window(sf::VideoMode({m_config.windowWidth(), m_config.windowHeight()}),
                   m_config.windowName())
        , m_room(m_simulator->getRoomCorners())
        , m_target(m_simulator->getTargetCorners()) {
        m_window.setFramerateLimit(m_config.framerateLimit());
    }

    auto run() -> Result<void> override {
        while (m_window.isOpen()) {
            while (const std::optional event = m_window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) { m_window.close(); }
            }

            m_simulator->runFrame();

            m_window.clear();
            m_window.draw(m_room);
            m_window.draw(m_target);

            for (const auto& material : m_simulator->getMaterialInfos()) {
                m_material.update(material);
                m_window.draw(m_material);
            }

            for (const auto& agent : m_simulator->getAgentInfos()) {
                m_agent.update(agent);
                m_window.draw(m_agent);
            }

            m_window.display();
        };

        return {};
    }

private:
    const scs::config::VisualizerConfig& m_config;
    std::unique_ptr<scs::sim::Simulator> m_simulator;
    sf::RenderWindow                     m_window;

    Room     m_room;
    Target   m_target;
    Material m_material;
    Agent    m_agent;
};

}    // namespace

auto Visualizer::create() -> std::unique_ptr<Visualizer> {
    return std::make_unique<VisualizerImpl>(scs::config::Config::instance().getVisualizerConfig());
}

}    // namespace scs::vis
