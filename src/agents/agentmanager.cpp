#include <agents/agentmanager.hpp>
#include <common/logger.hpp>

namespace scs::agents {

class AgentManagerImpl : public AgentManager {
public:
    AgentManagerImpl(sim::AgentSimulator& sim, const std::vector<data::AgentInfo>& agents)
        : m_agentInfos(agents) {
        m_agentControllers.reserve(m_agentInfos.size());
        for (auto& m_agentInfo : m_agentInfos) {
            m_agentControllers.emplace_back(makeAgentController(m_agentInfo.type),
                                            EnvironmentInterface::create(sim, m_agentInfo.id));
        }
    }

    [[nodiscard]] auto getAgentInfos() const -> const std::vector<data::AgentInfo>& override {
        return m_agentInfos;
    }

    [[nodiscard]] auto getAgentInfo(uint64_t agentID) const -> const data::AgentInfo& override {
        return m_agentInfos[agentID];
    }

    auto runAgents() -> void override {
        LOG(INFO) << "Running Agents (" << m_agentControllers.size() << ", " << m_agentInfos.size()
                  << ")";
        for (auto& [agentPtr, envPtr] : m_agentControllers) {
            LOG(INFO) << "Running agent " << envPtr->getID();
            agentPtr->run(*envPtr);
        }
    };

    [[nodiscard]] auto moveAgent(uint64_t agentID, const data::Vector& vec) -> bool override {
        data::AgentInfo& agent = getAgentInfo(agentID);
        agent.loc += vec;
        return true;
    }

private:
    [[nodiscard]] auto getAgentInfo(uint64_t agentID) -> data::AgentInfo& override {
        return m_agentInfos[agentID];
    }

    static auto makeAgentController(data::AgentInfo::Type agentType)
        -> std::unique_ptr<AgentController> {
        switch (agentType) {
            case data::AgentInfo::DEFAULT: return std::make_unique<impl::Default>();
        }
    }

    std::vector<data::AgentInfo> m_agentInfos;
    std::vector<std::pair<std::unique_ptr<AgentController>, std::unique_ptr<EnvironmentInterface>>>
        m_agentControllers;
};

auto AgentManager::create(sim::AgentSimulator& sim, const std::vector<data::AgentInfo>& agents)
    -> std::unique_ptr<AgentManager> {
    return std::make_unique<AgentManagerImpl>(sim, agents);
}
}    // namespace scs::agents
