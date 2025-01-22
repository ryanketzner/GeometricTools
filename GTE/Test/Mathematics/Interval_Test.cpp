#include <gtest/gtest.h>

#include "Mathematics/Interval.h"
#include "Mathematics/Vector.h"
#include "Mathematics/AlignedBox.h"
#include "Mathematics/GridAlignedBox.h"

using namespace gte;

// Alias for convenience
using DInterval = gte::Interval<double>;

TEST(IntervalDifference, SingleInterval_EmptyB)
{
    // B empty => result is A
    DInterval A(1.0, 4.0);
    DInterval B; // empty by default constructor

    auto result = gte::Difference(A, B);
    ASSERT_EQ(1u, result.size());
    EXPECT_EQ(1.0, result[0].min);
    EXPECT_EQ(4.0, result[0].max);
}

TEST(IntervalDifference, SingleInterval_EmptyA)
{
    // A empty => result is empty
    DInterval A; // empty
    DInterval B(1.0, 4.0);

    auto result = gte::Difference(A, B);
    EXPECT_TRUE(result.empty());
}

TEST(IntervalDifference, SingleInterval_AEqualsB)
{
    // A == B => result is empty
    DInterval A(1.0, 4.0);
    DInterval B(1.0, 4.0);

    auto result = gte::Difference(A, B);
    EXPECT_TRUE(result.empty());
}

TEST(IntervalDifference, SingleInterval_NoOverlap)
{
    // A = [1,2], B = [3,4], no overlap => result is A
    DInterval A(1.0, 2.0);
    DInterval B(3.0, 4.0);

    auto result = gte::Difference(A, B);
    ASSERT_EQ(1u, result.size());
    EXPECT_EQ(1.0, result[0].min);
    EXPECT_EQ(2.0, result[0].max);
}

TEST(IntervalDifference, SingleInterval_PartialOverlap)
{
    // A = [1,4], B = [2,3]
    // Intersection = [2,3]
    // => left piece:  [1, next_after(2, -∞)]
    // => right piece: [next_after(3, +∞), 4]

    DInterval A(1.0, 4.0);
    DInterval B(2.0, 3.0);

    auto result = gte::Difference(A, B);
    ASSERT_EQ(2u, result.size()); 

    // Precompute expected boundaries
    double leftMax = std::nextafter(2.0, -std::numeric_limits<double>::infinity());
    double rightMin = std::nextafter(3.0, +std::numeric_limits<double>::infinity());

    // Left piece
    EXPECT_EQ(1.0, result[0].min);
    EXPECT_EQ(leftMax, result[0].max);

    // Right piece
    EXPECT_EQ(rightMin, result[1].min);
    EXPECT_EQ(4.0, result[1].max);
}

TEST(IntervalDifference, SingleInterval_BCoversA)
{
    // B covers A completely => result is empty
    // A = [2,3], B = [1,5]
    DInterval A(2.0, 3.0);
    DInterval B(1.0, 5.0);

    auto result = gte::Difference(A, B);
    EXPECT_TRUE(result.empty());
}

// Now test the multi-interval version
TEST(IntervalDifference, MultipleIntervals_SimpleCase)
{
    // intervalsA = { [1,2], [4,5] }
    // intervalsB = { [1.5,1.7], [7,9] }
    // Overlap is only with [1.5,1.7] in the first interval [1,2].
    // The second interval [4,5] does not overlap with [7,9].
    //
    // So from [1,2], removing [1.5,1.7] yields:
    //   left piece :  [1, next_after(1.5, -∞)]
    //   right piece: [next_after(1.7, +∞), 2]
    // The interval [4,5] remains intact.

    std::vector<DInterval> intervalsA { {1.0, 2.0}, {4.0, 5.0} };
    std::vector<DInterval> intervalsB { {1.5, 1.7}, {7.0, 9.0} };

    auto result = gte::Difference(intervalsA, intervalsB);

    // Expect 3 intervals in ascending order:
    //   1) [1, next_after(1.5, -∞)]
    //   2) [next_after(1.7, +∞), 2]
    //   3) [4, 5]

    ASSERT_EQ(3u, result.size());

    double leftMax = std::nextafter(1.5, -std::numeric_limits<double>::infinity());
    double rightMin = std::nextafter(1.7, +std::numeric_limits<double>::infinity());

    // 1) [1, leftMax]
    EXPECT_EQ(1.0, result[0].min);
    EXPECT_EQ(leftMax, result[0].max);

    // 2) [rightMin, 2]
    EXPECT_EQ(rightMin, result[1].min);
    EXPECT_EQ(2.0, result[1].max);

    // 3) [4, 5]
    EXPECT_EQ(4.0, result[2].min);
    EXPECT_EQ(5.0, result[2].max);
}

TEST(IntervalDifference, MultipleIntervals_ComplexCase)
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
    //  2) [-11, -6 ]
    //  3) [ -3,  -2 ]
    //  4) [  2,   3 ]
    //  5) [  9,  12 ]
    //  6) [12.1, +∞]
    std::vector<DInterval> intervalsB {
        DInterval(-std::numeric_limits<double>::infinity(), -12.0),
        DInterval(-11.0, -6.0),
        DInterval(-3.0,  -2.0),
        DInterval( 2.0,   3.0),
        DInterval( 9.0,  12.0),
        DInterval(12.1,  std::numeric_limits<double>::infinity())
    };

    // Compute A \ B
    auto result = gte::Difference(intervalsA, intervalsB);

    EXPECT_EQ(6u, result.size());

    // For exact boundary checks, define helpful references:
    double neg12_plus = std::nextafter(-12.0, +std::numeric_limits<double>::infinity());
    double neg11_minus = std::nextafter(-11.0, -std::numeric_limits<double>::infinity());
    double neg6_plus   = std::nextafter(-6.0,  +std::numeric_limits<double>::infinity());
    double neg2_plus   = std::nextafter(-2.0,  +std::numeric_limits<double>::infinity());
    double nine_minus  = std::nextafter( 9.0,  -std::numeric_limits<double>::infinity());

    // Check them individually
    
    // 1) [neg12_plus, neg11_minus]
    EXPECT_EQ(neg12_plus, result[0].min);
    EXPECT_EQ(neg11_minus, result[0].max);
    
    // 2) [neg6_plus, -5]
    EXPECT_EQ(neg6_plus, result[1].min);
    EXPECT_EQ(-5.0,      result[1].max);

    // 3) [neg2_plus, -1]
    EXPECT_EQ(neg2_plus, result[2].min);
    EXPECT_EQ(-1.0,      result[2].max);

    // 4) [0, 1]
    EXPECT_EQ(0.0, result[3].min);
    EXPECT_EQ(1.0, result[3].max);

    // 5) [5, 6]
    EXPECT_EQ(5.0, result[4].min);
    EXPECT_EQ(6.0, result[4].max);

    // 6) [7, nine_minus]
    EXPECT_EQ(7.0,       result[5].min);
    EXPECT_EQ(nine_minus, result[5].max);

    // Finally, construct an 'expected_intervals' vector and compare with 'result' via operator==
    std::vector<DInterval> expected_intervals = {
    	DInterval(neg12_plus, neg11_minus),
        DInterval(neg6_plus, -5.0),
        DInterval(neg2_plus, -1.0),
        DInterval(0.0,       1.0),
        DInterval(5.0,       6.0),
        DInterval(7.0,       nine_minus)
    };

    ASSERT_EQ(expected_intervals, result);
}
