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