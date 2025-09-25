#pragma once

#include <common/macros.hpp>
#include <data/agentinfo.hpp>
#include <data/vector.hpp>
#include <memory>
#include <simulator/agentsimulator.hpp>

namespace scs::agents {

class EnvironmentInterface {
public:
    INTERFACE_CTOR_DTORS(EnvironmentInterface);

    static auto create(sim::AgentSimulator& sim, uint64_t agentID)
        -> std::unique_ptr<EnvironmentInterface>;

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
};

}    // namespace scs::agents
