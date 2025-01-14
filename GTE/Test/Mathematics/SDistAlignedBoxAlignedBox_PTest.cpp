#include <gtest/gtest.h>

#include "Mathematics/SDistAlignedBoxAlignedBox.h"
#include "Mathematics/IntrAlignedBoxAlignedBox.h"
#include "Mathematics/Vector.h"
#include "Mathematics/ContAlignedBox.h"
#include "Mathematics/GridAlignedBox.h"

using namespace gte;

// All tests the same except for dimension.

// Create an alligned box with min = 10, max = 100
// Then, generate one million random points between -200 and 200.
// Loop over the points and generate random boxes from each adjacent
// pair of points. Then, check that for each box, if the signed distance is negative,
// the boxes intersect.
TEST(TestSignedDistance, 1D)
{
	const int N = 1;
	using T = double;
	
	Vector<N,T> min1 = 10.0*Vector<N,T>::Ones();
	Vector<N,T> max1 = 100.0*Vector<N,T>::Ones();
	AlignedBox<N,T> box1{min1,max1};
	
	SDQuery<T, AlignedBox<N,T>, AlignedBox<N,T>> sd_query;
	TIQuery<T, AlignedBox<N,T>, AlignedBox<N,T>> ti_query;
	
	Vector<N,T> max = (T)200.0*Vector<N,T>::Ones();
    Vector<N,T> min = -max;
    AlignedBox<N,T> grid_box{min,max};
	
	int num = 1000000;
    std::vector<Vector<N,T>> grid = RandomGrid(num, grid_box);
    std::vector<AlignedBox<N,T>> boxes;
    for (int i = 0; i < num - 1; i++)
    {
    	Vector<N,T> point1 = grid[i];
    	Vector<N,T> point2 = grid[i+1];
    	
    	Vector<N,T> min, max;
    	for (int j = 0; j < N; j++)
    	{
    		min[j] = std::min(point1[j], point2[j]);
    		max[j] = std::max(point1[j], point2[j]);
    	}
    	
    	boxes.push_back(AlignedBox<N,T>{min, max});
    }
	
	for (auto& box : boxes)
		EXPECT_EQ(sd_query(box1, box).sDistance < 0.0, ti_query(box1, box).intersect);
}

TEST(TestSignedDistance, 2D)
{
	const int N = 2;
	using T = double;
	
	Vector<N,T> min1 = 10.0*Vector<N,T>::Ones();
	Vector<N,T> max1 = 100.0*Vector<N,T>::Ones();
	AlignedBox<N,T> box1{min1,max1};
	
	SDQuery<T, AlignedBox<N,T>, AlignedBox<N,T>> sd_query;
	TIQuery<T, AlignedBox<N,T>, AlignedBox<N,T>> ti_query;
	
	Vector<N,T> max = (T)200.0*Vector<N,T>::Ones();
    Vector<N,T> min = -max;
    AlignedBox<N,T> grid_box{min,max};
	
	int num = 1000000;
    std::vector<Vector<N,T>> grid = RandomGrid(num, grid_box);
    std::vector<AlignedBox<N,T>> boxes;
    for (int i = 0; i < num - 1; i++)
    {
    	Vector<N,T> point1 = grid[i];
    	Vector<N,T> point2 = grid[i+1];
    	
    	Vector<N,T> min, max;
    	for (int j = 0; j < N; j++)
    	{
    		min[j] = std::min(point1[j], point2[j]);
    		max[j] = std::max(point1[j], point2[j]);
    	}
    	
    	boxes.push_back(AlignedBox<N,T>{min, max});
    }
	
	for (auto& box : boxes)
		EXPECT_EQ(sd_query(box1, box).sDistance < 0.0, ti_query(box1, box).intersect);
}

TEST(TestSignedDistance, 3D)
{
	const int N = 3;
	using T = double;
	
	Vector<N,T> min1 = 10.0*Vector<N,T>::Ones();
	Vector<N,T> max1 = 100.0*Vector<N,T>::Ones();
	AlignedBox<N,T> box1{min1,max1};
	
	SDQuery<T, AlignedBox<N,T>, AlignedBox<N,T>> sd_query;
	TIQuery<T, AlignedBox<N,T>, AlignedBox<N,T>> ti_query;
	
	Vector<N,T> max = (T)200.0*Vector<N,T>::Ones();
    Vector<N,T> min = -max;
    AlignedBox<N,T> grid_box{min,max};
	
	int num = 1000000;
    std::vector<Vector<N,T>> grid = RandomGrid(num, grid_box);
    std::vector<AlignedBox<N,T>> boxes;
    for (int i = 0; i < num - 1; i++)
    {
    	Vector<N,T> point1 = grid[i];
    	Vector<N,T> point2 = grid[i+1];
    	
    	Vector<N,T> min, max;
    	for (int j = 0; j < N; j++)
    	{
    		min[j] = std::min(point1[j], point2[j]);
    		max[j] = std::max(point1[j], point2[j]);
    	}
    	
    	boxes.push_back(AlignedBox<N,T>{min, max});
    }
	
	for (auto& box : boxes)
		EXPECT_EQ(sd_query(box1, box).sDistance < 0.0, ti_query(box1, box).intersect);
}

TEST(TestSignedDistance, 4D)
{
	const int N = 4;
	using T = double;
	
	Vector<N,T> min1 = 10.0*Vector<N,T>::Ones();
	Vector<N,T> max1 = 100.0*Vector<N,T>::Ones();
	AlignedBox<N,T> box1{min1,max1};
	
	SDQuery<T, AlignedBox<N,T>, AlignedBox<N,T>> sd_query;
	TIQuery<T, AlignedBox<N,T>, AlignedBox<N,T>> ti_query;
	
	Vector<N,T> max = (T)200.0*Vector<N,T>::Ones();
    Vector<N,T> min = -max;
    AlignedBox<N,T> grid_box{min,max};
	
	int num = 1000000;
    std::vector<Vector<N,T>> grid = RandomGrid(num, grid_box);
    std::vector<AlignedBox<N,T>> boxes;
    for (int i = 0; i < num - 1; i++)
    {
    	Vector<N,T> point1 = grid[i];
    	Vector<N,T> point2 = grid[i+1];
    	
    	Vector<N,T> min, max;
    	for (int j = 0; j < N; j++)
    	{
    		min[j] = std::min(point1[j], point2[j]);
    		max[j] = std::max(point1[j], point2[j]);
    	}
    	
    	boxes.push_back(AlignedBox<N,T>{min, max});
    }
	
	for (auto& box : boxes)
		EXPECT_EQ(sd_query(box1, box).sDistance < 0.0, ti_query(box1, box).intersect);
}
