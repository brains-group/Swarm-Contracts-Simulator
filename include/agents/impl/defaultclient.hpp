#pragma once

#include <agents/clientcontroller.hpp>
#include <agents/clientsiminterface.hpp>

namespace scs::agents::impl {

class DefaultClient : public ClientController {
public:
    auto run(ClientSimInterface& sim) -> void override {
        sim.placeOrder({.r = 255, .g = 0, .b = 0});
        sim.placeOrder({.r = 0, .g = 255, .b = 0});
        sim.placeOrder({.r = 0, .g = 0, .b = 255});
    }
};

}    // namespace scs::agents::impl
