#include <gtest/gtest.h>
#include <algorithm>
#include <cmath>
#include <array>

#include <Mathematics/ContHypersphere.h>
#include <Mathematics/Vector2.h>
#include <Mathematics/AlignedBox.h>

using namespace gte;

template <size_t N>
void TestHypersphere() {
    double delta = 0.001;
    Vector<N, double> origin{(double)0};
    double radius = 1.0;

    std::array<double, N> init_min;
    std::array<double, N> init_max;

    init_min.fill(-1.0);
    init_max.fill(1.0);

    Vector<N, double> box_min(init_min);
    Vector<N, double> box_max(init_max);
    AlignedBox<N, double> unit_box(box_min, box_max);

    Hypersphere<N, double> unit_circle(origin, radius);

    unit_circle.radius = sqrt(static_cast<double>(N)) - delta;
    EXPECT_FALSE(InContainer(unit_box, unit_circle));

    unit_circle.radius = sqrt(static_cast<double>(N)) + delta;
    EXPECT_TRUE(InContainer(unit_box, unit_circle));
}

TEST(TestContHypersphere, TestN1) { TestHypersphere<1>(); }
TEST(TestContHypersphere, TestN2) { TestHypersphere<2>(); }
TEST(TestContHypersphere, TestN3) { TestHypersphere<3>(); }
TEST(TestContHypersphere, TestN4) { TestHypersphere<4>(); }
TEST(TestContHypersphere, TestN5) { TestHypersphere<5>(); }
TEST(TestContHypersphere, TestN6) { TestHypersphere<6>(); }