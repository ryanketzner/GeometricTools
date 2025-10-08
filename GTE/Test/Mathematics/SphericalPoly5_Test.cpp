#include <gtest/gtest.h>

#include <Mathematics/SphericalPolygon.h>
#include <Mathematics/FastSphericalPolygon.h>
#include <Mathematics/PointS2.h>
#include <Mathematics/Vector3.h>
#include <Mathematics/Math.h>

using namespace gte;

namespace {
using Real = double;

inline PointS2<Real> S2FromIncAz(Real inc, Real az)
{
    return PointS2<Real>(static_cast<Real>(GTE_C_HALF_PI) - inc, az, /*enforce_bounds=*/false);
}

inline std::vector<PointS2<Real>> BuildPoly_05()
{
    std::vector<PointS2<Real>> poly(4);
    poly[0] = S2FromIncAz(1.256637061, 0.0);
    poly[1] = S2FromIncAz(0.9424777961, 1.570796327);
    poly[2] = S2FromIncAz(0.6283185307, 4.71238898);
    poly[3] = S2FromIncAz(1.256637061, 0.0); // closed
    return poly;
}

inline PointS2<Real> Contained_05()
{
    // Legacy contained = {inc = π, az = 0}.
    return S2FromIncAz(static_cast<Real>(GTE_C_PI), 0.0);
}
} // namespace

class Poly_05 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        auto verts = BuildPoly_05();
        auto inside = Contained_05();
        base = new SphericalPolygon<Real>(verts, inside);
        fast = new FastSphericalPolygon<Real>(verts, inside);
    }

    void TearDown() override
    {
        delete base;
        delete fast;
    }

    void ExpectCrossings(Real inc, Real az, int expectedCrossings)
    {
        PointS2<Real> q = S2FromIncAz(inc, az);
        int cBase = base->NumCrossings(q);
        int cFast = fast->NumCrossings(q);
        ASSERT_EQ(expectedCrossings, cBase);
        ASSERT_EQ(cBase, cFast);
    }

    SphericalPolygon<Real>* base{};
    FastSphericalPolygon<Real>* fast{};
};

TEST_F(Poly_05, Query_01_numCrossings)
{
    // Arbitrary point with expected odd crossings.
    ExpectCrossings(0.04908738521, 4.71238898, 1);
}

TEST_F(Poly_05, Query_02_numCrossings)
{
    // Arbitrary point with expected odd crossings.
    ExpectCrossings(0.09817477042, 4.71238898, 1);
}

TEST_F(Poly_05, Query_03_numCrossings)
{
    // Arbitrary point with expected even crossings.
    ExpectCrossings(1.256637061, 1.570796327, 0);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}