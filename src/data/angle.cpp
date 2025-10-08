#include <cmath>

#include <data/angle.hpp>

namespace scs::data {

auto Angle::asRadians() const -> float { return m_radians; }

// NOLINTNEXTLINE(*-magic-numbers)
auto Angle::asDegrees() const -> float { return m_radians * (180.0F / PI); }

const Angle Angle::Zero = radians(0);

// NOLINTNEXTLINE(*-magic-numbers)
auto degrees(float angle) noexcept -> Angle { return radians(angle * (PI / 180.0F)); };

auto radians(float angle) noexcept -> Angle {
    Angle ret{};
    ret.m_radians = angle;
    return ret;
}

auto operator+(Angle a, Angle b) -> Angle { return radians(a.asRadians() + b.asRadians()); }
auto operator-(Angle a, Angle b) -> Angle { return radians(a.asRadians() - b.asRadians()); }
auto operator-(Angle a) -> Angle { return radians(-a.asRadians()); }

auto operator*(Angle a, float s) -> Angle { return radians(a.asRadians() * s); }
auto operator*(float s, Angle a) -> Angle { return radians(a.asRadians() * s); }
auto operator/(Angle a, float s) -> Angle { return radians(a.asRadians() / s); }

auto wrap_signed(Angle a) -> Angle {
    float x = std::fmod(a.asRadians(), TAU);
    if (x <= -PI) { x += TAU; }
    if (x > PI) { x -= TAU; }
    return radians(x);
}

auto wrap_unsigned(Angle a) -> Angle {
    float x = std::fmod(a.asRadians(), TAU);
    if (x < 0.0F) { x += TAU; }
    return radians(x);
}

auto almost_equal(Angle a, Angle b, float eps) -> bool {
    return std::fabs(a.asRadians() - b.asRadians()) <= eps;
}

auto shortest_delta(Angle from, Angle to) -> Angle { return wrap_signed(to - from); }

auto lerp_shortest(Angle from, Angle to, float t) -> Angle {
    const Angle d = shortest_delta(from, to);
    return wrap_signed(from + t * d);
}

}    // namespace scs::data
