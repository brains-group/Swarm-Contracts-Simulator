#pragma once

#include <algorithm>
#include <cstdint>
#include <ostream>

#include "contract.hpp"
#include "job.hpp"
#include "metrics.hpp"

struct Agent {
public:
    uint64_t id;

    uint64_t balance;

    JobVec abilityVec;

    Contract* contract;

    AgentMetrics metrics;
};

inline auto operator<<(std::ostream& ostr, const Agent& agent) -> std::ostream& {
    return ostr << "Agent[id: " << agent.id << ", balance " << agent.balance
                << ", ability: " << agent.abilityVec << "]";
}

constexpr std::string_view agentHeaders = "id, skill_sum, profit, num_contracts";

inline auto agentCSV(std::ostream& ostr, const Agent& agent) -> std::ostream& {
    return ostr << agent.id << ", " << std::ranges::fold_left(agent.abilityVec, 0, std::plus{})
                << ", " << agent.metrics.profit << ", " << agent.metrics.numContracts;
}
