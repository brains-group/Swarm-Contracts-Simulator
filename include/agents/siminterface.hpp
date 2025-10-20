#pragma once

#include <vector>

#include <common/macros.hpp>
#include <contracts/contract.hpp>
#include <data/part.hpp>
#include <simulator/handle.hpp>

namespace scs {

namespace sim {
class Simulator;
}

namespace agents {

class Agent;

class SimInterface {
public:
    SimInterface(sim::Simulator& sim, Agent& agent);

    // CONTRACTS
    [[nodiscard]] auto getContracts() const -> const std::vector<contracts::Contract>&;
    auto               createContract(const data::Part& part) -> contracts::Contract&;
    auto               acceptContract(contracts::Contract& contract) -> bool;
    auto               completeContract(contracts::Contract& contract) -> void;

    // MOVEMENT
    auto setTarget(std::shared_ptr<data::Point> target) -> void;

    DELETE_COPY(SimInterface);
    DELETE_MOVE(SimInterface);
    DEFAULT_DTOR(SimInterface);

private:
    sim::Simulator& m_sim;
    Agent&          m_agent;
};

}    // namespace agents

}    // namespace scs
