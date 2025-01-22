// Addon to GeometricTools

#pragma once

#include <cmath>
#include <limits>
#include <vector>
#include <Mathematics/Logger.h>

namespace gte
{
// T should be a floating-point type.
template <typename T>
class Interval
{
public:
	// Constructs an empty interval
	Interval() : 
	min(std::numeric_limits<T>::infinity()),
	max(-std::numeric_limits<T>::infinity())
	{
		
	}

	Interval(T min, T max) :
	min(min),
	max(max)
	{
	}
	
	static Interval Full()
	{
		return Interval(-std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity());
	}
	
	bool IsEmpty() const
	{
		return min > max;
		//return (min == std::numeric_limits<T>::infinity()) && (max == -std::numeric_limits<T>::infinity());
	}
	
	bool IsFull() const
	{
		return IsRightInfinite() && IsLeftInfinite();
	}
	
	bool IsRightInfinite() const
	{
		return std::isinf(max);
	}
	
	bool IsLeftInfinite() const 
	{
		return min == -std::numeric_limits<T>::infinity();
	}
	
	bool IsSingular() const
	{
		return min == max;
	}
	
    T& operator[](std::size_t index)
    {
        switch (index)
        {
        case 0:  return min;
        case 1:  return max;
        default:
            throw std::out_of_range("Interval index must be 0 or 1.");
        }
    }

    const T& operator[](std::size_t index) const
    {
        switch (index)
        {
        case 0:  return min;
        case 1:  return max;
        default:
            throw std::out_of_range("Interval index must be 0 or 1.");
        }
    }
    
    bool operator==(Interval<T> const& other) const
    {
        return (IsEmpty() && other.IsEmpty()) ? true : (min == other.min && max == other.max);
    }
    
    bool operator<(Interval<T> const& other) const
    {
        if (min < other.min)
        {
            return true;
        }
        else if (min > other.min)
        {
            return false;
        }
        // if mins are equal, compare max
        return (max < other.max);
    }
    
    // Need to double check these last three comparators
    bool operator>(Interval<T> const& other) const
    {
        return (other < *this);
    }

    bool operator<=(Interval<T> const& other) const
    {
        return !(other < *this);
    }

    bool operator>=(Interval<T> const& other) const
    {
        return !(*this < other);
    }

