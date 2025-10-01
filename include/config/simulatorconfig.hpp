#pragma once

#include <vector>

#include <common/macros.hpp>
#include <data/agentinfo.hpp>
#include <data/materialinfo.hpp>
#include <data/point.hpp>

namespace scs::config {

class SimulatorConfig {
public:
    INTERFACE_CTOR_DTORS(SimulatorConfig);

    [[nodiscard]] virtual auto roomCorners() const -> const std::vector<data::Point>&          = 0;
    [[nodiscard]] virtual auto targetCorners() const -> const std::vector<data::Point>&        = 0;
    [[nodiscard]] virtual auto materialInfos() const -> const std::vector<data::MaterialInfo>& = 0;

    [[nodiscard]] virtual auto initialAgentInfos() const -> const std::vector<data::AgentInfo>& = 0;
};

}    // namespace scs::config
