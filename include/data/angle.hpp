#pragma once

#include <common/macros.hpp>

namespace scs::data {

static constexpr float PI  = std::numbers::pi_v<float>;
static constexpr float TAU = 2.0 * std::numbers::pi_v<float>;

constexpr float DEFAULT_ANGLE_COMPARISON_EPSILON = 1e-6;

class Angle {
public:
    [[nodiscard]] auto asRadians() const -> float;
    [[nodiscard]] auto asDegrees() const -> float;

    static const Angle Zero;

private:
    friend auto degrees(float angle) noexcept -> Angle;
    friend auto radians(float angle) noexcept -> Angle;

    float m_radians = 0;
};

auto degrees(float angle) noexcept -> Angle;
auto radians(float angle) noexcept -> Angle;

auto operator+(Angle a, Angle b) -> Angle;
auto operator-(Angle a, Angle b) -> Angle;
auto operator-(Angle a) -> Angle;

auto operator*(Angle a, float s) -> Angle;
auto operator*(float s, Angle a) -> Angle;
auto operator/(Angle a, float s) -> Angle;

auto wrap_signed(Angle a) -> Angle;

auto wrap_unsigned(Angle a) -> Angle;

auto almost_equal(Angle a, Angle b, float eps = DEFAULT_ANGLE_COMPARISON_EPSILON) -> bool;

auto shortest_delta(Angle from, Angle to) -> Angle;

auto lerp_shortest(Angle from, Angle to, float t) -> Angle;

}    // namespace scs::data
