#pragma once

#include <functional>
#include <memory>
#include <ranges>
#include <utility>
#include <vector>

#include "Entity.h"
#include "FrameMetrics.h"
#include "Job.h"
#include "Agent.h"
#include "Contract.h"

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
   * Creates a new Agent
   *
   * @param args The arguments to forward to the Agent constructor
   *
   * @return A pointer to the newly created agent
   */
  template <typename AgentType, typename... Args>
    requires std::is_base_of_v<Agent, AgentType>
  AgentType* makeAgent(Args&&... args);

  /**
   * Creates a new Contract
   *
   * @param args The arguments to forward to the Contract constructor
   *
   * @return A pointer to the newly created contract
   */
  template <typename... Args> Contract* makeContract(Args&&... args);

  /**
   * Creates a new job
   */
  template <typename... Args> Job* makeJob(Args&&... args);

  /**
   * Checks if a given job has been claimed
   */
  bool isJobAvailable(const Job* job) const { return getStep() < job->claimed_step; }

  /**
   * Reports the available jobs
   */
  auto availableJobs() const {
    return m_jobs | std::views::transform([](const auto& job) { return job.get(); })
         | std::views::filter([this](const Job* job) { return isJobAvailable(job); });
  }

  /**
   * Checks if a given contract has been claimed
   */
  bool isContractAvailable(const Contract* contract) const {
    return isJobAvailable(contract->getJob());
  }

  /**
   * Reports the available contracts
   */
  auto availableContracts() const {
    return m_contracts
         | std::views::filter([this](const Contract* c) { return isContractAvailable(c); });
  }

  /**
   * Gets a pointer to an existing entity
   *
   * @param ID The ID of an entity to be returned
   *
   * @return A pointer to the entity with the given ID
   */
  Entity* getEntity(unsigned int ID) {
    return ID < m_entities.size() ? m_entities[ID].get()
                                  : m_newEntities[ID - m_entities.size()].get();
  }

  /**
   * Updtaes the metrics relevant to deactivating entities
   *
   * @param ID The ID of the entity to be deactivated
   */
  void notifyDeactivated(unsigned int ID);

  /**
   * Updates the metrics relevant to entity payments
   *
   * @param source_ID The ID of the entity making the payment
   * @param dest_ID The ID of the entity receiving the payment
   * @param amount The total value of the payment
   */
  void notifyPayment(unsigned int source_ID, unsigned int dest_ID, unsigned int amount);

  /**
   * Updates the metrics relevant to claiming jobs
   */
  void notifyClaimedJob(unsigned int job_ID);

 private:
  std::vector<FrameMetrics> m_frameMetrics;           // Data relevant to each simulation frame

  std::vector<std::unique_ptr<Job>> m_jobs;           // The jobs being simulated

  std::vector<std::unique_ptr<Entity>> m_entities;    // The entities being simulated

  // Entities which were created during the current simulation frame
  // Messy problems with iterating over a vector while we add to it
  std::vector<std::unique_ptr<Entity>> m_newEntities;
  /**
   * The below are all managed by the `m_entities` vector above. This provides a more convenient
   * interface to collecting common types
   */
  std::vector<Contract*> m_contracts;    // The contracts being simulated
  std::vector<Agent*>    m_agents;       // The agents being simulated

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

template <typename AgentType, typename... Args>
  requires std::is_base_of_v<Agent, AgentType>
AgentType* Simulator::makeAgent(Args&&... args) {
  m_newEntities.emplace_back(std::make_unique<AgentType>(m_entities.size() + m_newEntities.size(),
                                                         this, std::forward<Args>(args)...));
  AgentType* agent_raw = dynamic_cast<AgentType*>(m_newEntities.back().get());
  m_agents.emplace_back(agent_raw);
  ++m_frameMetrics.back().agents_created;
  m_frameMetrics.back().created_value += m_newEntities.back()->balance();
  return agent_raw;
}

template <typename... Args> Contract* Simulator::makeContract(Args&&... args) {
  std::unique_ptr<Entity>& contract = m_newEntities.emplace_back(std::make_unique<Contract>(
      m_entities.size() + m_newEntities.size(), this, std::forward<Args>(args)...));
  ++m_frameMetrics.back().contracts_created;
  Contract* contract_raw = dynamic_cast<Contract*>(contract.get());
  contract_raw->getClient()->pay(contract_raw, contract_raw->collateral);
  return m_contracts.emplace_back(contract_raw);
}

template <typename... Args> Job* Simulator::makeJob(Args&&... args) {
  std::unique_ptr<Job>& job = m_jobs.emplace_back(
      std::make_unique<Job>(m_jobs.size(), getStep(), std::forward<Args>(args)...));
  ++m_frameMetrics.back().jobs_created;
  return job.get();
}
