#pragma once

#include <array>
#include <vector>

#include "Logger.h"

constexpr unsigned int DIFFICULTY_VECTOR_DIMENSION = 3;
namespace base {

class Simulator;

class Contract {
 public:
  Contract(unsigned int ID_, Simulator* sim)
      : ID(ID_)
      , m_sim(sim) {}
  virtual ~Contract() {}

  void execute_wrapper() {
    if (executed) {
      LOG(ERROR) << "Executing previously executed contract " << ID;
    } else {
      LOG(INFO) << "Executing contract " << ID;
    }
    execute();
    executed = true;
  };

  bool shouldExecute_wrapper() { return !executed && shouldExecute(); }

  const unsigned int ID;

 protected:
  Simulator* m_sim;

 private:
  virtual void execute()       = 0;
  virtual bool shouldExecute() = 0;

  bool executed = false;
};
}    // namespace base
