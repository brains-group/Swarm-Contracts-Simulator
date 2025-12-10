#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <memory>
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
        , m_frameMetricsStream(m_config.frameMetricsPath())
        , m_agentMetricsStream(m_config.agentMetricsPath()) {
        LOG(INFO) << "Initializing simulation";

        if (!m_frameMetricsStream.good()) { LOG(FATAL) << "Failed to open frame metrics file"; }
        m_frameMetricsStream << frameMetricHeaders << "\n";

        if (!m_agentMetricsStream.good()) { LOG(FATAL) << "Failed to open agent metrics file"; }
        m_agentMetricsStream << agentHeaders << "\n";

        // We need IDs to match vector indices
        createAgents();

        LOG(DEBUG) << "Running with " << m_state.agents.size() << " agents";
        for (const auto& agent : m_state.agents) { LOG(FINE) << *agent; }
    }

    ~Simulator() {
        LOG(INFO) << "Tearing down simulation";

        m_frameMetricsStream.close();

        LOG(DEBUG) << "Dumping agent metrics to stream";

        for (auto& agent : m_state.agents) { agentCSV(m_agentMetricsStream, *agent) << "\n"; }
        m_agentMetricsStream.close();
    }

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
        // If we don't have a contract, look for one we should take
        if (agent.contract == nullptr) {
            lookForContract(agent);
            return;
        }

        // If we have a contract, work it
        workContract(agent);
    }

    auto createAgents() -> void {
        LOG(DEBUG) << "Creating agents";
        while (m_state.agents.size() < m_config.numAgents()) { generateRandomAgent(); }
    }

    auto generateRandomAgent() -> Agent& {
        uint64_t balance = m_config.agentBalanceDistribution()(m_rng);
        JobVec   ability =
            generateRandomJobVec(m_config.agentJobVecDistribution(), m_config.minAgentSkill());
        return createAgent(balance, ability);
    }

    auto createAgent(uint64_t balance, const JobVec& ability) -> Agent& {
        assert(m_state.frame == 0 && "Agents can only be created before running sim");
        Agent& agent = *m_state.agents.emplace_back(std::make_unique<Agent>(
            m_state.agents.size(), balance, ability, nullptr, AgentMetrics{}));
        LOG(FINE) << "Created agent " << agent;
        return agent;
    }

    auto createContracts() -> void {
        LOG(DEBUG) << "Creating contracts";
        uint64_t newContracts = 0;
        while (newContracts++ < m_config.newContractDistribution()(m_rng)) {
            generateRandomContract();
        }
    }

    auto generateRandomContract() -> Contract& {
        uint64_t reward   = m_config.contractRewardDistribution()(m_rng);
        JobVec   job      = generateRandomJobVec(m_config.contractJobVecDistribution());
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

    auto generateRandomJobVec(auto distribution, uint64_t minTotal = 0) -> JobVec {
        JobVec job = {};
        // Don't allow empty jobs
        uint64_t total = 0;
        while (*std::ranges::max_element(job) == 0 || total < minTotal) {
            total = 0;
            for (uint64_t& dim : job) { total += (dim = distribution(m_rng)); }
        }
        return job;
    }

    auto workContract(Agent& agent) -> void {
        if (agent.contract == nullptr) { return; }
        Contract& contract = *agent.contract;

        if (m_state.frame >= contract.expires) {
            m_frameMetrics.expiredContracts++;
            LOG(FINE) << *agent.contract << " expired under " << agent;
            agent.contract = nullptr;
            return;
        }

        uint64_t totalProg = 0;
        for (const auto& [abilityVal, jobVal] :
             std::views::zip(agent.abilityVec, contract.remainingWork)) {
            if (agent.balance == 0) { break; }

            uint64_t prog = std::min({agent.balance, abilityVal, jobVal});

            if (prog == 0) { continue; }

            totalProg += prog;
            agent.balance--;
            m_frameMetrics.cost++;
            jobVal -= prog;
        }

        if (*std::ranges::max_element(agent.contract->remainingWork) == 0) {
            m_frameMetrics.completedJobs++;
            m_frameMetrics.reward += agent.contract->reward;
            agent.balance += agent.contract->reward;
            agent.metrics.profit += agent.contract->reward;
            agent.metrics.numContracts++;
            LOG(FINE) << agent << " completed " << *agent.contract;
            agent.contract = nullptr;
        } else if (totalProg == 0) {
            LOG(FINE) << agent << " makde no progress on " << *agent.contract;
        } else {
            LOG(FINE) << agent << " worked on " << *agent.contract;
        }
    }

    auto lookForContract(Agent& agent) -> void {
        if (agent.contract != nullptr) { return; }

        auto availableContracts = m_state.contracts | std::views::filter([](const auto& c) -> bool {
                                      return !c->accepted;
                                  });

        Contract* newContract = nullptr;
        switch (m_config.assignmentMethod()) {
            case Config::AssignmentMethod::RANDOM:
                // Contracts are randomly generated, just pick the first available
                if (auto c_itr = std::ranges::find_if(
                        availableContracts, [](const auto& c) -> bool { return !c->accepted; });
                    c_itr != availableContracts.end()) {
                    newContract = c_itr->get();
                }
                break;
            case Config::AssignmentMethod::GREEDY:
            case Config::AssignmentMethod::WITH_SPLITTING:    // TODO: Refactor to separate these
                // Pick whatever is available that has the highest reward
                if (auto c_itr = std::ranges::max_element(
                        availableContracts,
                        [&agent](const auto& c1, const auto& c2) -> bool {
                            uint64_t c1Cost = costToComplete(agent.abilityVec, c1->remainingWork);
                            uint64_t c2Cost = costToComplete(agent.abilityVec, c2->remainingWork);
                            return (c1Cost < c1->reward ? c1->reward - c1Cost : 0)
                                 < (c2Cost < c2->reward ? c2->reward - c2Cost : 0);
                        });
                    c_itr != availableContracts.end()
                    && costToComplete(agent.abilityVec, (*c_itr)->remainingWork)
                           < (*c_itr)->reward) {
                    newContract = c_itr->get();
                }
                break;
        }

        if (newContract != nullptr) {
            agent.contract           = newContract;
            agent.contract->accepted = true;
            LOG(FINE) << agent << " accepted contract " << *agent.contract;
        } else {
            LOG(FINE) << agent << " did not choose a contract";
        }
    }

    const Config&   m_config;
    std::mt19937_64 m_rng;

    State m_state;

    // Metrics for each frame are streamed directly to output
    std::ofstream m_frameMetricsStream;
    FrameMetrics  m_frameMetrics{};

    std::ofstream m_agentMetricsStream;
};
