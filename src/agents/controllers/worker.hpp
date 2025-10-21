#pragma once

#include <cstdint>

#include <agents/controller.hpp>
#include <agents/siminterface.hpp>

#include "contracts/contract.hpp"

namespace scs::agents {

class WorkerController : public Controller {
public:
    auto run(SimInterface& sim) -> void override {
        // Look for a new contract when I don't have one
        if (m_contractID == UINT64_MAX) {
            for (const std::shared_ptr<contracts::Contract>& contract : sim.getContracts()) {
                if (contract->getWorker() == nullptr) {
                    if (sim.acceptContract(contract->getID())) {
                        m_contractID = contract->getID();
                        break;
                    }
                }
            }

            // If we can't find a contract, nothing to do
            if (m_contractID == UINT64_MAX) { return; }
        }

        // If we are holding the part in the contract, go to the target
        if (sim.hasPart() && sim.currentPart() == sim.getContract(m_contractID)->getPart()) {
            // Check if we are close enough to the target, if so, somplete
            if (sim.canSendPart() && sim.sendPart()) {
                m_contractID = UINT64_MAX;
                return;
            }

            // If we aren't pointed at the target, set our goal
            if (!sim.hasGoal() || sim.currentGoal() != sim.getTargetPoint()) {
                sim.setGoal(agents::MakeGoal(sim.getTargetPoint()));
            }
        }
    }

private:
    uint64_t m_contractID = UINT64_MAX;
};

}    // namespace scs::agents
