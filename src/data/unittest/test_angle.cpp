#include <data/angle.hpp>
#include <gtest/gtest.h>

// NOLINTBEGIN(*-magic-numbers)

namespace scs::data {

TEST(AngleConstruction, DegreesAndRadians) {
    Angle a = degrees(180.0);
    EXPECT_NEAR(a.asRadians(), PI, 1e-6);
    EXPECT_NEAR(a.asDegrees(), 180.0, 1e-6);

    Angle b = radians(PI / 2.0);
    EXPECT_NEAR(b.asDegrees(), 90.0, 1e-6);
    EXPECT_TRUE(almost_equal(Angle::Zero, radians(0.0)));
}

TEST(AngleArithmetic, AddSubScaleNoWrap) {
    Angle a = degrees(30.0);
    Angle b = degrees(45.0);

    Angle s = a + b;    // 75 deg
    EXPECT_NEAR(s.asDegrees(), 75.0, 1e-5);

    Angle d = b - a;    // 15 deg
    EXPECT_NEAR(d.asDegrees(), 15.0, 1e-5);

    Angle n = -a;    // -30 deg
    EXPECT_NEAR(n.asDegrees(), -30.0, 1e-5);

    Angle m1 = a * 2.0;    // 60 deg
    Angle m2 = 3.0 * a;    // 90 deg
    Angle q  = b / 2.0;    // 22.5 deg
    EXPECT_NEAR(m1.asDegrees(), 60.0, 1e-5);
    EXPECT_NEAR(m2.asDegrees(), 90.0, 1e-5);
    EXPECT_NEAR(q.asDegrees(), 22.5, 1e-5);
}

TEST(AngleWrap, SignedAndUnsigned) {
    Angle a = degrees(190.0);
    // Signed wrap => (-pi, pi]   190 -> -170
    EXPECT_NEAR(wrap_signed(a).asDegrees(), -170.0, 1e-5);

    Angle b = degrees(-10.0);
    // Unsigned wrap => [0, 360)  -10 -> 350
    EXPECT_NEAR(wrap_unsigned(b).asDegrees(), 350.0, 1e-5);

    // Edge boundaries
    EXPECT_NEAR(wrap_signed(degrees(180.0)).asRadians(), PI, 1e-6);
    EXPECT_NEAR(wrap_signed(degrees(-180.0)).asRadians(), PI, 1e-6);    // (-pi,pi] -> pi
    EXPECT_NEAR(wrap_unsigned(degrees(360.0)).asRadians(), 0.0, 1e-6);
}

TEST(AngleDeltaAndLerp, ShortestPath) {
    // From 170° to -170°: naive di is -340°, shortest is +20°
    Angle from = degrees(170.0);
    Angle to   = degrees(-170.0);

    Angle d = shortest_delta(from, to);
    EXPECT_NEAR(d.asDegrees(), 20.0, 1e-4);

    // Halway should be +10° along shortest arc from 170° -> -170°
    Angle mid = lerp_shortest(from, to, 0.5);
    // From 170° + 10° = 180° -> wraps to pi
    EXPECT_NEAR(wrap_signed(mid).asRadians(), PI, 1e-5);

    // Another simple case: 0° -> 90°, t=0.25 => 22.5°
    Angle q = lerp_shortest(degrees(0.0), degrees(90.0), 0.25);
    EXPECT_NEAR(q.asDegrees(), 22.5, 1e-5);
}

TEST(AngleEquality, AlmostEqual) {
    Angle a = degrees(60.0);
    Angle b = radians((PI / 3.0) + 1e-7);
    EXPECT_TRUE(almost_equal(a, b, 1e-6));
    EXPECT_FALSE(almost_equal(a, b, 1e-9));
}

}    // namespace scs::data

// NOLINTEND(*-magic-numbers)
