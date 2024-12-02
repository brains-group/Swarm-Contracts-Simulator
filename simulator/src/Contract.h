#pragma once

#include "Entity.h"

class Contract : public Entity {
  public:
    Contract(unsigned int ID_, Simulator* sim) : Entity(ID_, sim) {}

    constexpr static std::string_view csv_headers() {
      return "ID,activation_step,deactivation_step,";
    }

  private:
};

inline std::ostream& operator<<(std::ostream& ostr, const Contract& c) {
  auto vals = { c.ID, c.activationStep(), c.deactivationStep() };
  for (const auto& val : vals) {
    ostr << val << ",";
  }
  return ostr;
}