#include <agents/environmentinterface.hpp>
#include <cstdlib>
#include <simulator/agentsimulator.hpp>

namespace scs::agents {

namespace {

class EnvironmentInterfaceImpl : public EnvironmentInterface {
public:
    EnvironmentInterfaceImpl(sim::AgentSimulator& sim, uint64_t agentID)
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

private:
    sim::AgentSimulator& m_sim;
    uint64_t             m_agentID;

    unsigned int m_lastMoveFrame = 0;
};

}    // namespace

auto EnvironmentInterface::create(sim::AgentSimulator& sim, uint64_t agentID)
    -> std::unique_ptr<EnvironmentInterface> {
    return std::make_unique<EnvironmentInterfaceImpl>(sim, agentID);
}

}    // namespace scs::agents
