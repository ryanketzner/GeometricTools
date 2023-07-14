#include <gtest/gtest.h>
#include "Mathematics/GridHypersphere.h"
#include "Mathematics/Vector3.h"

using namespace gte;

// Generate a grid of 1 million points on the surface of a 3D sphere.
// Sphere is centered at zero with a radius of 100. Verify that the magnitude
// of each point is 100, and that the centroid of all of the points is close 
// to zero;
TEST(TestGridHypersphere, TestRandomSurfaceGrid3D)
{
    int num = 1000000;
    unsigned int seed = 1;
    double radius = 100.0;
    Sphere3<double> sphere(Vector3<double>::Zero(), radius);
    std::vector<Vector3<double>> grid = RandomSurfaceGrid(num, sphere, seed);

    // Verify that the magnitude of each point is 100
    for (const auto& point : grid)
        EXPECT_DOUBLE_EQ(Length(point), radius);

    // Verify that the centroid of all the points is close to zero
    double tol = .1;
    Vector3<double> centroid = Vector3<double>::Zero();
    for (auto& point : grid)
        centroid = centroid + point/(double)num;

    for (int i = 0; i < 3; i++)
        EXPECT_NEAR(centroid[i],0.0,tol);
}