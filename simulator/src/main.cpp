#include "Logger.h"
#include "Simulator.h"

#include "Agent.h"
#include "Contract.h"

class ExampleAgent : public Agent {
  public:
    ExampleAgent(unsigned int ID_, Simulator* sim) : Agent(ID_, sim) {}

    void act() {}
};


// These three methods need to be implemented
void Simulator::init() {
  LOG(INFO) << "Simulator init";
  makeEntity<ExampleAgent>();
}

bool Simulator::shouldTerminate() const {
  return getStep() == 10;
}

void Simulator::step() {
  LOG(INFO) << "Big stepper";
}

int main() {
  Logger::addConsole(TRACE);

  Simulator sim;
  sim.run();

}