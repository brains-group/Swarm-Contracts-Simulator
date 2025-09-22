#pragma once

#include <common/macros.hpp>
#include <data/agentinfo.hpp>
#include <memory>
#include <simulator/simulator.hpp>

namespace scs::agents {

class EnvironmentInterface {
public:
    INTERFACE_CTOR_DTORS(EnvironmentInterface);

    static auto create(sim::Simulator& sim, uint64_t agentID)
        -> std::unique_ptr<EnvironmentInterface>;
};

}    // namespace scs::agents
