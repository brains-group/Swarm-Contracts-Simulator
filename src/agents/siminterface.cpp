#include <agents/agent.hpp>
#include <agents/siminterface.hpp>
#include <simulator/simulator.hpp>

namespace scs::agents {

SimInterface::SimInterface(sim::Simulator& sim, Agent& agent)
    : m_sim(sim)
    , m_agent(agent) {}

}    // namespace scs::agents
