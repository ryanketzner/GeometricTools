#include <gtest/gtest.h>
#include <algorithm>
#include <cmath>
#include <array>
#include <vector>

#include <Mathematics/IntrAlignedBoxConvexPolyhedronH.h>
#include <Mathematics/Vector.h>
#include <Mathematics/AlignedBox.h>

using namespace gte;

template <size_t N>
void TestIntrBox() {
    double delta = 0.001;
    
    std::vector<Halfspace<N, double>> halfspaces;

    // Generate convex polytope of first orthant
    for (int i = 0; i < N; i++)
    {
        halfspaces.push_back({Vector<N,double>(i), 1.0});
        halfspaces.push_back({-Vector<N,double>(i), 0.0});
    }

    ConvexPolyhedronH<N, double> poly(halfspaces);

    // Create aligned box inside first orthant
    std::array<double, N> init_min;
    std::array<double, N> init_max;

    init_min.fill(.5);
    init_max.fill(1.1);

    Vector<N, double> box_min(init_min);
    Vector<N, double> box_max(init_max);
    AlignedBox<N, double> box(box_min, box_max);

    TIQuery<double, AlignedBox<N,double>, ConvexPolyhedronH<N,double>>  query;

    EXPECT_TRUE(query(box, poly).intersect);
    
    // Now test box from 1.1 to 2.2.
    box.min = box_max;
    box.max = box_max + box_max;

    EXPECT_FALSE(query(box, poly).intersect);
}

TEST(TestIntrConvexPolyhedronH, TestN1) { TestIntrBox<1>(); }
TEST(TestIntrConvexPolyhedronH, TestN2) { TestIntrBox<2>(); }
TEST(TestIntrConvexPolyhedronH, TestN3) { TestIntrBox<3>(); }
TEST(TestIntrConvexPolyhedronH, TestN4) { TestIntrBox<4>(); }
TEST(TestIntrConvexPolyhedronH, TestN5) { TestIntrBox<5>(); }
TEST(TestIntrConvexPolyhedronH, TestN6) { TestIntrBox<6>(); }