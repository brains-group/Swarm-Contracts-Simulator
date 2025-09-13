#pragma once

#include <common/macros.hpp>
#include <memory>
#include <simulator/config.hpp>
#include <simulator/simulator.hpp>

namespace vis {

class Visualizer {
public:
    INTERFACE_CTOR_DTORS(Visualizer);

    static auto create() -> std::unique_ptr<Visualizer>;

    virtual auto run() -> Result<void> = 0;
};

}    // namespace vis
