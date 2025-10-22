#pragma once
#include <cstdint>
#include <vector>

#include <common/macros.hpp>
#include <contracts/contract.hpp>
#include <data/part.hpp>
#include <simulator/handle.hpp>

#include "data/material.hpp"

namespace scs {

namespace sim {
class Simulator;
}

namespace agents {

class Agent;

class SimInterface {
public:
    SimInterface(sim::Simulator& sim, std::shared_ptr<Agent> agent);

    // SIM INFO
    [[nodiscard]] auto getTargetPoint() const -> const data::Point&;
    [[nodiscard]] auto getMaterialPoint(const data::Material& mat) const
        -> std::optional<data::Point>;

    // CONTRACTS
    [[nodiscard]] auto getContracts() const
        -> const std::vector<std::shared_ptr<contracts::Contract>>&;
    [[nodiscard]] auto getContract(uint64_t id) const -> std::shared_ptr<contracts::Contract>;
    [[nodiscard]] auto createContract(const data::Part& part) const
        -> std::shared_ptr<contracts::Contract>;
    auto               acceptContract(uint64_t id) -> bool;
    [[nodiscard]] auto canSendPart() const -> bool;
    [[nodiscard]] auto sendPart() const -> bool;

    // MOVEMENT
    auto setGoal(std::shared_ptr<data::Point> target) -> void;

    // CURRENT STATE
    [[nodiscard]] auto hasLoc() const -> bool;
    [[nodiscard]] auto getLoc() const -> const data::Point&;

    [[nodiscard]] auto hasPart() const -> bool;
    [[nodiscard]] auto currentPart() const -> const agents::Part&;

    [[nodiscard]] auto canGetMaterial(const data::Material& mat) const -> bool;
    [[nodiscard]] auto getMaterial(const data::Material& mat) const -> bool;

    [[nodiscard]] auto hasGoal() const -> bool;
    [[nodiscard]] auto currentGoal() const -> const data::Point&;

    DELETE_COPY(SimInterface);
    DELETE_MOVE(SimInterface);
    DEFAULT_DTOR(SimInterface);

private:
    sim::Simulator&        m_sim;
    std::shared_ptr<Agent> m_agent;
};

}    // namespace agents

}    // namespace scs
