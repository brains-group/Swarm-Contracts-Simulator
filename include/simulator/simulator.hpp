#pragma once

#include <optional>

#include <agents/agent.hpp>
#include <agents/siminterface.hpp>
#include <config/config.hpp>
#include <config/simulatorconfig.hpp>
#include <contracts/contract.hpp>

#include "data/angle.hpp"
#include "data/point.hpp"

// FIXME: Revisit const correctness everywhere after changing to shared pointer vectors, everything
// is wrong

namespace scs::sim {

class Simulator {
public:
    explicit Simulator(config::SimulatorConfig& config = config::Config::instance())
        : m_config(config)
        , m_agents(m_config.initialAgents()) {
        for (const std::shared_ptr<agents::Agent>& agent : m_agents) {
            agent->setSimInterface(agents::MakeSimInterface(*this, agent));
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

    [[nodiscard]] auto getAgents() const -> const std::vector<std::shared_ptr<agents::Agent>>& {
        return m_agents;
    }

    auto runFrame() -> void {
        for (const std::shared_ptr<agents::Agent>& agentptr : m_agents) {
            agents::Agent& agent = *agentptr;
            if (agent.hasController()) { agent.getController().run(agent.getSimInterface()); }
            // TODO: Make this avoid collisions and stuff
            if (agent.hasGoal()) {
                data::Transform&           transform = agent.getTransform();
                data::Point&               goal      = agent.getGoal();
                std::optional<data::Angle> goalAngle = data::angle_to(transform.loc, goal, 1);
                if (!goalAngle) {    // We are at the goal if we can't find an angle
                    agent.setGoal(nullptr);
                    continue;
                }
                data::Angle steer = data::shortest_delta(transform.rot, goalAngle.value());
                if (steer > data::degrees(3)) { steer = data::degrees(3); }
                if (steer < data::degrees(3)) { steer = data::degrees(-3); }
                transform.rot = transform.rot + steer;
                transform.loc = data::advance(transform.loc, transform.rot, 1);
            }
        }
    }

    [[nodiscard]] auto getContracts() const
        -> const std::vector<std::shared_ptr<contracts::Contract>>& {
        return m_contracts;
    }

    [[nodiscard]] auto getContract(uint64_t id) -> std::shared_ptr<contracts::Contract> {
        return m_contracts[id];
    }
    [[nodiscard]] auto getContract(uint64_t id) const
        -> const std::shared_ptr<contracts::Contract>& {
        return m_contracts[id];
    }

    auto createContract(std::shared_ptr<agents::Agent> client, const data::Part& part)
        -> std::shared_ptr<contracts::Contract> {
        return m_contracts.emplace_back(
            std::make_shared<contracts::Contract>(m_contracts.size(), client, part));
    }

private:
    // NOTE: This is why we cannot copy the type, references aren't reassignable.
    // If we want this in the future, use a pointer
    config::SimulatorConfig& m_config;

    // TODO: Shared pointers are gross here, this is a bandaid fix for pointer invalidatoin on
    // realloc
    std::vector<std::shared_ptr<agents::Agent>>       m_agents;
    std::vector<std::shared_ptr<contracts::Contract>> m_contracts;
};

}    // namespace scs::sim
