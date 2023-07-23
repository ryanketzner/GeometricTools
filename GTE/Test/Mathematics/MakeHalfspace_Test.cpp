#include <gtest/gtest.h>

#include "Mathematics/MakeHalfspace.h"
#include "Mathematics/ContHalfspace.h"

using namespace gte;

// Make an ellipsoid centered at the origin with extents 100, 100, 80
// Test that the polar halfspace of the extremal point of the ellipsoid
// along the each axis is touching the ellipsoid at that point.
// Test that the polar halfspace of a point along each axis outside of
// the ellipsoid contains the corresponding extremal point of the ellipsoid
TEST(TestMakeHalfspace, TestMakePolarHalfspace3_Ellipsoid)
{
    Vector3<double> extent({100, 100, 80});
    Ellipsoid3<double> ellipsoid;
    ellipsoid.extent = extent;

    for (int i = 0; i < 3; i++)
    {
        Vector3<double> ax = ellipsoid.axis[i]*ellipsoid.extent[i];
        Halfspace3<double> halfspace = MakePolarHalfspace3(ax,ellipsoid);

        EXPECT_DOUBLE_EQ(Dot(halfspace.normal,ax), (double)1);
    }

    for (int i = 0; i < 3; i++)
    {
        Vector3<double> ax = ellipsoid.axis[i]*ellipsoid.extent[i] 
            + Vector3<double>((int)i)*10.0;
        Halfspace3<double> halfspace = MakePolarHalfspace3(ax,ellipsoid);

        EXPECT_TRUE(InContainer(ax,halfspace));
    }
}