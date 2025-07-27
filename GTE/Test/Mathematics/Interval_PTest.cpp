#include <gtest/gtest.h>
#include "Mathematics/Interval.h"
#include <random>
#include <vector>
#include <algorithm>

using namespace gte;

// Alias for convenience
using DInterval = gte::Interval<double>;
int param = 100;

// Generates a vector of random, disjoint intervals within [minVal, maxVal]
std::vector<DInterval> GenerateDisjointIntervals(int numIntervals, double minVal, double maxVal, std::mt19937& rng)
{
    std::vector<DInterval> intervals;
    if (numIntervals <= 0) return intervals;

    std::uniform_real_distribution<double> dist(minVal, maxVal);
    double lastMax = minVal;

    for (int i = 0; i < numIntervals; ++i)
    {
        double start = dist(rng);
        double end = dist(rng);

        if (start > end) std::swap(start, end); // Ensure start <= end

        // Make sure intervals don't overlap with previous
        if (start < lastMax)
        {
            start = lastMax;
        }

        if (start >= end) continue; // Skip degenerate intervals

        intervals.emplace_back(start, end); // Add interval
        lastMax = end; // Update lastMax for next interval
    }

    // Sort and merge to guarantee disjointness
    if (intervals.empty()) return intervals;

    std::sort(intervals.begin(), intervals.end(), [](const DInterval& a, const DInterval& b) {
        return a.min < b.min;
    });

    std::vector<DInterval> merged;
    merged.push_back(intervals[0]);

    for (size_t i = 1; i < intervals.size(); ++i)
    {
        if (intervals[i].min > merged.back().max)
        {
            merged.push_back(intervals[i]); // No overlap, add new
        }
        else
        {
            // Overlap, extend the last interval
            if (intervals[i].max > merged.back().max)
            {
                merged.back().max = intervals[i].max;
            }
        }
    }

    return merged;
}

// Generates a vector of random, disjoint intervals within [minVal, maxVal]
// This method provides a more uniform and dense distribution of disjoint intervals
// compared to generating random interval endpoints and then merging them.
std::vector<DInterval> GenerateDisjointIntervals_2(int numIntervals, double minVal, double maxVal, std::mt19937& rng)
{
    std::vector<DInterval> intervals;
    if (numIntervals <= 0) return intervals;

    // Generate 2*numIntervals random points to serve as endpoints.
    int numPoints = 2 * numIntervals;
    std::vector<double> points(numPoints);
    std::uniform_real_distribution<double> dist(minVal, maxVal);
    for (int i = 0; i < numPoints; ++i)
    {
        points[i] = dist(rng);
    }

    // Sort the points to create ordered endpoints.
    std::sort(points.begin(), points.end());

    // Create intervals from adjacent pairs of points.
    for (int i = 0; i < numPoints; i += 2)
    {
        // Ensure the interval is not degenerate.
        if (i + 1 < numPoints && points[i] < points[i + 1])
        {
            intervals.emplace_back(points[i], points[i + 1]);
        }
    }

    return intervals;
}


// Checks if a point is inside any of the intervals in the collection
bool IsIn(double point, const std::vector<DInterval>& intervals)
{
    for (const auto& interval : intervals)
    {
        if (point >= interval.min && point <= interval.max)
        {
            return true; // Found containing interval
        }
    }
    return false;
}

// Probabilistic test for the Union operation
TEST(IntervalPTest, Union)
{
    std::mt19937 rng(12345); // Fixed seed for reproducibility
    std::vector<DInterval> A = GenerateDisjointIntervals_2(param, -100.0, 100.0, rng);
    std::vector<DInterval> B = GenerateDisjointIntervals_2(param, -100.0, 100.0, rng);

    std::vector<DInterval> C = DInterval::Union(A, B); // Union of A and B

    std::uniform_real_distribution<double> dist(-100.0, 100.0);
    for (int i = 0; i < 100000; ++i)
    {
        double p = dist(rng); // Random point in range
        bool inA = IsIn(p, A);
        bool inB = IsIn(p, B);
        bool inC = IsIn(p, C);
        // Point should be in C if it's in A or B
        ASSERT_EQ(inC, inA || inB);
    }
}

