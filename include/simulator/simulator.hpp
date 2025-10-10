#pragma once

#include <agents/agent.hpp>
#include <config/config.hpp>
#include <config/simulatorconfig.hpp>
#include <contracts/contract.hpp>

namespace scs::sim {

class Simulator {
public:
    explicit Simulator(config::SimulatorConfig& config = config::Config::instance())
        : m_config(config)
        , m_agents(m_config.initialAgents()) {}
    DELETE_COPY_MOVE(Simulator);
    DEFAULT_DTOR(Simulator);

    [[nodiscard]] auto getRoomRocners() const -> const std::vector<data::Point>& {
        return m_config.roomCorners();
    }

    [[nodiscard]] auto getTargetCorners() const -> const std::vector<data::Point>& {
        return m_config.targetCorners();
    }

    [[nodiscard]] auto getMaterialStores() const -> const std::vector<data::MaterialStore>& {
        return m_config.materialStores();
    }

    [[nodiscard]] auto getAgents() const -> const std::vector<agents::Agent>& { return m_agents; }

    auto runFrame() -> void {
        for (agents::Agent& agent : m_agents) {
            if (agent.hasController()) { agent.getController().run(); }
        }
    }

private:
    // NOTE: This is why we cannot copy the type, references aren't reassignable.
    // If we want this in the future, use a pointer
    config::SimulatorConfig& m_config;

    std::vector<agents::Agent>       m_agents;
    std::vector<contracts::Contract> m_contracts;
};

}    // namespace scs::sim
