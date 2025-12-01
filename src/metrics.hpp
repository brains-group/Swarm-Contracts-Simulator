#pragma once

#include <cstdint>
#include <ostream>

struct FrameMetrics {
    uint64_t frameNumber;
    uint64_t newContracts;
    uint64_t completedJobs;
    uint64_t cost;
    uint64_t reward;
};

constexpr std::string_view frameMetricHeaders =
    "frameNumber, newContracts, completedJobs, cost, reward";

inline auto operator<<(std::ostream& ostr, const FrameMetrics& fm) -> std::ostream& {
    return ostr << fm.frameNumber << ", " << fm.newContracts << ", " << fm.completedJobs << ", "
                << fm.cost << ", " << fm.reward;
}
