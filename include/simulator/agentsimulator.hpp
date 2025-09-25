#pragma once

#include <common/macros.hpp>
#include <simulator/simulator.hpp>

#include "data/vector.hpp"

namespace scs::sim {

class AgentSimulator : public Simulator {
public:
    INTERFACE_CTOR_DTORS(AgentSimulator);

    [[nodiscard]] virtual auto moveAgent(uint64_t agentID, const data::Vector& vec) -> bool = 0;
};

}    // namespace scs::sim

