#include <gtest/gtest.h>
#include <cmath>
#include "Mathematics/SDistAlignedBoxS2AlignedHalfspaceLat_Box.h"
#include "Mathematics/AlignedBoxS2.h"
#include "Mathematics/Math.h"

using namespace gte;

TEST(SDistAlignedBoxS2AlignedHalfspaceLat, OnBoundary)
{
    using Real = double;
    
    // Test box with boundary exactly on the halfspace
    AlignedHalfspaceLat<Real> halfspace(GTE_C_PI / 6.0, true);  // 30° north
    // Box with latMax exactly at 30°
    AlignedBoxS2<Real> box(GTE_C_PI / 12.0, GTE_C_PI / 6.0, -GTE_C_PI / 4.0, GTE_C_PI / 4.0);  // lat [15°, 30°], lon [-45°, 45°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLat<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be approximately zero (on boundary)
    EXPECT_NEAR(result.sDistance, 0.0, 1e-10);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLat, InsideNorth)
{
    using Real = double;
    
    // Test box inside the north halfspace
    AlignedHalfspaceLat<Real> halfspace(0.0, true);  // Equator, north side
    // Box entirely north of equator
    AlignedBoxS2<Real> box(GTE_C_PI / 12.0, GTE_C_PI / 6.0, -GTE_C_PI / 4.0, GTE_C_PI / 4.0);  // lat [15°, 30°], lon [-45°, 45°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLat<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be negative (inside)
    EXPECT_LT(result.sDistance, 0.0);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLat, OutsideNorth)
{
    using Real = double;
    
    // Test box outside the north halfspace
    AlignedHalfspaceLat<Real> halfspace(0.0, true);  // Equator, north side
    // Box entirely south of equator
    AlignedBoxS2<Real> box(-GTE_C_PI / 6.0, -GTE_C_PI / 12.0, -GTE_C_PI / 4.0, GTE_C_PI / 4.0);  // lat [-30°, -15°], lon [-45°, 45°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLat<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be positive (outside)
    EXPECT_GT(result.sDistance, 0.0);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLat, InsideSouth)
{
    using Real = double;
    
    // Test box inside the south halfspace
    AlignedHalfspaceLat<Real> halfspace(0.0, false);  // Equator, south side
    // Box entirely south of equator
    AlignedBoxS2<Real> box(-GTE_C_PI / 6.0, -GTE_C_PI / 12.0, -GTE_C_PI / 4.0, GTE_C_PI / 4.0);  // lat [-30°, -15°], lon [-45°, 45°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLat<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be negative (inside)
    EXPECT_LT(result.sDistance, 0.0);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLat, OutsideSouth)
{
    using Real = double;
    
    // Test box outside the south halfspace
    AlignedHalfspaceLat<Real> halfspace(0.0, false);  // Equator, south side
    // Box entirely north of equator
    AlignedBoxS2<Real> box(GTE_C_PI / 12.0, GTE_C_PI / 6.0, -GTE_C_PI / 4.0, GTE_C_PI / 4.0);  // lat [15°, 30°], lon [-45°, 45°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLat<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be positive (outside)
    EXPECT_GT(result.sDistance, 0.0);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLat, BoundaryCases)
{
    using Real = double;
    
    // Test box near poles
    AlignedHalfspaceLat<Real> halfspace(GTE_C_HALF_PI - 0.1, true);  // Near north pole
    // Box with latMax even closer to north pole
    AlignedBoxS2<Real> box(GTE_C_HALF_PI - 0.15, GTE_C_HALF_PI - 0.05, -GTE_C_PI / 4.0, GTE_C_PI / 4.0);  // lat [near pole], lon [-45°, 45°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLat<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be inside (negative)
    EXPECT_LT(result.sDistance, 0.0);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLat, KnownDistance30Degrees)
{
    using Real = double;
    
    // Test box exactly 30 degrees away from halfspace
    AlignedHalfspaceLat<Real> halfspace(0.0, true);  // Equator, north side
    Real thirtyDeg = 30.0 * GTE_C_PI / 180.0;
    // Box with latMin at 30° (so distance from latMax would be different, but latMin is 30° away)
    AlignedBoxS2<Real> box(thirtyDeg, thirtyDeg + GTE_C_PI / 12.0, -GTE_C_PI / 4.0, GTE_C_PI / 4.0);  // lat [30°, 45°], lon [-45°, 45°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLat<Real>> query;
    auto result = query(box, halfspace);
    
    // Box is inside (north of equator), so negative distance
    // Distance should be exactly 30 degrees (from latMin to halfspace)
    EXPECT_LT(result.sDistance, 0.0);
    EXPECT_NEAR(std::abs(result.sDistance), thirtyDeg, 1e-10);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLat, KnownDistance45Degrees)
{
    using Real = double;
    
    // Test box exactly 15 degrees away from halfspace (halfspace at 30°, box at 45°)
    AlignedHalfspaceLat<Real> halfspace(GTE_C_PI / 6.0, true);  // 30° north, north side
    Real thirtyDeg = 30.0 * GTE_C_PI / 180.0;
    Real fifteenDeg = 15.0 * GTE_C_PI / 180.0;
    // Box with latMin at 45° (15° away from halfspace at 30°)
    AlignedBoxS2<Real> box(thirtyDeg + fifteenDeg, thirtyDeg + fifteenDeg + GTE_C_PI / 12.0, -GTE_C_PI / 4.0, GTE_C_PI / 4.0);  // lat [45°, 60°], lon [-45°, 45°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLat<Real>> query;
    auto result = query(box, halfspace);
    
    // Box is inside (north of halfspace), so negative
    EXPECT_LT(result.sDistance, 0.0);
    EXPECT_NEAR(std::abs(result.sDistance), fifteenDeg, 1e-10);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLat, CloseToBoundary)
{
    using Real = double;
    
    // Test box very close to boundary (1 degree away)
    AlignedHalfspaceLat<Real> halfspace(0.0, true);  // Equator, north side
    Real oneDeg = 1.0 * GTE_C_PI / 180.0;
    // Small box with latMin at 1°
    AlignedBoxS2<Real> box(oneDeg, oneDeg + GTE_C_PI / 36.0, -GTE_C_PI / 12.0, GTE_C_PI / 12.0);  // lat [1°, 6°], lon [-15°, 15°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLat<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be inside, distance approximately 1 degree
    EXPECT_LT(result.sDistance, 0.0);
    EXPECT_NEAR(std::abs(result.sDistance), oneDeg, 1e-8);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLat, FarFromBoundary)
{
    using Real = double;
    
    // Test box far from boundary (60 degrees away)
    AlignedHalfspaceLat<Real> halfspace(0.0, true);  // Equator, north side
    Real sixtyDeg = 60.0 * GTE_C_PI / 180.0;
    // Box with latMin at 60°
    AlignedBoxS2<Real> box(sixtyDeg, sixtyDeg + GTE_C_PI / 12.0, -GTE_C_PI / 4.0, GTE_C_PI / 4.0);  // lat [60°, 75°], lon [-45°, 45°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLat<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be inside, distance exactly 60 degrees
    EXPECT_LT(result.sDistance, 0.0);
    EXPECT_NEAR(std::abs(result.sDistance), sixtyDeg, 1e-10);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLat, VeryCloseToBoundary)
{
    using Real = double;
    
    // Test box extremely close to boundary (0.1 degrees)
    AlignedHalfspaceLat<Real> halfspace(0.0, true);  // Equator, north side
    Real tinyDeg = 0.1 * GTE_C_PI / 180.0;
    // Very small box with latMin at 0.1°
    AlignedBoxS2<Real> box(tinyDeg, tinyDeg + GTE_C_PI / 180.0, -GTE_C_PI / 24.0, GTE_C_PI / 24.0);  // lat [0.1°, 1.1°], lon [-7.5°, 7.5°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLat<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be inside, distance approximately 0.1 degrees
    EXPECT_LT(result.sDistance, 0.0);
    EXPECT_NEAR(std::abs(result.sDistance), tinyDeg, 1e-8);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLat, OutsideKnownDistance)
{
    using Real = double;
    
    // Test box outside, known distance away
    AlignedHalfspaceLat<Real> halfspace(0.0, true);  // Equator, north side
    Real thirtyDeg = 30.0 * GTE_C_PI / 180.0;
    // Box with latMax at -30° (outside, south of equator)
    AlignedBoxS2<Real> box(-thirtyDeg - GTE_C_PI / 12.0, -thirtyDeg, -GTE_C_PI / 4.0, GTE_C_PI / 4.0);  // lat [-45°, -30°], lon [-45°, 45°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLat<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be outside (south of equator), so positive distance
    EXPECT_GT(result.sDistance, 0.0);
    EXPECT_NEAR(result.sDistance, thirtyDeg, 1e-10);
}

