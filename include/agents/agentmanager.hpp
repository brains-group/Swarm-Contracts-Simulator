#pragma once

#include <memory>
#include <vector>

#include <agents/agentcontroller.hpp>
#include <agents/agentsiminterface.hpp>
#include <agents/impl/defaultagent.hpp>
#include <common/macros.hpp>
#include <data/agentinfo.hpp>
#include <data/clientinfo.hpp>
#include <simulator/clientsimulator.hpp>
#include <simulator/simulator.hpp>

namespace scs::agents {

class AgentManager {
public:
    INTERFACE_CTOR_DTORS(AgentManager);

    static auto create(sim::AgentSimulator& aSim, const std::vector<data::AgentInfo>& agents,
                       sim::ClientSimulator& cSim, const std::vector<data::ClientInfo>& clients)
        -> std::unique_ptr<AgentManager>;

    [[nodiscard]] virtual auto getAgentInfos() const -> const std::vector<data::AgentInfo>&   = 0;
    [[nodiscard]] virtual auto getAgentInfo(uint64_t agentID) const -> const data::AgentInfo& = 0;
    virtual auto               runAgents() -> void                                            = 0;

    [[nodiscard]] virtual auto getClientInfos() const -> const std::vector<data::ClientInfo>& = 0;
    [[nodiscard]] virtual auto getClientInfo(uint64_t clientID) const
        -> const data::ClientInfo&    = 0;
    virtual auto runClients() -> void = 0;

    [[nodiscard]] virtual auto moveAgent(uint64_t agentID, const data::Vector& vec) -> bool = 0;

private:
    [[nodiscard]] virtual auto getAgentInfo(uint64_t agentID) -> data::AgentInfo&   = 0;
    [[nodiscard]] virtual auto getClientInfo(uint64_t agentID) -> data::ClientInfo& = 0;
};
}    // namespace scs::agents
