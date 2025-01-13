#include <gtest/gtest.h>

#include "Mathematics/SDistPointCone.h"
#include "Mathematics/Vector.h"
#include "Mathematics/ContCone.h"
#include "Mathematics/GridAlignedBox.h"

using namespace gte;

TEST(TestSignedDistance, 1D)
{
	const int N = 1;
	using T = double;
	
	Vector<N,T> center = Vector<N,T>::Zero();
	Vector<N,T> direction = Vector<N,T>::Ones();
	Normalize(direction);
	
	T angle = GTE_C_QUARTER_PI;
	Ray<N,T> ray(center, direction);
	Cone<N,T> cone(ray, angle);
	
	SDQuery<T, Vector<N,T>, Cone<N,T>> query;
	
	Vector<N,T> max = (T)1000.0*Vector<N,T>::Ones();
    Vector<N,T> min = -max;
    AlignedBox<N,T> box{min,max};
	
	int num = 1000000;
    std::vector<Vector<N,T>> grid = RandomGrid(num, box);
	
	for (auto& point : grid)
		EXPECT_EQ(query(point, cone).sDistance < 0.0, InContainer(point, cone));
}

TEST(TestSignedDistance, 2D)
{
	const int N = 2;
	using T = double;
	
	Vector<N,T> center = Vector<N,T>::Zero();
	Vector<N,T> direction = Vector<N,T>::Ones();
	Normalize(direction);
	
	T angle = GTE_C_QUARTER_PI;
	Ray<N,T> ray(center, direction);
	Cone<N,T> cone(ray, angle);
	
	SDQuery<T, Vector<N,T>, Cone<N,T>> query;
	
	Vector<N,T> max = (T)1000.0*Vector<N,T>::Ones();
    Vector<N,T> min = -max;
    AlignedBox<N,T> box{min,max};
	
	int num = 1000000;
    std::vector<Vector<N,T>> grid = RandomGrid(num, box);
	
	for (auto& point : grid)
		EXPECT_EQ(query(point, cone).sDistance < 0.0, InContainer(point, cone));
}

TEST(TestSignedDistance, 3D)
{
	const int N = 3;
	using T = double;
	
	Vector<N,T> center = Vector<N,T>::Zero();
	Vector<N,T> direction = Vector<N,T>::Ones();
	Normalize(direction);
	
	T angle = GTE_C_QUARTER_PI;
	Ray<N,T> ray(center, direction);
	Cone<N,T> cone(ray, angle);
	
	SDQuery<T, Vector<N,T>, Cone<N,T>> query;
	
	Vector<N,T> max = (T)1000.0*Vector<N,T>::Ones();
    Vector<N,T> min = -max;
    AlignedBox<N,T> box{min,max};
	
	int num = 1000000;
    std::vector<Vector<N,T>> grid = RandomGrid(num, box);
	
	for (auto& point : grid)
		EXPECT_EQ(query(point, cone).sDistance < 0.0, InContainer(point, cone));
}

TEST(TestSignedDistance, 4D)
{
	const int N = 4;
	using T = double;
	
	Vector<N,T> center = Vector<N,T>::Zero();
	Vector<N,T> direction = Vector<N,T>::Ones();
	Normalize(direction);
	
	T angle = GTE_C_QUARTER_PI;
	Ray<N,T> ray(center, direction);
	Cone<N,T> cone(ray, angle);
	
	SDQuery<T, Vector<N,T>, Cone<N,T>> query;
	
	Vector<N,T> max = (T)1000.0*Vector<N,T>::Ones();
    Vector<N,T> min = -max;
    AlignedBox<N,T> box{min,max};
	
	int num = 1000000;
    std::vector<Vector<N,T>> grid = RandomGrid(num, box);
	
	for (auto& point : grid)
		EXPECT_EQ(query(point, cone).sDistance < 0.0, InContainer(point, cone));
}

