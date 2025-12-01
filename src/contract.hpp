#pragma once

#include <cstdint>
#include <ostream>

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
