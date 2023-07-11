
#include <gtest/gtest.h>
#include <Mathematics/ContAlignedBoxS2.h>

using namespace gte;

TEST(TestContAlignedBoxS2, TestInContainer)
{
    double latMin = -GTE_C_QUARTER_PI;
    double latMax = GTE_C_HALF_PI;
    double lonMin = -GTE_C_QUARTER_PI;
    double lonMax = GTE_C_HALF_PI;
    AlignedBoxS2<double> box({latMin,latMax,lonMin,lonMax});

    PointS2<double> point(0.0,0.0);
    EXPECT_TRUE(InContainer(point,box));

    point = PointS2<double>(0.0,GTE_C_PI);
    EXPECT_FALSE(InContainer(point,box));

    point = PointS2<double>(GTE_C_HALF_PI,GTE_C_HALF_PI);
    EXPECT_TRUE(InContainer(point,box));
}

// Generate points at Pi/6,Pi/5,Pi/4,Pi/3, and Pi/2 lat/lon and verify that
// the resulting AlignedBoxS2 ranges from Pi/6 to Pi/2 in lat/lon
TEST(TestContAlignedBoxS2,TestGetContainer)
{
    std::vector<double> lats({GTE_C_PI/6.0, GTE_C_PI/5.0, GTE_C_PI/4.0,
        GTE_C_PI/3.0, GTE_C_PI/2.0});
    std::vector<double> lons = lats;

    std::vector<PointS2<double>> points;
    for (int i = 0; i < lats.size(); i++)
        points.push_back(PointS2<double>{lats[i],lons[i]});

    AlignedBoxS2<double> box;
    GetContainer(points,box);

    AlignedBoxS2<double> expected_box(GTE_C_PI/6.0, GTE_C_PI/2.0, GTE_C_PI/6.0,
        GTE_C_PI/2.0);

    EXPECT_EQ(expected_box,box);
}

// Generate points at Pi/4, -Pi/4, Pi lon, zero lat and verify that
// the resulting AlignedBoxS2 ranges from -Pi/4 to Pi in lon and 0 to 0 in lat
TEST(TestContAlignedBoxS2,TestGetContainer_2)
{
    std::vector<double> lons({GTE_C_QUARTER_PI,-GTE_C_QUARTER_PI,GTE_C_PI});
    std::vector<double> lats({0.0,0.0,0.0});

    std::vector<PointS2<double>> points;
    for (int i = 0; i < lats.size(); i++)
        points.push_back(PointS2<double>{lats[i],lons[i]});

    AlignedBoxS2<double> box;
    GetContainer(points,box);

    AlignedBoxS2<double> expected_box(0.0, 0.0, -GTE_C_QUARTER_PI,
        GTE_C_PI);

    EXPECT_EQ(expected_box,box);
}