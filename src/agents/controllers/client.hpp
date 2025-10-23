#pragma once

#include <agents/controller.hpp>
#include <agents/siminterface.hpp>

#include "data/material.hpp"

namespace scs::agents {

class ClientController : public Controller {
public:
    auto run(SimInterface& sim) -> void override {
        // We want to maintain 5 uncompleted contracts at all times
        int num = std::ranges::count_if(
            sim.getContracts(), [](const auto& contract) { return !contract->isComplete(); });

        while (num++ < 5) {
            auto contractPtr = sim.createContract(nextOrder(sim));
            LOG(INFO) << "Created contract " << contractPtr->getID();
        }
    }

    auto nextOrder(SimInterface& sim) -> data::Part {
        switch (sim.getContracts().size() % 4) {
            case 0:
                return {data::Material::Red, data::Material::Blue, data::Material::Green,
                        data::Material::Yellow};
            case 1: return {data::Material::Green, data::Material::Blue, data::Material::Magenta};
            case 2: return {data::Material::Yellow};
            case 3: return {data::Material::Cyan, data::Material::Red};
        }
        std::unreachable();
    }

private:
};

}    // namespace scs::agents
