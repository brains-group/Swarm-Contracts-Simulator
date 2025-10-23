#pragma once

#include <vector>

#include <agents/agent.hpp>
#include <common/macros.hpp>
#include <data/materialstore.hpp>
#include <data/station.hpp>

namespace scs::config {

class SimulatorConfig {
public:
    INTERFACE_CTOR_DTORS(SimulatorConfig);

    [[nodiscard]] virtual auto roomCorners() const -> const std::vector<data::Point>&   = 0;
    [[nodiscard]] virtual auto targetCorners() const -> const std::vector<data::Point>& = 0;
    [[nodiscard]] virtual auto stations() const -> const std::vector<data::Station>&    = 0;
    [[nodiscard]] virtual auto materialStores() const
        -> const std::vector<data::MaterialStore>& = 0;
    [[nodiscard]] virtual auto initialAgents() const
        -> const std::vector<std::shared_ptr<agents::Agent>>& = 0;
};

}    // namespace scs::config
