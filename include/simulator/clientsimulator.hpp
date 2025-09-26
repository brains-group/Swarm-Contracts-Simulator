#pragma once

#include <common/macros.hpp>
#include <data/color.hpp>
#include <simulator/simulator.hpp>

namespace scs::sim {

class ClientSimulator : virtual public Simulator {
public:
    INTERFACE_CTOR_DTORS(ClientSimulator);

    virtual auto placeOrder(uint64_t clientID, const data::Color& color) -> void = 0;
};

}    // namespace scs::sim
