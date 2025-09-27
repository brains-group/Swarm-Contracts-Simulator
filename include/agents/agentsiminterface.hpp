#pragma once

#include <memory>
#include <optional>

#include <common/macros.hpp>
#include <data/agentinfo.hpp>
#include <data/color.hpp>
#include <data/vector.hpp>
#include <simulator/agentsimulator.hpp>

namespace scs::agents {

class AgentSimInterface {
public:
    INTERFACE_CTOR_DTORS(AgentSimInterface);

    static auto create(sim::AgentSimulator& sim, uint64_t agentID)
        -> std::unique_ptr<AgentSimInterface>;

    // UTIL
    [[nodiscard]] virtual auto randomFloat(float min, float max) const -> float = 0;
    [[nodiscard]] virtual auto randomInt(int min, int max) const -> int         = 0;
    [[nodiscard]] virtual auto frameNumber() const -> unsigned int              = 0;
    [[nodiscard]] virtual auto getID() const -> uint64_t                        = 0;

    // LOCATION & MOVEMENT
    [[nodiscard]] virtual auto getLocation() const -> const data::Point&      = 0;
    [[nodiscard]] virtual auto canMove() const -> bool                        = 0;
    [[nodiscard]] virtual auto canMove(const data::Vector& vec) const -> bool = 0;
    [[nodiscard]] virtual auto move(const data::Vector& vec) const -> bool    = 0;

    // ORDERS
    [[nodiscard]] virtual auto holdingOrder() const -> bool                              = 0;
    [[nodiscard]] virtual auto currentOrder() const -> const std::optional<data::Color>& = 0;
    [[nodiscard]] virtual auto takeNextOrder() -> const std::optional<data::Color>&      = 0;
    [[nodiscard]] virtual auto collectOrder() -> bool                                    = 0;
    [[nodiscard]] virtual auto dropOrder() -> bool                                       = 0;
    [[nodiscard]] virtual auto getTargetBearing() const -> data::Vector                  = 0;
    [[nodiscard]] virtual auto getMaterialBearing(const data::Color& color) const
        -> data::Vector = 0;
};

}    // namespace scs::agents
