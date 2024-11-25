#include <gtest/gtest.h>
#include "Mathematics/PointS2.h"
#include "Mathematics/GridHyperellipsoid.h"

using namespace gte;

// Generate a million random points on the WGS84 ellipsoid, and test that the point is the
// same after being converted from Cartesian to Geodetic and back again.
TEST(TestPointS2, TestForwardBackward)
{
    using Real = double;
    int num = 1000000;
    
    // These are the same parameters used for WGS84 in the PointS2 class.
    Real a = 6378137.0;
    Real f = 1.0/298.257223563;
    Vector3<Real> extent({a,a,a*(1.0-f)});
    Ellipsoid3<Real> ellipsoid;
    ellipsoid.extent = extent;
    std::vector<Vector3<Real>> grid = RandomSurfaceGrid(num, ellipsoid);

    // Note: Test fails with tol = 1e-9
    Real tol = 1e-8;
    for (auto& point : grid)
        for (int i = 0; i < 3; i++)
            EXPECT_NEAR(point[i], GeodeticToCart(CartToGeodetic(point, a, f), 0.0, a, f)[i], tol);
}