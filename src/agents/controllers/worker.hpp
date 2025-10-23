#pragma once

#include <cstdint>

#include <agents/controller.hpp>
#include <agents/siminterface.hpp>

#include "contracts/contract.hpp"
#include "data/material.hpp"
#include "data/station.hpp"

namespace scs::agents {

class WorkerController : public Controller {
public:
    enum class WorkerState : uint8_t {
        IDLE,
        PART_PLANNING,
        GETTING_MATERIAL,
        STASH_ADD,
        STASH_MIX,
        COLLECT_STASH_ADD,
        COLLECT_STASH_MIX,
        DELIVERING_COMPLETE_PART,
    };

    auto run(SimInterface& sim) -> void override {
        switch (m_state) {
            case WorkerState::IDLE:                     lookForContract(sim); break;
            case WorkerState::PART_PLANNING:            planPart(sim); break;
            case WorkerState::GETTING_MATERIAL:         getMaterial(sim); break;
            case WorkerState::STASH_ADD:                stashPart(sim, data::Station::ADD); break;
            case WorkerState::STASH_MIX:                stashPart(sim, data::Station::MIX); break;
            case WorkerState::COLLECT_STASH_ADD:        getStash(sim, data::Station::ADD); break;
            case WorkerState::COLLECT_STASH_MIX:        getStash(sim, data::Station::MIX); break;
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
                        m_state            = WorkerState::PART_PLANNING;
                        m_currentPartIndex = 0;
                        return;
                    }
                }
            }
        }
    }

    auto planPart(SimInterface& sim) -> void {
        const data::Part& part = sim.getContract(m_contractID)->getPart();

        if (sim.hasPart()) {
            if (sim.currentPart() == part) {
                m_state = WorkerState::DELIVERING_COMPLETE_PART;
                return;
            }
            m_state = m_mixing ? WorkerState::STASH_MIX : WorkerState::STASH_ADD;
            return;
        }

        // See the next valid material
        while (!m_mixing && m_currentPartIndex < part.size() && !part[m_currentPartIndex]) {
            ++m_currentPartIndex;
        }

        if (m_currentPartIndex >= part.size()) {
            // We have made all the parts, go collect the stash
            m_state = WorkerState::COLLECT_STASH_ADD;
            return;
        }

        // We are done mixing
        if (m_mixing && m_mixQueue.empty()) {
            m_state = WorkerState::COLLECT_STASH_MIX;
            return;
        }

        // We found the next needed material
        m_state = WorkerState::GETTING_MATERIAL;
    }

    auto getMaterial(SimInterface& sim) -> void {
        // Pull from mix queue if available
        const data::Material& mat =
            m_mixing ? m_mixQueue.back()
                     : *sim.getContract(m_contractID)->getPart()[m_currentPartIndex];

        // Try to grab the material
        if (sim.canGetMaterial(mat) && sim.getMaterial(mat)) {
            if (m_mixing) {
                m_mixQueue.pop_back();
            } else {
                m_currentPartIndex++;
            }
            m_state = WorkerState::PART_PLANNING;
            return;
        }

        std::optional<data::Point> materialPoint = sim.getMaterialPoint(mat);

        // If we can't see the material, add to mix queue
        if (!materialPoint) {
            // TODO: Handle non special colors?
            if (mat.r != 0) { m_mixQueue.emplace_back(data::Material::Red); }
            if (mat.g != 0) { m_mixQueue.emplace_back(data::Material::Green); }
            if (mat.b != 0) { m_mixQueue.emplace_back(data::Material::Blue); }
            m_mixing = true;
            return;
        }

        // If we haven't set our goal there, go
        if (!sim.hasGoal()) { sim.setGoal(agents::MakeGoal(*materialPoint)); }
    }

    auto stashPart(SimInterface& sim, data::Station::Type type) -> void {
        // Try to stash the current part
        if (sim.canStashPart(type) && sim.stashPart(type)) {
            m_state = WorkerState::PART_PLANNING;
            return;
        }

        // If we haven't set our goal there, go
        if (!sim.hasGoal()) { sim.setGoal(agents::MakeGoal(*sim.getStationPoint(type))); }
    }

    auto getStash(SimInterface& sim, data::Station::Type type) -> void {
        if (sim.canCollectStash(type) && sim.collectStash(type)) {
            if (m_mixing && m_mixQueue.empty()) {
                m_mixing = false;
                m_currentPartIndex++;
            }
            m_state = WorkerState::PART_PLANNING;
            return;
        }

        // If we haven't set our goal there, go
        if (!sim.hasGoal()) { sim.setGoal(agents::MakeGoal(*sim.getStationPoint(type))); }
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

    std::vector<data::Material> m_mixQueue;
    bool                        m_mixing = false;
};

}    // namespace scs::agents
