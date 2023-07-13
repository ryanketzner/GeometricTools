#include <gtest/gtest.h>
#include <Mathematics/GridAlignedBox.h>
#include <Mathematics/Vector3.h>
#include <Mathematics/ContAlignedBox.h>

using namespace gte;

TEST(TestGridAlignedBox, TestRandomGrid_3D)
{
    unsigned int seed = 1;
    Vector3<double> min{-1.0,-1.0,-1.0};
    Vector3<double> max{-1.0,-1.0,-1.0};
    AlignedBox3<double> box{min,max};

    int num = 1000;
    std::vector<Vector3<double>> points = RandomGrid(num, box, seed);

    for (const auto& point : points)
    {
        InContainer(point,box);
    }
}

TEST(TestGridAlignedBox, TestRandomGrid_6D)
{
    unsigned int seed = 1;
    Vector<6,double> min{-1.0,-1.0,-1.0,-1.0,-1.0,-1.0};
    Vector<6,double> max{-1.0,-1.0,-1.0,-1.0,-1.0,-1.0};
    AlignedBox<6,double> box{min,max};

    int num = 1000;
    std::vector<Vector<6,double>> points = RandomGrid(num, box, seed);

    for (const auto& point : points)
    {
        InContainer(point,box);
    }
}