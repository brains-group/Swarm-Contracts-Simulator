#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <ranges>
#include <vector>

#include "BaseAgent.h"
#include "BaseContract.h"
#include "Logger.h"

namespace base {

class Simulator {
 public:
  // BASE IMPLEMENTATION

  Simulator();
  virtual ~Simulator();

  void run();

  // UTILITY METHODS
  unsigned int getStep() const;

 protected:
  template <class AgentType> AgentType* addAgent(bool active = true);

  template <class ContractType> ContractType* addContract();

 private:
  unsigned int                           m_step_num  = 0;
  std::vector<std::unique_ptr<Agent>>    m_agents    = {};
  std::vector<std::unique_ptr<Contract>> m_contracts = {};

  // METHODS TO OVERRIDE

  /**
   * Called each step to update the state of the simulation.
   * This is called AFTER agents act on the state for the current step.
   */
  virtual void step() = 0;

  /**
   * The exit condition for the simulator. When this returns true, the simulator
   * will stop running. This is checked once per step, after updating the state.
   */
  virtual bool shouldTerminate() const = 0;
};

// TEMPLATE IMPLEMTNATIONS

template <class AgentType> AgentType* Simulator::addAgent(bool active) {
  LOG(INFO) << "Adding agent with ID " << m_agents.size();
  return dynamic_cast<AgentType*>(
      m_agents.emplace_back(std::make_unique<AgentType>(m_agents.size(), this, active)).get());
}

template <class ContractType> ContractType* Simulator::addContract() {
  LOG(INFO) << "Adding contract with ID " << m_contracts.size();
  return dynamic_cast<ContractType*>(
      m_contracts.emplace_back(std::make_unique<ContractType>(m_contracts.size(), this)).get());
}

}    // namespace base