	// Public access to data members, 
	// since this class has no invariants
	T min;
	T max;
};

template <typename T>
Interval<T> Intersection(Interval<T> i1, Interval<T> i2)
{
    T new_min = std::max(i1.min, i2.min);
    T new_max = std::min(i1.max, i2.max);

    // If the intersection is empty new_min will be > new_max,
    // denoting an empty interval
    return Interval<T>(new_min, new_max);
}

// Intervals are always closed (except at -inf and inf)
// In this sense, the intervals are not defined on the real numbers,
// only on the set of rational numbers available to floating point type T
template <typename T>
std::vector<Interval<T>> Complement(Interval<T> interval)
{
	if (interval.IsEmpty())
		return {Interval<T>::Full()};
	else if (interval.IsFull())
		return {Interval<T>()};
	else if (interval.IsLeftInfinite())
		return {Interval<T>(std::nextafter(interval.max, std::numeric_limits<T>::infinity()),
		std::numeric_limits<T>::infinity())};
	else if (interval.IsRightInfinite())
		return {Interval<T>(-std::numeric_limits<T>::infinity(),
		std::nextafter(interval.min, -std::numeric_limits<T>::infinity()))};
	else
	{
		Interval<T> first(-std::numeric_limits<T>::infinity(),
			std::nextafter(interval.min, std::numeric_limits<T>::infinity()));
		
		Interval<T> second(std::nextafter(interval.max, std::numeric_limits<T>::infinity()),
			std::numeric_limits<T>::infinity());
		
		return {first,second};
	}
}

// Intersect two collections of intervals. Assumes each collection is sorted,
// and that neither collection has any empty intervals. Assumes each collection is
// disjoint. That is, no two intervals in interval1 (or interval two) intersect.
template <typename T>
std::vector<Interval<T>> Intersection(std::vector<Interval<T>> const& intervals1,
	std::vector<Interval<T>> const& intervals2)
{
    std::vector<Interval<T>> result;
    result.reserve(std::min(intervals1.size(), intervals2.size())); // Rough guess

    // We use two indices to sweep over both sorted sets of intervals.
    std::size_t i = 0;
    std::size_t j = 0;

    while (i < intervals1.size() && j < intervals2.size())
    {
        // Compute the intersection between intervals1[i] and intervals2[j].
        T new_min = std::max(intervals1[i].min, intervals2[j].min);
        T new_max = std::min(intervals1[i].max, intervals2[j].max);

        // If valid, add to the result.
        if (new_min <= new_max)
        {
            result.emplace_back(new_min, new_max);
        }

        // Move the pointer that has the earlier end boundary,
        // because that interval can't intersect any further interval from the other set.
        if (intervals1[i].max < intervals2[j].max)
        {
            ++i;
        }
        else if (intervals2[j].max < intervals1[i].max)
        {
            ++j;
        }
        else
        {
            // If they end at the same value, move both
            ++i;
            ++j;
        }
    }

    return result;
}

// From CHATGPT
template <typename T>
std::vector<Interval<T>> Difference(Interval<T> A, Interval<T> B)
{
    std::vector<Interval<T>> result;

    // 1. If A is empty or A == B, the result is empty.
    if (A.IsEmpty() || A == B)
    {
        return result; // empty
    }

    // 2. If B is empty, nothing to remove from A => A remains.
    if (B.IsEmpty())
    {
        result.push_back(A);
        return result;
    }

    // 3. Compute intersection I = A ∩ B.
    //    If I is empty, A and B do not overlap => A - B = A.
    Interval<T> I = Intersection(A, B);
    if (I.IsEmpty())
    {
        // No overlap.
        result.push_back(A);
        return result;
    }

    // 4. Now subtract the (non-empty) intersection I from A.
    //    This can yield up to two intervals.

    // -- Left piece: [A.min, nextafter(I.min, -∞)], if I.min > A.min.
    if (I.min > A.min)
    {
        T leftMin = A.min;
        T leftMax = std::nextafter(I.min, -std::numeric_limits<T>::infinity());

        // Only add if leftMax >= leftMin, to ensure a valid closed interval in floating‐point.
        if (leftMax >= leftMin)
        {
            result.emplace_back(leftMin, leftMax);
        }
    }

    // -- Right piece: [nextafter(I.max, +∞), A.max], if I.max < A.max.
    if (I.max < A.max)
    {
        T rightMin = std::nextafter(I.max, +std::numeric_limits<T>::infinity());
        T rightMax = A.max;

        // Only add if rightMin <= rightMax.
        if (rightMin <= rightMax)
        {
            result.emplace_back(rightMin, rightMax);
        }
    }

    return result;
}

// From CHATGPT
template <typename T>
std::vector<Interval<T>> Difference(
    std::vector<Interval<T>> const& intervalsA,
    std::vector<Interval<T>> const& intervalsB)
{
    // The result can be as large as intervalsA in the worst case 
    // (if there's little or no overlap).
    // But it might also split intervals, so we can't give a perfect upper bound.
    std::vector<Interval<T>> result;
    result.reserve(intervalsA.size());

    // Index for traversing B
    std::size_t j = 0;

    // For each interval in A, subtract out any overlapping portions in B.
    for (std::size_t i = 0; i < intervalsA.size(); ++i)
    {
        Interval<T> currentA = intervalsA[i];

        // If A is empty (should never happen if "no empty intervals" is guaranteed),
        // skip. But let's be defensive:
        if (currentA.IsEmpty())
        {
            continue;
        }

        // Advance j until B[j] can possibly overlap currentA.
        // That is, we skip all B[j] whose max < currentA.min.
        while (j < intervalsB.size() &&
               intervalsB[j].max < currentA.min)
        {
            ++j;
        }

        // Now subtract from currentA using relevant B intervals.
        // We'll create a local copy of j so we don't lose track 
        // of the "global" pointer we use for the next A-interval. 
        std::size_t localJ = j;

        // We'll keep removing overlaps from currentA as long as currentA is non-empty,
        // and as long as B[localJ] starts before currentA ends.
        while (!currentA.IsEmpty() &&
               localJ < intervalsB.size() &&
               intervalsB[localJ].min <= currentA.max)
        {
            // Compute intersection I = currentA ∩ B[localJ].
            T iMin = std::max(currentA.min, intervalsB[localJ].min);
            T iMax = std::min(currentA.max, intervalsB[localJ].max);

            // If there's no overlap, nothing to remove.
            if (iMin > iMax)
            {
                ++localJ;
                continue;
            }

            // There is an overlap with [iMin, iMax].
            // We'll remove it from currentA.

            // 1) "Left piece" of currentA that lies before iMin.
            if (iMin > currentA.min)
            {
                T leftMin = currentA.min;
                T leftMax = std::nextafter(iMin, -std::numeric_limits<T>::infinity());

                if (leftMax >= leftMin)
                {
                    // Add this left piece to the final result.
                    result.emplace_back(leftMin, leftMax);
                }
            }

            // 2) Update currentA to start just after iMax
            //    because we've subtracted [iMin, iMax].
            T newMin = std::nextafter(iMax, +std::numeric_limits<T>::infinity());
            // If newMin > currentA.max, currentA becomes empty.
            currentA.min = newMin;

            // Move to the next B interval
            ++localJ;
        }

        // If there's anything left in currentA after subtracting all relevant B intervals,
        // add it to the result.
        if (!currentA.IsEmpty())
        {
            result.push_back(currentA);
        }
    }

    return result;
}
}
