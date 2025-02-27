#include <gtest/gtest.h>
#include <array>

#include <Mathematics/Vector.h>
#include <Mathematics/ContCone.h>
#include <Mathematics/AlignedBox.h>
#include <Mathematics/GridAlignedBox.h>

// Tests for InContainer(AlignedBox, Cone)

using namespace gte;

// Generate one million random points in the -10,10 aligned box.
// Then, for each adjacent pair of points, generate an aligned box.
// Then, ensure that if the cone contains the aligned box, it contains
// all the vertices of the aligned box.
template <size_t N>
void TestCone(Vector<N,double> const& origin, Vector<N,double> const& direction, double angle = GTE_C_QUARTER_PI)
{
    unsigned int seed = 1;
    constexpr unsigned int NUM_VERTICES = (1 << N);

    Ray<N,double> ray{origin,direction};
    Cone<N,double> cone(ray,angle);

    Vector<N,double> min = -10.0*Vector<N,double>::Ones();
    Vector<N,double> max = 10.0*Vector<N,double>::Ones();
    AlignedBox<N,double> box{min,max};

    int num = 1000000;
    std::vector<Vector<N,double>> points = RandomGrid(num, box, seed);
    std::vector<AlignedBox<N,double>> boxes(num);

    // Generate boxes using adjacent random points in grid
    for (int i = 0; i < num - 1; i++)
    {
    	boxes[i] = AlignedBox<N,double>(points[i],points[i+1]);
    }

    for (auto& box : boxes)
    {
    	// If the box is in the cone,
    	// all of its vertices should be in the cone.
    	if (InContainer(box, cone))
    	{
    		std::array<Vector<N,double>,NUM_VERTICES> vertices;
    		box.GetVertices(vertices);
    		for (auto& vertex : vertices)
    			EXPECT_TRUE(InContainer(vertex,cone));
    	}
    }
}

// A unit cone centered at the origin
template <size_t N>
void TestUnitCone()
{
	Vector<N,double> origin = Vector<N,double>::Zero();
    Vector<N,double> direction = Vector<N,double>::Unit(0);

    TestCone<N>(origin, direction);
}

// Generate a random cone by generating two random points
// in the box -10,10. Use one of the random points for
// the vertex, and the next one (normalized) for the direction
template <size_t N>
void TestRandomCone()
{
	// Generate box from -10 to 10
    Vector<N,double> min = -10.0*Vector<N,double>::Ones();
    Vector<N,double> max = 10.0*Vector<N,double>::Ones();
    AlignedBox<N,double> box{min,max};

    // Generate two random points
    int num = 2;
    std::vector<Vector<N,double>> points = RandomGrid(num, box);
    Normalize(points[1]);

    // Use these points to define cone origin and direction
	TestCone<N>(points[0], points[1]);
}

TEST(TestUnitCone, TestN1) { TestUnitCone<1>(); }
TEST(TestUnitCone, TestN2) { TestUnitCone<2>(); }
//TEST(TestUnitCone, TestN3) { TestUnitCone<3>(); }

TEST(TestRandomCone, TestN1) { TestRandomCone<1>(); }
TEST(TestRandomCone, TestN2) { TestRandomCone<2>(); }
//TEST(TestRandomCone, TestN3) { TestRandomCone<3>(); }