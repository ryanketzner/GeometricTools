// Test the constructive solid geometry system,
// consisting of intersection and complement operations.

#include <gtest/gtest.h>
#include <vector>
#include <cmath>

// Include necessary headers from Geometric Tools.
#include <Mathematics/GridAlignedBox.h>
#include <Mathematics/Vector3.h>
#include <Mathematics/ContHypersphere.h>
#include <Mathematics/ContShapeIntersection.h>
#include <Mathematics/ContShapeComplement.h>
#include <Mathematics/IntrVectorShape.h>
#include <Mathematics/Hypersphere.h>

// Include the composite shapes header that defines Intersection and Complement.
#include "Mathematics/Intersection.h"
#include "Mathematics/Complement.h"


using namespace gte;

// Generate a grid of one millions points in the box -1 : 1
// Then, form a csg object big_sphere ∩ small_sphere' ∩ tiny_sphere'
// Verify that the inclusion status of the points is appropriate
TEST(TestNestedSpheres, TestInContainer)
{
    using Real = double;
    // Define a box from -1 to 1 in each dimension.
    Vector3<double> min{-1.0, -1.0, -1.0};
    Vector3<double> max{ 1.0,  1.0,  1.0};
    AlignedBox3<double> box{min, max};

    // Generate a random grid of points inside the box.
    unsigned int seed = 1;
    int num = 1000000;
    std::vector<Vector3<double>> grid = RandomGrid(num, box, seed);

    // Create the big sphere (radius 1), small sphere (radius 0.5), and tiny sphere (radius 0.25)
    Hypersphere<3, double> bigSphere;
    bigSphere.center = Vector3<double>{0.0, 0.0, 0.0};
    bigSphere.radius = 1.0;

    Hypersphere<3, double> smallSphere;
    smallSphere.center = Vector3<double>{0.0, 0.0, 0.0};
    smallSphere.radius = 0.5;

    Hypersphere<3, double> tinySphere;
    tinySphere.center = Vector3<double>{0.0, 0.0, 0.0};
    tinySphere.radius = 0.25;

    // Create the composite shape for the spherical shell:
    // old_shell = bigSphere ∩ (complement of smallSphere)
    Intersection<Hypersphere<3, double>, Complement<Hypersphere<3, double>>> old_shell(
        bigSphere,
        Complement<Hypersphere<3, double>>(smallSphere)
    );

    // Now further remove the tiny sphere from the shell:
    // shell = old_shell ∩ (complement of tinySphere)
    // Doesn't do anything (the set shell is the same as the set old_shell),
    // but useful to add to test capability of the CSG logic
    Intersection<decltype(old_shell), Complement<Hypersphere<3, double>>> shell(
        old_shell,
        Complement<Hypersphere<3, double>>(tinySphere)
    );

    for (auto const& p : grid)
    {
        bool result = InContainer(p, shell);
        bool result_big = InContainer(p, bigSphere);
        bool result_small = InContainer(p,smallSphere);

        if (result)
        {
            EXPECT_TRUE(result_big);
            EXPECT_FALSE(result_small);
        }
        else
            EXPECT_FALSE(result_big && !result_small);
    }
}
