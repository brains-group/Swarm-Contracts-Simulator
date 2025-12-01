#pragma once

#include <cstdint>
#include <ostream>

#include "contract.hpp"
#include "job.hpp"

struct Agent {
public:
    uint64_t id;

    uint64_t balance;

    JobVec abilityVec;

    Contract* contract;
};

inline auto operator<<(std::ostream& ostr, const Agent& agent) -> std::ostream& {
    return ostr << "Agent[id: " << agent.id << ", balance " << agent.balance
                << ", ability: " << agent.abilityVec << "]";
}
