#pragma once

#include <common/macros.hpp>
#include <data/agentinfo.hpp>
#include <data/clientinfo.hpp>
#include <data/materialinfo.hpp>
#include <data/point.hpp>
#include <vector>

namespace scs::config {

class SimulatorConfig {
public:
    INTERFACE_CTOR_DTORS(SimulatorConfig);

    [[nodiscard]] virtual auto roomCorners() const -> const std::vector<data::Point>&          = 0;
    [[nodiscard]] virtual auto targetCorners() const -> const std::vector<data::Point>&        = 0;
    [[nodiscard]] virtual auto materialInfos() const -> const std::vector<data::MaterialInfo>& = 0;

    [[nodiscard]] virtual auto initialAgentInfos() const -> const std::vector<data::AgentInfo>& = 0;
    [[nodiscard]] virtual auto initialClientInfos() const
        -> const std::vector<data::ClientInfo>& = 0;
};

}    // namespace scs::config
