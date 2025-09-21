#pragma once

#include <vector>

#include <common/macros.hpp>
#include <data/point.hpp>

namespace scs::config {

class SimulatorConfig {
public:
    INTERFACE_CTOR_DTORS(SimulatorConfig);

    [[nodiscard]] virtual auto roomCorners() const -> const std::vector<data::Point>&   = 0;
    [[nodiscard]] virtual auto targetCorners() const -> const std::vector<data::Point>& = 0;
};

}    // namespace scs::config
