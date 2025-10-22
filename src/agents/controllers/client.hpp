#pragma once

#include <agents/controller.hpp>
#include <agents/siminterface.hpp>

#include "data/material.hpp"

namespace scs::agents {

class ClientController : public Controller {
public:
    auto run(SimInterface& sim) -> void override {
        // Create a new contract when the posted one is completed
        if (m_contractID == UINT64_MAX) {
            m_contractID = sim.createContract(nextOrder(sim))->getID();
            LOG(INFO) << "Created contract " << m_contractID;
        }

        // Reset current contract if completed
        if (sim.getContract(m_contractID)->isComplete()) { m_contractID = UINT64_MAX; }
    }

    auto nextOrder(SimInterface& sim) -> data::Part {
        switch (sim.getContracts().size() % 3) {
            case 0: return {data::Material::Red};
            case 1: return {data::Material::Green};
            case 2: return {data::Material::Blue};
        }
        std::unreachable();
    }

private:
    uint64_t m_contractID = UINT64_MAX;
};

}    // namespace scs::agents
