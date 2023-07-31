#include <gtest/gtest.h>
#include <iostream>

#include "Mathematics/MakeAlignedBoxS2.h"
#include "Mathematics/ContAlignedBoxS2.h"
#include "Mathematics/ContRectView3.h"
#include "Mathematics/GridHypersphere.h"
#include "Mathematics/GridHyperellipsoid.h"
#include "Mathematics/ContCone.h"

using namespace gte;

// Make a RectView3 with position at x = 7000, pointing opposite the x-axis
// Test that the resulting AlignedBoxS2 contains the x-axis.
TEST(TestMakeAlignedBoxS2, TestMakeFootPrintBoxS2_Sphere)
{
    Vector3<double> pos({7000.0, 0.0, 0.0});
    Vector3<double> u((int)2); // z
    Vector3<double> r((int)1); // y
    double angle = GTE_C_QUARTER_PI;
    RectView3<double> view(u, r, pos, angle, angle);

    double radius = 6378;
    Sphere3<double> sphere({Vector3<double>::Zero(), radius});

    AlignedBoxS2<double> box = MakeFootprintBoxS2(view,sphere);

    // Test that box contains the x-axis
    Vector3<double> x_c = Vector3<double>((int)0);
    EXPECT_TRUE(InContainer(CartToGeographic(x_c), box));
}

// Make a RectView3 with position at x = 7000, pointing opposite the x-axis
// Test that the resulting AlignedBoxS2 contains the x-axis.
TEST(TestMakeAlignedBoxS2, TestMakeFootPrintBoxS2_Ellipsoid)
{
    Vector3<double> pos({7000.0, 0.0, 0.0});
    Vector3<double> u((int)2); // z
    Vector3<double> r((int)1); // y
    double angle = GTE_C_QUARTER_PI;
    RectView3<double> view(u, r, pos, angle, angle);

    Vector3<double> extent({6357.0,6357.0,6378.0});;
    Ellipsoid3<double> ellipsoid;
    ellipsoid.extent = extent;

    AlignedBoxS2<double> box = MakeFootprintBoxS2(view,ellipsoid);

    // Test that box contains the x-axis
    Vector3<double> x_c = Vector3<double>((int)0);
    EXPECT_TRUE(InContainer(CartToGeographic(x_c), box));
}


// Make a RectView3 with position at x = 7000, pointing opposite the x-axis.
// Create a sphere centered at the origin with radius 6378. Create a random
// grid on the sphere with a million points, and verify that each point
// contained by view and its polar halfspace is contained by the footprint box.
TEST(TestMakeAlignedBoxS2, TestMakeFootPrintBoxS2_Sphere_2)
{   
    unsigned int seed = 1;

    Vector3<double> pos({7000.0, 0.0, 0.0});
    Vector3<double> u((int)2); // z
    Vector3<double> r((int)1); // y
    double angle = GTE_C_QUARTER_PI;
    RectView3<double> view(u, r, pos, angle, angle);

    double radius = 6378;
    Sphere3<double> sphere({Vector3<double>::Zero(), radius});

    AlignedBoxS2<double> box = MakeFootprintBoxS2(view,sphere);

    // Test that box contains every point that the view contains
    int num = 1000000;
    std::vector<Vector3<double>> grid = RandomSurfaceGrid(num,sphere,seed);
    Halfspace3<double> polar = MakePolarHalfspace3(pos,sphere);

    for (const auto& point : grid)
    {
        if (InContainer(point,view) && InContainer(point,polar))
            EXPECT_TRUE(InContainer(CartToGeographic(point),box));
    }

    std::cout << box.latMin << "," << box.latMax << std::endl;
    std::cout << box.lonMin << "," << box.lonMax << std::endl;

}

