#pragma once

#include <filesystem>
#include <random>
#include <vector>

#include "agent.hpp"
#include "contract.hpp"
#include "singleton.hpp"

class Config : public Singleton<Config> {
public:
    [[nodiscard]] auto numFrames() const -> std::size_t { return m_numFrames; }

    [[nodiscard]] auto frameMetricsPath() const -> std::filesystem::path {
        return m_frameMetricsPath;
    }

    [[nodiscard]] auto initialAgents() const -> const std::vector<Agent>& {
        return m_initialAgents;
    }

    [[nodiscard]] auto initialContracts() const -> const std::vector<Contract>& {
        return m_initialContracts;
    }

    // Unfortunately these methods have to either return copies or be non const
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
    const std::size_t m_numFrames = 10;

    const std::filesystem::path m_frameMetricsPath = "frameMetrics.csv";

    const std::vector<Agent> m_initialAgents = {
        {.id = 0, .balance = 10, .abilityVec = {1, 2, 3}, .contract = nullptr},
        {.id = 1, .balance = 10, .abilityVec = {3, 2, 1}, .contract = nullptr},
    };
    const std::vector<Contract> m_initialContracts;

    const std::uniform_int_distribution<uint64_t> m_contractRewardDist{10, 20};
    const std::uniform_int_distribution<uint64_t> m_contractDurationDist{10, 20};
    const std::uniform_int_distribution<uint64_t> m_contractJobVecDist{0, 5};
};

