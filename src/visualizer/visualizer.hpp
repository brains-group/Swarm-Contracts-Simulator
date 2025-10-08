#pragma once

#include <SFML/Graphics.hpp>
#include <common/logger.hpp>
#include <common/macros.hpp>
#include <config/config.hpp>
#include <config/visualizerconfig.hpp>
#include <simulator/simulator.hpp>

#include "drawables/agent.hpp"
#include "drawables/room.hpp"
#include "drawables/target.hpp"

namespace scs::vis {

class Visualizer {
public:
    explicit Visualizer(config::VisualizerConfig& config = config::Config::instance())
        : m_config(config)
        , m_simulator(config::Config::instance())
        , m_window(sf::VideoMode({m_config.windowWidth(), m_config.windowHeight()}),
                   m_config.windowName())
        , m_room(m_simulator.getRoomRocners())
        , m_target(m_simulator.getTargetCorners()) {
        m_window.setFramerateLimit(m_config.framerateLimit());
    }
    DELETE_COPY_MOVE(Visualizer);
    DEFAULT_DTOR(Visualizer);

    void run() {
        while (m_window.isOpen()) {
            m_window.clear();

            m_window.draw(m_room);
            m_window.draw(m_target);

            for (const data::Agent& agent : m_simulator.getAgents()) {
                const data::Transform& trans = agent.getTransform();
                m_agent.setPosition({trans.loc.x, trans.loc.y});
                m_agent.setRotation(sf::radians(trans.rot.asRadians()));
                m_agent.setScale({trans.size, trans.size});
                m_window.draw(m_agent);
            }

            m_window.display();

            while (std::optional<sf::Event> event = m_window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) { m_window.close(); }
            };
        }
    }

private:
    // NOTE: This is why we cannot copy the type, references aren't reassignable.
    // If we want this in the future, use a pointer
    config::VisualizerConfig& m_config;

    sim::Simulator m_simulator;

    sf::RenderWindow m_window;

    drawables::Room   m_room;
    drawables::Target m_target;
    drawables::Agent  m_agent;
};

}    // namespace scs::vis
