#include <agents/agentmanager.hpp>
#include <agents/impl/defaultclient.hpp>
#include <common/logger.hpp>
#include <data/agentinfo.hpp>

namespace scs::agents {

class AgentManagerImpl : public AgentManager {
public:
    AgentManagerImpl(sim::AgentSimulator& aSim, const std::vector<data::AgentInfo>& agents)
        : m_agentInfos(agents) {
        m_agentControllers.reserve(m_agentInfos.size());
        for (auto& agentInfo : m_agentInfos) {
            m_agentControllers.emplace_back(makeAgentController(agentInfo.type),
                                            AgentSimInterface::create(aSim, agentInfo.id));
        }
        LOG(INFO) << "Received " << agents.size() << " (" << m_agentInfos.size()
                  << "), constructed " << m_agentControllers.size();
    }

    [[nodiscard]] auto getAgentInfos() const -> const std::vector<data::AgentInfo>& override {
        return m_agentInfos;
    }

    [[nodiscard]] auto getAgentInfo(uint64_t agentID) const -> const data::AgentInfo& override {
        return m_agentInfos[agentID];
    }

    auto runAgents() -> void override {
        for (auto& [agentPtr, envPtr] : m_agentControllers) { agentPtr->run(*envPtr); }
    }

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
            case data::AgentInfo::CLIENT: return std::make_unique<impl::DefaultClient>();
            case data::AgentInfo::WORKER: return std::make_unique<impl::DefaultWorker>();
        }
    }

    std::vector<data::AgentInfo> m_agentInfos;
    std::vector<std::pair<std::unique_ptr<AgentController>, std::unique_ptr<AgentSimInterface>>>
        m_agentControllers;
};

auto AgentManager::create(sim::AgentSimulator& aSim, const std::vector<data::AgentInfo>& agents)
    -> std::unique_ptr<AgentManager> {
    return std::make_unique<AgentManagerImpl>(aSim, agents);
}
}    // namespace scs::agents
