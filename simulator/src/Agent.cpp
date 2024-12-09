#include "Agent.h"

#include "Simulator.h"
#include "Contract.h"

bool Agent::acceptContract(Contract* contract) {
  if (!contract->claimContract(this)) {
    LOG(WARN) << "Agent " << ID << " tried claiming previously claimed contract " << contract->ID;
    return false;
  }
  return true;
}

bool Agent::working() const { return m_sim->getStep() < m_cooldown_step; }

bool Agent::doJob(Job* job) {
    if (working()) {
      LOG(ERROR) << "Cannot start new job until previous job is complete (cooldown: " << cooldownRemaining() << ")";
      return false;
    }
    if (m_balance < job->cost(m_ability)) {
      LOG(ERROR) << "Cannot do job, balance too low (balance: " << m_balance << ", cost: " << job->cost(m_ability) << ")";
      return false; 
    }
    // Job should either be available or in a contract
    if (m_sim->isJobAvailable(job)) {
      claimJob(job);
    }
    LOG(INFO) << "Agent " << ID << " starting job " << job->ID << " - Cost: " << job->cost(m_ability) << ", Duration: " << job->duration(m_ability) << ", Reward: " << job->reward(m_ability);
    job->started_step = m_sim->getStep();
    job->completed_step = m_sim->getStep() + job->duration(m_ability);
    m_cooldown_step = job->completed_step;
    m_balance -= job->cost(m_ability);
    // Reward will be taken care of by contract if contrcated
    if (job->contracted_step > m_sim->getStep()) {
      m_reward_after_cooldown += job->reward(m_ability);
    }
    return true;
  }

  bool Agent::contractJob(Job* job, unsigned int reward, unsigned int collateral) {
    if (!m_sim->isJobAvailable(job)) {
      LOG(ERROR) << "Tried to contract a claimed job ";
      return false;
    }
    claimJob(job);
    LOG(INFO) << "Agent " << ID << " contracting job " << job->ID;
    m_sim->makeContract(job, reward, collateral, job->deadline_step, this);
    return true;
  }

  unsigned int Agent::cooldownRemaining() const { return m_cooldown_step - m_sim->getStep(); }

void Agent::claimJob(Job* job) {
    LOG(INFO) << "Agent " << ID << " claimed job " << job->ID;
    job->claimed_step = m_sim->getStep();
    m_sim->notifyClaimedJob(job->ID);
  }