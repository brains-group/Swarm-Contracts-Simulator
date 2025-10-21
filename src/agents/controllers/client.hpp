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
            m_contractID = sim.createContract({data::Material::Green})->getID();
        }

        // Reset current contract if completed
        if (sim.getContract(m_contractID)->isComplete()) { m_contractID = UINT64_MAX; }
    }

private:
    uint64_t m_contractID = UINT64_MAX;
};

}    // namespace scs::agents
