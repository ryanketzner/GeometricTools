#include <gtest/gtest.h>
#include <algorithm>
#include <cmath>
#include <array>

#include <Mathematics/IntrAlignedBoxHalfspace.h>

using namespace gte;

template <size_t N>
void TestIntrHalfspace() {
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
    double constant = Length(box_min);
    Halfspace<N, double> halfspace(origin, constant);

    TIQuery<double, AlignedBox<N,double>, Halfspace<N,double>> query;

    halfspace.constant = Length(box_min) - delta;
    EXPECT_FALSE(query(unit_box, halfspace).intersect);

    halfspace.constant = Length(box_min) + delta;
    EXPECT_TRUE(query(unit_box, halfspace).intersect);
}

TEST(TestIntrHalfspace, TestN1) { TestIntrHalfspace<1>(); }
TEST(TestIntrHalfspace, TestN2) { TestIntrHalfspace<2>(); }
TEST(TestIntrHalfspace, TestN3) { TestIntrHalfspace<3>(); }
TEST(TestIntrHalfspace, TestN4) { TestIntrHalfspace<4>(); }
TEST(TestIntrHalfspace, TestN5) { TestIntrHalfspace<5>(); }
TEST(TestIntrHalfspace, TestN6) { TestIntrHalfspace<6>(); }