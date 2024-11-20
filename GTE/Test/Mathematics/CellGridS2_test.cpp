#include <gtest/gtest.h>

#include <Mathematics/CellGridS2.h>

using namespace gte;

// Create a cell grid representing the segments of the GTOPO30 elevation model.
// Test points and make sure they return the index of the appropriate segment.
// Segments numbers (in the data vector) are given in row-major order from left to right, bottom to top.
// See https://www.researchgate.net/publication/264159030_Global_Multi-resolution_Terrain_Elevation_Data_2010_GMTED2010/figures?lo=1
// for figure
TEST(TestFullGrid, GTOPO30)
{
    using Real = double;

    Real deg2rad = GTE_C_PI/180.0;
    std::vector<Real> lat_bounds({-GTE_C_HALF_PI, -60.0*deg2rad, -10.0*deg2rad, 40.0*deg2rad, GTE_C_HALF_PI});
    std::vector<int> num_lon({6,9,9,9});

    AlignedBoxS2<Real> box = AlignedBoxS2<Real>::Full();

    std::vector<int> data({1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33});

    CellGridS2<Real,int> grid(lat_bounds, num_lon, box, data);

    // Test several points
    EXPECT_EQ(grid.at(-GTE_C_HALF_PI, -GTE_C_PI), 1); // bottom left point
    EXPECT_EQ(grid.at(-GTE_C_HALF_PI, GTE_C_PI), 6); // bottom right point
    EXPECT_EQ(grid.at(GTE_C_HALF_PI, GTE_C_PI), 33); // top right point
    EXPECT_EQ(grid.at(GTE_C_HALF_PI, -GTE_C_PI), 25); // top left point

    EXPECT_EQ(grid.at(-10.0*deg2rad, GTE_C_PI), 24); // point on boundaries

    EXPECT_EQ(grid.at(0.0,0.0), 20); // point in middle of cell
}