#pragma once

#include <iostream>
#include <string>

#include "Entity.h"
#include "Logger.h"
#include "Job.h"

class Simulator;
class Contract;

class Agent : public Entity {
 public:
  Agent(unsigned int ID_, Simulator* sim, int balance_,
        const std::array<unsigned int, DIM>& ability_vec)
      : Entity(ID_, sim, balance_)
      , m_ability(ability_vec) {
    LOG(INFO) << "Created agent " << ID << " with starting balance " << balance() << " and ability " << m_ability;
  }

  bool working() const;

  bool doJob(Job* job);

  bool acceptContract(Contract* contract);

  bool contractJob(Job* job, unsigned int reward, unsigned int collateral);

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
  const std::array<unsigned int, DIM> m_ability = {0};

  unsigned int cooldownRemaining() const;

 private:
  unsigned int m_cooldown_step = 0;
  unsigned int m_reward_after_cooldown = 0;

  void claimJob(Job* job);

  friend class Simulator;

};
