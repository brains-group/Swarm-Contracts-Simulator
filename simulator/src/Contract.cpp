#include "Contract.h"

#include "Agent.h"

#include "Simulator.h"

bool Contract::claimContract(Agent* worker) {
    if (m_worker) { return false; }
    if (!worker->pay(this, collateral)) { return false;}
    return m_worker = worker;
  }

void Contract::act() {
  if (m_sim->getStep() == deadline_step) {
    // Pay out worker if job complete, pay out client if not
    if (m_job->completed_step <= m_sim->getStep()) {
      pay(m_worker, balance());
    } else {
      pay(m_client, balance());
    }
    deactivate();
  }
}