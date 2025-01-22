#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include "Mathematics/Interval.h"

using namespace gte;

using DInterval = gte::Interval<double>;

//
// Single-Interval Intersection Tests
//
TEST(IntervalIntersection, SingleInterval_EmptyB)
{
    // B empty => intersection is empty
    DInterval A(1.0, 4.0);
    DInterval B; // empty

    auto result = gte::Intersection(A, B);
    EXPECT_TRUE(result.IsEmpty());
}

TEST(IntervalIntersection, SingleInterval_EmptyA)
{
    // A empty => intersection is empty
    DInterval A; // empty
    DInterval B(1.0, 4.0);

    auto result = gte::Intersection(A, B);
    EXPECT_TRUE(result.IsEmpty());
}

TEST(IntervalIntersection, SingleInterval_AEqualsB)
{
    // A == B => intersection is the same as A (or B)
    DInterval A(1.0, 4.0);
    DInterval B(1.0, 4.0);

    auto result = gte::Intersection(A, B);
    EXPECT_FALSE(result.IsEmpty());
    EXPECT_DOUBLE_EQ(1.0, result.min);
    EXPECT_DOUBLE_EQ(4.0, result.max);
}

TEST(IntervalIntersection, SingleInterval_NoOverlap)
{
    // A=[1,2], B=[3,4], no overlap => empty
    DInterval A(1.0, 2.0);
    DInterval B(3.0, 4.0);

    auto result = gte::Intersection(A, B);
    EXPECT_TRUE(result.IsEmpty());
}

TEST(IntervalIntersection, SingleInterval_PartialOverlap)
{
    // A=[1,4], B=[2,3], intersection=[2,3]
    DInterval A(1.0, 4.0);
    DInterval B(2.0, 3.0);

    auto result = gte::Intersection(A, B);
    EXPECT_FALSE(result.IsEmpty());
    EXPECT_DOUBLE_EQ(2.0, result.min);
    EXPECT_DOUBLE_EQ(3.0, result.max);
}

TEST(IntervalIntersection, SingleInterval_BCoversA)
{
    // B covers A => intersection is A
    // A=[2,3], B=[1,5]
    DInterval A(2.0, 3.0);
    DInterval B(1.0, 5.0);

    auto result = gte::Intersection(A, B);
    EXPECT_FALSE(result.IsEmpty());
    EXPECT_DOUBLE_EQ(2.0, result.min);
    EXPECT_DOUBLE_EQ(3.0, result.max);
}

TEST(IntervalIntersection, MultipleIntervals_ComplexCase)
{
    // intervalsA (7 intervals), sorted and disjoint
    //  1) [-∞,  -11]
    //  2) [-10, -5 ]
    //  3) [ -3, -1 ]
    //  4) [  0,  1 ]
    //  5) [  2,  3 ]
    //  6) [  5,  6 ]
    //  7) [  7, 10 ]
    std::vector<DInterval> intervalsA {
        DInterval(-std::numeric_limits<double>::infinity(), -11.0),
        DInterval(-10.0, -5.0),
        DInterval(-3.0,  -1.0),
        DInterval( 0.0,   1.0),
        DInterval( 2.0,   3.0),
        DInterval( 5.0,   6.0),
        DInterval( 7.0,  10.0)
    };

    // intervalsB (6 intervals), sorted and disjoint
    //  1) [-∞,  -12]
    //  2) [-8, -4 ]
    //  3) [ -3,  -2 ]
    //  4) [  2,   3 ]
    //  5) [  9,  12 ]
    //  6) [12.1, +∞]
    std::vector<DInterval> intervalsB {
        DInterval(-std::numeric_limits<double>::infinity(), -12.0),
        DInterval(-8.0, -4.0),
        DInterval(-3.0,  -2.0),
        DInterval( 2.0,   3.0),
        DInterval( 9.0,  12.0),
        DInterval(12.1,  std::numeric_limits<double>::infinity())
    };
    
    // Expected result
    // 1) [-∞, -12]
    // 2) [-8, -6]
    // 3) [-3, -2]
    // 4) [2,3]
    // 5) [9, 10]

    // Compute A intersect B
    auto result = gte::Intersection(intervalsA, intervalsB);

    // Construct expected_intervals vector and compare with result via operator==
    std::vector<DInterval> expected_intervals = {
        DInterval(-std::numeric_limits<double>::infinity(), -12.0),
        DInterval(-8.0, -5.0),
        DInterval(-3.0, -2.0),
        DInterval(2.0, 3.0),
        DInterval(9.0, 10.0)
    };
    
    /*
    std::cout << "Expected: " << std::endl;
    for (auto& interval : expected_intervals)
    {
    	std::cout << interval.min << ", " << interval.max << std::endl;
    }
    
    std::cout << "Result: " << std::endl;
    for (auto& interval : result)
    {
    	std::cout << interval.min << ", " << interval.max << std::endl;
    }
    */

    ASSERT_EQ(expected_intervals, result);
}
