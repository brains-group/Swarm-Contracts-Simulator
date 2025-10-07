#include <data/point.hpp>
#include <gtest/gtest.h>

// NOLINTBEGIN(*-magic-numbers)

namespace scs::data {

TEST(PointBasics, AggregateInitAndArithmetic) {
    Point a{.x = 1.0, .y = 2.0};
    Point b{.x = 3.5, .y = -1.0};

    Point s = a + b;
    EXPECT_NEAR(s.x, 4.5, 1e-6);
    EXPECT_NEAR(s.y, 1.0, 1e-6);

    Point d = a - b;
    EXPECT_NEAR(d.x, -2.5, 1e-6);
    EXPECT_NEAR(d.y, 3.0, 1e-6);

    Point u = -a;
    EXPECT_NEAR(u.x, -1.0, 1e-6);
    EXPECT_NEAR(u.y, -2.0, 1e-6);
}

TEST(PointBasics, ScalarOps) {
    Point p{.x = 2.0, .y = -4.0};

    Point m1 = p * 0.5;
    EXPECT_NEAR(m1.x, 1.0, 1e-6);
    EXPECT_NEAR(m1.y, -2.0, 1e-6);

    Point m2 = 2.0 * p;
    EXPECT_NEAR(m2.x, 4.0, 1e-6);
    EXPECT_NEAR(m2.y, -8.0, 1e-6);

    Point q = p / 2.0;
    EXPECT_NEAR(q.x, 1.0, 1e-6);
    EXPECT_NEAR(q.y, -2.0, 1e-6);
}

TEST(PointGeometry, DotCross) {
    Point a{.x = 1.0, .y = 2.0};
    Point b{.x = 3.0, .y = 4.0};

    EXPECT_NEAR(dot(a, b), 11.0, 1e-6);        // 1*3 + 2*4 = 11
    EXPECT_NEAR(cross_z(a, b), -2.0, 1e-6);    // 1*4 - 2*3 = -2
}

TEST(PointGeometry, NormAndDistance) {
    Point p{.x = 3.0, .y = 4.0};
    EXPECT_NEAR(norm2(p), 25.0, 1e-6);
    EXPECT_NEAR(norm(p), 5.0, 1e-6);

    Point a{.x = 1.0, .y = 2.0};
    Point b{.x = 4.0, .y = 6.0};
    EXPECT_NEAR(distance2(a, b), 25.0, 1e-6);
    EXPECT_NEAR(distance(a, b), 5.0, 1e-6);
}

TEST(PointGeometry, NormalizeSae) {
    Point p{.x = 10.0, .y = 0.0};
    Point n = normalize(p);
    EXPECT_NEAR(norm(n), 1.0, 1e-6);
    EXPECT_NEAR(n.x, 1.0, 1e-6);
    EXPECT_NEAR(n.y, 0.0, 1e-6);

    // Near-zero vector returns {0,0}
    Point z{.x = 0.0, .y = 0.0};
    Point nz = normalize(z);
    EXPECT_NEAR(nz.x, 0.0, 1e-7);
    EXPECT_NEAR(nz.y, 0.0, 1e-7);

    // Very small vector behaves like zero within eps
    Point tiny{.x = 1e-13, .y = -1e-13};
    Point nt = normalize(tiny, /*eps=*/1e-12);
    EXPECT_NEAR(nt.x, 0.0, 1e-7);
    EXPECT_NEAR(nt.y, 0.0, 1e-7);
}

TEST(PointHelpers, LerpAndAlmostEqual) {
    Point a{.x = 0.0, .y = 0.0};
    Point b{.x = 10.0, .y = -10.0};

    Point m = lerp(a, b, 0.5);
    EXPECT_NEAR(m.x, 5.0, 1e-6);
    EXPECT_NEAR(m.y, -5.0, 1e-6);

    Point close{.x = 5.0000005, .y = -5.0000004};
    EXPECT_TRUE(almost_equal(m, close, 1e-5));
    EXPECT_FALSE(almost_equal(m, close, 1e-7));
}

}    // namespace scs::data

// NOLINTEND(*-magic-numbers)
