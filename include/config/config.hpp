#pragma once

#include <common/macros.hpp>
#include <common/singleton.hpp>
#include <config/simulatorconfig.hpp>
#include <config/visualizerconfig.hpp>

namespace scs::config {

// TODO: Make this all read in from a config file

// NOLINTBEGIN(*magic-numbers*)
class Config
    : public Singleton<Config>
    , public VisualizerConfig
    , public SimulatorConfig {
public:
    DEFAULT_CTOR_DTOR(Config);
    DELETE_COPY_MOVE(Config);

    // Visualizer Config
    [[nodiscard]] auto getVisualizerConfig() const -> const VisualizerConfig& { return *this; }
    [[nodiscard]] auto windowWidth() const -> unsigned int override { return 1920; }
    [[nodiscard]] auto windowHeight() const -> unsigned int override { return 1080; }
    [[nodiscard]] auto windowName() const -> std::string override {
        return "Swarm Contract Simulator Visualization";
    }
    [[nodiscard]] auto framerateLimit() const -> unsigned int override { return 144; }

    // Simulator Config
    [[nodiscard]] auto getSimulatorConfig() const -> const SimulatorConfig& { return *this; }
};
// NOLINTEND(*magic-numbers*)

}    // namespace scs::config
