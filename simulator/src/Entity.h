#pragma once

#include <limits>
#include <string>

#include "Logger.h"

class Simulator;

/**
 * Entities are the things that are updated each frame in the simulation.
 *
 * For Swarm Contracts, this consists of Contracts and Agents, however each
 * of these can be broken up further.
 *
 * Some other rules / assumptions about entities:
 * - Every simulation step, each entity receives the input state, and can choose to act upon that
 *   state.
 * - Once created, they are not deleted. They may be "deactivated" and safely ignored, but they
 *   must exist through the lifetime of the program.
 * - Entities may only be constructed by the Simulator class. This is so their ID corresponds with
 *   their position in the vector, and to avoid other problems
 */
class Entity {
 public:
  Entity(unsigned int ID_, Simulator* sim, int m_balance);
  virtual ~Entity() { LOG(WARN) << "DESTROYING ENTITY " << ID; }

  const unsigned int ID;    // Each entity has a unique ID, assigned by the simulator

  /**
   * Returns whether the entity is currently active or not. Entities are not deleted until
   * termination of the simulation, merely marked inactive
   */
  bool isActive() const;

  /**
   * This is run each step by the simulator, and allows the entity to modify the state based on
   * the current state of the simulation.
   */
  virtual void act() = 0;

  /**
   * This function stops an entity from calling `act()` on future steps, effectively "killing" it
   */
  void deactivate();

  /**
   * Pays the entity represented by `ID` the amount in `amount`. This will return false and do
   * nothing if the entity does not have a sufficient balance.
   * 
   * @param recipient_ID The ID of the recipient
   * @param amount The amount to pay
   * 
   * @return Whether the transaction was completed
   */
  bool pay(unsigned int recipient_ID, unsigned int amount);

  bool pay(Entity* recipient, unsigned int amount);

  /**
   * Returns the step the entity was activated
   */
  unsigned int activationStep() const { return m_activation_step; }

  /**
   * Returns the step the entity was deactivated
   */
  unsigned int deactivationStep() const { return m_deactivation_step; }

  /**
   * Returns the current balance of the entity
   */
  unsigned int balance() const { return m_balance; }

  /**
   * Returns the CSV headers for dumping this entity to a file
   *
   * IF CHANGING THIS, ALSO CHANGE `dump_csv` BELOW
   */
  static std::string csv_headers();

  /**
   * Returns a CSV row for dumping this entity to a file
   *
   * IF CHANGING THIS, ALSO CHANGE `csv_headers` ABOVE
   */
  std::string dump_csv() const;

 protected:

  Simulator* m_sim;
  unsigned int       m_balance;

 private:
  const unsigned int m_activation_step;
  unsigned int       m_deactivation_step = std::numeric_limits<unsigned int>::max();

  const unsigned int m_starting_balance;
};
