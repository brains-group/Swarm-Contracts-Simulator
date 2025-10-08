#pragma once

#include <common/macros.hpp>
#include <common/singleton.hpp>
#include <config/simulatorconfig.hpp>
#include <config/visualizerconfig.hpp>
#include <data/agent.hpp>

namespace scs::config {

// TODO: Make this all read in from a config file

class Config
    : public Singleton<Config>
    , public VisualizerConfig
    , public SimulatorConfig {
public:
    DEFAULT_CTOR_DTOR(Config);
    DELETE_COPY_MOVE(Config);

    // Visualizer Config
    [[nodiscard]] auto windowWidth() const -> unsigned int override { return m_windowWidth; }
    [[nodiscard]] auto windowHeight() const -> unsigned int override { return m_windowHeight; }
    [[nodiscard]] auto windowName() const -> std::string override { return m_windowName; }
    [[nodiscard]] auto framerateLimit() const -> unsigned int override { return m_framerateLimit; }

    // Simulator Config
    [[nodiscard]] auto roomCorners() const -> const std::vector<data::Point>& override {
        return m_roomCorners;
    }
    [[nodiscard]] auto targetCorners() const -> const std::vector<data::Point>& override {
        return m_targetCorners;
    }
    [[nodiscard]] auto initialAgents() const -> const std::vector<data::Agent>& override {
        return m_agents;
    };

private:
    // Visualizer Config
    const unsigned int m_windowWidth    = 1920;
    const unsigned int m_windowHeight   = 1080;
    const std::string  m_windowName     = "Swarm Contract Simulator Visualization";
    const unsigned int m_framerateLimit = 144;

    // Simulator Config
    const std::vector<data::Point> m_roomCorners = {
        {.x = 100, .y = 100},
        {.x = 100, .y = 900},
        {.x = 900, .y = 900},
        {.x = 900, .y = 100},
        {.x = 100, .y = 100},
    };
    const std::vector<data::Point> m_targetCorners = {
        {.x = 100, .y = 100},
        {.x = 100, .y = 200},
        {.x = 200, .y = 200},
        {.x = 200, .y = 100},
        {.x = 100, .y = 100},
    };
    const std::vector<data::Agent> m_agents = {
        data::Agent(
            std::make_shared<data::Transform>(data::Point(100, 100), data::Angle::Zero, 50)),
        data::Agent(
            std::make_shared<data::Transform>(data::Point(200, 200), data::degrees(30), 60)),
        data::Agent(
            std::make_shared<data::Transform>(data::Point(300, 300), data::degrees(60), 70)),
        data::Agent(
            std::make_shared<data::Transform>(data::Point(400, 400), data::degrees(90), 80)),
    };
};

}    // namespace scs::config
