#include <agents/environmentinterface.hpp>
#include <simulator/simulator.hpp>

namespace scs::agents {

namespace {

class EnvironmentInterfaceImpl : public EnvironmentInterface {
public:
    EnvironmentInterfaceImpl(sim::Simulator& sim, uint64_t agentID)
        : m_sim(sim)
        , m_agentID(agentID) {
        UNUSED(m_sim);
        UNUSED(m_agentID);
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
