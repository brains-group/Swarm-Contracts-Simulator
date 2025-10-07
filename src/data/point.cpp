#include <cmath>

#include <data/point.hpp>

namespace scs::data {

// basic arithmetic
auto operator+(const Point& a, const Point& b) -> Point { return {.x = a.x + b.x, .y = a.y + b.y}; }
auto operator-(const Point& a, const Point& b) -> Point { return {.x = a.x - b.x, .y = a.y - b.y}; }
auto operator-(const Point& a) -> Point { return {.x = -a.x, .y = -a.y}; }

// scalar ops
auto operator*(const Point& p, float s) -> Point { return {.x = p.x * s, .y = p.y * s}; }
auto operator*(float s, const Point& p) -> Point { return {.x = p.x * s, .y = p.y * s}; }
auto operator/(const Point& p, float s) -> Point { return {.x = p.x / s, .y = p.y / s}; }

// geometry helpers
auto dot(const Point& a, const Point& b) -> float { return (a.x * b.x) + (a.y * b.y); }

auto cross_z(const Point& a, const Point& b) -> float { return (a.x * b.y) - (a.y * b.x); }

auto norm2(const Point& p) -> float { return dot(p, p); }

auto norm(const Point& p) -> float { return std::sqrt(norm2(p)); }

auto distance2(const Point& a, const Point& b) -> float { return norm2(a - b); }

auto distance(const Point& a, const Point& b) -> float { return std::sqrt(distance2(a, b)); }

auto normalize(const Point& p, float eps) -> Point {
    const float n = norm(p);
    if (n <= eps) { return {.x = 0.0F, .y = 0.0F}; }
    const float inv = 1.0F / n;
    return {.x = p.x * inv, .y = p.y * inv};
}

auto lerp(const Point& a, const Point& b, float t) -> Point {
    return {.x = a.x + (t * (b.x - a.x)), .y = a.y + (t * (b.y - a.y))};
}

auto almost_equal(const Point& a, const Point& b, float eps) -> bool {
    return std::fabs(a.x - b.x) <= eps && std::fabs(a.y - b.y) <= eps;
}
}    // namespace scs::data
