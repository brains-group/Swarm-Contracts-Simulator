#pragma once

#include <agents/agentcontroller.hpp>
#include <agents/environmentinterface.hpp>

namespace scs::agents::impl {

class Default : public AgentController {
public:
    auto run(const EnvironmentInterface& sim) -> void override {
        while (sim.canMove()) {
            data::Vector vec{.dir = sim.randomFloat(0, 360), .len = 1};
            if (sim.move(vec)) { break; }
        }
    }
};

}    // namespace scs::agents::impl
