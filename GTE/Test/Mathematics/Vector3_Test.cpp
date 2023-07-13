
#include <gtest/gtest.h>

#include "Mathematics/Vector3.h"

using namespace gte;

TEST(TestVector3, TestComputeOrthogonalComplement)
{
    Vector3<double> z{0.0,0.0,1.0};

    std::array<Vector3<double>,2> complement = ComputeOrthogonalComplement(z);

    EXPECT_EQ(Cross(complement[0],complement[1]),z);
}

TEST(TestVector3, TestComputeOrthogonalComplement_Original)
{
    Vector3<double> x{1.0,0.0,0.0};
    Vector3<double> y{0.0,1.0,0.0};
    Vector3<double> z{0.0,0.0,1.0};

    Vector3<double> vecs[3];
    vecs[0] = x;

    ComputeOrthogonalComplement(1,vecs);

    EXPECT_EQ(Cross(vecs[0],vecs[1]),vecs[2]);
}