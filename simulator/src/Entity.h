#pragma once

#include <limits>

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

    Entity(unsigned int ID_, Simulator* sim);
    virtual ~Entity() = default;

    const unsigned int ID; // Each entity has a unique ID, assigned by the simulator

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

    unsigned int activationStep() const { return m_activation_step; }
    unsigned int deactivationStep() const { return m_deactivation_step; }

  protected:


  private:

    Simulator* m_sim;

    const unsigned int m_activation_step;
    unsigned int m_deactivation_step = std::numeric_limits<unsigned int>::max();

};