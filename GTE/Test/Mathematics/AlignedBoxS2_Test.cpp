#include <gtest/gtest.h>

#include "Mathematics/Math.h"
#include "Mathematics/AlignedBoxS2.h"

using namespace gte;

// Test expansion of a box starting from the empty box
TEST(TestAlignedBoxS2, TestExpand_1)
{
    AlignedBoxS2<double> box = AlignedBoxS2<double>::Empty();

    // Test expansion from empty to single point (Pi/4,Pi/4)
    double lat = GTE_C_QUARTER_PI, lon = GTE_C_QUARTER_PI;
    box.Expand(lat,lon);
    EXPECT_EQ(box.latMin,lat);
    EXPECT_EQ(box.latMax,lat);
    EXPECT_EQ(box.lonMin,lon);
    EXPECT_EQ(box.lonMax,lon);

    // Test expansion from single point using point (0,0)
    double lat2 = 0, lon2 = 0;
    box.Expand(lat2,lon2);
    EXPECT_EQ(box.latMin,lat2);
    EXPECT_EQ(box.latMax,lat);
    EXPECT_EQ(box.lonMin,lon2);
    EXPECT_EQ(box.lonMax,lon);

    // Test expansion using point (Pi/4,-Pi)
    // Internally, -Pi should be represented as Pi
    double lat3 = -GTE_C_QUARTER_PI, lon3 = -GTE_C_PI;
    box.Expand(lat3,lon3);
    EXPECT_EQ(box.latMin,lat3);
    EXPECT_EQ(box.latMax,lat);
    EXPECT_EQ(box.lonMin,lon2);
    EXPECT_EQ(box.lonMax,GTE_C_PI);
}

TEST(TestAlignedBoxS2, TestCounterclockwiseDist)
{
    double lon1 = 0, lon2 = GTE_C_QUARTER_PI;
    double expected = GTE_C_QUARTER_PI;
    EXPECT_EQ(expected,CounterclockwiseDist(lon1,lon2));

    lon2 = -GTE_C_QUARTER_PI;
    expected = GTE_C_TWO_PI - GTE_C_QUARTER_PI;
    EXPECT_EQ(expected,CounterclockwiseDist(lon1,lon2));

    lon2 = -GTE_C_PI;
    expected = GTE_C_PI;
    EXPECT_EQ(expected,CounterclockwiseDist(lon1,lon2));

    lon1 = -GTE_C_HALF_PI;
    lon2 = GTE_C_HALF_PI;
    expected = GTE_C_PI;
    EXPECT_EQ(expected,CounterclockwiseDist(lon1,lon2));
}

TEST(TestAlignedBoxS2, TestClockwiseDist)
{
    double lon1 = 0, lon2 = GTE_C_QUARTER_PI;
    double expected = GTE_C_TWO_PI - GTE_C_QUARTER_PI;
    EXPECT_EQ(expected,ClockwiseDist(lon1,lon2));

    lon2 = -GTE_C_QUARTER_PI;
    expected = GTE_C_QUARTER_PI;
    EXPECT_EQ(expected,ClockwiseDist(lon1,lon2));

    lon2 = -GTE_C_PI;
    expected = GTE_C_PI;
    EXPECT_EQ(expected,ClockwiseDist(lon1,lon2));

    lon1 = -GTE_C_HALF_PI;
    lon2 = GTE_C_HALF_PI;
    expected = GTE_C_PI;
    EXPECT_EQ(expected,ClockwiseDist(lon1,lon2));
}

// Construct an aligned box from 0 to Pi/4 lat, Pi/2 to Pi/4 lon
// verify that it is inverted and split it into two non-inverted boxes
TEST(TestAlignedBoxS2, TestSplit)
{
    double latMin = 0.0, latMax = GTE_C_QUARTER_PI;
    double lonMin = GTE_C_HALF_PI, lonMax = GTE_C_QUARTER_PI;
    AlignedBoxS2<double> box(latMin,latMax,lonMin,lonMax);

    auto split_boxes = box.Split();
    AlignedBoxS2<double> first = split_boxes[0];
    AlignedBoxS2<double> second = split_boxes[1];

    EXPECT_EQ(first.lonMin,box.lonMin);
    EXPECT_EQ(first.lonMax,GTE_C_PI);
    EXPECT_EQ(second.lonMin,-GTE_C_PI);
    EXPECT_EQ(second.lonMax,box.lonMax);
}