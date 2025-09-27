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

inline auto operator-(const Point& p1, const Point& p2) -> Point {
    return {.x = p1.x - p2.x, .y = p1.y - p2.y};
}

inline auto toVec(const Point& p) -> Vector {
    return {.dir = std::atan2(p.y, p.x), .len = std::sqrt((p.y * p.y) + (p.x * p.x))};
}

inline auto toPoint(const Vector& v) -> Point {
    return Point{.x = v.len * std::cos(v.dir), .y = v.len * std::sin(v.dir)};
}

inline auto operator+=(Point& p, const Vector& vec) { return p += toPoint(vec); }
}    // namespace scs::data
