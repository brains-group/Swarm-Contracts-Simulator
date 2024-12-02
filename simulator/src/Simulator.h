#pragma once

#include <memory>
#include <utility>
#include <vector>
#include "FrameMetrics.h"

class Entity;
class Agent;
class Contract;

/**
 * This is the driver class of the simulator. It manages setup and teardown, as well as the
 * evolution of the state.
 */
class Simulator {
 public:
  /**
   * On startup, the simulator does the following:
   * - Opens logs to "data/YYYY-MM-DD_HH-MM-SS.XXX/info.log". Enabling logging elsewhere should not
   *   cause any problems
   */
  Simulator();

  /**
   * On teardown, the simulator does the following:
   * - Dumps frame metrics to "data/YYYY-MM-DD_HH-MM-SS.XXX/metrics.csv"
   */
  ~Simulator();

  /**
   * This method runs the simulation. The main loop does the following:
   * - Check the `shouldTerminate()` function, and break if this returns true
   * - Updates the entities in the order they were created. This is done by:
   *    - Calling `isActive()` on the entity, and skipping if this is false
   *    - Calling `act()` on the entity, providing a const reference to the current state
   * - Updates the state of the simulatioon by calling `step()`
   * - Increments the step number, and repeats the loop
   */
  void run();

  /**
   * Returns the current step number of the simulation
   */
  unsigned int getStep() const { return m_frameMetrics.size() - 1; }

  /**
   * Creates a new entity
   */
  template <typename T>
    requires std::is_base_of_v<Entity, T>
  T* makeEntity();

 private:
  std::vector<FrameMetrics>            m_frameMetrics;    // Data relevant to each simulation frame
  std::vector<std::unique_ptr<Entity>> m_entities;        // The entities being simulated
  std::vector<Agent*>                  m_agents;
  std::vector<Contract*>               m_contracts;

  // THE BELOW METHODS NEED TO BE IMPLEMENTED BY THE USER ------------------------------------------
  /**
   * Inheritance gets messy with this class, but since only one simulator type should be needed
   * (as opposed to different strategies for agents) inheritance shouldn't be necessary.
   */

  /**
   * This is used for initial setup of the simulation. It is the first thing done when `run()` is
   * called. Favor this over doing things in the constructor (exceptions in constructors are bad)
   */
  void init();

  /**
   * This method is used to terminate the simulation. It allows the freedom to terminate on a
   * certain step, once all etities are inactive, or some other condition.
   *
   * This method MUST be implemented by children classes
   */
  bool shouldTerminate() const;

  /**
   * This method is used to increment the simulation. Outside this function, the entities may
   * update the state by calling `act()`, so this is not the only way to update the state.
   */
  void step();
};

template <typename T>
  requires std::is_base_of_v<Entity, T>
T* Simulator::makeEntity() {
  std::unique_ptr<T> entity = std::make_unique<T>(m_entities.size(), this);
  if constexpr (std::is_base_of_v<Agent, T>) {
    ++m_frameMetrics.back().agents_created;
    m_agents.emplace_back(entity.get());
  } else if constexpr (std::is_base_of_v<Contract, T>) {
    ++m_frameMetrics.back().contracts_created;
    m_contracts.emplace_back(entity.get());
  } else {
    std::unreachable();
  }
  T* entity_raw = entity.get();
  m_entities.emplace_back(std::move(entity));
  return entity_raw;
}
