#pragma once

#include <optional>

#include <agents/agent.hpp>
#include <agents/siminterface.hpp>
#include <config/config.hpp>
#include <config/simulatorconfig.hpp>
#include <contracts/contract.hpp>

#include "data/angle.hpp"
#include "data/point.hpp"

namespace scs::sim {

class Simulator {
public:
    explicit Simulator(config::SimulatorConfig& config = config::Config::instance())
        : m_config(config)
        , m_agents(m_config.initialAgents()) {
        for (agents::Agent& agent : m_agents) {
            agent.setSimInterface(std::make_shared<agents::SimInterface>(*this, agent));
        }
    }
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
            if (agent.hasController()) { agent.getController().run(agent.getSimInterface()); }
            // TODO: Make this avoid collisions and stuff
            if (agent.hasGoal()) {
                data::Transform&           transform = agent.getTransform();
                data::Point&               goal      = agent.getGoal();
                std::optional<data::Angle> goalAngle = data::angle_to(transform.loc, goal);
                if (!goalAngle) {    // We are at the goal if we can't find an angle
                    agent.setGoal(nullptr);
                    continue;
                }
                data::Angle steer = data::shortest_delta(transform.rot, goalAngle.value());
                if (steer > data::degrees(1)) { steer = data::degrees(1); }
                if (steer < data::degrees(1)) { steer = data::degrees(-1); }
                transform.rot = transform.rot + steer;
                transform.loc = data::advance(transform.loc, transform.rot, 1);
            }
        }
    }

    [[nodiscard]] auto getContracts() const -> const std::vector<contracts::Contract>& {
        return m_contracts;
    }

    auto createContract(agents::Agent& client, const data::Part& part) -> contracts::Contract& {
        return m_contracts.emplace_back(&client, part);
    }

private:
    // NOTE: This is why we cannot copy the type, references aren't reassignable.
    // If we want this in the future, use a pointer
    config::SimulatorConfig& m_config;

    std::vector<agents::Agent>       m_agents;
    std::vector<contracts::Contract> m_contracts;
};

}    // namespace scs::sim
