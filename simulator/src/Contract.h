#pragma once

#include <sstream>

#include "Entity.h"
#include "Job.h"

class Agent;

class Contract : public Entity {
 public:
  Contract(unsigned int ID_, Simulator* sim, Job* job, unsigned int reward_,
           unsigned int collateral_, unsigned int deadline_step_, Agent* client)
      : Entity(ID_, sim, 0)
      , reward(reward_)
      , collateral(collateral_)
      , deadline_step(deadline_step_)
      , m_job(job)
      , m_client(client) {
    LOG(INFO) << "Created contract " << ID << " with job " << m_job->ID;
  }

  bool claimContract(Agent* worker);

  void act() override;

  Job*       getJob() { return m_job; }
  const Job* getJob() const { return m_job; }

  Agent* getClient() { return m_client; }
  Agent* getWorker() { return m_worker; }

  static std::string csv_headers() {
    std::ostringstream ss;
    ss << Entity::csv_headers();
    return ss.str();
  }

  std::string dump_csv() const {
    std::ostringstream ss;
    ss << Entity::dump_csv();
    return ss.str();
  }

  const unsigned int reward;
  const unsigned int collateral;
  const unsigned int deadline_step;

 private:
  Job* m_job;

  Agent* m_client;              // The creator of the contract
  Agent* m_worker = nullptr;    // The acceptor of the contract

  // Adjudicators TBD...
};
