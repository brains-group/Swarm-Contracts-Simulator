#pragma once

#include <filesystem>
#include <random>

#include "singleton.hpp"

#ifndef SIM_TYPE
    #define SIM_TYPE GREEDY
#endif

class Config : public Singleton<Config> {
public:
    enum class AssignmentMethod : uint8_t {
        RANDOM,
        GREEDY,
        WITH_SPLITTING,
    };

    [[nodiscard]] auto numFrames() const -> std::size_t { return m_numFrames; }

    [[nodiscard]] auto frameMetricsPath() const -> std::filesystem::path {
        return m_frameMetricsPath;
    }

    [[nodiscard]] auto agentMetricsPath() const -> std::filesystem::path {
        return m_agentMetricsPath;
    }

    [[nodiscard]] auto assignmentMethod() const -> AssignmentMethod { return m_contractAssignment; }

    [[nodiscard]] auto numAgents() const -> std::size_t { return m_numAgents; }

    [[nodiscard]] auto minAgentSkill() const -> std::size_t { return m_minAgentSkill; }

    [[nodiscard]] auto agentBalanceDistribution() const -> std::uniform_int_distribution<uint64_t> {
        return m_agentBalanceDist;
    }

    [[nodiscard]] auto agentJobVecDistribution() const -> std::uniform_int_distribution<uint64_t> {
        return m_agentJobVecDist;
    }

    [[nodiscard]] auto newContractDistribution() const -> std::uniform_int_distribution<uint64_t> {
        return m_newContractDist;
    }

    [[nodiscard]] auto contractRewardDistribution() const
        -> std::uniform_int_distribution<uint64_t> {
        return m_contractRewardDist;
    }

    [[nodiscard]] auto contractDurationDistribution() const
        -> std::uniform_int_distribution<uint64_t> {
        return m_contractDurationDist;
    }

    [[nodiscard]] auto contractJobVecDistribution() const
        -> std::uniform_int_distribution<uint64_t> {
        return m_contractJobVecDist;
    }

private:
    const std::size_t m_numFrames = 100;

    const AssignmentMethod m_contractAssignment = AssignmentMethod::SIM_TYPE;

    const std::filesystem::path m_frameMetricsPath = "data/" QUOTIFY(SIM_TYPE) "frameMetrics.csv";
    const std::filesystem::path m_agentMetricsPath = "data/" QUOTIFY(SIM_TYPE) "agentMetrics.csv";

    const std::size_t m_numAgents     = 200;
    const std::size_t m_minAgentSkill = 10;

    const std::uniform_int_distribution<uint64_t> m_agentBalanceDist{100, 100};
    const std::uniform_int_distribution<uint64_t> m_agentJobVecDist{0, 10};

    const std::uniform_int_distribution<uint64_t> m_newContractDist{50, 50};
    const std::uniform_int_distribution<uint64_t> m_contractRewardDist{10, 20};
    const std::uniform_int_distribution<uint64_t> m_contractDurationDist{10, 20};
    const std::uniform_int_distribution<uint64_t> m_contractJobVecDist{0, 20};
};

