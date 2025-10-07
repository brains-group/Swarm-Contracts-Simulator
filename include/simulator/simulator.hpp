#pragma once

#include <agents/agent.hpp>

#include "config/config.hpp"
#include "config/simulatorconfig.hpp"

namespace scs::sim {

class Simulator {
public:
    explicit Simulator(config::SimulatorConfig& config = config::Config::instance())
        : m_config(config) {}
    DELETE_COPY_MOVE(Simulator);
    DEFAULT_DTOR(Simulator);

    [[nodiscard]] auto getRoomRocners() const -> const std::vector<data::Point>& {
        return m_config.roomCorners();
    }

    [[nodiscard]] auto getTargetCorners() const -> const std::vector<data::Point>& {
        return m_config.targetCorners();
    }

private:
    // NOTE: This is why we cannot copy the type, references aren't reassignable.
    // If we want this in the future, use a pointer
    config::SimulatorConfig& m_config;
};

}    // namespace scs::sim
