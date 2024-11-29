#include <iostream>

#include "BaseAgent.h"
#include "BaseContract.h"
#include "BaseSimulator.h"
#include "Logger.h"

class ExampleContract : public base::Contract {
 public:
  ExampleContract(unsigned int ID_, base::Simulator* sim)
      : base::Contract(ID_, sim) {
    LOG(INFO) << "Contract " << ID << " has been created";
  }

  void execute() override { LOG(INFO) << "I AM CONTRACT " << ID << " AND I AM BEING EXECUTED!!"; }

  bool shouldExecute() override { return m_sim->getStep() == 6; }
};

class ExampleAgent : public base::Agent {
 public:
  explicit ExampleAgent(unsigned int ID_, base::Simulator* sim, bool active)
      : base::Agent(ID_, sim, active) {
    LOG(INFO) << "Agent " << ID << ", reporting for duty!";
  }

  void act() override { LOG(TRACE) << "I am doing some very important work!"; }

 private:
};

class LazyAgent : public base::Agent {
 public:
  explicit LazyAgent(unsigned int ID, base::Simulator* sim, bool active)
      : base::Agent(ID, sim, active) {
    LOG(INFO) << "Hello World! I am a lazy agent. ZZZzzzzzz....";
  }

  void act() override { LOG(TRACE) << "ZZzzzzz..."; }

 private:
};

class ExampleSimulator : public base::Simulator {
 public:
  ExampleSimulator() {
    LOG(INFO) << "Adding some agents...";
    addAgent<ExampleAgent>();
  }

 private:
  void step() override {
    LOG(DEBUG) << "The world is changing...";
    if (getStep() == 3) {
      LOG(INFO) << "Creating a contract";
      addContract<ExampleContract>();
    }

    if (getStep() == 5) {
      LOG(INFO) << "Adding a lazy agent";
      addAgent<LazyAgent>();
    }
  }
  [[nodiscard]] bool shouldTerminate() const override { return getStep() >= 10; }
};

int main() {
  Logger::addConsole(TRACE);

  ExampleSimulator sim;
  sim.run();
}
