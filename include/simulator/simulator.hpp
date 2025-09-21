#pragma once

#include <memory>
#include <vector>

#include <common/macros.hpp>
#include <data/point.hpp>

namespace scs::sim {

class Simulator {
public:
    INTERFACE_CTOR_DTORS(Simulator);

    static auto create() -> std::unique_ptr<Simulator>;

    virtual auto load() -> Result<void> = 0;
    virtual auto runFrame() -> void     = 0;

    [[nodiscard]] virtual auto getRoomCorners() const -> const std::vector<data::Point>&   = 0;
    [[nodiscard]] virtual auto getTargetCorners() const -> const std::vector<data::Point>& = 0;
};

}    // namespace scs::sim