// Make a RectView3 with position at x = 7000, pointing opposite the x-axis.
// Create an oblate spheroid with minor axis 6357, major 6378. Create a random
// grid on the ellipsoid with a million points, and verify that each point
// contained by view and its polar halfspace is contained by the footprint box.
TEST(TestMakeAlignedBoxS2, TestMakeFootPrintBoxS2_Ellipsoid_2)
{   
    unsigned int seed = 1;

    Vector3<double> pos({7000.0, 0.0, 0.0});
    Vector3<double> u((int)2); // z
    Vector3<double> r((int)1); // y
    double angle = GTE_C_QUARTER_PI;
    RectView3<double> view(u, r, pos, angle, angle);

    Vector3<double> extent({6357.0,6357.0,6378.0});;
    Ellipsoid3<double> ellipsoid;
    ellipsoid.extent = extent;

    AlignedBoxS2<double> box = MakeFootprintBoxS2(view,ellipsoid);

    // Test that box contains every point that the view contains
    int num = 1000000;
    std::vector<Vector3<double>> grid = RandomSurfaceGrid(num,ellipsoid,seed);
    Halfspace3<double> polar = MakePolarHalfspace3(pos,ellipsoid);

    for (const auto& point : grid)
    {
        if (InContainer(point,view) && InContainer(point,polar))
            EXPECT_TRUE(InContainer(CartToGeographic(point),box));
    }

    std::cout << box.latMin << "," << box.latMax << std::endl;
    std::cout << box.lonMin << "," << box.lonMax << std::endl;
}

// Make a RectView3 with position at z = 7000, pointing opposite the z-axis.
// Create a sphere centered at the origin with radius 6378. Create a random
// grid on the sphere with a million points, and verify that each point
// contained by view and its polar halfspace is contained by the footprint box.
TEST(TestMakeAlignedBoxS2, TestMakeFootPrintBoxS2_Sphere_3)
{   
    unsigned int seed = 1;

    Vector3<double> pos({0.0, 0.0, 7000.0});
    Vector3<double> u((int)1); // y
    Vector3<double> r((int)0); // x
    double angle = GTE_C_QUARTER_PI;
    RectView3<double> view(u, r, pos, angle, angle);

    double radius = 6378;
    Sphere3<double> sphere({Vector3<double>::Zero(), radius});

    AlignedBoxS2<double> box = MakeFootprintBoxS2(view,sphere);

    // Test that box contains every point that the view contains
    int num = 1000000;
    std::vector<Vector3<double>> grid = RandomSurfaceGrid(num,sphere,seed);
    Halfspace3<double> polar = MakePolarHalfspace3(pos,sphere);

    for (const auto& point : grid)
    {
        if (InContainer(point,view) && InContainer(point,polar))
            EXPECT_TRUE(InContainer(CartToGeographic(point),box));
    }

    std::cout << box.latMin << "," << box.latMax << std::endl;
    std::cout << box.lonMin << "," << box.lonMax << std::endl;
}

// Test making footprint box from a point at position z = 7000 over a sphere
// centered at the origin with radius 6378. Create a random grid on the sphere
// with a million points, and verify that each point contained by the polar 
// halfspace is contained by the footprint box
TEST(TestMakeAlignedBoxS2, TestMakeFootPrintBoxS2_Sphere_4)
{
    int seed;

    Vector3<double> pos({0.0, 0.0, 7000.0});

    double radius = 6378;
    Sphere3<double> sphere({Vector3<double>::Zero(), radius});

    AlignedBoxS2<double> box = MakeFootprintBoxS2(pos,sphere);

    // Test that box contains every point that the halfspace contains
    int num = 1000000;
    std::vector<Vector3<double>> grid = RandomSurfaceGrid(num,sphere,seed);
    Halfspace3<double> polar = MakePolarHalfspace3(pos,sphere);

    for (const auto& point : grid)
    {
        if (InContainer(point,polar))
            EXPECT_TRUE(InContainer(CartToGeographic(point),box));
    }

    std::cout << box.latMin << "," << box.latMax << std::endl;
    std::cout << box.lonMin << "," << box.lonMax << std::endl;
}

