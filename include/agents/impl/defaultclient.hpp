#pragma once

#include <agents/agentcontroller.hpp>
#include <agents/agentsiminterface.hpp>

namespace scs::agents::impl {

class DefaultClient : public AgentController {
public:
    auto run(AgentSimInterface& sim) -> void override {
        sim.placeOrder({.r = 255, .g = 0, .b = 0});
        sim.placeOrder({.r = 0, .g = 255, .b = 0});
        sim.placeOrder({.r = 0, .g = 0, .b = 255});
    }
};

}    // namespace scs::agents::impl
