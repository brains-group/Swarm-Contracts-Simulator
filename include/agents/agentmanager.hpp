#pragma once

#include <memory>
#include <vector>

#include <agents/agentcontroller.hpp>
#include <agents/environmentinterface.hpp>
#include <agents/impl/default.hpp>
#include <common/macros.hpp>
#include <data/agentinfo.hpp>
#include <simulator/simulator.hpp>

namespace scs::agents {

class AgentManager {
public:
    INTERFACE_CTOR_DTORS(AgentManager);

    static auto create(sim::Simulator& sim, const std::vector<data::AgentInfo>& agents)
        -> std::unique_ptr<AgentManager>;

    [[nodiscard]] virtual auto getAgentInfos() const -> const std::vector<data::AgentInfo>&   = 0;
    [[nodiscard]] virtual auto getAgentInfo(uint64_t agentID) const -> const data::AgentInfo& = 0;
    virtual auto               runAgents() -> void                                            = 0;
};
}    // namespace scs::agents
