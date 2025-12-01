#pragma once

#include <algorithm>
#include <fstream>
#include <random>
#include <ranges>
#include <vector>

#include "agent.hpp"
#include "config.hpp"
#include "contract.hpp"
#include "job.hpp"
#include "logger.hpp"
#include "macros.hpp"
#include "metrics.hpp"
#include "state.hpp"

class Simulator {
public:
    DELETE_COPY_MOVE(Simulator);

    explicit Simulator(const Config& config, uint64_t seed)
        : m_config(config)
        , m_rng(seed)
        , m_frameMetricsStream(m_config.frameMetricsPath()) {
        LOG(INFO) << "Initializing simulation";

        if (!m_frameMetricsStream.good()) { LOG(FATAL) << "Failed to open stream file"; }
        m_frameMetricsStream << frameMetricHeaders << "\n";

        // We need IDs to match vector indices
        m_state.agents = std::views::enumerate(m_config.initialAgents())
                       | std::views::transform([](const auto& tup) -> std::unique_ptr<Agent> {
                             const auto& [idx, agent]        = tup;
                             std::unique_ptr<Agent> agentPtr = std::make_unique<Agent>(agent);
                             agentPtr->id                    = idx;
                             return agentPtr;
                         })
                       | std::ranges::to<std::vector<std::unique_ptr<Agent>>>();

        LOG(DEBUG) << "Running with " << m_state.agents.size() << " agents";
        for (const auto& agent : m_state.agents) { LOG(FINE) << *agent; }
    }

    ~Simulator() { LOG(INFO) << "Tearing down simulation"; }

    auto run() -> void {
        while (m_state.frame++ < m_config.numFrames()) {
            m_frameMetrics             = {};
            m_frameMetrics.frameNumber = m_state.frame;
            LOG(INFO) << "Running frame " << m_state.frame;

            createContracts();

            LOG(DEBUG) << "Running agents";
            for (auto& agent : m_state.agents) { runAgent(*agent); }

            LOG(FINE) << "Dumping frame metrics to stream";
            m_frameMetricsStream << m_frameMetrics << "\n";
        }
    }

private:
    auto runAgent(Agent& agent) -> void {
        LOG(FINE) << "Running " << agent;

        while (true) {
            // If we don't have a contract, look for one we should take
            lookForContract(agent);

            // We didn't find a contract to pick up
            if (agent.contract == nullptr) { break; }

            // If we have a contract, work it (and if we did work, break)
            if (workContract(agent)) { break; }
        }
    }

    auto createContracts() -> void {
        LOG(DEBUG) << "Creating contracts";
        while (std::ranges::count_if(m_state.contracts,
                                     [this](const auto& c) -> bool {
                                         return c->expires > m_state.frame
                                             && *std::ranges::max_element(c->remainingWork) > 0;
                                     })
               < 2) {
            generateRandomContract();
        }
    }

    auto generateRandomContract() -> Contract& {
        uint64_t reward = m_config.contractRewardDistribution()(m_rng);
        JobVec   job    = {};
        // Don't allow empty jobs
        while (*std::ranges::max_element(job) == 0) {
            for (uint64_t& dim : job) { dim = m_config.contractJobVecDistribution()(m_rng); }
        }
        uint64_t duration = m_config.contractDurationDistribution()(m_rng);

        return createContract(reward, job, duration);
    }

    auto createContract(uint64_t reward, const JobVec& job, uint64_t duration) -> Contract& {
        Contract& contract = *m_state.contracts.emplace_back(
            std::make_unique<Contract>(m_state.contracts.size(), reward, m_state.frame,
                                       m_state.frame + duration, job, job, false));
        m_frameMetrics.newContracts++;
        LOG(FINE) << "Created contract " << contract;
        return contract;
    }

    auto workContract(Agent& agent) -> bool {
        bool didWork = false;
        if (agent.contract == nullptr) { return didWork; }
        Contract& contract = *agent.contract;

        if (m_state.frame >= contract.expires) {
            LOG(FINE) << *agent.contract << " expired under " << agent;
            agent.contract = nullptr;
            return didWork;
        }

        for (const auto& [abilityVal, jobVal] :
             std::views::zip(agent.abilityVec, contract.remainingWork)) {
            if (agent.balance == 0) { break; }

            uint64_t prog = std::min({agent.balance, abilityVal, jobVal});

            if (prog == 0) { continue; }

            didWork = true;
            agent.balance--;
            m_frameMetrics.cost++;
            jobVal -= prog;
        }
        if (*std::ranges::max_element(agent.contract->remainingWork) == 0) {
            m_frameMetrics.completedJobs++;
            m_frameMetrics.reward += agent.contract->reward;
            agent.balance += agent.contract->reward;
            LOG(FINE) << agent << " completed " << *agent.contract;
            agent.contract = nullptr;
        } else {
            LOG(FINE) << agent << " worked on " << *agent.contract;
        }
        return didWork;
    }

    auto lookForContract(Agent& agent) -> void {
        if (agent.contract != nullptr) { return; }
        for (const auto& contract :
             m_state.contracts
                 | std::views::filter([](const auto& c) -> bool { return !c->accepted; })) {
            // For now, just accept the first contract that isn't taken
            contract->accepted = true;
            agent.contract     = contract.get();
            LOG(FINE) << agent << " accepted " << *contract;
            break;
        }
    }

    const Config&   m_config;
    std::mt19937_64 m_rng;

    State m_state;

    std::ofstream m_frameMetricsStream;
    FrameMetrics  m_frameMetrics{};
};
