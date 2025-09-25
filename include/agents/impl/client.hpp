#pragma once

#include <agents/agentcontroller.hpp>
#include <agents/environmentinterface.hpp>

namespace scs::agents::impl {

class Client : public AgentController {
public:
    auto run(const EnvironmentInterface& sim) -> void override { UNUSED(sim); }
};

}    // namespace scs::agents::impl
