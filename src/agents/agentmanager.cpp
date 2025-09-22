#include <agents/agentmanager.hpp>

namespace scs::agents {

class AgentManagerImpl : public AgentManager {
public:
    AgentManagerImpl(sim::Simulator& sim, const std::vector<data::AgentInfo>& agents)
        : m_agentInfos(agents) {
        m_agentControllers.reserve(m_agentInfos.size());
        for (unsigned int i = 0; i < m_agentControllers.size(); i++) {
            m_agentControllers.emplace_back(makeAgentController(m_agentInfos[i].type),
                                            EnvironmentInterface::create(sim, m_agentInfos[i].id));
        }
    }

    [[nodiscard]] auto getAgentInfos() const -> const std::vector<data::AgentInfo>& override {
        return m_agentInfos;
    }

    auto runAgents() -> void override {
        for (auto& [agentPtr, envPtr] : m_agentControllers) { agentPtr->run(*envPtr); }
    };

private:
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

auto AgentManager::create(sim::Simulator& sim, const std::vector<data::AgentInfo>& agents)
    -> std::unique_ptr<AgentManager> {
    return std::make_unique<AgentManagerImpl>(sim, agents);
}
}    // namespace scs::agents
