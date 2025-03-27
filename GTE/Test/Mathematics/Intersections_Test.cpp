#include <gtest/gtest.h>
#include <algorithm>
#include <cmath>
#include <array>
#include <utility>

#include <Mathematics/Intersections.h>
#include <Mathematics/IntrIntersectionsShape.h>
#include <Mathematics/GridAlignedBox.h>
#include <Mathematics/IntrAlignedBoxAlignedBox.h>
#include <Mathematics/IntrAlignedBoxHalfspace.h>

using namespace gte;

// Helper function to convert an std::array of objects to a tuple.
template <typename T, std::size_t N, std::size_t... I>
auto make_tuple_from_array_impl(const std::array<T, N>& arr, std::index_sequence<I...>)
{
    return std::make_tuple(arr[I]...);
}

template <typename T, std::size_t N>
auto make_tuple_from_array(const std::array<T, N>& arr)
{
    return make_tuple_from_array_impl(arr, std::make_index_sequence<N>{});
}

template <size_t N>
void IntrIntersectionBox()
{
	unsigned int seed = 0;
	using Real = double;

	// Create box from -1 to 1
	Vector<N,Real> min = -1.0*Vector<N,Real>::Ones();
	Vector<N,Real> max = Vector<N,Real>::Ones();
	AlignedBox<N,Real> box(min,max);

	// Generate num random points inside the box
	int num = 100;
	std::vector<Vector<N,Real>> points = RandomGrid<N,Real>(num, box, seed);

	std::sort(points.begin(), points.end());

	std::vector<AlignedBox<N,Real>> boxes;

	// Create boxes from points
	for (int i = 0; i < num - 1; i++)
	{
		AlignedBox<N,Real> box(points[i], points[i+1]);
		boxes.push_back(box);
	}

	TIQuery<Real, AlignedBox<N,Real>, AlignedBox<N,Real>> box_query;

	// An aligned box can be represented as the intersection of halfspaces. Check that
	// if box1 and box2 intersect, then the halfspace representation of box1 and box2 also intersect.
	for (int i = 0; i < num - 1; i++)
	{
		AlignedBox<N,Real> box1 = boxes.at(i);
		auto halfspaces1 = box1.GetHalfspaces();
		auto halfspaces1_tup = make_tuple_from_array(halfspaces1);
		Intersections intersections1(halfspaces1_tup);
		TIQuery<Real, AlignedBox<N,Real>, decltype(intersections1)> intersections_query;


		for (int j = 0; j < num - 1; j++)
		{
			AlignedBox<N,Real> box2 = boxes.at(j);
			EXPECT_EQ(intersections_query(box2,intersections1).intersect, box_query(box1,box2).intersect);
		}
	}

}

TEST(IntrIntersectionBox, TestN2) { IntrIntersectionBox<2>(); }
TEST(IntrIntersectionBox, TestN3) { IntrIntersectionBox<3>(); }
