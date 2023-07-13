#include <gtest/gtest.h>

#include <Mathematics/ContRectView3Cone3.h>
#include <Mathematics/ContRectView3.h>
#include <Mathematics/ContCone.h>
#include <Mathematics/GridAlignedBox.h>

using namespace gte;

// Create a cone centered at the origin pointed towards the z axis with an
// angle of Pi/4. Test that the fov in each direction of the RectView3 is
// Pi/2, that the origin is the same, and that the dot products of the u and
// r vectors with the cone direction are zero.
TEST(TestContRectView3Cone3,TestInContainer)
{
    Vector3<double> origin{0.0,0.0,0.0};
    Vector3<double> direction{0.0,0.0,1.0};
    Ray3<double> ray{origin,direction};
    double angle = GTE_C_QUARTER_PI;

    Cone3<double> cone(ray,angle);
    RectView3<double> view;
    GetContainer(cone,view);

    // Test that cone origin and rectview origin are same
    EXPECT_EQ(view.vertex,origin);
    // Test that FOV angles are twice the cone angle
    EXPECT_EQ(GTE_C_QUARTER_PI*2.0,view.GetAngleHeight());
    EXPECT_EQ(GTE_C_QUARTER_PI*2.0,view.GetAngleWidth());

    // Dot product of the up/right vectors and the cone direction should be 0
    EXPECT_EQ(0.0,Dot(view.uVector,direction));
    EXPECT_EQ(0.0,Dot(view.rVector,direction));
}

// Create a random million-point grid on a box from -10 to 10 on each axis.
// Create a cone centered at the origin pointed towards the z axis.
// Test that the RectView which contains the cone contains every
// point in the grid which is contained in the cone.
TEST(TestContRectView3Cone3,TestInContainer_2)
{
    unsigned int seed = 1;

    Vector3<double> origin{0.0,0.0,0.0};
    Vector3<double> direction{0.0,0.0,1.0};
    Ray3<double> ray{origin,direction};
    double angle = GTE_C_QUARTER_PI;

    Cone3<double> cone(ray,angle);
    RectView3<double> view;
    GetContainer(cone,view);

    int num = 1000000;
    Vector3<double> min{-10.0,-10.0,-10.0};
    Vector3<double> max{10.0,10.0,10.0};
    AlignedBox3<double> box{min,max};
    std::vector<Vector3<double>> grid = RandomGrid(num,box,seed);

    for (const auto& point : grid)
    {
        bool inCone = InContainer(point,cone);
        bool inView = InContainer(point,view);

        if (inCone)
            EXPECT_TRUE(inView);
    }
}