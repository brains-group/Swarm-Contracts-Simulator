#pragma once

#include <SFML/Graphics.hpp>
#include <common/logger.hpp>
#include <common/macros.hpp>
#include <config/config.hpp>
#include <config/visualizerconfig.hpp>
#include <simulator/simulator.hpp>

#include "drawables/agent.hpp"
#include "drawables/materialstore.hpp"
#include "drawables/room.hpp"
#include "drawables/target.hpp"
#include "texturemanager.hpp"

namespace scs::vis {

class Visualizer {
public:
    explicit Visualizer(config::VisualizerConfig& config = config::Config::instance())
        : m_config(config)
        , m_simulator(config::Config::instance())
        , m_window(sf::VideoMode({m_config.windowWidth(), m_config.windowHeight()}),
                   m_config.windowName())
        , m_assetManager("assets")
        , m_room(m_simulator.getRoomRocners())
        , m_target(m_simulator.getTargetCorners())
        , m_agent(m_assetManager.getTexture("textures/agent.png")) {
        m_window.setFramerateLimit(m_config.framerateLimit());
    }
    DELETE_COPY_MOVE(Visualizer);
    DEFAULT_DTOR(Visualizer);

    void run() {
        while (m_window.isOpen()) {
            m_simulator.runFrame();

            m_window.clear();

            m_window.draw(m_room);
            m_window.draw(m_target);
            for (const data::MaterialStore& mat : m_simulator.getMaterialStores()) {
                m_materialStore.setPosition({mat.space.loc.x, mat.space.loc.y});
                m_materialStore.setScale({mat.space.size.x, mat.space.size.y});
                m_materialStore.setColor({mat.material.r, mat.material.g, mat.material.b});
                m_window.draw(m_materialStore);
            }

            for (const agents::Agent& agent : m_simulator.getAgents()) {
                if (!agent.hasTransform()) { continue; }
                const data::Transform& trans = agent.getTransform();

                m_agent.setPosition({trans.loc.x, trans.loc.y});
                m_agent.setRotation(sf::radians(trans.rot.asRadians()));
                m_agent.setScale({trans.size, trans.size});
                if (agent.hasPart()) {
                    m_agent.setPart(agent.getPart());
                } else {
                    m_agent.setPart({});
                }
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

    scs::vis::AssetManager m_assetManager;

    drawables::Room          m_room;
    drawables::Target        m_target;
    drawables::MaterialStore m_materialStore;
    drawables::Agent         m_agent;
};

}    // namespace scs::vis
