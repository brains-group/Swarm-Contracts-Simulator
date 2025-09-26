#include <agents/agentmanager.hpp>
#include <agents/clientcontroller.hpp>
#include <agents/impl/defaultclient.hpp>
#include <common/logger.hpp>
#include <data/agentinfo.hpp>
#include <data/clientinfo.hpp>
#include <simulator/clientsimulator.hpp>

namespace scs::agents {

class AgentManagerImpl : public AgentManager {
public:
    AgentManagerImpl(sim::AgentSimulator& aSim, const std::vector<data::AgentInfo>& agents,
                     sim::ClientSimulator& cSim, const std::vector<data::ClientInfo>& clients)
        : m_agentInfos(agents)
        , m_clientInfos(clients) {
        m_agentControllers.reserve(m_agentInfos.size());
        for (auto& agentInfo : m_agentInfos) {
            m_agentControllers.emplace_back(makeAgentController(agentInfo.type),
                                            AgentSimInterface::create(aSim, agentInfo.id));
        }
        LOG(INFO) << "Received " << agents.size() << " (" << m_agentInfos.size()
                  << "), constructed " << m_agentControllers.size();

        m_clientControllers.reserve(m_clientInfos.size());
        for (auto& clientInfo : m_clientInfos) {
            m_clientControllers.emplace_back(makeClientController(clientInfo.type),
                                             ClientSimInterface::create(cSim, clientInfo.id));
        }

        LOG(INFO) << "Received " << clients.size() << " (" << m_clientInfos.size()
                  << "), constructed " << m_clientControllers.size();
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

    [[nodiscard]] auto getClientInfos() const -> const std::vector<data::ClientInfo>& override {
        return m_clientInfos;
    }

    [[nodiscard]] auto getClientInfo(uint64_t clientID) const -> const data::ClientInfo& override {
        // FIXME: DO THIS IN A BETTER WAY, THIS IS FOR A QUICK PoC
        return m_clientInfos[clientID - m_agentInfos.size()];
    }

    auto runClients() -> void override {
        for (auto& [clientPtr, envPtr] : m_clientControllers) { clientPtr->run(*envPtr); }
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
            case data::AgentInfo::DEFAULT: return std::make_unique<impl::DefaultAgent>();
        }
    }

    [[nodiscard]] auto getClientInfo(uint64_t clientID) -> data::ClientInfo& override {
        return m_clientInfos[clientID];
    }

    static auto makeClientController(data::ClientInfo::Type clientType)
        -> std::unique_ptr<ClientController> {
        switch (clientType) {
            case data::ClientInfo::DEFAULT: return std::make_unique<impl::DefaultClient>();
        }
    }

    std::vector<data::AgentInfo> m_agentInfos;
    std::vector<std::pair<std::unique_ptr<AgentController>, std::unique_ptr<AgentSimInterface>>>
                                  m_agentControllers;
    std::vector<data::ClientInfo> m_clientInfos;
    std::vector<std::pair<std::unique_ptr<ClientController>, std::unique_ptr<ClientSimInterface>>>
        m_clientControllers;
};

auto AgentManager::create(sim::AgentSimulator& aSim, const std::vector<data::AgentInfo>& agents,
                          sim::ClientSimulator& cSim, const std::vector<data::ClientInfo>& clients)
    -> std::unique_ptr<AgentManager> {
    return std::make_unique<AgentManagerImpl>(aSim, agents, cSim, clients);
}
}    // namespace scs::agents
