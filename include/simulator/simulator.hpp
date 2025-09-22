#pragma once

#include <common/macros.hpp>
#include <data/agentinfo.hpp>
#include <data/materialinfo.hpp>
#include <data/point.hpp>
#include <memory>
#include <vector>

namespace scs::sim {

class Simulator {
public:
    INTERFACE_CTOR_DTORS(Simulator);

    static auto create() -> std::unique_ptr<Simulator>;

    virtual auto runFrame() -> void = 0;

    [[nodiscard]] virtual auto getRoomCorners() const -> const std::vector<data::Point>&    = 0;
    [[nodiscard]] virtual auto getTargetCorners() const -> const std::vector<data::Point>&  = 0;
    [[nodiscard]] virtual auto getAgentInfos() const -> const std::vector<data::AgentInfo>& = 0;
    [[nodiscard]] virtual auto getMaterialInfos() const
        -> const std::vector<data::MaterialInfo>& = 0;
};

}    // namespace scs::sim
