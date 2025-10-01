#pragma once

#include <agents/agentcontroller.hpp>
#include <agents/agentsiminterface.hpp>

namespace scs::agents::impl {

class DefaultWorker : public AgentController {
public:
    auto run(AgentSimInterface& sim) -> void override {
        if (!sim.currentOrder() && !sim.takeNextOrder()) { return; }

        data::Color order = sim.currentOrder().value();

        data::Vector bearing =
            sim.holdingOrder() ? sim.getTargetBearing() : sim.getMaterialBearing(order);
        if (bearing.len < 1) {
            auto _ = sim.holdingOrder() ? sim.dropOrder() : sim.collectOrder();
        } else {
            bearing.len = 1;
            auto _      = sim.move(bearing);
        }
    }
};

}    // namespace scs::agents::impl
