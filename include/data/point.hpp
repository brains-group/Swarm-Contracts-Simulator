#pragma once

#include <cmath>
#include <data/vector.hpp>

namespace scs::data {

struct Point {
    float x;
    float y;
};

inline auto operator+=(Point& p, const Point& p2) {
    p.x += p2.x;
    p.y += p2.y;
    return p;
}

inline auto operator+=(Point& p, const Vector& vec) {
    return p += Point{.x = vec.len * std::cos(vec.dir), .y = vec.len * std::cos(vec.dir)};
}

}    // namespace scs::data
