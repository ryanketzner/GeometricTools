#include <gtest/gtest.h>

#include "Mathematics/CDistAlignedBoxAlignedBox.h"
#include "Mathematics/Vector.h"
#include "Mathematics/ContAlignedBox.h"
#include "Mathematics/GridAlignedBox.h"

using namespace gte;

// All tests the same except for dimension.

// Create an alligned box with min = 10, max = 100
// Then, generate one million random points between -200 and 200.
// Loop over the points and generate random cubes starting at each
// point with a width of 20.0. Confirm that if the containment distance
// is strictly negative, the box is contained, and if the containment
// distance is greater than or equal to zero, the box is not contained.
TEST(TestContainmentDistance, 1D)
{
	const int N = 1;
	using T = double;
	
	Vector<N,T> min1 = 10.0*Vector<N,T>::Ones();
	Vector<N,T> max1 = 100.0*Vector<N,T>::Ones();
	AlignedBox<N,T> box1{min1,max1};
	
	CDQuery<T, AlignedBox<N,T>, AlignedBox<N,T>> cd_query;
	
	Vector<N,T> max = (T)200.0*Vector<N,T>::Ones();
    Vector<N,T> min = -max;
    AlignedBox<N,T> grid_box{min,max};
	
	int num = 1000000;
    std::vector<Vector<N,T>> grid = RandomGrid(num, grid_box);
    std::vector<AlignedBox<N,T>> boxes;
    for (int i = 0; i < num - 1; i++)
    {
    	Vector<N,T> min = grid[i];
    	Vector<N,T> max = grid[i] + 20.0*Vector<N,T>::Ones();
    	
    	boxes.push_back(AlignedBox<N,T>{min, max});
    }
	
	for (auto& box : boxes)
		// Doesn't work for <=. Need to go through edge cases and find out why.
		EXPECT_EQ(cd_query(box1, box).cDistance < 0.0, InContainer(box,box1)); 
}

TEST(TestContainmentDistance, 2D)
{
	const int N = 2;
	using T = double;
	
	Vector<N,T> min1 = 10.0*Vector<N,T>::Ones();
	Vector<N,T> max1 = 100.0*Vector<N,T>::Ones();
	AlignedBox<N,T> box1{min1,max1};
	
	CDQuery<T, AlignedBox<N,T>, AlignedBox<N,T>> cd_query;
	
	Vector<N,T> max = (T)200.0*Vector<N,T>::Ones();
    Vector<N,T> min = -max;
    AlignedBox<N,T> grid_box{min,max};
	
	int num = 1000000;
    std::vector<Vector<N,T>> grid = RandomGrid(num, grid_box);
    std::vector<AlignedBox<N,T>> boxes;
    for (int i = 0; i < num - 1; i++)
    {
    	Vector<N,T> min = grid[i];
    	Vector<N,T> max = grid[i] + 20.0*Vector<N,T>::Ones();
    	
    	boxes.push_back(AlignedBox<N,T>{min, max});
    }
	
	for (auto& box : boxes)
		// Doesn't work for <=. Need to go through edge cases and find out why.
		EXPECT_EQ(cd_query(box1, box).cDistance < 0.0, InContainer(box,box1)); 
}

TEST(TestContainmentDistance, 3D)
{
	const int N = 3;
	using T = double;
	
	Vector<N,T> min1 = 10.0*Vector<N,T>::Ones();
	Vector<N,T> max1 = 100.0*Vector<N,T>::Ones();
	AlignedBox<N,T> box1{min1,max1};
	
	CDQuery<T, AlignedBox<N,T>, AlignedBox<N,T>> cd_query;
	
	Vector<N,T> max = (T)200.0*Vector<N,T>::Ones();
    Vector<N,T> min = -max;
    AlignedBox<N,T> grid_box{min,max};
	
	int num = 1000000;
    std::vector<Vector<N,T>> grid = RandomGrid(num, grid_box);
    std::vector<AlignedBox<N,T>> boxes;
    for (int i = 0; i < num - 1; i++)
    {
    	Vector<N,T> min = grid[i];
    	Vector<N,T> max = grid[i] + 20.0*Vector<N,T>::Ones();
    	
    	boxes.push_back(AlignedBox<N,T>{min, max});
    }
	
	for (auto& box : boxes)
		// Doesn't work for <=. Need to go through edge cases and find out why.
		EXPECT_EQ(cd_query(box1, box).cDistance < 0.0, InContainer(box,box1)); 
}

TEST(TestContainmentDistance, 4D)
{
	const int N = 4;
	using T = double;
	
	Vector<N,T> min1 = 10.0*Vector<N,T>::Ones();
	Vector<N,T> max1 = 100.0*Vector<N,T>::Ones();
	AlignedBox<N,T> box1{min1,max1};
	
	CDQuery<T, AlignedBox<N,T>, AlignedBox<N,T>> cd_query;
	
	Vector<N,T> max = (T)200.0*Vector<N,T>::Ones();
    Vector<N,T> min = -max;
    AlignedBox<N,T> grid_box{min,max};
	
	int num = 1000000;
    std::vector<Vector<N,T>> grid = RandomGrid(num, grid_box);
    std::vector<AlignedBox<N,T>> boxes;
    for (int i = 0; i < num - 1; i++)
    {
    	Vector<N,T> min = grid[i];
    	Vector<N,T> max = grid[i] + 20.0*Vector<N,T>::Ones();
    	
    	boxes.push_back(AlignedBox<N,T>{min, max});
    }
	
	for (auto& box : boxes)
		// Doesn't work for <=. Need to go through edge cases and find out why.
		EXPECT_EQ(cd_query(box1, box).cDistance < 0.0, InContainer(box,box1)); 
}

