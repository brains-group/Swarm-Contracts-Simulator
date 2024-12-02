#pragma once

#include <string>
#include <iostream>

struct FrameMetrics {
  unsigned int contracts_created = 0;
  unsigned int agents_created = 0;
  unsigned int agents_deactivated = 0;

  constexpr static std::string_view csv_headers() {
    return "contracts_created, agents_created, agents_deactivated,";
  }
};

inline std::ostream& operator<<(std::ostream& ostr, const FrameMetrics& fm) {
  auto vals = { fm.contracts_created, fm.agents_created, fm.agents_deactivated };
  for (const auto& val : vals) {
    ostr << val << ",";
  }
  return ostr;
}