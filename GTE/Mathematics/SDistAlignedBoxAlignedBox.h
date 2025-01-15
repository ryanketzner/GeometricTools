// Addon to GeometricTools
#pragma once

#include <Mathematics/SDQuery.h>
#include <Mathematics/AlignedBox.h>
#include <Mathematics/Vector.h>

namespace gte
{
    template <int32_t N, typename T>
    class SDQuery<T, AlignedBox<N, T>, AlignedBox<N, T>>
    {
    public:
        struct Result
        {
            Result()
                :
                sDistance(static_cast<T>(0))
            {
            }

            T sDistance;
        };

        Result operator()(AlignedBox<N,T> const& box1, AlignedBox<N,T> const& box2, bool robust = false)
        {
            Result result{};

            // We'll accumulate squared gap distances for non-overlapping dimensions.
            T distSquared = static_cast<T>(0);

            // Track if there's a gap in at least one dimension.
            bool haveGap = false;

            // Keep track of the minimum overlap across dimensions
            // (only used if boxes actually overlap in all dimensions).
            T minOverlap = std::numeric_limits<T>::infinity();

            // Single pass over the dimensions
            for (int32_t i = 0; i < N; ++i)
            {
                // Overlap along axis i
                T minmax = std::min(box1.max[i], box2.max[i]);
                T maxmin = std::max(box1.min[i], box2.min[i]);
                T overlap = minmax - maxmin;

                if (overlap <= static_cast<T>(0))
                {
                    // Negative overlap => gap along this dimension
                    haveGap = true;
                    distSquared += overlap * overlap;  // accumulate square of the gap
                }
                else
                {
                	T minmin = std::min(box1.min[i], box2.min[i]);
                	T maxmax = std::max(box1.max[i], box2.max[i]);
                	
                	T min_penetration = std::min(maxmin - minmin, maxmax - minmax);
                	overlap = min_penetration + overlap;
                    // Non-negative overlap => they touch or intersect in this dimension
                    if (overlap < minOverlap)
                        minOverlap = overlap;
                }
            }

            if (haveGap)
                // At least one dimension has a gap => boxes are separate.
                // Return the positive Euclidean gap distance.
                result.sDistance = static_cast<T>(std::sqrt(distSquared));
            else
                // Boxes overlap in all dimensions (or just touch).
                // minOverlap is >= 0 at this point.
                //   - If minOverlap > 0 => true overlap => negative distance
                //   - If minOverlap = 0 => just touching => returns 0
                result.sDistance = -minOverlap;

            return result;
        }
    };
}
