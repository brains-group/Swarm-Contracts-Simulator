#pragma once

#include <memory>

#include <common/macros.hpp>
#include <data/agentinfo.hpp>
#include <data/vector.hpp>
#include <simulator/simulator.hpp>

namespace scs::agents {

class EnvironmentInterface {
public:
    INTERFACE_CTOR_DTORS(EnvironmentInterface);

    static auto create(sim::Simulator& sim, uint64_t agentID)
        -> std::unique_ptr<EnvironmentInterface>;

    [[nodiscard]] virtual auto getLocation() const -> const data::Point&      = 0;
    [[nodiscard]] virtual auto canMove(const data::Vector& vec) const -> bool = 0;
};

}    // namespace scs::agents
