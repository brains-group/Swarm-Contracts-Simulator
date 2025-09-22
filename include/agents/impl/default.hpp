#pragma once

#include <agents/agentcontroller.hpp>
#include <agents/environmentinterface.hpp>

namespace scs::agents::impl {

class Default : public AgentController {
public:
    auto run(const EnvironmentInterface& sim) -> void override { UNUSED(sim); }
};

};    // namespace scs::agents::impl
