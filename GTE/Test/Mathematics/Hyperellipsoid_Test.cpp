#include <gtest/gtest.h>

#include "Mathematics/Hyperellipsoid.h"
#include "Mathematics/Vector.h"

using namespace gte;

// Create an ellipsoid centered at zero
// with extents along x and y equal to one
// and along z equal to one half.
// Then, scale it to touch the point [2,3,2]^T
// Confirm that the resulting ellipsoid touches
// this point
TEST(Test3D, ScaleToPoint)
{
    using Real = double;
    const int N = 3;

    // Default constructor sets center to zero
    // and extents to one
    Vector<N,Real> extents({1.0,1.0,.5});
    Hyperellipsoid<N,Real> ellipsoid(Vector<N,Real>::Zero(), extents);

    Vector<N,Real> point({2.0,3.0,2.0});

    ellipsoid.ScaleToPoint(point);

    Matrix<N,N,Real> M;
    ellipsoid.GetM(M);

    Real f = Dot(point, M*point);

    EXPECT_DOUBLE_EQ(f,1.0);
}


// Create an ellipsoid centered at zero
// with extents along x and y equal to one
// and along z equal to one half.
// Then, scale it to touch the point [.1,.2,-.3]^T
// Confirm that the resulting ellipsoid touches
// this point
TEST(Test3D, ScaleToPoint_2)
{
    using Real = double;
    const int N = 3;

    // Default constructor sets center to zero
    // and extents to one
    Vector<N,Real> extents({1.0,1.0,.5});
    Hyperellipsoid<N,Real> ellipsoid(Vector<N,Real>::Zero(), extents);

    Vector<N,Real> point({.1,.2,-.3});

    ellipsoid.ScaleToPoint(point);

    Matrix<N,N,Real> M;
    ellipsoid.GetM(M);

    Real f = Dot(point, M*point);

    EXPECT_DOUBLE_EQ(f,1.0);
}