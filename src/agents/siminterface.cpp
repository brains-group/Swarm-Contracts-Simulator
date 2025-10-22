#include <utility>

#include <agents/agent.hpp>
#include <agents/siminterface.hpp>
#include <contracts/contract.hpp>
#include <data/part.hpp>
#include <data/point.hpp>
#include <simulator/simulator.hpp>

#include "data/material.hpp"

namespace scs::agents {

SimInterface::SimInterface(sim::Simulator& sim, std::shared_ptr<Agent> agent)
    : m_sim(sim)
    , m_agent(std::move(agent)) {}

auto SimInterface::getTargetPoint() const -> const data::Point& {
    return m_sim.getTargetCorners()[0];
}

auto SimInterface::getMaterialPoint(const data::Material& mat) const -> std::optional<data::Point> {
    for (const auto& store : m_sim.getMaterialStores()) {
        if (store.material == mat) { return store.space.loc; }
    }
    return std::nullopt;
}

auto SimInterface::getContracts() const
    -> const std::vector<std::shared_ptr<contracts::Contract>>& {
    return m_sim.getContracts();
}

auto SimInterface::getContract(uint64_t id) const -> std::shared_ptr<contracts::Contract> {
    return m_sim.getContract(id);
}

auto SimInterface::createContract(const data::Part& part) const
    -> std::shared_ptr<contracts::Contract> {
    return m_sim.createContract(m_agent, part);
}

auto SimInterface::acceptContract(uint64_t id) -> bool {
    return m_sim.getContract(id)->setWorker(m_agent);
}

auto SimInterface::canSendPart() const -> bool {
    return m_agent->hasPart() && m_agent->hasTransform()
        && data::distance(m_agent->getTransform().loc, getTargetPoint()) < 5;
}

auto SimInterface::sendPart() const -> bool {
    if (!canSendPart()) { return false; }
    for (const std::shared_ptr<contracts::Contract>& contract : getContracts()) {
        if (!contract->isComplete() && contract->getWorker() == m_agent
            && contract->getPart() == m_agent->getPart()) {
            contract->markComplete();
            m_agent->setPart(nullptr);
            return true;
        }
    }
    return false;
}

auto SimInterface::setGoal(std::shared_ptr<data::Point> target) -> void {
    m_agent->setGoal(std::move(target));
}

auto SimInterface::hasLoc() const -> bool { return m_agent->hasTransform(); }
auto SimInterface::getLoc() const -> const data::Point& { return m_agent->getTransform().loc; }

auto SimInterface::hasPart() const -> bool { return m_agent->hasPart(); }
auto SimInterface::currentPart() const -> const agents::Part& { return m_agent->getPart(); }

auto SimInterface::canGetMaterial(const data::Material& mat) const -> bool {
    std::optional<data::Point> matLoc = getMaterialPoint(mat);

    return matLoc && hasLoc() && !hasPart() && data::distance(*matLoc, getLoc()) < 5;
}
auto SimInterface::getMaterial(const data::Material& mat) const -> bool {
    if (!canGetMaterial(mat)) { return false; }

    m_agent->setPart(agents::MakePart(data::Part({data::Material(mat)})));
    return true;
}

auto SimInterface::hasGoal() const -> bool { return m_agent->hasGoal(); }
auto SimInterface::currentGoal() const -> const data::Point& { return m_agent->getGoal(); }

}    // namespace scs::agents
