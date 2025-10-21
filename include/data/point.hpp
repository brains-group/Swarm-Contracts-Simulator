#pragma once

#include <optional>

#include <data/angle.hpp>

namespace scs::data {

constexpr float DEFAULT_NORMALIZE_EPSILON  = 1e-12F;
constexpr float DEFAULT_COMPARISON_EPSILON = 1e-6F;

struct Point {
    float x;
    float y;
};

inline auto operator==(const Point& p1, const Point& p2) -> bool {
    return p1.x == p2.x && p1.y == p2.y;
}

// Basic arithmetic
auto operator+(const Point& a, const Point& b) -> Point;
auto operator-(const Point& a, const Point& b) -> Point;
auto operator-(const Point& a) -> Point;    // unary minus

// Scalar ops
auto operator*(const Point& p, float s) -> Point;
auto operator*(float s, const Point& p) -> Point;
auto operator/(const Point& p, float s) -> Point;
// undefined for s==0

// Geometry helpers (free functions)
auto dot(const Point& a, const Point& b) -> float;          // a·b
auto cross_z(const Point& a, const Point& b) -> float;      // z-component of a×b in 2D
auto norm2(const Point& p) -> float;                        // squared length
auto norm(const Point& p) -> float;                         // length
auto distance2(const Point& a, const Point& b) -> float;    // squared distance
auto distance(const Point& a, const Point& b) -> float;     // distance

// Normalize safely (returns {0,0} for near-zero vectors)
auto normalize(const Point& p, float eps = DEFAULT_NORMALIZE_EPSILON) -> Point;

// Linear interpolation
auto lerp(const Point& a, const Point& b, float t) -> Point;

// Floating point comparison helper (component-wise)
auto almost_equal(const Point& a, const Point& b, float eps = DEFAULT_COMPARISON_EPSILON) -> bool;

// Angle conversion
auto angle_to(const Point& from, const Point& to, float eps = DEFAULT_NORMALIZE_EPSILON)
    -> std::optional<Angle>;

// Move `p` forward by `distance` along `heading`
auto advance(const Point& p, Angle heading, float distance) -> Point;

}    // namespace scs::data
