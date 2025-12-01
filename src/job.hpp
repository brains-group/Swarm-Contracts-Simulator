#pragma once

#include <array>
#include <ostream>

using JobVec = std::array<uint64_t, 3>;

inline auto operator<<(std::ostream& ostr, const JobVec& vec) -> std::ostream& {
    ostr << "{";
    for (std::size_t i = 0; i < vec.size(); i++) {
        ostr << vec.at(i);
        if (i != vec.size() - 1) { ostr << ", "; }
    }
    ostr << "}";
    return ostr;
}
