#pragma once

#include <common/macros.hpp>
#include <common/singleton.hpp>
#include <config/simulatorconfig.hpp>
#include <config/visualizerconfig.hpp>
#include <data/agentinfo.hpp>
#include <data/materialinfo.hpp>

namespace scs::config {

// TODO: Move to anonymous namespace in cpp file
constexpr unsigned int AGENT_SIZE = 50;

// TODO: Make this all read in from a config file

class Config
    : public Singleton<Config>
    , public VisualizerConfig
    , public SimulatorConfig {
public:
    DEFAULT_CTOR_DTOR(Config);
    DELETE_COPY_MOVE(Config);

    // Visualizer Config
    [[nodiscard]] auto getVisualizerConfig() const -> const VisualizerConfig& { return *this; }
    [[nodiscard]] auto windowWidth() const -> unsigned int override { return m_windowWidth; }
    [[nodiscard]] auto windowHeight() const -> unsigned int override { return m_windowHeight; }
    [[nodiscard]] auto windowName() const -> std::string override { return m_windowName; }
    [[nodiscard]] auto framerateLimit() const -> unsigned int override { return m_framerateLimit; }

    // Simulator Config
    [[nodiscard]] auto getSimulatorConfig() const -> const SimulatorConfig& { return *this; }
    [[nodiscard]] auto roomCorners() const -> const std::vector<data::Point>& override {
        return m_roomCorners;
    }
    [[nodiscard]] auto targetCorners() const -> const std::vector<data::Point>& override {
        return m_targetCorners;
    }
    [[nodiscard]] auto materialInfos() const -> const std::vector<data::MaterialInfo>& override {
        return m_materialInfos;
    }
    [[nodiscard]] auto initialAgentInfos() const -> const std::vector<data::AgentInfo>& override {
        return m_initialAgentInfos;
    }

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
    const std::vector<data::MaterialInfo> m_materialInfos{
        {.loc = {.x = 850, .y = 100}, .size = 50, .color = {.r = 255, .g = 0, .b = 0}},
        {.loc = {.x = 850, .y = 850}, .size = 50, .color = {.r = 0, .g = 255, .b = 0}},
        {.loc = {.x = 100, .y = 850}, .size = 50, .color = {.r = 0, .g = 0, .b = 255}},
    };
    const std::vector<data::AgentInfo> m_initialAgentInfos = {
        {.id       = 0,
         .loc      = {.x = 250, .y = 250},
         .size     = AGENT_SIZE,
         .physical = true,
         .type     = data::AgentInfo::WORKER                                                            },
        {.id       = 1,
         .loc      = {.x = 350, .y = 350},
         .size     = AGENT_SIZE,
         .physical = true,
         .type     = data::AgentInfo::WORKER                                                            },
        {.id       = 2,
         .loc      = {.x = 450, .y = 450},
         .size     = AGENT_SIZE,
         .physical = true,
         .type     = data::AgentInfo::WORKER                                                            },
        {.id       = 3,
         .loc      = {.x = 170, .y = 283},
         .size     = AGENT_SIZE,
         .physical = true,
         .type     = data::AgentInfo::WORKER                                                            },
        {.id       = 4,
         .loc      = {.x = 164, .y = 282},
         .size     = AGENT_SIZE,
         .physical = true,
         .type     = data::AgentInfo::WORKER                                                            },
        {.id       = 5,
         .loc      = {.x = 274, .y = 173},
         .size     = AGENT_SIZE,
         .physical = true,
         .type     = data::AgentInfo::WORKER                                                            },
        {.id       = 6,
         .loc      = {.x = 147, .y = 173},
         .size     = AGENT_SIZE,
         .physical = true,
         .type     = data::AgentInfo::WORKER                                                            },
        {.id       = 7,
         .loc      = {.x = 047, .y = 284},
         .size     = AGENT_SIZE,
         .physical = true,
         .type     = data::AgentInfo::WORKER                                                            },
        {.id       = 8,
         .loc      = {.x = 381, .y = 111},
         .size     = AGENT_SIZE,
         .physical = true,
         .type     = data::AgentInfo::WORKER                                                            },
        {.id       = 9,
         .loc      = {.x = 383, .y = 132},
         .size     = AGENT_SIZE,
         .physical = true,
         .type     = data::AgentInfo::WORKER                                                            },
        {     .id = 10,         .loc = {}, .size = 0, .physical = false, .type = data::AgentInfo::CLIENT},
    };
};    // namespace scs::config

}    // namespace scs::config
