#include <iostream>

#include "BaseSimulator.h"
#include "Logger.h"

class ExampleSimulator;

class ExampleAgent : public base::Agent {
 public:
  explicit ExampleAgent(unsigned int ID_, unsigned int step)
      : base::Agent(ID_, step) {
    LOG(INFO) << "Agent " << ID << ", reporting for duty!";
  }

  void act(base::Simulator* /*sim*/) override {
    LOG(TRACE) << "I am doing some very important work!";
  }

 private:
};

class LazyAgent : public base::Agent {
 public:
  explicit LazyAgent(unsigned int ID, unsigned int step)
      : base::Agent(ID, step) {
    LOG(INFO) << "Hello World! I am a lazy agent. ZZZzzzzzz....";
  }

  void act(base::Simulator* /*sim*/) override { LOG(TRACE) << "ZZzzzzz..."; }

 private:
};

class ExampleSimulator : public base::Simulator {
 public:
  ExampleSimulator() {
    LOG(INFO) << "Adding some agents...";
    addAgent<ExampleAgent>();
    addAgent<LazyAgent>();
  }

 private:
  void step(unsigned int /* step_num */) override { LOG(DEBUG) << "The world is changing..."; }
  bool shouldTerminate() override { return getStep() > 10; }
};

int main() {
  Logger::addConsole(TRACE);

  ExampleSimulator sim;
  sim.run();
}
