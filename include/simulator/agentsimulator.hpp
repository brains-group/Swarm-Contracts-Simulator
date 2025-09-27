#pragma once

#include <optional>

#include <common/macros.hpp>
#include <data/vector.hpp>
#include <simulator/simulator.hpp>

namespace scs::sim {

class AgentSimulator : virtual public Simulator {
public:
    INTERFACE_CTOR_DTORS(AgentSimulator);

    [[nodiscard]] virtual auto moveAgent(uint64_t agentID, const data::Vector& vec) -> bool    = 0;
    [[nodiscard]] virtual auto takeNextOrder(uint64_t m_agentID) -> std::optional<data::Color> = 0;
};

}    // namespace scs::sim

