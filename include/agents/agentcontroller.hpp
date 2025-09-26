#pragma once

#include <agents/agentsiminterface.hpp>
#include <common/macros.hpp>

namespace scs::agents {

class AgentController {
public:
    INTERFACE_CTOR_DTORS(AgentController);

    virtual auto run(AgentSimInterface& sim) -> void = 0;
};

}    // namespace scs::agents
