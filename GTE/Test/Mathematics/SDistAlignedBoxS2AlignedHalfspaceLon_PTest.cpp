#include <gtest/gtest.h>
#include <cmath>
#include "Mathematics/SDistAlignedBoxS2AlignedHalfspaceLon_Box.h"
#include "Mathematics/AlignedBoxS2.h"
#include "Mathematics/Math.h"

using namespace gte;

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, OnBoundary)
{
    using Real = double;
    
    // Test box with boundary exactly on the halfspace
    AlignedHalfspaceLon<Real> halfspace(GTE_C_PI / 4.0, true);  // 45° east
    // Box with lonMax exactly at 45°
    AlignedBoxS2<Real> box(GTE_C_PI / 6.0, GTE_C_PI / 3.0, GTE_C_PI / 6.0, GTE_C_PI / 4.0);  // lat [30°, 60°], lon [30°, 45°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be approximately zero (on boundary)
    EXPECT_NEAR(result.sDistance, 0.0, 1e-10);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, InsideRight)
{
    using Real = double;
    
    // Test box inside the right/east halfspace
    AlignedHalfspaceLon<Real> halfspace(0.0, true);  // Prime meridian, right side
    // Box entirely east of meridian
    AlignedBoxS2<Real> box(-GTE_C_PI / 6.0, GTE_C_PI / 6.0, GTE_C_PI / 8.0, GTE_C_PI / 4.0);  // lat [-30°, 30°], lon [22.5°, 45°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be negative (inside)
    EXPECT_LT(result.sDistance, 0.0);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, OutsideRight)
{
    using Real = double;
    
    // Test box outside the right/east halfspace
    AlignedHalfspaceLon<Real> halfspace(0.0, true);  // Prime meridian, right side
    // Box entirely west of meridian
    AlignedBoxS2<Real> box(-GTE_C_PI / 6.0, GTE_C_PI / 6.0, -GTE_C_PI / 4.0, -GTE_C_PI / 8.0);  // lat [-30°, 30°], lon [-45°, -22.5°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be positive (outside)
    EXPECT_GT(result.sDistance, 0.0);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, InsideLeft)
{
    using Real = double;
    
    // Test box inside the left/west halfspace
    AlignedHalfspaceLon<Real> halfspace(0.0, false);  // Prime meridian, left side
    // Box entirely west of meridian
    AlignedBoxS2<Real> box(-GTE_C_PI / 6.0, GTE_C_PI / 6.0, -GTE_C_PI / 4.0, -GTE_C_PI / 8.0);  // lat [-30°, 30°], lon [-45°, -22.5°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be negative (inside)
    EXPECT_LT(result.sDistance, 0.0);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, WrapAround)
{
    using Real = double;
    
    // Test wrap-around: box at 179° and halfspace at -179°
    // The shorter distance should be 2° (not 358°)
    AlignedHalfspaceLon<Real> halfspace(-GTE_C_PI + GTE_C_PI / 180.0, true);  // -179°, right
    // Box with lonMax at 179°
    Real latRange = GTE_C_PI / 6.0;
    AlignedBoxS2<Real> box(-latRange, latRange, GTE_C_PI - 2.0 * GTE_C_PI / 180.0, GTE_C_PI - GTE_C_PI / 180.0);  // lat [-30°, 30°], lon [178°, 179°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Should pick shorter path (2° = ~0.035 radians)
    // Box is to the right of halfspace, so should be inside (negative)
    EXPECT_LT(result.sDistance, 0.0);
    EXPECT_NEAR(std::abs(result.sDistance), 2.0 * GTE_C_PI / 180.0, 1e-6);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, WrapAroundOpposite)
{
    using Real = double;
    
    // Test wrap-around in opposite direction
    AlignedHalfspaceLon<Real> halfspace(GTE_C_PI - GTE_C_PI / 180.0, true);  // 179°, right
    // Box with lonMax at -179°
    Real latRange = GTE_C_PI / 6.0;
    AlignedBoxS2<Real> box(-latRange, latRange, -GTE_C_PI + GTE_C_PI / 180.0, -GTE_C_PI + 2.0 * GTE_C_PI / 180.0);  // lat [-30°, 30°], lon [-179°, -178°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Should pick shorter path, box is to the left, so outside (positive)
    EXPECT_GT(result.sDistance, 0.0);
    EXPECT_NEAR(result.sDistance, 2.0 * GTE_C_PI / 180.0, 1e-6);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, KnownDistance30Degrees)
{
    using Real = double;
    
    // Test box exactly 30 degrees away from halfspace
    AlignedHalfspaceLon<Real> halfspace(0.0, true);  // Prime meridian, right side
    Real thirtyDeg = 30.0 * GTE_C_PI / 180.0;
    // Box with lonMin at 30° (so distance from lonMax would be different, but lonMin is 30° away)
    Real latRange = GTE_C_PI / 6.0;
    AlignedBoxS2<Real> box(-latRange, latRange, thirtyDeg, thirtyDeg + GTE_C_PI / 12.0);  // lat [-30°, 30°], lon [30°, 45°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Box is inside (east of meridian), so negative distance
    // Distance should be exactly 30 degrees (from lonMin to halfspace)
    EXPECT_LT(result.sDistance, 0.0);
    EXPECT_NEAR(std::abs(result.sDistance), thirtyDeg, 1e-10);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, KnownDistance45Degrees)
{
    using Real = double;
    
    // Test box exactly 45 degrees away from halfspace
    AlignedHalfspaceLon<Real> halfspace(GTE_C_PI / 4.0, true);  // 45° east, right side
    Real fortyFiveDeg = 45.0 * GTE_C_PI / 180.0;
    // Box with lonMin at 90° (45° away from halfspace at 45°)
    Real latRange = GTE_C_PI / 6.0;
    AlignedBoxS2<Real> box(-latRange, latRange, fortyFiveDeg + fortyFiveDeg, GTE_C_PI / 2.0 + GTE_C_PI / 12.0);  // lat [-30°, 30°], lon [90°, 105°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Box is inside (east of halfspace), so negative
    EXPECT_LT(result.sDistance, 0.0);
    EXPECT_NEAR(std::abs(result.sDistance), fortyFiveDeg, 1e-10);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, CloseToBoundary)
{
    using Real = double;
    
    // Test box very close to boundary (1 degree away)
    AlignedHalfspaceLon<Real> halfspace(0.0, true);  // Prime meridian, right side
    Real oneDeg = 1.0 * GTE_C_PI / 180.0;
    // Small box with lonMin at 1°
    Real latRange = GTE_C_PI / 12.0;
    AlignedBoxS2<Real> box(-latRange, latRange, oneDeg, oneDeg + GTE_C_PI / 36.0);  // lat [-15°, 15°], lon [1°, 6°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be inside, distance approximately 1 degree
    EXPECT_LT(result.sDistance, 0.0);
    EXPECT_NEAR(std::abs(result.sDistance), oneDeg, 1e-8);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, FarFromBoundary)
{
    using Real = double;
    
    // Test box far from boundary (90 degrees away)
    AlignedHalfspaceLon<Real> halfspace(0.0, true);  // Prime meridian, right side
    Real ninetyDeg = 90.0 * GTE_C_PI / 180.0;
    // Box with lonMin at 90°
    Real latRange = GTE_C_PI / 6.0;
    AlignedBoxS2<Real> box(-latRange, latRange, ninetyDeg, ninetyDeg + GTE_C_PI / 12.0);  // lat [-30°, 30°], lon [90°, 105°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be inside, distance exactly 90 degrees
    EXPECT_LT(result.sDistance, 0.0);
    EXPECT_NEAR(std::abs(result.sDistance), ninetyDeg, 1e-10);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, VeryCloseToBoundary)
{
    using Real = double;
    
    // Test box extremely close to boundary (0.1 degrees)
    AlignedHalfspaceLon<Real> halfspace(0.0, true);  // Prime meridian, right side
    Real tinyDeg = 0.1 * GTE_C_PI / 180.0;
    // Very small box with lonMin at 0.1°
    Real latRange = GTE_C_PI / 24.0;
    AlignedBoxS2<Real> box(-latRange, latRange, tinyDeg, tinyDeg + GTE_C_PI / 180.0);  // lat [-7.5°, 7.5°], lon [0.1°, 1.1°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be inside, distance approximately 0.1 degrees
    EXPECT_LT(result.sDistance, 0.0);
    EXPECT_NEAR(std::abs(result.sDistance), tinyDeg, 1e-8);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, OutsideKnownDistance)
{
    using Real = double;
    
    // Test box outside, known distance away
    AlignedHalfspaceLon<Real> halfspace(0.0, true);  // Prime meridian, right side
    Real thirtyDeg = 30.0 * GTE_C_PI / 180.0;
    // Box with lonMax at -30° (outside, west of meridian)
    Real latRange = GTE_C_PI / 6.0;
    AlignedBoxS2<Real> box(-latRange, latRange, -thirtyDeg - GTE_C_PI / 12.0, -thirtyDeg);  // lat [-30°, 30°], lon [-45°, -30°]
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Should be outside (west of meridian), so positive distance
    EXPECT_GT(result.sDistance, 0.0);
    EXPECT_NEAR(result.sDistance, thirtyDeg, 1e-10);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, WrappedBoxOverlaps)
{
    using Real = double;
    
    // Test wrapped box that overlaps with halfspace
    // Box wraps: [170°, 180°] ∪ [-180°, -170°] (wraps antimeridian)
    // Halfspace at 175° east (lon >= 175°)
    AlignedHalfspaceLon<Real> halfspace(175.0 * GTE_C_PI / 180.0, true);  // 175° east
    Real latRange = GTE_C_PI / 6.0;
    // Wrapped box: lonMin=170°, lonMax=-170° (inverted)
    AlignedBoxS2<Real> box(-latRange, latRange, 
                            170.0 * GTE_C_PI / 180.0, 
                            -170.0 * GTE_C_PI / 180.0);
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Box overlaps (175° is in [170°, 180°] interval), so should be negative
    EXPECT_LT(result.sDistance, 0.0);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, WrappedBoxOutside)
{
    using Real = double;
    
    // Test wrapped box that doesn't overlap with halfspace
    // Box wraps: [170°, 180°] ∪ [-180°, -170°]
    // Halfspace at 0° east (lon >= 0°)
    AlignedHalfspaceLon<Real> halfspace(0.0, true);  // Prime meridian, east
    Real latRange = GTE_C_PI / 6.0;
    // Wrapped box: lonMin=170°, lonMax=-170°
    AlignedBoxS2<Real> box(-latRange, latRange, 
                            170.0 * GTE_C_PI / 180.0, 
                            -170.0 * GTE_C_PI / 180.0);
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Box doesn't overlap (0° is not in either interval), so should be positive
    EXPECT_GT(result.sDistance, 0.0);
}

TEST(SDistAlignedBoxS2AlignedHalfspaceLon, WrappedBoxBoundaryInside)
{
    using Real = double;
    
    // Test wrapped box with boundary exactly on box edge
    // Box wraps: [170°, 180°] ∪ [-180°, -170°]
    // Halfspace at 180° (π) east
    AlignedHalfspaceLon<Real> halfspace(GTE_C_PI, true);  // 180° east
    Real latRange = GTE_C_PI / 6.0;
    AlignedBoxS2<Real> box(-latRange, latRange, 
                            170.0 * GTE_C_PI / 180.0, 
                            -170.0 * GTE_C_PI / 180.0);
    
    SDQuery<Real, AlignedBoxS2<Real>, AlignedHalfspaceLon<Real>> query;
    auto result = query(box, halfspace);
    
    // Boundary is on box edge (π), so should be negative (overlaps)
    EXPECT_LE(result.sDistance, 0.0);
}

