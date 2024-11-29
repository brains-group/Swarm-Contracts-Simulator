#pragma once

#include "Logger.h"

namespace base {

class Simulator;

class Agent {
 public:
  // BASE IMPLEMENTATION

  Agent(unsigned int ID_, Simulator* sim, bool active);
  virtual ~Agent() {}

  const unsigned int ID;

  // METHODS TO OVERRIDE

  /**
   * Called each simulation step for each agent, giving the agent a chance to act
   */
  virtual void act() = 0;

  // UTILITY METHODS

  bool activate();
  bool active() const;

 private:
  unsigned int activation_step   = std::numeric_limits<unsigned int>::max();
  unsigned int deactivation_step = std::numeric_limits<unsigned int>::max();

  Simulator* m_sim;
};
}    // namespace base
