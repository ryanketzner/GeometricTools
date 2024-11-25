#include <gtest/gtest.h>
#include "Mathematics/PointS2.h"

using namespace gte;

TEST(TestPointS2, TestCartToGeographic)
{
    Vector3<double> v({-1.0,0.0,0.0});
    PointS2<double> p = CartToGeographic(v);

    double expected_lon = GTE_C_PI;
    double expected_lat = 0.0;

    EXPECT_EQ(expected_lon,p.Lon());
    EXPECT_EQ(expected_lat,p.Lat());
}

TEST(TestPointS2, TestCartToGeodetic)
{
    Vector3<double> v({-1.0,0.0,0.0});
    PointS2<double> p = CartToGeodetic(v);

    double expected_lon = GTE_C_PI;
    double expected_lat = 0.0;

    EXPECT_EQ(expected_lon,p.Lon());
    EXPECT_EQ(expected_lat,p.Lat());
}

// Test the point 1,1,0, and verify that
// lat is zero and lon is 45 degrees
TEST(TestPointS2, TestCartToGeodetic_2)
{
    Vector3<double> v({1.0,1.0,0.0});
    PointS2<double> p = CartToGeodetic(v);

    double expected_lon = GTE_C_QUARTER_PI;
    double expected_lat = 0.0;

    EXPECT_EQ(expected_lon,p.Lon());
    EXPECT_EQ(expected_lat,p.Lat());
}

// Test a point very close to pole
TEST(TestPointS2, TestCartToGeodetic_3)
{
    double epsilon = std::numeric_limits<double>::epsilon();
    Vector3<double> v({epsilon,epsilon,1.0});
    PointS2<double> p = CartToGeodetic(v);

    double expected_lon = GTE_C_QUARTER_PI;
    double expected_lat = GTE_C_HALF_PI;

    EXPECT_EQ(expected_lon,p.Lon());
    EXPECT_EQ(expected_lat,p.Lat());
}

// Test a point and compare output to webtool
// https://www.apsalin.com/cartesian-to-geodetic-on-ellipsoid/
TEST(TestPointS2, TestCartToGeodetic_4)
{
    double epsilon = std::numeric_limits<double>::epsilon();
    Vector3<double> v({7000000,7000000,7000000});
    PointS2<double> p = CartToGeodetic(v);

    double expected_lon = 45.0;
    double expected_lat = 35.3597258690;

    double tol = 1e-9;
    double rad2deg = 180.0/GTE_C_PI;
    EXPECT_NEAR(expected_lon,p.Lon()*rad2deg,tol);
    EXPECT_NEAR(expected_lat,p.Lat()*rad2deg,tol);
}