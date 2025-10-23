#pragma once

#include <agents/controller.hpp>
#include <common/logger.hpp>
#include <common/macros.hpp>
#include <common/singleton.hpp>
#include <config/simulatorconfig.hpp>
#include <config/visualizerconfig.hpp>
#include <data/material.hpp>
#include <data/station.hpp>

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
    [[nodiscard]] auto assetBasePath() const -> const std::filesystem::path& override {
        return m_assetBasePath;
    }
    [[nodiscard]] auto agentTexturePath() const -> const std::filesystem::path& override {
        return m_agentTexturePath;
    }
    [[nodiscard]] auto stationTexturePath() const -> const std::filesystem::path& override {
        return m_stationTexturePath;
    }

    [[nodiscard]] auto windowWidth() const -> unsigned int override { return m_windowWidth; }
    [[nodiscard]] auto windowHeight() const -> unsigned int override { return m_windowHeight; }
    [[nodiscard]] auto windowName() const -> std::string override { return m_windowName; }
    [[nodiscard]] auto framerateLimit() const -> unsigned int override { return m_framerateLimit; }

    [[nodiscard]] auto getOrderQueueArea() const -> const data::Rect& override {
        return m_orderQueueArea;
    }

    // Simulator Config
    [[nodiscard]] auto roomCorners() const -> const std::vector<data::Point>& override {
        return m_roomCorners;
    }
    [[nodiscard]] auto targetCorners() const -> const std::vector<data::Point>& override {
        return m_targetCorners;
    }
    [[nodiscard]] auto stations() const -> const std::vector<data::Station>& override {
        return m_stations;
    }
    [[nodiscard]] auto materialStores() const -> const std::vector<data::MaterialStore>& override {
        return m_materialStores;
    }
    [[nodiscard]] auto initialAgents() const
        -> const std::vector<std::shared_ptr<agents::Agent>>& override {
        return m_agents;
    };

private:
    // Visualizer Config
    const std::filesystem::path m_assetBasePath      = "assets";
    const std::filesystem::path m_agentTexturePath   = "textures/agent.png";
    const std::filesystem::path m_stationTexturePath = "textures/station.png";

    const unsigned int m_windowWidth    = 1800;
    const unsigned int m_windowHeight   = 900;
    const std::string  m_windowName     = "Swarm Contract Simulator Visualization";
    const unsigned int m_framerateLimit = 144;

    const data::Rect m_orderQueueArea = {
        .loc  = {.x = 1570,  .y = 50},
        .size = { .x = 180, .y = 800},
    };

    // Simulator Config
    const std::vector<data::Point> m_roomCorners = {
        {  .x = 50,  .y = 50},
        {  .x = 50, .y = 850},
        {.x = 1550, .y = 850},
        {.x = 1550,  .y = 50},
        {  .x = 50,  .y = 50},
    };
    const std::vector<data::Point> m_targetCorners = {
        {.x = 1550,  .y = 50},
        {.x = 1450,  .y = 50},
        {.x = 1450, .y = 150},
        {.x = 1550, .y = 150},
        {.x = 1550,  .y = 50},
    };
    const std::vector<data::MaterialStore> m_materialStores = {
        {  .material = data::Material::Red,
         .space =
         {
         .loc  = {.x = 50, .y = 50},
         .size = {.x = 100, .y = 100},
         }},
        {.material = data::Material::Green,
         .space =
         {
         .loc  = {.x = 50, .y = 750},
         .size = {.x = 100, .y = 100},
         }},
        { .material = data::Material::Blue,
         .space =
         {
         .loc  = {.x = 1450, .y = 750},
         .size = {.x = 100, .y = 100},
         }},
    };
    const std::vector<data::Station> m_stations = {
        { .loc = {.x = 500, .y = 450}, .size = 50, .type = data::Station::Type::MIX},
        {.loc = {.x = 1100, .y = 450}, .size = 50, .type = data::Station::Type::ADD},
    };
    const std::vector<std::shared_ptr<agents::Agent>> m_agents = {
        std::make_shared<agents::Agent>(
            agents::Controller::create(agents::Controller::Type::Worker),
            agents::MakeTransform(data::Point(200, 200), data::degrees(90), 30), nullptr,
            agents::MakeBalance(100), nullptr),
        std::make_shared<agents::Agent>(
            agents::Controller::create(agents::Controller::Type::Worker),
            agents::MakeTransform(data::Point(1400, 200), data::degrees(90), 30), nullptr,
            agents::MakeBalance(100), nullptr),
        std::make_shared<agents::Agent>(
            agents::Controller::create(agents::Controller::Type::Worker),
            agents::MakeTransform(data::Point(1400, 700), data::degrees(90), 30), nullptr,
            agents::MakeBalance(100), nullptr),
        std::make_shared<agents::Agent>(
            agents::Controller::create(agents::Controller::Type::Worker),
            agents::MakeTransform(data::Point(200, 700), data::degrees(90), 30), nullptr,
            agents::MakeBalance(100), nullptr),
        std::make_shared<agents::Agent>(
            agents::Controller::create(agents::Controller::Type::Worker),
            agents::MakeTransform(data::Point(800, 450), data::degrees(90), 30), nullptr,
            agents::MakeBalance(100), nullptr),
        std::make_shared<agents::Agent>(
            agents::Controller::create(agents::Controller::Type::Client), nullptr, nullptr,
            agents::MakeBalance(100), nullptr)};
};

}    // namespace scs::config
