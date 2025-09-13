#pragma once

#include <common/macros.hpp>
#include <memory>
#include <simulator/agent.hpp>
#include <simulator/config.hpp>
#include <vector>

namespace sim {

class Simulator {
public:
    INTERFACE_CTOR_DTORS(Simulator);

    static auto create(std::shared_ptr<const sim::Config> config) -> std::unique_ptr<Simulator>;

    virtual auto runFrame() -> void = 0;

    virtual auto getConfig() -> const Config&             = 0;
    virtual auto getAgents() -> const std::vector<Agent>& = 0;
};

}    // namespace sim
