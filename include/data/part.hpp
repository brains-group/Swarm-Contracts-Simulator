#pragma once

#include <algorithm>
#include <optional>
#include <ranges>
#include <vector>

#include <data/material.hpp>

namespace scs::data {

using Part = std::vector<std::optional<data::Material>>;

inline auto operator==(const Part& p1, const Part& p2) -> bool {
    auto getLastFilled = [](const Part& p) -> unsigned int {
        unsigned int lastFilled = p.size();
        for (unsigned int i = lastFilled; i-- > 0;) {
            if (p[i]) {
                lastFilled = i;
                break;
            }
        }
        return lastFilled;
    };

    unsigned int p1Last = getLastFilled(p1);
    unsigned int p2Last = getLastFilled(p2);

    // One of them is empty
    if (p1Last == p1.size() || p2Last == p2.size()) {
        // Only the same part if both are empty
        return p1Last == p2Last;
    }

    // At least one is not empty
    return p1Last == p2Last
        && std::ranges::all_of(std::views::zip(p1, p2), [](const auto& t) -> bool {
               const auto& [m1, m2] = t;
               return m1 == m2;
           });
}

}    // namespace scs::data
