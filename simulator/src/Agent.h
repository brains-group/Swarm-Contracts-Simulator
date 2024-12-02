#pragma once

#include "Entity.h"

#include <iostream>
#include <string>

class Agent : public Entity {
  public:
    Agent(unsigned int ID_, Simulator* sim) : Entity(ID_, sim) {}

    constexpr static std::string_view csv_headers() {
      return "ID,activation_step,deactivation_step,";
    }

  private:
};

inline std::ostream& operator<<(std::ostream& ostr, const Agent& a) {
  auto vals = { a.ID, a.activationStep(), a.deactivationStep() };
  for (const auto& val : vals) {
    ostr << val << ",";
  }
  return ostr;
}