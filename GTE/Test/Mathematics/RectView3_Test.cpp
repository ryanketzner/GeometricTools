#include <gtest/gtest.h>

#include "Mathematics/RectView3.h"
#include "Mathematics/ContRectView3.h"
#include <iostream>

using namespace gte;

// Build a rectangular view with u = x, r = y, vertex = 0
// Test various points for containment 
TEST(TestRectView3, TestInContainer)
{
    Vector3<double> u({1.0,0.0,0.0});
    Vector3<double> r({0.0,1.0,0.0});
    Vector3<double> vertex({0.0,0.0,0.0});

    double widthAngle = M_PI/8.0;
    double heightAngle = M_PI/10.0;

    RectView3<double> view(u,r,vertex,heightAngle,widthAngle);

    // z-axis should be in container
    Vector3<double> query({0.0,0.0,1.0});
    EXPECT_TRUE(InContainer(query,view));

    // -z-axis should not be in container
    query = -query;
    EXPECT_FALSE(InContainer(query,view));

    // Rotate z-axis ccw about y by slightly less than heightAngle/2
    // Should be in container
    double theta = heightAngle/2.0 - .001;
    query = Vector3<double>({std::sin(theta),0.0,std::cos(theta)});
    EXPECT_TRUE(InContainer(query,view));

    // Rotate z-axis ccw about y by slightly more than heightAngle/2
    // Should not be in container
    theta = heightAngle/2.0 + .001;
    query = Vector3<double>({std::sin(theta),0.0,std::cos(theta)});
    EXPECT_FALSE(InContainer(query,view));

    // Rotate z-axis cw about y by slightly less than widthAngle/2
    // Should be in container
    theta = widthAngle/2.0 - .001;
    query = Vector3<double>({0.0,std::sin(theta),std::cos(theta)});
    EXPECT_TRUE(InContainer(query,view));

    // Rotate z-axis cw about y by slightly more than widthAngle/2
    // Should not be in container
    theta = widthAngle/2.0 + .001;
    query = Vector3<double>({0.0,std::sin(theta),std::cos(theta)});
    EXPECT_FALSE(InContainer(query,view));
}

// Build a rectangular view with u = x, r = y, vertex at z = -100
// Test various points for containment.
TEST(TestRectView3, TestInContainer_2)
{
    Vector3<double> u({1.0,0.0,0.0});
    Vector3<double> r({0.0,1.0,0.0});
    Vector3<double> vertex({0.0,0.0,-100.0});

    double widthAngle = M_PI/8.0;
    double heightAngle = M_PI/10.0;

    RectView3<double> view(u,r,vertex,heightAngle,widthAngle);

    // z-axis should be in container
    Vector3<double> query({0.0,0.0,1.0});
    EXPECT_TRUE(InContainer(query,view));

    // -z-axis should be in container
    query = -query;
    EXPECT_TRUE(InContainer(query,view));

    // Rotate z-axis ccw about y by slightly less than heightAngle/2
    // Should be in container
    double theta = heightAngle/2.0 - .001;
    query = Vector3<double>({std::sin(theta),0.0,std::cos(theta)});
    EXPECT_TRUE(InContainer(query,view));

    // Rotate z-axis cw about y by slightly less than widthAngle/2
    // Should be in container
    theta = widthAngle/2.0 - .001;
    query = Vector3<double>({0.0,std::sin(theta),std::cos(theta)});
    EXPECT_TRUE(InContainer(query,view));
}

// Test that all normals are inward-facing by confirming that the dot product
// of each normal with the boresight is positive
TEST(TestRectView3, TestNormals)
{
    Vector3<double> u({1.0,0.0,0.0});
    Vector3<double> r({0.0,1.0,0.0});
    Vector3<double> z({0.0,0.0,1.0});
    Vector3<double> vertex({0.0,0.0,0.0});

    double widthAngle = M_PI/8.0;
    double heightAngle = M_PI/10.0;

    RectView3<double> view(u,r,vertex,heightAngle,widthAngle);


    for (int i = 0; i < 4; i++)
    {
        EXPECT_TRUE(Dot(view.halfspaces[i].normal,z) > 0.0);
    }

}

// Build a rectangular view with u = x, r = y, vertex = 0
// Test that all corner rays are inward-facing by confirming that the dot
// product of each corner with the boresight is positive
TEST(TestRectView3, TestGetCorners)
{
    Vector3<double> u({1.0,0.0,0.0});
    Vector3<double> r({0.0,1.0,0.0});
    Vector3<double> z({0.0,0.0,1.0});
    Vector3<double> vertex({0.0,0.0,0.0});

    double widthAngle = M_PI/8.0;
    double heightAngle = M_PI/10.0;

    RectView3<double> view(u,r,vertex,heightAngle,widthAngle);

    std::array<Ray3<double>,4> corners;
    view.GetCorners(corners);

    // The dot product of each corner with the boresight (z-axis) should be
    // positive
    for (int i = 0; i < 4; i++)
        EXPECT_TRUE(Dot(corners[i].direction,z) > 0.0);
}