// Probabilistic test for IsDisjoint
TEST(IntervalPTest, IsDisjoint)
{
    std::mt19937 rng(12345);
    std::vector<DInterval> A = GenerateDisjointIntervals_2(param, -100.0, 100.0, rng);
    ASSERT_TRUE(DInterval::IsDisjoint(A)); // Should be disjoint by construction

    // Manually create an overlap and check that IsDisjoint returns false
    if (A.size() > 1)
    {
        A[0].max = A[1].min + 0.1; // Force overlap
        ASSERT_FALSE(DInterval::IsDisjoint(A));
    }
}

// Probabilistic test for Intersection
TEST(IntervalPTest, Intersection)
{
    std::mt19937 rng(12345);
    std::vector<DInterval> A = GenerateDisjointIntervals_2(param, -100.0, 100.0, rng);
    std::vector<DInterval> B = GenerateDisjointIntervals_2(param, -100.0, 100.0, rng);

    std::vector<DInterval> C = DInterval::Intersection(A, B); // Intersection of A and B

    std::uniform_real_distribution<double> dist(-100.0, 100.0);
    for (int i = 0; i < 100000; ++i)
    {
        double p = dist(rng);
        bool inA = IsIn(p, A);
        bool inB = IsIn(p, B);
        bool inC = IsIn(p, C);
        // Point should be in C if and only if it's in both A and B
        ASSERT_EQ(inC, inA && inB);
    }
}

// Probabilistic test for Difference
TEST(IntervalPTest, Difference)
{
    std::mt19937 rng(12345);
    std::vector<DInterval> A = GenerateDisjointIntervals_2(param, -100.0, 100.0, rng);
    std::vector<DInterval> B = GenerateDisjointIntervals_2(param, -100.0, 100.0, rng);

    std::vector<DInterval> C = DInterval::Difference(A, B); // A minus B

    std::uniform_real_distribution<double> dist(-100.0, 100.0);
    for (int i = 0; i < 100000; ++i)
    {
        double p = dist(rng);
        bool inA = IsIn(p, A);
        bool inB = IsIn(p, B);
        bool inC = IsIn(p, C);
        // Point should be in C if it's in A and not in B
        ASSERT_EQ(inC, inA && !inB);
    }
    
    // Verify the volume relationship: Volume(A - B) = Volume(A) - Volume(A ∩ B)
    double volA = DInterval::Volume(A);
    std::vector<DInterval> I = DInterval::Intersection(A, B);
    double volI = DInterval::Volume(I);
    double volC = DInterval::Volume(C);

    // Use EXPECT_NEAR for floating-point comparison.
    EXPECT_NEAR(volC, volA - volI, 1e-9);
}

// Probabilistic test for FillGaps
TEST(IntervalPTest, FillGaps)
{
    std::mt19937 rng(12345);
    std::vector<DInterval> A = GenerateDisjointIntervals(param, -100.0, 100.0, rng);
    
    double tolerance = 1.0;
    std::vector<DInterval> B = DInterval::FillGaps(A, tolerance); // Fill small gaps in A

    // All points in A should also be in B
    std::uniform_real_distribution<double> dist(-100.0, 100.0);
    for (int i = 0; i < 100000; ++i)
    {
        double p = dist(rng);
        if (IsIn(p, A))
        {
            ASSERT_TRUE(IsIn(p, B)); // B should cover all of A
        }
    }

    // Gaps smaller than tolerance should be filled in B
    for (size_t i = 0; i < A.size() - 1; ++i)
    {
        double gap = A[i+1].min - A[i].max;
        if (gap < tolerance)
        {
            double p = A[i].max + gap / 2.0; // Point in the gap
            ASSERT_TRUE(IsIn(p, B)); // Should now be covered
        }
    }
}