// Test making footprint box from a point at position x = 7000 over a sphere
// centered at the origin with radius 6378. Create a random grid on the sphere
// with a million points, and verify that each point contained by the polar 
// halfspace is contained by the footprint box
TEST(TestMakeAlignedBoxS2, TestMakeFootPrintBoxS2_Sphere_5)
{
    int seed;

    Vector3<double> pos({6000.0,0.0,3000.0});

    double radius = 6378;
    Sphere3<double> sphere({Vector3<double>::Zero(), radius});

    AlignedBoxS2<double> box = MakeFootprintBoxS2(pos,sphere);

    // Test that box contains every point that the halfspace contains
    int num = 1000000;
    std::vector<Vector3<double>> grid = RandomSurfaceGrid(num,sphere,seed);
    Halfspace3<double> polar = MakePolarHalfspace3(pos,sphere);

    for (const auto& point : grid)
    {
        if (InContainer(point,polar))
        {
            EXPECT_TRUE(InContainer(CartToGeographic(point),box));
        }
    }
}

// Make a cone with position at x = 7000, pointing opposite the x-axis.
// Create a sphere centered at the origin with radius 6378. Create a random
// grid on the sphere with a million points, and verify that each point
// contained by view and its polar halfspace is contained by the footprint box.
// The cone's half-angle is Pi/4;
TEST(TestMakeAlignedBoxS2, TestMakeFootPrintBoxS2_Sphere_6)
{
    int seed;

    Vector3<double> pos({7000.0, 0.0, 0.0});
    Vector3<double> dir({-1.0, 0.0, 0.0});
    Ray3<double> ray(pos, dir);
    double angle = GTE_C_QUARTER_PI;
    Cone3<double> cone(ray, angle);
    RectView3<double> view;
    GetContainer(cone,view);

    double radius = 6378;
    Sphere3<double> sphere({Vector3<double>::Zero(), radius});

    AlignedBoxS2<double> box = MakeFootprintBoxS2(cone, sphere);

    // Test that box contains every point that the halfspace contains
    int num = 1000000;
    std::vector<Vector3<double>> grid = RandomSurfaceGrid(num,sphere,seed);
    Halfspace3<double> polar = MakePolarHalfspace3(pos,sphere);

    std::cout << box.latMin << "," << box.latMax << std::endl;
    std::cout << box.lonMin << "," << box.lonMax << std::endl;


    for (const auto& point : grid)
    {
        if (InContainer(point, cone) && InContainer(point, polar))
            ASSERT_TRUE(InContainer(CartToGeographic(point),box));
    }
}


// Test making footprint box from a point at position x = 7000 over a sphere
// centered at the origin with radius 6378. Create a random grid on the sphere
// with 10 million points, and verify that each point contained by the box 
// halfspace is contained by the footprint box
TEST(TestMakeAlignedBoxS2, TestMakeFootPrintBoxS2_Sphere_7)
{
    int seed;

    Vector3<double> pos({7000.0, 0.0, 0.0});
    Vector3<double> u((int)2); // z
    Vector3<double> r((int)1); // y
    double rot_angle = -GTE_C_PI/3.0;
    AxisAngle<3,double> rot(u, rot_angle);
    r = Rotate(rot,r);
    double angle = GTE_C_PI/64.0;
    RectView3<double> view(u, r, pos, angle, angle);

    double radius = 6378;
    Sphere3<double> sphere({Vector3<double>::Zero(), radius});

    AlignedBoxS2<double> box = MakeFootprintBoxS2(view,sphere);

    // Test that box contains every point that the halfspace contains
    int num = 10000000;
    std::vector<Vector3<double>> grid = RandomSurfaceGrid(num,sphere,seed);
    Halfspace3<double> polar = MakePolarHalfspace3(pos,sphere);

    std::cout << box.latMin << "," << box.latMax << std::endl;
    std::cout << box.lonMin << "," << box.lonMax << std::endl;

    for (const auto& point : grid)
    {
        if (InContainer(point,polar) && InContainer(point,view))
        {
            EXPECT_TRUE(InContainer(CartToGeographic(point),box));
        }
    }
}