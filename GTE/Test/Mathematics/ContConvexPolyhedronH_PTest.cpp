#include <gtest/gtest.h>
#include <algorithm>
#include <cmath>
#include <array>
#include <vector>

#include <Mathematics/ContConvexPolyhedronH.h>
#include <Mathematics/Vector.h>
#include <Mathematics/AlignedBox.h>
#include <Mathematics/GridAlignedBox.h>

using namespace gte;

template <size_t N>
void TestContVector() {
    double delta = 0.001;
    
    std::vector<Halfspace<N, double>> halfspaces;

    // Generate convex polytope of first orthant
    for (int i = 0; i < N; i++)
    {
        // Lower bound: x_i >= 0
        halfspaces.push_back({Vector<N,double>(i), 0.0});
        // Upper bound: x_i <= 1  <=>  -x_i >= -1
        halfspaces.push_back({-Vector<N,double>(i), -1.0});
    }

    ConvexPolyhedronH<N, double> poly(halfspaces);

    // Create aligned box for first orthant
    std::array<double, N> init_min;
    std::array<double, N> init_max;

    init_min.fill(0.0);
    init_max.fill(1.0);

    Vector<N, double> box_min(init_min);
    Vector<N, double> box_max(init_max);
    AlignedBox<N, double> box(box_min, box_max);

    int num = 10;
    std::vector<Vector<N,double>> grid = RandomGrid<N,double>(num, box);

    for (auto& point : grid)
        EXPECT_TRUE(InContainer(point, poly));
    
    // Now test box from 1 to 2.
    box.min = box_max;
    box.max = box_max + box_max;

    grid = RandomGrid<N,double>(num, box);

    for (auto& point : grid)
        EXPECT_FALSE(InContainer(point, poly));
}

TEST(TestContConvexPolyhedronH, TestN1) { TestContVector<1>(); }
TEST(TestContConvexPolyhedronH, TestN2) { TestContVector<2>(); }
TEST(TestContConvexPolyhedronH, TestN3) { TestContVector<3>(); }
TEST(TestContConvexPolyhedronH, TestN4) { TestContVector<4>(); }
TEST(TestContConvexPolyhedronH, TestN5) { TestContVector<5>(); }
TEST(TestContConvexPolyhedronH, TestN6) { TestContVector<6>(); }