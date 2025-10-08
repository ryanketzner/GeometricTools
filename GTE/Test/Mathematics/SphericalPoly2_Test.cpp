#include <gtest/gtest.h>

#include <Mathematics/SphericalPolygon.h>
#include <Mathematics/FastSphericalPolygon.h>
#include <Mathematics/PointS2.h>
#include <Mathematics/Vector3.h>
#include <Mathematics/Math.h>

using namespace gte;

namespace {
using Real = double;

// Legacy datasets use (inc, az) where inc is co-latitude in [0, π].
// Convert to PointS2's latitude via lat = π/2 − inc.
inline PointS2<Real> S2FromIncAz(Real inc, Real az)
{
    return PointS2<Real>(static_cast<Real>(GTE_C_HALF_PI) - inc, az, /*enforce_bounds=*/false);
}

inline std::vector<PointS2<Real>> BuildPoly_02()
{
    std::vector<PointS2<Real>> poly(17);
    poly[0]  = S2FromIncAz(1.047197551, 0.0);
    poly[1]  = S2FromIncAz(0.7853981634, 0.3926990817);
    poly[2]  = S2FromIncAz(1.047197551, 0.7853981634);
    poly[3]  = S2FromIncAz(0.7853981634, 1.178097245);
    poly[4]  = S2FromIncAz(1.047197551, 1.570796327);
    poly[5]  = S2FromIncAz(0.7853981634, 1.963495408);
    poly[6]  = S2FromIncAz(1.047197551, 2.35619449);
    poly[7]  = S2FromIncAz(0.7853981634, 2.748893572);
    poly[8]  = S2FromIncAz(1.047197551, 3.141592654);
    poly[9]  = S2FromIncAz(0.7853981634, 3.534291735);
    poly[10] = S2FromIncAz(1.047197551, 3.926990817);
    poly[11] = S2FromIncAz(0.7853981634, 4.319689899);
    poly[12] = S2FromIncAz(1.047197551, 4.71238898);
    poly[13] = S2FromIncAz(0.7853981634, 5.105088062);
    poly[14] = S2FromIncAz(1.047197551, 5.497787144);
    poly[15] = S2FromIncAz(0.7853981634, 5.890486225);
    poly[16] = S2FromIncAz(1.047197551, 0.0); // closed
    return poly;
}

inline PointS2<Real> Contained_02()
{
    // Legacy contained = {inc=0, az=0} → lat = +π/2 (north pole).
    return S2FromIncAz(0.0, 0.0);
}
} // namespace

class Poly_02 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        auto verts = BuildPoly_02();
        auto inside = Contained_02();
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

TEST_F(Poly_02, Query_01_numCrossings)
{
    // Inside at same clock angle as a vertex (starting point).
    ExpectCrossings(0.7853981634, 0.0, 0);
}

TEST_F(Poly_02, Query_02_numCrossings)
{
    // Outside at same clock angle as a vertex (starting point).
    ExpectCrossings(1.256637061, 0.0, 1);
}

TEST_F(Poly_02, Query_03_numCrossings)
{
    // Inside along z-axis.
    ExpectCrossings(0.0, 0.0, 0);
}

TEST_F(Poly_02, Query_05_numCrossings)
{
    // Inside, middle of a segment in clock angle.
    ExpectCrossings(0.5235987756, 0.1963495408, 0);
}

TEST_F(Poly_02, Query_06_numCrossings)
{
    // Outside, middle of a segment in clock angle.
    ExpectCrossings(1.256637061, 0.1963495408, 1);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}