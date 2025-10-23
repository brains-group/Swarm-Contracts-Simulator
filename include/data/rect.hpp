#pragma once

#include <data/point.hpp>

namespace scs::data {

struct Rect {
    Point loc;
    Point size;
};

inline auto getPoints(const Rect& rect) -> std::vector<Point> {
    return {
        rect.loc,
        {.x = rect.loc.x + rect.size.x,               .y = rect.loc.y},
        rect.loc + rect.size,
        {              .x = rect.loc.x, .y = rect.loc.y + rect.size.y}
    };
}

}    // namespace scs::data
