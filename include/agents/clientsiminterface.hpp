#pragma once

#include <common/macros.hpp>
#include <data/color.hpp>
#include <simulator/clientsimulator.hpp>

namespace scs::agents {

class ClientSimInterface {
public:
    INTERFACE_CTOR_DTORS(ClientSimInterface);

    static auto create(sim::ClientSimulator& sim, uint64_t clientID)
        -> std::unique_ptr<ClientSimInterface>;

    virtual auto placeOrder(const data::Color& order) const -> void = 0;
};

}    // namespace scs::agents
