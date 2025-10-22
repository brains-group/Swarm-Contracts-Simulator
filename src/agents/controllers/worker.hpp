#pragma once

#include <cstdint>

#include <agents/controller.hpp>
#include <agents/siminterface.hpp>

#include "contracts/contract.hpp"

namespace scs::agents {

class WorkerController : public Controller {
public:
    enum class WorkerState : uint8_t {
        IDLE,
        GETTING_MATERIAL,
        DELIVERING_COMPLETE_PART,
    };

    auto run(SimInterface& sim) -> void override {
        switch (m_state) {
            case WorkerState::IDLE:                     lookForContract(sim); break;
            case WorkerState::GETTING_MATERIAL:         getMaterial(sim); break;
            case WorkerState::DELIVERING_COMPLETE_PART: deliverPart(sim); break;
        }
    }

    auto lookForContract(SimInterface& sim) -> void {
        if (m_contractID == UINT64_MAX) {
            for (const std::shared_ptr<contracts::Contract>& contract : sim.getContracts()) {
                if (contract->getWorker() == nullptr) {
                    if (sim.acceptContract(contract->getID())) {
                        m_contractID = contract->getID();
                        LOG(INFO) << "Accepted contract " << m_contractID;
                        m_state = WorkerState::GETTING_MATERIAL;
                        return;
                    }
                }
            }
        }
    }

    auto getMaterial(SimInterface& sim) -> void {
        const data::Part& part = sim.getContract(m_contractID)->getPart();

        // See the next valid material
        while (!part[m_currentPartIndex]) {
            if (++m_currentPartIndex > part.size()) {
                // We are out of parts to get, this should be unreachable?
                LOG(ERROR) << "Out of parts to get (" << m_currentPartIndex << " > " << part.size();
                m_state      = WorkerState::IDLE;
                m_contractID = UINT64_MAX;
                return;
            }
        }

        const data::Material& mat = *part[m_currentPartIndex];

        // Try to grab the material
        if (sim.canGetMaterial(mat) && sim.getMaterial(mat)) {
            m_state = WorkerState::DELIVERING_COMPLETE_PART;
            return;
        }

        // Otherwise, try to get to the goal
        if (!sim.hasGoal()) { sim.setGoal(agents::MakeGoal(*sim.getMaterialPoint(mat))); }
    }

    auto deliverPart(SimInterface& sim) -> void {
        if (sim.canSendPart() && sim.sendPart()) {
            LOG(INFO) << "Completed contract " << m_contractID;
            m_contractID = UINT64_MAX;
            m_state      = WorkerState::IDLE;
            return;
        }

        // If we aren't pointed at the target, set our goal
        if (!sim.hasGoal() || sim.currentGoal() != sim.getTargetPoint()) {
            sim.setGoal(agents::MakeGoal(sim.getTargetPoint()));
        }
    }

private:
    WorkerState m_state      = WorkerState::IDLE;
    uint64_t    m_contractID = UINT64_MAX;

    unsigned int m_currentPartIndex = 0;
};

}    // namespace scs::agents
