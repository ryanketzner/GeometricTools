#include <gtest/gtest.h>
#include <algorithm>
#include <cmath>
#include <array>

#include <Mathematics/ContHalfspace.h>
#include <Mathematics/AlignedBox.h>

using namespace gte;

template <size_t N>
void TestContHalfspace() {
    double delta = 0.001;

    std::array<double, N> init_min;
    std::array<double, N> init_max;

    init_min.fill(1.0);
    init_max.fill(2.0);

    Vector<N, double> box_min(init_min);
    Vector<N, double> box_max(init_max);
    AlignedBox<N, double> unit_box(box_min, box_max);

    Vector<N, double> origin{init_min};
    Normalize(origin, true);
    double constant = Length(box_max);
    Halfspace<N, double> halfspace(origin, constant);

    halfspace.constant = Length(box_max) - delta;
    EXPECT_FALSE(InContainer(unit_box, halfspace));

    halfspace.constant = Length(box_max) + delta;
    EXPECT_TRUE(InContainer(unit_box, halfspace));
}

TEST(TestContHalfspace, TestN1) { TestContHalfspace<1>(); }
TEST(TestContHalfspace, TestN2) { TestContHalfspace<2>(); }
TEST(TestContHalfspace, TestN3) { TestContHalfspace<3>(); }
TEST(TestContHalfspace, TestN4) { TestContHalfspace<4>(); }
TEST(TestContHalfspace, TestN5) { TestContHalfspace<5>(); }
TEST(TestContHalfspace, TestN6) { TestContHalfspace<6>(); }