// Addon to GeometricTools
#pragma once

#include <Mathematics/CDQuery.h>
#include <Mathematics/SDistAlignedBoxAlignedBox.h>
#include <Mathematics/AlignedBox.h>
#include <Mathematics/Vector.h>

namespace gte
{
    template <int32_t N, typename T>
    class CDQuery<T, AlignedBox<N, T>, AlignedBox<N, T>>
    {
    public:
        struct Result
        {
            Result()
                :
                cDistance(static_cast<T>(0))
            {
            }

            T cDistance;
        };

		// (box1, box2) != (box2, box1) in general.
		// Query returns containment distance of box2 from box1
		// Negative when box 1 contains box 2
		// For now, this query assumes box2 is a cube
        Result operator()(AlignedBox<N,T> const& box1, AlignedBox<N,T> const& box2)
        {
            Result result{};
            result.cDistance = SDQuery<T, AlignedBox<N,T>, AlignedBox<N,T>>{}(box1,box2).sDistance + (box2.max[0] - box2.min[0]);
            return result;
        }
    };
}
