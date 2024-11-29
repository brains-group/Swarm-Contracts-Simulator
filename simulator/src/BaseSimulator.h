#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <ranges>
#include <vector>

#include "Logger.h"

namespace base {

class Simulator;

class Agent {
 public:
  // BASE IMPLEMENTATION

  explicit Agent(unsigned int ID_, unsigned int step)
      : ID(ID_)
      , activation_step(step) {}
  virtual ~Agent() {}

  const unsigned int ID;
  const unsigned int activation_step;

  // METHODS TO OVERRIDE

  /**
   * Called each simulation step for each agent, giving the agent a chance to act
   *
   * @param sim A pointer to the simulator, used to retrieve the current state to act on
   */
  virtual void act(Simulator* sim) = 0;

 private:
};

class Simulator {
 public:
  // BASE IMPLEMENTATION

  Simulator();
  virtual ~Simulator();

  void run();

 protected:
  template <class AgentType> AgentType* addAgent();
  template <class AgentType> AgentType* getAgent(unsigned int index);
  auto                                  getAgents() {
    return m_agents | std::ranges::views::transform(std::mem_fn(&std::unique_ptr<Agent>::get));
  }
  unsigned int getStep() const;

 private:
  unsigned int                        m_step_num = 0;
  std::vector<std::unique_ptr<Agent>> m_agents   = {};

  // METHODS TO OVERRIDE

  /**
   * Called each step to update the state of the simulation.
   * This is called AFTER agents act on the state for the current step.
   *
   * @param step_num The current step being simulated
   */
  virtual void step(unsigned int step_num) = 0;

  /**
   * The exit condition for the simulator. When this returns true, the simulator
   * will stop running. This is checked once per step, after updating the state.
   */
  virtual bool shouldTerminate() = 0;
};

// TEMPLATE IMPLEMTNATIONS

template <class AgentType> AgentType* Simulator::addAgent() {
  LOG(INFO) << "Adding agent with ID " << m_agents.size();
  return dynamic_cast<AgentType*>(
      m_agents.emplace_back(std::make_unique<AgentType>(m_agents.size(), m_step_num)).get());
}

template <class AgentType> AgentType* Simulator::getAgent(unsigned int index) {
  return dynamic_cast<AgentType*>(m_agents[index]);
}

}    // namespace base
