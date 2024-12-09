#include <numeric>

#include "Agent.h"
#include "Contract.h"
#include "Job.h"
#include "Logger.h"
#include "Simulator.h"

class ExampleAgent : public Agent {
 public:
  ExampleAgent(unsigned int ID_, Simulator* sim, int balance,
               const std::array<unsigned int, DIM>& ability)
      : Agent(ID_, sim, balance, ability) {}

  void act() {
    // Check for available jobs
    for (Job* job : m_sim->availableJobs()) {
      unsigned int reward   = job->reward(m_ability);
      unsigned int cost     = job->cost(m_ability);
      unsigned int duration = job->duration(m_ability);

      LOG(DEBUG) << "Agent " << ID << " Analyzing Job " << job->ID << " Reward: " << reward
                 << " Cost: " << cost << " Duration: " << duration;

      // Do the job if
      if (reward > cost                  // The job is profitable for the agent
          && m_sim->getStep() + duration
                 < job->deadline_step    // The agent can finish the job before the deadline
          && balance() > cost            // The agent can afford to do the job
          && !working()                  // The agent is not currently doing another job
      ) {
        doJob(job);
      }
      // Make a contract for the job if
      else if (reward > 2                      // Reward is sufficiently high (value is arbitrary)
               && balance() > (reward >> 1)    // Can afford to make a contract
      ) {
        contractJob(job, reward >> 1, reward >> 1);
      }
    }

    if (working()) {
      return;
    }

    // If not working, look at contracts for available jobs
    for (Contract* contract : m_sim->availableContracts()) {
      Job* job = contract->getJob();
      unsigned int reward   = contract->reward;
      unsigned int cost     = job->cost(m_ability);
      unsigned int duration = job->duration(m_ability);

      // Accept the contract if
      if (reward > cost                  // The job is profitable for the agent
          && m_sim->getStep() + duration
                 < contract->deadline_step    // The agent can finish the job before the deadline
          && balance() > cost + contract->collateral            // The agent can afford to do the job + collateral
          && !working()                  // The agent is not currently doing another job
      ) {
        acceptContract(contract);
        doJob(job);
      }
    }
  }
};

// These methods need to be implemented by the user, these are just simple examples
void Simulator::init() {
  LOG(INFO) << "Simulator init";
  std::array<unsigned int, DIM> client_vec = {3, 0};
  makeAgent<ExampleAgent>(100, client_vec);
  std::array<unsigned int, DIM> worker_vec = {0, 3};
  makeAgent<ExampleAgent>(100, worker_vec);
  std::array<unsigned int, DIM> contract_job_vec = {0, 4};
  makeJob(5, contract_job_vec);
}

bool Simulator::shouldTerminate() const { return getStep() == 10; }

void Simulator::step() {
  if (getStep() == 5) {
    std::array<unsigned int, DIM> job_vec = {0, 1};    // Easy job that an agent will do
    makeJob(100, job_vec);
  }
}

/*
  Below are a set of example cost and reward functions. This example calculates cost and reward from
  the itemwise difference between the job vector and the ability vector of an Agent.
*/

unsigned int Job::cost(const std::array<unsigned int, DIM>& ability) const {
  unsigned int cost = 0;
  for (unsigned int i = 0; i < DIM; i++) {
    if (job_vector[i] > ability[i]) { cost += job_vector[i] - ability[i]; }
  }
  return cost;
}

unsigned int Job::reward(const std::array<unsigned int, DIM>& ability) const {
  unsigned int reward = 0;
  for (unsigned int i = 0; i < DIM; i++) {
    if (ability[i] > job_vector[i]) { reward += ability[i] - job_vector[i]; }
  }
  return reward;
}

/*
  An example duration function is below. It simply finds the sum of elements in the job vector
*/
unsigned int Job::duration(const std::array<unsigned int, DIM>& /* ability */) const {
  return std::accumulate(job_vector.begin(), job_vector.end(), 0);
}

int main() {
  Logger::addConsole(INFO);

  Simulator sim;
  sim.run();
}
