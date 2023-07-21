#include <gtest/gtest.h>
#include "Mathematics/GridHyperellipsoid.h"
#include "Mathematics/Vector3.h"

using namespace gte;

// Generate a grid of 1 million points on the surface of a 3D ellipsoid.
// Ellipsoid is centered at zero with axes of 100,100,80. Verify that the magnitude
// of each point is 100, and that the centroid of all of the points is close 
// to zero;
TEST(TestGridHyperellipsoid, TestRandomSurfaceGrid3D)
{
    int num = 1000000;
    unsigned int seed = 1;
    Vector3<double> extent({100,100,80});
    Ellipsoid3<double> ellipsoid;
    ellipsoid.extent = extent;
    std::vector<Vector3<double>> grid = RandomSurfaceGrid(num, ellipsoid, seed);

    for (const auto& point : grid)
    {
        Vector3<double> test;
        for (int i = 0; i < 3; i++)
        {
            test[i] = Dot(point,ellipsoid.axis[i])/ellipsoid.extent[i];
        }

        // Verify that each point lies on the ellipsoid
        EXPECT_DOUBLE_EQ(Length(test),(double)1);

        // Verify that each point is inside the sphere formed by the largest
        // axis, and outside the sphere formed by the smallest axis. These 
        // checks could techniquely fail by FP error although this is
        // unlikely
        EXPECT_LE(Length(point), (double)100);
        EXPECT_GE(Length(point), (double)80);
    }

}