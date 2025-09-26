#pragma once

#include <agents/clientsiminterface.hpp>
#include <common/macros.hpp>

namespace scs::agents {

class ClientController {
public:
    INTERFACE_CTOR_DTORS(ClientController);

    virtual auto run(ClientSimInterface& sim) -> void = 0;
};

}    // namespace scs::agents
