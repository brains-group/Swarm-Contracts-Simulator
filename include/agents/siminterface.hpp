#pragma once

#include <common/macros.hpp>

namespace scs {

namespace sim {
class Simulator;
}

namespace agents {

class Agent;

class SimInterface {
public:
    SimInterface(sim::Simulator& sim, Agent& agent);

    DELETE_COPY(SimInterface);
    DELETE_MOVE(SimInterface);
    DEFAULT_DTOR(SimInterface);

private:
    sim::Simulator& m_sim;
    Agent&          m_agent;
};

}    // namespace agents

}    // namespace scs
