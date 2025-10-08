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

inline std::vector<PointS2<Real>> BuildPoly_03()
{
    std::vector<PointS2<Real>> poly(5);
    poly[0] = S2FromIncAz(0.7853981634, 0.7853981634);
    poly[1] = S2FromIncAz(0.7853981634, 2.35619449);
    poly[2] = S2FromIncAz(0.7853981634, 3.926990817);
    poly[3] = S2FromIncAz(0.7853981634, 5.497787144);
    poly[4] = S2FromIncAz(0.7853981634, 0.7853981634); // closed
    return poly;
}

inline PointS2<Real> Contained_03()
{
    return S2FromIncAz(0.0, 0.0);
}
} // namespace

class Poly_03 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        auto verts = BuildPoly_03();
        auto inside = Contained_03();
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

TEST_F(Poly_03, Query_01_numCrossings)
{
    // Inside along z-axis.
    ExpectCrossings(0.0, 0.0, 0);
}

TEST_F(Poly_03, Query_03_numCrossings)
{
    // Inside at a vertex clock angle (starting point).
    ExpectCrossings(0.6283185307, 0.7853981634, 0);
}

TEST_F(Poly_03, Query_04_numCrossings)
{
    // Outside at a vertex clock angle (starting point).
    ExpectCrossings(1.047197551, 0.7853981634, 1);
}

TEST_F(Poly_03, Query_05_numCrossings)
{
    // Inside, middle of a segment in clock angle.
    ExpectCrossings(0.3141592654, 1.570796327, 0);
}

TEST_F(Poly_03, Query_06_numCrossings)
{
    // Outside, middle of a segment in clock angle.
    ExpectCrossings(1.047197551, 1.570796327, 1);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}