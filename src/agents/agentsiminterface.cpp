#include <cstdlib>

#include <agents/agentsiminterface.hpp>
#include <simulator/agentsimulator.hpp>

#include "data/color.hpp"
#include "data/materialinfo.hpp"
#include "data/point.hpp"
#include "data/vector.hpp"

namespace scs::agents {

namespace {

class AgentSimInterfaceImpl : public AgentSimInterface {
public:
    AgentSimInterfaceImpl(const AgentSimInterfaceImpl&)            = delete;
    AgentSimInterfaceImpl(AgentSimInterfaceImpl&&)                 = delete;
    AgentSimInterfaceImpl& operator=(const AgentSimInterfaceImpl&) = delete;
    AgentSimInterfaceImpl& operator=(AgentSimInterfaceImpl&&)      = delete;
    AgentSimInterfaceImpl(sim::AgentSimulator& sim, uint64_t agentID)
        : m_sim(sim)
        , m_agentID(agentID) {}

    [[nodiscard]] auto randomFloat(float min, float max) const -> float override {
        // TODO: Use c++ random for this
        // NOLINTNEXTLINE
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (max - min));
    }

    [[nodiscard]] auto randomInt(int min, int max) const -> int override {
        // TODO: Use c++ random for this
        // NOLINTNEXTLINE
        return min + rand() % (max - min);
    }

    [[nodiscard]] auto frameNumber() const -> unsigned int override {
        return m_sim.getFrameNumber();
    }

    [[nodiscard]] auto getID() const -> uint64_t override { return m_agentID; }

    [[nodiscard]] auto getLocation() const -> const data::Point& override {
        return m_sim.getAgentInfo(m_agentID).loc;
    }

    [[nodiscard]] auto canMove() const -> bool override {
        return m_lastMoveFrame < m_sim.getFrameNumber();
    }

    [[nodiscard]] auto canMove(const data::Vector& vec) const -> bool override {
        // TODO: Collisions (yuck)
        return canMove() && vec.len <= 1;
    }

    [[nodiscard]] auto move(const data::Vector& vec) const -> bool override {
        if (!canMove(vec)) { return false; }
        return m_sim.moveAgent(m_agentID, vec);
    }

    auto placeOrder(const data::Color& order) const -> void override {
        m_sim.placeOrder(m_agentID, order);
    }

    [[nodiscard]] auto holdingOrder() const -> bool override { return m_holdingOrder; }

    [[nodiscard]] auto currentOrder() const -> const std::optional<data::Color>& override {
        return m_currentOrder;
    }

    [[nodiscard]] auto takeNextOrder() -> const std::optional<data::Color>& override {
        m_currentOrder = m_sim.takeNextOrder(m_agentID);
        return m_currentOrder;
    }

    [[nodiscard]] auto collectOrder() -> bool override {
        if (m_currentOrder.has_value() && !holdingOrder()
            && getMaterialBearing(m_currentOrder.value()).len < 1) {
            m_holdingOrder = true;
            return true;
        }
        return false;
    }

    [[nodiscard]] auto dropOrder() -> bool override {
        if (m_currentOrder.has_value() && holdingOrder() && getTargetBearing().len < 1) {
            m_currentOrder = std::nullopt;
        }
        m_holdingOrder = false;
        return true;
    }

    // TODO: These methods (get bearing) will fail if the thing you are looking for isn't found)
    [[nodiscard]] auto getTargetBearing() const -> data::Vector override {
        const data::Point& agentLoc = m_sim.getAgentInfo(m_agentID).loc;

        data::Vector bearing{.dir = 0, .len = std::numeric_limits<float>::max()};
        for (const auto& corner : m_sim.getTargetCorners()) {
            data::Vector candidate = data::toVec(corner - agentLoc);
            if (candidate.len < bearing.len) { bearing = candidate; }
        }
        return bearing;
    }

    [[nodiscard]] auto getMaterialBearing(const data::Color& color) const -> data::Vector override {
        const data::Point& agentLoc = m_sim.getAgentInfo(m_agentID).loc;
        data::Vector       bearing{.dir = 0, .len = std::numeric_limits<float>::max()};
        for (const auto& material : m_sim.getMaterialInfos()) {
            if (material.color != color) { continue; }
            data::Vector candidate = data::toVec(material.loc - agentLoc);
            if (candidate.len < bearing.len) { bearing = candidate; }
        }
        return bearing;
    }

private:
    sim::AgentSimulator&       m_sim;
    uint64_t                   m_agentID;
    std::optional<data::Color> m_currentOrder;
    bool                       m_holdingOrder;

    unsigned int m_lastMoveFrame = 0;
};

}    // namespace

auto AgentSimInterface::create(sim::AgentSimulator& sim, uint64_t agentID)
    -> std::unique_ptr<AgentSimInterface> {
    return std::make_unique<AgentSimInterfaceImpl>(sim, agentID);
}

}    // namespace scs::agents
