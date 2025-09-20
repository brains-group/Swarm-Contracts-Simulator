#pragma once

#include <memory>

#include <common/macros.hpp>
#include <simulator/simulator.hpp>

namespace scs::vis {

class Visualizer {
public:
    INTERFACE_CTOR_DTORS(Visualizer);

    static auto create() -> std::unique_ptr<Visualizer>;

    virtual auto run() -> Result<void> = 0;
};

}    // namespace scs::vis
