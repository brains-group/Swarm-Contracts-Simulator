#include <utility>

#include <agents/agent.hpp>
#include <agents/siminterface.hpp>
#include <data/part.hpp>
#include <simulator/simulator.hpp>

#include "contracts/contract.hpp"

namespace scs::agents {

SimInterface::SimInterface(sim::Simulator& sim, Agent& agent)
    : m_sim(sim)
    , m_agent(agent) {}

auto SimInterface::getContracts() const -> const std::vector<contracts::Contract>& {
    return m_sim.getContracts();
}

auto SimInterface::createContract(const data::Part& part) -> contracts::Contract& {
    return m_sim.createContract(m_agent, part);
}

auto SimInterface::acceptContract(contracts::Contract& contract) -> bool {
    return contract.setWorker(&m_agent);
}

auto SimInterface::completeContract(contracts::Contract& contract) -> void {
    contract.markComplete();
}

auto SimInterface::setTarget(std::shared_ptr<data::Point> target) -> void {
    m_agent.setGoal(std::move(target));
}

}    // namespace scs::agents
