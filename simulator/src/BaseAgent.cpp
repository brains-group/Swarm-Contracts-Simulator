#include "BaseAgent.h"

#include "BaseSimulator.h"
#include "Logger.h"

namespace base {

Agent::Agent(unsigned int ID_, Simulator* sim, bool active)
    : ID(ID_)
    , m_sim(sim) {
  if (active) { activate(); }
}

bool Agent::activate() {
  const unsigned int sim_step = m_sim->getStep();
  if (activation_step < sim_step) {
    LOG(WARN) << "Attempted to activate agent " << ID << ", previously activated on step "
              << activation_step;
  } else if (sim_step > deactivation_step) {
    LOG(ERROR) << "Attempted to activate agent " << ID << ", previously deactivated on step "
               << deactivation_step;
  } else {
    LOG(DEBUG) << "Activating agent " << ID;
    activation_step = sim_step;
  }
  return active();
}

bool Agent::active() const {
  return activation_step <= m_sim->getStep() && deactivation_step > m_sim->getStep();
}

}    // namespace base
