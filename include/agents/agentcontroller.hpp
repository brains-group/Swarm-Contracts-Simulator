#pragma once

#include <agents/environmentinterface.hpp>
#include <common/macros.hpp>

namespace scs::agents {

class AgentController {
public:
    INTERFACE_CTOR_DTORS(AgentController);

    virtual auto run(const EnvironmentInterface& sim) -> void = 0;
};

}    // namespace scs::agents
