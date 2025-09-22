#include <agents/environmentinterface.hpp>
#include <simulator/simulator.hpp>

namespace scs::agents {

namespace {

class EnvironmentInterfaceImpl : public EnvironmentInterface {
public:
    EnvironmentInterfaceImpl(sim::Simulator& sim, uint64_t agentID)
        : m_sim(sim)
        , m_agentID(agentID) {}

    [[nodiscard]] auto getLocation() const -> const data::Point& override {
        return m_sim.getAgentInfo(m_agentID).loc;
    }
    [[nodiscard]] auto canMove(const data::Vector& vec) const -> bool override {
        UNUSED(vec);
        return false;
    }

private:
    sim::Simulator& m_sim;
    uint64_t        m_agentID;
};

}    // namespace

auto EnvironmentInterface::create(sim::Simulator& sim, uint64_t agentID)
    -> std::unique_ptr<EnvironmentInterface> {
    return std::make_unique<EnvironmentInterfaceImpl>(sim, agentID);
}

}    // namespace scs::agents
