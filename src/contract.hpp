#pragma once

#include <cstdint>
#include <limits>
#include <ostream>
#include <ranges>

#include "job.hpp"

struct Contract {
    uint64_t id;

    uint64_t reward;
    uint64_t created;
    uint64_t expires;

    JobVec job;
    JobVec remainingWork;
    bool   accepted;
};

inline auto operator<<(std::ostream& ostr, const Contract& contract) -> std::ostream& {
    return ostr << "Contract[id: " << contract.id << ", reward: " << contract.reward
                << ", job: " << contract.job << ", " << contract.remainingWork << "]";
}

inline auto costToComplete(const JobVec& ability, const JobVec& difficulty) {
    uint64_t cost = 0;
    for (const auto& [a, d] : std::views::zip(ability, difficulty)) {
        if (a == 0) { return std::numeric_limits<uint64_t>::max(); }
        cost += d / a;
    }
    return cost;
}
