#pragma once

#include <common/macros.hpp>
#include <memory>
#include <simulator/agent.hpp>
#include <simulator/config.hpp>
#include <vector>

#include "simulator/materialstore.hpp"

namespace sim {

class Simulator {
public:
    INTERFACE_CTOR_DTORS(Simulator);

    static auto create(std::shared_ptr<const sim::Config> config) -> std::unique_ptr<Simulator>;

    virtual auto runFrame() -> void = 0;

    [[nodiscard]] virtual auto getConfig() const -> const Config&                             = 0;
    [[nodiscard]] virtual auto getAgents() const -> const std::vector<Agent>&                 = 0;
    [[nodiscard]] virtual auto getMaterialStores() const -> const std::vector<MaterialStore>& = 0;
    [[nodiscard]] virtual auto getTargetPosition() const -> Point                             = 0;
};

}    // namespace sim
