#pragma once

#include <memory>
#include <vector>

#include <common/macros.hpp>
#include <simulator/agent.hpp>

#include "simulator/materialstore.hpp"

namespace scs::sim {

class Simulator {
public:
    INTERFACE_CTOR_DTORS(Simulator);

    static auto create() -> std::unique_ptr<Simulator>;

    virtual auto runFrame() -> void = 0;

    [[nodiscard]] virtual auto getAgents() const -> const std::vector<Agent>&                 = 0;
    [[nodiscard]] virtual auto getMaterialStores() const -> const std::vector<MaterialStore>& = 0;
    [[nodiscard]] virtual auto getTargetPosition() const -> Point                             = 0;
};

}    // namespace scs::sim
