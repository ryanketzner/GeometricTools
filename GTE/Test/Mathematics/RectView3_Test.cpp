#include <gtest/gtest.h>

#include "Mathematics/RectView3.h"
#include "Mathematics/ContRectView3.h"

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